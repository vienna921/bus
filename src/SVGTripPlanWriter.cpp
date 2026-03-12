#include "SVGTripPlanWriter.h"
#include "SVGWriter.h"
#include <unordered_map>

struct SimpleConfig : public CTripPlanWriter::SConfig{
    std::unordered_set<std::string> DFlags;
    std::unordered_set<std::string> DEnabledFlags;
    std::unordered_map<std::string, std::any> DOptions;
    std::unordered_map<std::string, EOptionType> DOptionTypes;
    
    void RegInt(std::string_view k, int v){
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::Int;
    }
    void RegDbl(std::string_view k, double v){
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::Double;
    }
    void RegStr(std::string_view k, std::string v){
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::String;
    }

    SimpleConfig(){
        DFlags.insert(std::string(CSVGTripPlanWriter::MotorwayEnabled));
        DFlags.insert(std::string(CSVGTripPlanWriter::PrimaryEnabled));
        DFlags.insert(std::string(CSVGTripPlanWriter::SecondaryEnabled));
        DFlags.insert(std::string(CSVGTripPlanWriter::TertiaryEnabled));
        DFlags.insert(std::string(CSVGTripPlanWriter::ResidentialEnabled));
        DEnabledFlags.insert(std::string(CSVGTripPlanWriter::MotorwayEnabled));
        DEnabledFlags.insert(std::string(CSVGTripPlanWriter::PrimaryEnabled));
        DEnabledFlags.insert(std::string(CSVGTripPlanWriter::SecondaryEnabled));
        DEnabledFlags.insert(std::string(CSVGTripPlanWriter::TertiaryEnabled));
        DEnabledFlags.insert(std::string(CSVGTripPlanWriter::ResidentialEnabled));
        RegStr(CSVGTripPlanWriter::BusColor0,  "#8E24AA");
        RegStr(CSVGTripPlanWriter::BusColor1, "#F57C00");
        RegDbl(CSVGTripPlanWriter::BusStopRadius, 8.0);
        RegInt(CSVGTripPlanWriter::BusStroke, 8);
        RegStr(CSVGTripPlanWriter::DestinationColor, "#FF0000");
        RegDbl(CSVGTripPlanWriter::DestinationRadius, 8.0);
        RegStr(CSVGTripPlanWriter::LabelBackground, "#FFFFFF80");
        RegStr(CSVGTripPlanWriter::LabelColor, "#000000");
        RegInt(CSVGTripPlanWriter::LabelMargin, 8);
        RegStr(CSVGTripPlanWriter::LabelPaintOrder, "stroke fill");
        RegInt(CSVGTripPlanWriter::LabelSize, 16);
        RegInt(CSVGTripPlanWriter::MotorwayStroke, 6);
        RegInt(CSVGTripPlanWriter::PrimaryStroke, 4);
        RegInt(CSVGTripPlanWriter::ResidentialStroke, 2);
        RegInt(CSVGTripPlanWriter::SecondaryStroke, 2);
        RegStr(CSVGTripPlanWriter::SourceColor, "#00FF00");
        RegDbl(CSVGTripPlanWriter::SourceRadius, 8.0);
        RegStr(CSVGTripPlanWriter::StreetColor, "#B0B0B0");
        RegInt(CSVGTripPlanWriter::SVGHeight, 540);
        RegInt(CSVGTripPlanWriter::SVGMarginPixels, 30);
        RegInt(CSVGTripPlanWriter::SVGWidth, 960);
        RegInt(CSVGTripPlanWriter::TertiaryStroke, 2);
    
    }
    bool FlagEnabled(std::string_view f) const override{
        return DEnabledFlags.count(std::string(f));
    }
    void EnableFlag(std::string_view f) override{
        DEnabledFlags.insert(std::string(f));
    }
    void DisableFlag(std::string_view f) override{
        DEnabledFlags.erase(std::string(f));
    }
    std::unordered_set<std::string> ValidFlags() const override{
        return DFlags;
    }
    std::unordered_set<std::string> ValidOptions() const override{
        std::unordered_set<std::string> keys;
        for(auto it = DOptions.begin(); it != DOptions.end(); it++){
            keys.insert(it->first);
        }
        return keys;
    }
    EOptionType GetOptionType(std::string_view k) const override{
        auto it = DOptionTypes.find(std::string(k));
        return it!=DOptionTypes.end() ? it->second : EOptionType::None;
    }
    std::any GetOption(std::string_view k) const override{
        auto it = DOptions.find(std::string(k));
        return it != DOptions.end() ? it->second : std::any{};
    }
    void SetOption(std::string_view k, int v) override {
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::Int;
    }
    void SetOption(std::string_view k, double v) override{
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::Double;
    }
    void SetOption(std::string_view k, const std::string &v) override{
        DOptions[std::string(k)] = v;
        DOptionTypes[std::string(k)] = EOptionType::String;
    }
    void ClearOption(std::string_view k) override{
        DOptions.erase(std::string(k));
        DOptionTypes.erase(std::string(k));
    }
};

struct CSVGTripPlanWriter::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;

    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DConfig = std::make_shared<SimpleConfig>();
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if (!sink || plan.size() < 2){
            return false;
        }

        int width = 800;
        int height = 800;
        //write opening <svg> tag'
        std::string svgHeader = "<svg width='" + std::to_string(width) + "' height='" + std::to_string(height) + "' xmlns='http://ww.w3.org/2000/svg'>\n";
        std::vector<char> buffer(svgHeader.begin(), svgHeader.end());
        sink->Write(buffer);
        CSVGWriter writer(sink,width, height);
       
        TAttributes style;
        style.push_back({"stroke","red"});
        style.push_back({"stroke-width","2"});
        style.push_back({"fill","none"});
        // draw all lines
        for(size_t i = 0; i + 1 < plan.size(); i++){
            auto fromStop = DBusSystem->StopByID(plan[i].DStopID);
            auto toStop = DBusSystem->StopByID(plan[i+1].DStopID);
            if(!fromStop || !toStop){
                continue;
            }
            auto fromNode = DStreetMap->NodeByID(fromStop->NodeID());
            auto toNode = DStreetMap->NodeByID(toStop->NodeID());
            if (!fromNode || !toNode){
                continue;
            }
            
            // if (fromStop->Description().empty() && fromNode->HasAttribute("name")){
            //     fromStop->Description(fromNode->GetAttribute("name"));
            // }
            // if(toStop->Description().empty() && toNode->HasAttribute("name")){
            //     toStop->Description(toNode->GetAttribute("name"));
            // }
            auto fromLoc = fromNode->Location();
            auto toLoc = toNode->Location();
            SSVGPoint start{fromLoc.DLongitude, -fromLoc.DLatitude};
            SSVGPoint end{toLoc.DLongitude, -toLoc.DLatitude};
            writer.Line(start,end,style);
        }
        std::string svgTail = "</svg\n";
        std::vector<char> buff(svgTail.begin(), svgTail.end());
        sink->Write(buff);
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

