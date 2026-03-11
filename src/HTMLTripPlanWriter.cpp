#include "HTMLTripPlanWriter.h"

struct CHTMLTripPlanWriter::SImplementation{
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
        if (!sink){
            return false;
        }
        std::string html = "<html><body><ul>\n";
        sink->Write(std::vector<char>(html.begin(), html.end()));
        for (const auto &step : plan){
            std::string line = "<li>step</li>\n";
            sink->Write(std::vector<char>(line.begin(), line.end()));
        }
        std::string end = "</ul></body></html>\n";
        sink->Write(std::vector<char>(end.begin(), end.end()));
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

