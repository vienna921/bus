#include "HTMLTripPlanWriter.h"

struct CHTMLTripPlanWriter::SImplementation{
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
        sink->Write("<html><body><ul>\n");
        for (const auto &step : plan){
            std::string line = "<li>" + step.Description() + "</li>\n";
            sink->Write(line);
        }
        sink->Write("</ul></body></html>\n");
        return true;
    }
};



CHTMLTripPlanWriter::CHTMLTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(streetmap,bussystem);
}

CHTMLTripPlanWriter::~CHTMLTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CHTMLTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CHTMLTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

