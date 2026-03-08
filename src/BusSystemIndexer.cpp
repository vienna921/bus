#include "BusSystemIndexer.h"

#include <algorithm>
#include <unordered_map>
#include <limits>

struct CBusSystemIndexer::SImplementation{
    std::shared_ptr<CBusSystem> DBusSystem;
    std::vector<std::shared_ptr<SStop>> DSortedStops;
    std::vector<std::shared_ptr<SRouteIndexer>> DSortedRoutes;
    std::unordered_map<std::string, std::shared_ptr<SRouteIndexer>> DRouteByName;
    std::unordered_map<TStopID, std::unordered_set<std::string>> DStopToRoutes;
};
class CRouteIndexer : public CBusSystemIndexer::SRouteIndexer{
    public:
        size_t FindStopIndex(TStopID stopid, size_t start = 0) const override{
            for(size_t i = start; i < StopIDs.size(); i++){
                if(StopIDs[i] == stopid){
                    return i;
                }
            }
            return std::numeric_limits<size_t>::max();
        }
        std::vector<TStopID> StopIDsSourceDestination(TStopID src, TStopID dest) const override {
            std::vector<TStopID> Result;
            auto SrcIndex = FindStopIndex(src);
            if(SrcIndex == std::numeric_limits<size_t>::max()){
                return Result;
            }
            auto DestIndex = FindStopIndex(dest, SrcIndex);
            if(DestIndex == std::numeric_limits<size_t>::max()){
                return Result;
            }
            for(size_t i = SrcIndex; i<=DestIndex; i++){
                Result.push_back(StopIDs[i]);
            }
            return Result;
        }
}; 
// Constructor for the Bus System Indexer
CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->DBusSystem = bussystem;

    for(size_t i = 0; i<bussystem->StopCount(); i++){
        auto Stop = bussystem->StopByIndex(i);
        DImplementation->DSortedStops.push_back(Stop);
    }
    std::sort(DImplementation->DSortedStops.begin(), DImplementation->DSortedStops.end(), [](const std::shared_ptr<SStop> &a, const std::shared_ptr<SStop> &b){
        return a->ID() < b->ID();
    });
    for(size_t i = 0; i<bussystem->RouteCount(); i++){
        auto Route = bussystem->RouteByIndex(i);
        auto RouteIndexer = std::make-shared<CRouteIndexer>();
        RouteIndexer->Name = Route->Name();
        RouteIndexer->StopIDs = Route->StopIDs();
        RouteIndexer->Trips = Route->Trips();
        DImplementation->DSortedRoutes.push_back(RouteIndexer);
    }
    std::sort(DImplementation->DSortedRoutes.begin(), DImplementation->DSortedRoutes.end(), [](const auto &A, const auto &B){
        return A->Name() < B->Name();
    });
    for(auto &Route:DImplementation->DSortedRoutes){
        DImplementation->DRouteByName[Route->Name()] = Route;
        for(auto StopID:Route->StopIDs){
            DImplementation->DStopToRoutes[StopID].insert(Route->Name());
        }
    }
};

// Destructor for the Bus System Indexer
CBusSystemIndexer::~CBusSystemIndexer(){

}
// Returns the number of stops in the CBusSystem being indexed
size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->DSortedStops.size();
}
// Returns the number of routes in the CBusSystem being indexed
size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->DSortedRoutes.size();
}
// Returns the SStop specified by the index where the stops are sorted by
// their ID, nullptr is returned if index is greater than equal to
// StopCount()
std::shared_ptr<CBusSystemIndexer::SStop> CBusSystemIndexer::SortedStopByIndex(size_t index) const noexcept{
    if(index >= DImplementation->DSortedStops.size()){
        return nullptr;
    }
    return DImplementation->DSortedStops[index];
}
// Returns the SRouteIndexer specified by the index where the routes are
// sorted by their Name, nullptr is returned if index is greater than equal
// to RouteCount()
std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::SortedRouteByIndex(size_t index) const
noexcept{
    if(index>=DImplementation->DSortedRoutes.size()){
        return nullptr;
    }
    return DImplementation->DSortedRoutes[index];
}
// Returns a route indexer by the route name
std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::RouteByName(const std::string &name) const
noexcept{
    auto indexer = DImplementation->DRouteByName.find(name);

    if(indexer == DImplementation->DRouteByName.end()){
        return nullptr;
    }
    return DImplementation->DSortedRoutes[index];
}
// Finds all names of routes that stop at the stopid
bool CBusSystemIndexer::RoutesByStopID(TStopID stopid, std::unordered_set< std::string >
&routes) const noexcept{
    auto indexer = DImplementation->DStopToRoutes.find(stopid);
    if(indexer == DImplementation->DStopToRoutes.end()){
        return false;
    }
    routes = indexer->second;
    return true;
}
// Finds all routes that have the src and dest in their route.
bool CBusSystemIndexer::RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_set<
std::string > &routes) const noexcept{
    auto Src = DImplementation->DStopToRoutes.find(src);
    auto Dest = DImplementation->DStopToRoutes.find(dest);

    if(Src == DImplementation->DStopToRoutes.end() || Dest == DImplementation->DStopToROutes.end()){
        return false;
    }
    for(const auto &Route : Src->second){
        if(Dest->second.count(Route)){
            routes.insert(Route);
        }
    }
    return !routes.empty();
}
// Finds all stops that have a the two routes in common
bool CBusSystemIndexer::StopIDsByRoutes(const std::string &route1, const std::string &route2,
std::unordered_set< TStopID > &stops) const noexcept{
    auto R1 = RouteByName(route1);
    auto R2 = RouteByName(route2);
    if(!R1 || !R2){
        return false;
    }
    for(auto Stop1:R1->StopIDs){
        for(auto Stop2:R2->StopIDs){
            if(Stop1 == Stop2){
                stops.insert(Stop1);
            }
        }
    }
    return !stops.empty();
}
// SRouteIndexer specific functions
// Finds the stop index of the stopid starting at start. If it is not found,
// std::numeric_limits<size_t>::max() is returned.
size_t SRouteIndexer::FindStopIndex(TStopID stopid, size_t start = 0) const{
    for(size_t i = start; i<Route->StopIDs.size(); i++){
        if(Route->StopIDs[i] == stopid){
            return i;
        }
    }
    return std::numeric_limits<size_t>::max();
}
// Returns the stop IDs of the stops between the src and destination.
std::vector< TStopID > SRouteIndexer::StopIDsSourceDestination(TStopID src, TStopID dest)
const{
    std::vector<TStopID> result;
    auto srcIndex = FindStopIndex(src);
    auto destIndex = FindStopIndex(dest, srcIndex);

    if(srcIndex == std::numeric_limits<size_t>::max() || destIndex == std::numeric_limits<size_t>::max()){
        return result;
    }
    for(size_t i = srcIndex; i<= destIndex; i++){
        result.push_back(Route->StopIDs[i]);
    }
    return result;
}