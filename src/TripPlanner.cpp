#include "TripPlanner.h"


struct CTripPlanner::SImplementation{
    std::shared_ptr<CBusSystemIndexer> Indexer;
        SImplementation(std::shared_ptr<CBusSystem> bussystem) : Indexer(std::make_shared<CBusSystemIndexer>(bussystem)){
            
        }

        ~SImplementation(){

        }
        
        std::shared_ptr<CBusSystemIndexer> BusSystemIndexer() const{
            //return nullptr;
            return Indexer;
        }
        
        std::shared_ptr< CBusSystemIndexer::SRouteIndexer > FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const{
            //return nullptr;
            std::unordered_set<std::string> routes;
            if(!Indexer->RoutesByStopIDs(src, dest, routes)){
                return nullptr;
            }
            std::shared_ptr<CBusSystemIndexer::SRouteIndexer> bestRoute;
            TStopTime bestTime(std::chrono::hours(24));

            for(const auto &rname : routes){
                auto route = Indexer->RouteByName(rname);
                if(!route){
                    continue;
                }
                size_t stopIndex = route->FindStopIndex(src);
                for(size_t t = 0; t<route->TripCount(); t++){
                    auto time = route->GetStopTime(stopIndex, t);
                    if(time.to_duration()>=leaveat.to_duration() && time.to_duration()<bestTime.to_duration()){
                        bestTime = time;
                        bestRoute = route;
                    }
                }
            }
            return bestRoute;
        }
        
        std::shared_ptr< CBusSystemIndexer::SRouteIndexer > FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const{
            // return nullptr;
            std::unordered_set<std::string> routes;
            if(!Indexer->RoutesByStopIDs(src, dest, routes)){
                return nullptr;
            }
            std::shared_ptr<CBusSystemIndexer::SRouteIndexer> bestRoute;
            TStopTime bestTime(std::chrono::hours(0));

            for(const auto &rname:routes){
                auto route = Indexer->RouteByName(rname);
                if(!route){
                    continue;
                }

                size_t destIndex = route->FindStopIndex(dest);
                for(size_t t=0; t<route->TripCount(); t++){
                    auto time = route->GetStopTime(destIndex, t);
                    if(time.to_duration() <= arriveby.to_duration() && time.to_duration() > bestTime.to_duration()){
                        bestTime = time;
                        bestRoute = route;
                    }
                }
            }
            return bestRoute;
        }
        
        bool FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const{
            plan.clear();
            //direct route
            auto directRoute = FindDirectRouteLeaveTime(src, dest, leaveat);
            if(directRoute){
                size_t srcIndex = directRoute->FindStopIndex(src);
                size_t destIndex = directRoute->FindStopIndex(dest);
                // find earliest trip leaving after leaveat
                for(size_t t=0; t<directRoute->TripCount(); t++){
                    auto time = directRoute->GetStopTime(srcIndex, t);
                    if(time.to_duration()>= leaveat.to_duration()){
                        plan.push_back({time, src, directRoute->Name()});
                        plan.push_back({directRoute->GetStopTime(destIndex,t), dest, ""});
                        return true;
                    }
                }
                return false;
            }
            
            // one-transfer route
            TStopTime bestArrival(std::chrono::hours(24));
            for(size_t i=0; i<Indexer->RouteCount(); i++){
                auto route1 = Indexer->SortedRouteByIndex(i);
                size_t srcIndex = route1->FindStopIndex(src);
                if(srcIndex == std::numeric_limits<size_t>::max()){
                    continue;
                }
                for(size_t j = srcIndex + 1; j<route1->StopCount(); j++){
                    TStopID transfer = route1->GetStopID(j);
                    if(transfer==src || transfer == dest){
                        continue;
                    }

                    std::unordered_set<std::string> connectingRoutes;
                    if(!Indexer->RoutesByStopIDs(transfer, dest, connectingRoutes)){
                        continue;
                    }
                    for(const auto &rname2:connectingRoutes){
                        auto route2 = Indexer->RouteByName(rname2);
                        if(!route2){
                            continue;
                        }
                        size_t transferIndex2 = route2->FindStopIndex(transfer);
                        size_t destIndex2 = route2->FindStopIndex(dest);
                        if(transferIndex2 == std::numeric_limits<size_t>::max()||destIndex2 == std::numeric_limits<size_t>::max()){
                            continue;
                        }
                        for(size_t t1 = 0; t1<route1->TripCount(); t1++){
                            auto time1= route1->GetStopTime(srcIndex, t1);
                            if(time1.to_duration()<leaveat.to_duration()){
                                continue;
                            }
                            auto arriveTransfer = route1->GetStopTime(j, t1);

                            for(size_t t2 = 0; t2<route2->TripCount(); t2++){
                                auto departTransfer = route2->GetStopTime(transferIndex2, t2);
                                auto arrivalDest = route2->GetStopTime(destIndex2, t2);
                                if(departTransfer.to_duration() >= arriveTransfer.to_duration() && arrivalDest.to_duration() < bestArrival.to_duration()){
                                    bestArrival = arrivalDest;
                                    plan.clear();
                                    plan.push_back({time1, src, route1->Name()});
                                    plan.push_back({departTransfer, transfer, route2->Name()});
                                    plan.push_back({arrivalDest, dest, ""});
                                }
                            }
                        }
                    }
                }
            }
            if(!plan.empty()){
                return true;
            }
            return false;
        }
        
        bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const{
            plan.clear();
            //direct route
            auto directRoute = FindDirectRouteArrivalTime(src, dest, arriveby);
            if(directRoute){
                size_t srcIndex = directRoute->FindStopIndex(src);
                size_t destIndex = directRoute->FindStopIndex(dest);
                for(size_t t=0; t<directRoute->TripCount(); t++){
                    auto arrivalTime = directRoute->GetStopTime(destIndex, t);
                    if(arrivalTime.to_duration()<=arriveby.to_duration()){
                        plan.push_back({directRoute->GetStopTime(srcIndex, t), src, directRoute->Name()});
                        plan.push_back({arrivalTime, dest, ""});
                        return true;
                    }
                }
                return false;
            }
            //one-transfer route
            for(size_t i=0; i<Indexer->RouteCount(); i++){
                auto route2 = Indexer->SortedRouteByIndex(i);
                size_t destIndex = route2->FindStopIndex(dest);
                if(destIndex == std::numeric_limits<size_t>::max()){
                    continue;;
                }
                for(size_t j = 0; j<route2->StopCount(); j++){
                    TStopID transfer = route2->GetStopID(j);
                    if(transfer == src || transfer == dest){
                        continue;
                    }
                    std::unordered_set<std::string> connectingRoutes;
                    if(!Indexer->RoutesByStopIDs(src, transfer, connectingRoutes)){
                        continue;
                    }
                    for(const auto &rname1:connectingRoutes){
                        auto route1 = Indexer->RouteByName(rname1);
                        size_t srcIndex1 = route1->FindStopIndex(src);
                        size_t transferIndex1 = route1->FindStopIndex(transfer);
                        size_t transferIndex2 = route2->FindStopIndex(transfer);
                        if(srcIndex1 == std::numeric_limits<size_t>::max() || transferIndex1 == std::numeric_limits<size_t>::max() || transferIndex2 == std::numeric_limits<size_t>::max()){
                            continue;
                        }
                        for(size_t t2=0; t2<route2->TripCount(); t2++){
                            auto arrivalTime = route2->GetStopTime(destIndex, t2);
                            if(arrivalTime.to_duration() > arriveby.to_duration()){
                                continue;
                            }
                            auto departTransfer = route2->GetStopTime(transferIndex2, t2);
                            for(size_t t1 = 0; t1<route1->TripCount(); t1++){
                                auto arriveTransfer = route1->GetStopTime(transferIndex1, t1);
                                auto departSrc = route1->GetStopTime(srcIndex1, t1);
                                if(arriveTransfer.to_duration() <= departTransfer.to_duration()){
                                    plan.push_back({departSrc, src, route1->Name()});
                                    plan.push_back({departTransfer, transfer, route2->Name()});
                                    plan.push_back({arrivalTime, dest, ""});
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }        
};


CTripPlanner::CTripPlanner(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CTripPlanner::~CTripPlanner(){

}

std::shared_ptr<CBusSystemIndexer> CTripPlanner::BusSystemIndexer() const{
    return DImplementation->BusSystemIndexer();
}

std::shared_ptr< CTripPlanner::SRoute > CTripPlanner::FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const{
    return DImplementation->FindDirectRouteLeaveTime(src,dest,leaveat);
}

std::shared_ptr< CTripPlanner::SRoute > CTripPlanner::FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const{
    return DImplementation->FindDirectRouteArrivalTime(src,dest,arriveby);
}

bool CTripPlanner::FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const{
    return DImplementation->FindRouteLeaveTime(src,dest,leaveat,plan);
}

bool CTripPlanner::FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const{
    return DImplementation->FindRouteArrivalTime(src,dest,arriveby,plan);
}

