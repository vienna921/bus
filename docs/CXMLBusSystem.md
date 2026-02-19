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
### std::size_t StopCount() const;
- returns the total number of stops in the system

### std::shared_ptr<SStop> StopByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index of stop if exists and nullptr if index does not exist
- returns the stop at a given index

### std::shared_ptr<SStop> StopByID(TStopID id) const;
- parameter:
    - TStopId id -> Stop ID
- returns pointer to a stop with the given stop ID or nullptr if not found

### std::size_t RouteCount() const;
- returns the number of routes in the system

### std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> specified index you want to find route with
- returns pointer to route at specified index or nullptr if not valid

### std::shared_ptr<SRoute> RouteByName(const std::string &name) const;
- parameter:
    - std::string &name -> name you want to find route with
- returns pointer to route with given name or nullptr if not valid

### std::shared_ptr<SPath> PathByIndex(std::size_t index) const;
- parameter:
    - std::size_t index -> index you want to find path with
- returns path at given index

###  std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const;
- parameters:
    - TStopID start -> starting bus stop ID
    - TStopID end -> destination bus stop ID
- returns a pointer to SPath structure of the route from start to end stop and nullptr if not valid

## Private Methods
### void ParseStops(std::shared_ptr<CXMLReader> systemsource);
- parameter:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
- parses <stops> section in XML
- calls ParseStop() for each <stop> tag until </stops>
- stores stops in DStopsByIndex and DStopsByID

### void ParseStop(std::shared_ptr<CXMLReader> systemsource, const SXMLEntity &entity);
- parameters:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
    - const SXMLEntity &entity -> reference entity
- reads the attributes and parses <stop> element to store in containers

### void ParseRoutes(std::shared_ptr<CXMLReader> systemsource);
- parameter:
    - std::shared_ptr<CXMLReader> systemsource -> XML contianing bussystem, stops, routes
- parses <reoutes> section
- calls ParseRoute() for each <route> until </routes>
- stores routes in DRoutesByIndex and DRoutesByName

### void ParseRoute(std::shared_ptr<CXMLReader> systemsource, const SXMLEntity &entity);
- parameters:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
    - const SXMLEntity &entity -> reference entity
- parses a single <route> element
- reads <stop> child elements and adds stop IDs to route until </route>

### void ParsePaths(std::shared_ptr<CXMLReader> pathsource);
- parameter:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
- parses <paths> section
- Calls ParsePath() for each <path> until </paths>

### void ParsePath(std::shared_ptr<CXMLReader> pathsource, const SXMLEntity &entity);
- parameters:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
    - const SXMLEntity &entity -> reference entity
- parses a single <path> element
- reads <node> children and takes node IDs to be stored in SPath::DNodes

### void ParseBusSystem(std::shared_ptr<CXMLReader> systemsource);
- parameter:
    - std::shared_ptr<CXMLReader> systemsource -> XML containing bussystem, stops, routes
- parses the main bus system XML file
- validates <bussystem> and parses <stops> and <routes> if present

## Examples
### Construction
std::string BusXML = "<bussystem>\n"
                     "<stops>\n"
                     "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                     "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                     "</stops>\n"
                     "<routes>\n"
                     "   <route name=\"A\">\n"
                     "        <stop id=\"1\"/>\n"
                     "        <stop id=\"2\"/>\n"
                     "    </route>\n"
                     "</routes>\n"
                     "</bussystem>";
std::string PathXML = "<paths>\n"
                      "   <path source=\"321\" destination=\"311\">\n"
                      "      <node id=\"321\"/>\n"
                      "      <node id=\"315\"/>\n"
                      "      <node id=\"311\"/>\n"
                      "   </path>\n"
                      "</paths>"

auto BusSource = std::make_shared<CStringDataSource>(BusXML);
auto PathSource = std::make_shared<CStringDataSource>(PathXML);

auto BusReader = std::make_shared<CXMLReader>(BusSource);
auto PathReader = std::make_shared<CXMLReader>(PathSource);

CXMLBusSystem BusSystem(BusReader, PathReader);

### Access Stops
for(std::size_t i = 0; i<BusSystem.StopCount(); i++){
    auto Stop = BusSystem.StopByIndex(i);
    std::cout << Stop->DDescription << std::endl;
}

## Access Paths
auto Path = BusSystem.PathByStopIDs(1,2);

if(Path){
    for(auto Node : Path->DNodes){
        std::cout << Node << std::endl;
    }
}
