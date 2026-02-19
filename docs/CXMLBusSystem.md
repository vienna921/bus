# CXMLBusSystem
## Overview
- CXMLBusSystem class implements the CBusSystem interface
- It parses Bus system XML (Stops, Routes) and Path XML (paths between nodes)

## Constructor and Destructor
CXMLBusSystem::CXMLBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource);
- parameters: 
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
    - std::shared_ptr<CXMLReader> pathsource -> XML contain paths
- reads <bussystem>, parses <stops>, <routes>, and <paths>, and validates </bussystem>

CXMLBusSystem::~CXMLBusSystem();
- Destructor. Cleans up internal resources

## Public Methods
std::size_t StopCount() const;
- returns the total number of stops in the system

std::shared_ptr<SStop> StopByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index of stop if exists and nullptr if index does not exist
- returns the stop at a given index

std::shared_ptr<SStop> StopByID(TStopID id) const;
- parameter:
    - TStopId id -> Stop ID
- returns pointer to a stop with the given stop ID or nullptr if not found

std::size_t RouteCount() const;
- returns the number of routes in the system

std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> specified index you want to find route with
- returns pointer to route at specified index or nullptr if not valid

std::shared_ptr<SRoute> RouteByName(const std::string &name) const;
- parameter:
    - std::string &name -> name you want to find route with
- returns pointer to route with given name or nullptr if not valid

std::size_t PathCount() const;
- returns number of parsed paths

std::shared_ptr<SPath> PathByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index you want to find path with
- returns path at given index

## Private Methods
