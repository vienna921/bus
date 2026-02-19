# CXMLBusSystem
## Overview
- CBusSystem is an interface that shows the public transportation bus system built on top of CStreetMap
- It accesses bus stops and routes and computes paths between stops
- Serves as a base class that defines the structure of stops, routes, and path

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

##### std::string Description(const std::string &description) noexcept
- parameter:
    - const std::string &description -> reference to stop description
- sets and returns stop description

### SRoute
- represents a bus route (ordered sequence of stops)
#### Methods
##### std::string Name() const noexcept
- returns name of route

##### std::size_t StopCount() const noexcept
- returns number of stops in the route

##### TStopID GetStopID(std::size_t index) const noexcept
- parameter:
    - std::size_t index -> specified index to get stop ID
- returns stop ID at index or InvalidStopID if invalid

### SPath
- represents a path between two stops (ordered sequence of street map nodes)
#### Methods
##### CStreetMap::TNodeID StartNodeID() const noexcept
- returns starting street map node ID

##### CStreetMap::TNodeID EndNodeID() const noexcept
- returns ending street map node ID

##### std::size_t NodeCount() const noexcept
- returns number of nodes in path

##### CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept
- parameter:
    - std::size_t index -> specified index to get node ID
- returns node ID at index

## Main Methods
### std::size_t StopCount() const noexcept
- returns total number of stops in system

### std::size_t RouteCount() const noexcept
- returns total number of routes in system

### std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept
- parameter
    - std::size_t index -> specified index to stop
- returns stop at index

### std::shared_ptr<SStop> StopByID(TStopID id) const noexcept
- parameter
    - TStopID id -> specified ID
- returns stop with id and nullptr if stop invalid

### std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept
- parameter
    - std::size_t index -> index to return route
- returns route at index or nullptr if index invalid

### std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept
- parameter:
    - const std::string &name -> reference to name to get route
- returns route with name and nullptr if name invalid

### std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept
- parameters:
    - TStopID start -> starting stop ID
    - TStopID end -> destination stop ID
- returns pointer to SPath object (path between two stops) and nullptr if invalid

## Example
### Accessing Stops
std::shared_ptr<CBusSystem> BusSystem = 
for (std::size_t i=0; i< BusSystem->StopCount(); i++){
    auto Stop = BusSystem->StopByIndex(i);
    std::cout << Stop->Description() << std::endl;
}
### Accessing Routes
auto Route = BusSystem->RouteByName("Name");
if(Route){
    for(std::size_t i=0; i < Route->StopCount(); i++){
        std::cout << Route->GetStopID(i) << std::endl;
    }
}
### Computing Path
auto Path = BusSystem->PathByStopIDs(1,3);
if(Path){
    for(std::size_t i=0; i< Path->NodeCount(); i++){
        std::cout << Path->GetNodeID(i)<< std::endl;
    }
}