# CSVGTripPlanWriter
## Overview
- CSVGTripPlanWriter is a C++ class for generating SVG representations of plans on a street map with bus routee

## Constructor
CSVGTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem);
- Parameters
    - streetmap -> shared pointer to street map containing nodes and ways
    - bussystem -> shared pointer to the bus system containing stops, routes, and paths

## Destructor
~CSVGTripPlanWriter();
- cleans up internal implementation
## Methods
### std::shared_ptr<CTripPlanWriter::SConfig> Config() const;
- returns shared pointer to configuration object used by writer
### bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan);
- parameters
    - sink -> pointer to CDataSink for writing SVG output
    - plan -> TTravelPlan containing ordered stops for a route
- returns true if SVG successfully written else false


## Exmaple usage (assuming a CStreetMap implementation)
auto streetmap = std::make_shared<CStreetMap>();
auto bussystem = std::make_shared<CBusSystem>();
CSVGTripPlanWriter writer(streetmap, bussystem);
auto config = writer.Config();

config->SetOption(CSVGTripPlanWriter::BusStopRadius, 10.0);
TTravelPlan plan = {};
auto sink = std::make-shared<CFileDataSink>("plan.svg");

if(writer.WritePlan(sink, plan)){
    std::cout<<"SVG travel plan written successfully\n";
}

