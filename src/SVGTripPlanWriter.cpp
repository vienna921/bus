#include "SVGTripPlanWriter.h"
#include "SVGWriter.h"

struct CSVGTripPlanWriter::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;

    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return nullptr;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if (!sink || plan.size() < 2){
            return false;
        }

        int width = 800;
        int height = 800;

        CSVGWriter writer(sink,width, height);
        TAttributes style;
        style.push_back({"stroke","red"});
        style.push_back({"stroke-width","2"});
        style.push_back({"fill","none"});

        for(size_t i = 0; i + 1 < plan.size(); i++){
            auto fromStop = DBusSystem->StopByID(plan[i].DStopID);
            auto toStop = DBusSystem->StopByID(plan[i+1].DStopID);
            if(!fromStop || !toStop){
                continue;
            }
            auto fromNode = DStreetMap->NodeByIndex(fromStop->NodeID());
            auto toNode = DStreetMap->NodeByIndex(toStop->NodeID());
            if (!fromNode || !toNode){
                continue;
            }

            if (fromStop->Description().empty() && fromNode->HasAttribute("name")){
                fromStop->Description(fromNode->GetAttribute("name"));
            }
            if(toStop->Description().empty() && toNode->HasAttribute("name")){
                toStop->Description(toNode->GetAttribute("name"));
            }
            auto fromLoc = fromNode->Location();
            auto toLoc = toNode->Location();
            SSVGPoint start{fromLoc.DLongitude, -fromLoc.DLatitude};
            SSVGPoint end{toLoc.DLongitude, -toLoc.DLatitude};
            writer.Line(start,end,style);
        }
        
         
        
        return true;
    }
};



CSVGTripPlanWriter::CSVGTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(streetmap,bussystem);
}

CSVGTripPlanWriter::~CSVGTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CSVGTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CSVGTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

