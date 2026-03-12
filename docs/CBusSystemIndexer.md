# CBusSystemIndexer
## Overview
- CBusSystemIndexer is an indexer that indexes the CBusSystem for easy lookup of stops and routes
- It returns SRouteIndexer

## Nested Interfaces
### SStop
- represents a bus stop
#### Methods
##### TStopID ID() const noexcept
- returns ID of the stop

##### CStreetMap::TNodeID NodeID() const noexcept
- returns street map node ID associated with the stop
- connects stop to street map graph

##### std::string Description() const noexcept
- returns stop description

### SRouteIndexer
- represents an indexed bus route
#### Methods
##### std::string Name() const noexcept
- returns name of route

##### std::size_t StopCount() const noexcept
- returns number of stops in the route

##### std::size_t TripCount() const noexcept
- returns the number of trips for this route

##### CBusSystem::TStopID GetStopID(std::size_t index) const noexcept
- paramter
    - std::size_t index -> specified index to get stopID
- returns the stopID at the given index or InvalidStopID if invalid

##### CBusSystem::TStopTime GetStopTime(std::size_t stopIndex, std::size_t tripIndex) const noexcept
- parameters
    - std::size_t stopIndex -> specified stop index to get stop time
    - std::size_t tripIndex -> specified trip index to get stop time
- returns stop time given a stop and trip index

##### size_t FindStopIndex(CBusSystem::TStopID stopid, size_t start = 0) const
- parameters
    - TStopID stopid -> stopid you want to search for
    - size_t start -> the position you want to start searching
- returns the index of a stop ID, starting from start and max() if not found

##### std::vector<CBusSystem::TStopID> StopIDsSourceDestination(CBusSystem::TStopID src, CBusSystem::TStopID dest)const
- parameters
    - TStopID src -> start of the first stop ID
    - TStopID dest -> end of the last stop ID
- returns all stop IDs along this route between src and dest, inclusive

##### void AddStop(CBusSystem::TStopID stopid)
- parameter
    - TStopID stopid -> the stop id to add to the route
- adds a stop ID to the route

## Main Methods
### std::size_t StopCount() const noexcept
- returns total number of stops in system

### std::size_t RouteCount() const noexcept
- returns total number of routes in system

### std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept
- parameter
    - std::size_t index -> specified index to stop
- returns stop by its sorted index and nullptr if invalid

### std::shared_ptr<SRouteIndexer> SortedRouteByIndex(std::size_t index) const noexcept
- parameter
    - std::size_t index -> index to return route
- returns route by its sorted index or nullptr if index invalid

### std::shared_ptr<SRouteIndexer> RouteByName(const std::string &name) const noexcept
- parameter:
    - const std::string &name -> reference to name to get route
- returns route with name and nullptr if name invalid

### bool RoutesByStopID(TStopID stopid, std::unordered_set<std::string> &routes) const noexcept
- parameters:
    - TStopID stopid -> the stop ID you want to see routes
    - std::unordered_set<std::string> &routes -> the routes for that stop ID
- returns all routes that include the specified stopID

### bool RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_set<std::string> &routes) const noexcept
- parameters:
    - TStopID src -> start of the stops
    - TStopID dest -> end of the stops
    - std::unordered_set<std::string> &routes -> the routes for the stop ID
- returns all routes that include a sequence of stops from src to dest

### bool StopIDsByRoutes(const std::string &route1, const std::string &route2, std::unordered_set<TStopID> &stops) const noexcept
- parameters
    - const std::string &route1 -> name of the first bus route you want to compare
    - const std::string &route2 -> name of the second bus route you want to compare against the first
    - std::unordered_set<TStopID> &stops -> output parameter that stores all stopIDs on both route1 and route2
- returns all stop IDs that are common between two routes
## Example
### Accessing Sorted Stops
auto Indexer = std::make_shared<CBusSystemIndexer>(BusSystem);
for(size_t i = 0; i<Indexer->StopCount(); i++){
    auto Stop = Indexer->SortedStopByIndex(i);
    std::cout <<Stop->Description()<< std::endl;
}
### Accessing Routes
auto Route = Indexer->RouteByName("Name");
if(Route){
    for(std::size_t i=0; i < Route->StopCount(); i++){
        std::cout << Route->GetStopID(i) << std::endl;
    }
}
### Routes by Stop
std::unordered_set<std::string> Routes;
if(Indexer->RoutesByStopID(28, Routes)){
    for(auto &name:Routes){
        std::cout << name << std::endl;
    }
}
### Stop IDs Between Two Stops on a Route
auto Route = Indexer->RouteByName("F");
auto StopsSeq = Route->StopIDsSourceDestination(28, 82);
for(auto stopid : StopsSeq){
    std::cout << stopid << std::endl;
}