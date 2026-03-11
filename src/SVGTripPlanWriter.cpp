#include "SVGTripPlanWriter.h"
#include "SVGWriter.h"

struct CSVGTripPlanWriter::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;

    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DConfig = std::make_shared<SConfig>();
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if (!sink){
            return false;
        }

        int width = 800;
        int height = 800;

        CSVGWriter writer(sink, width, height);
        TAttributes style;
        style["stroke"] = "red";
        style["stroke-width"] = "2";
        style["fill"] = "none";

        for(size_t i = 0; i + 1 < plan.size(); i++){
            auto from = plan[i].Location();
            auto to = plan[i+1].Location();
            SSVGPoint start;
            start.DX = from.DLongitude;
            start.DY = from.DLatitude;
            SSVGPoint end;
            end.DX = to.DLongitude;
            end. DY = to.DLatitude;
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

