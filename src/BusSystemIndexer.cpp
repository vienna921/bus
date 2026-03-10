#include "BusSystemIndexer.h"
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <vector>

struct CBusSystemIndexer::SImplementation{
    std::vector<std::shared_ptr<SStop>> DSortedStops;
    std::vector<std::shared_ptr<SRouteIndexer>> DSortedRoutes;
    
    std::unordered_map<TStopID, std::shared_ptr<SStop>> DStopByID;
    std::unordered_map<std::string, std::shared_ptr<SRouteIndexer>> DRouteByName;
    std::unordered_map<TStopID, std::unordered_set<std::string>> DStopToRoutes;
};
class CRouteIndexer : public CBusSystemIndexer::SRouteIndexer{
    private:
        std::string DName;
        std::vector<CBusSystemIndexer::TStopID> DStopIDs;
        std::vector<std::vector<CBusSystem::TStopTime>> DTripTimes;
    
    public:
        CRouteIndexer(const std::shared_ptr<CBusSystem::SRoute> &Route){
            DName = Route->Name();

            for(size_t j = 0; j<Route->StopCount(); j++){
                DStopIDs.push_back(Route->GetStopID(j));
            }
            for(size_t t = 0; t<Route->TripCount(); t++){
                std::vector<CBusSystem::TStopTime> TripTimes;
                for(size_t s = 0; s<Route->StopCount(); s++){
                    TripTimes.push_back(Route->GetStopTime(s,t));
                }
                DTripTimes.push_back(TripTimes);
            }
        }
        std::string Name() const noexcept override{
            return DName;
        }
        std::size_t StopCount() const noexcept override{
            return DStopIDs.size();
        }
        std::size_t TripCount() const noexcept override{
            return DTripTimes.size();
        }
        CBusSystem::TStopID GetStopID(std::size_t index) const noexcept override{
            if(index<DStopIDs.size()){
                return DStopIDs[index];
            }
            return CBusSystem::InvalidStopID;
        }
        CBusSystem::TStopTime GetStopTime(std::size_t stopIndex, std::size_t tripIndex) const noexcept override{
            if(stopIndex < DStopIDs.size() && tripIndex < DTripTimes.size()){
                return DTripTimes[tripIndex][stopIndex];
            }
            
            return CBusSystem::TStopTime(std::chrono::seconds(0));
        }

        size_t FindStopIndex(CBusSystem::TStopID stopid, size_t start = 0) const override{
            for(size_t i = start; i<DStopIDs.size(); i++){
                if(DStopIDs[i] == stopid){
                    return i;
                }
            }
            return std::numeric_limits<size_t>::max();
        }
        std::vector<CBusSystem::TStopID> StopIDsSourceDestination(CBusSystem::TStopID src, CBusSystem::TStopID dest) const override{
            std::vector<CBusSystem::TStopID> Result;

            auto SrcIndex = FindStopIndex(src);
            if(SrcIndex == std::numeric_limits<size_t>::max()){
                return Result;
            }

            auto DestIndex = FindStopIndex(dest, SrcIndex);
            if(DestIndex == std::numeric_limits<size_t>::max()){
                return Result;
            }
            for(size_t i = SrcIndex; i<=DestIndex; i++){
                Result.push_back(DStopIDs[i]);
            }
            return Result;
        }
        void AddStop(CBusSystem::TStopID stopid){
            DStopIDs.push_back(stopid);
        }
};
bool StopCompare(const std::shared_ptr<CBusSystemIndexer::SStop> &A, const std::shared_ptr<CBusSystemIndexer::SStop> &B){
    return A->ID() < B->ID();
}

bool RouteCompare(const std::shared_ptr<CBusSystemIndexer::SRouteIndexer> &A, const std::shared_ptr<CBusSystemIndexer::SRouteIndexer> &B){
    return A->Name() < B->Name();
}

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>();
    // stop index
    for(size_t i = 0; i<bussystem->StopCount(); i++){
        auto Stop = bussystem->StopByIndex(i);

        DImplementation->DSortedStops.push_back(Stop);
        DImplementation->DStopByID[Stop->ID()] = Stop;
    }
    std::sort(DImplementation->DSortedStops.begin(), DImplementation->DSortedStops.end(), StopCompare);

    // route index
    for(size_t i = 0; i<bussystem->RouteCount(); i++){
        auto Route = bussystem->RouteByIndex(i);
        auto RouteIndexer = std::make_shared<CRouteIndexer>(Route);
        for(size_t j = 0; j<Route->StopCount(); j++){
            auto StopID = Route->GetStopID(j);
            DImplementation->DStopToRoutes[StopID].insert(Route->Name());
        }   
        DImplementation->DSortedRoutes.push_back(RouteIndexer);
        DImplementation->DRouteByName[Route->Name()] = RouteIndexer;    
    }
    std::sort(DImplementation->DSortedRoutes.begin(), DImplementation->DSortedRoutes.end(), RouteCompare);
}

CBusSystemIndexer::~CBusSystemIndexer(){

}

std::size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->DSortedStops.size();
}
std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->DSortedRoutes.size();
}
std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    if(index < DImplementation->DSortedStops.size()){
        return DImplementation->DSortedStops[index];
    }
    return nullptr;
}
std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
    if(index<DImplementation->DSortedRoutes.size()){
        return DImplementation->DSortedRoutes[index];
    }
    return nullptr;
}
std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::RouteByName(const std::string &name) const noexcept{
    auto Found = DImplementation->DRouteByName.find(name);
    if(Found != DImplementation->DRouteByName.end()){
        return Found->second;
    }
    return nullptr;
}
bool CBusSystemIndexer::RoutesByStopID(TStopID stopid, std::unordered_set<std::string> &routes) const noexcept{
    auto Found = DImplementation->DStopToRoutes.find(stopid);
    if(Found == DImplementation->DStopToRoutes.end()){
        return false;
    }
    routes = Found->second;
    return true;
}

bool CBusSystemIndexer::RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_set<std::string> &routes) const noexcept{
    routes.clear();
    for(auto &Route:DImplementation->DSortedRoutes){
        auto SrcIndex = Route->FindStopIndex(src);
        if(SrcIndex == std::numeric_limits<size_t>::max()){
            continue;
        }
        auto DestIndex = Route->FindStopIndex(dest, SrcIndex);
        if(DestIndex != std::numeric_limits<size_t>::max()){
            routes.insert(Route->Name());
        }
    }
    return !routes.empty();
}

bool CBusSystemIndexer::StopIDsByRoutes(const std::string &route1, const std::string &route2, std::unordered_set<TStopID> &stops) const noexcept{
    auto R1 = RouteByName(route1);
    auto R2 = RouteByName(route2);
    if(!R1 || !R2){
        return false;
    }
    for(size_t i = 0; i<R1->StopCount(); i++){
        auto StopID = R1->GetStopID(i);
        if(R2->FindStopIndex(StopID) != std::numeric_limits<size_t>::max()){
            stops.insert(StopID);
        }
    }
    return !stops.empty();
}