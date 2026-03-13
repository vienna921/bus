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
    //draw circles
        void drawCircle(CSVGWriter &writer, const SSVGPoint &pt, double radius, const std::string &color){
            TAttributes style;
            style.push_back({"fill", color});
            style.push_back({"stroke", "black"});
            style.push_back({"stroke-width", "1"});
            writer.Circle(pt, radius, style);
        }
    
    // converts geo coordinates to SVG pixel point
        SSVGPoint toPoint(double lon, double lat, double minLon, double maxLat, double lonRange, double latRange, int margin, int drawW, int drawH){
            SSVGPoint pt;
            pt.DX = margin + (lon-minLon) / lonRange * drawW;
            pt.DY = margin + (maxLat - lat) / latRange * drawH;
            return pt;
        }
    // nodePoint
        std::shared_ptr<CStreetMap::SNode> nodePoint(CStreetMap::TNodeID id){
            return DStreetMap->NodeByID(id);
        }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if (!sink || plan.size() < 2){
            return false;
        }
        auto cfg = std::dynamic_pointer_cast<SimpleConfig>(DConfig);
        //load SVG config
        int width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::SVGWidth));
        int height = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::SVGHeight));
        int margin = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::SVGMarginPixels));
        
        int drawW = width - 2*margin;
        int drawH = height - 2*margin;

        // bounding box
        double minLon = 1e18, maxLon = -1e18;
        double minLat = 1e18, maxLat = -1e18;
        for(size_t i = 0; i<DStreetMap->NodeCount(); i++){
            auto node = DStreetMap->NodeByIndex(i);
            if(!node){
                continue;
            }
            auto loc = node->Location();
            if(loc.DLongitude<minLon){
                minLon = loc.DLongitude;
            }
            if(loc.DLongitude>maxLon){
                maxLon = loc.DLongitude;
            }
            if(loc.DLatitude<minLat){
                minLat = loc.DLatitude;
            }
            if(loc.DLatitude>maxLat){
                maxLat = loc.DLatitude;
            }
        }

        double lonRange = maxLon - minLon ? maxLon-minLon : 1;
        double latRange = maxLat-minLat ? maxLat - minLat : 1;
        int drawW = width - 2 * margin;
        int drawH = height - 2 * margin;
    
        //write opening <svg> tag'
        std::string svgHeader = "<svg width='" + std::to_string(width) + "' height='" + std::to_string(height) + "' xmlns='http://www.w3.org/2000/svg'>\n";
        std::vector<char> buffer(svgHeader.begin(), svgHeader.end());
        sink->Write(buffer);
        CSVGWriter writer(sink,width, height);
        
        for(size_t i = 0; i<plan.size(); i++){
            auto stop = DBusSystem->StopByID(plan[i].DStopID);
            if(!stop){
                continue;
            }
            if(stop->Description().empty()){
                auto node = DStreetMap->NodeByID(stop->NodeID());
                if(node && node->HasAttribute("name")){
                    stop->Description(node->GetAttribute("name"));
                }
            }
        }
  

        // config values
        std::string busColor0 = std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::BusColor0));
        std::string busColor1 = std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::BusColor1));
        int busStroke = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::BusStroke));
        double stopRadius = std::any_cast<double>(cfg->GetOption(CSVGTripPlanWriter::BusStopRadius));
        std::string srcColor = std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::SourceColor));
        double srcRadius = std::any_cast<double>(cfg->GetOption(CSVGTripPlanWriter::SourceRadius));
        std::string dstColor = std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::DestinationColor));
        double dstRadius = std::any_cast<double>(cfg->GetOption(CSVGTripPlanWriter::DestinationRadius));


        // draw street map lines
        
        for(size_t i = 0; i<DStreetMap->WayCount(); i++){
            auto way = DStreetMap->WayByIndex(i);
             std::string type = way->GetAttribute("highway");
                if(type.empty()){
                    continue;
                }
            for(size_t j = 0; j+ 1<way->NodeCount(); j++){
                auto n1 = DStreetMap->NodeByID(way->GetNodeID(j));
                auto n2 = DStreetMap->NodeByID(way->GetNodeID(j+1));
                if(!n1 || !n2){
                    continue;
                }
                auto l1 = n1->Location();
                auto l2 = n2->Location();

                auto p1 = toPoint(l1.DLongitude, l1.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
                auto p2 = toPoint(l2.DLongitude, l2.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
            
                int width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::ResidentialStroke));
                if(type=="motorway"){
                     width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::MotorwayStroke));
                }
                else if(type == "primary"){
                     width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::PrimaryStroke));
                }
                else if(type == "secondary"){
                     width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::SecondaryStroke));
                }
                else if(type == "tertiary"){
                     width = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::TertiaryStroke));
                }

                TAttributes streetStyle;
                streetStyle.push_back({"stroke", std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::StreetColor))});
                streetStyle.push_back({"stroke-width", std::to_string(width)});
                streetStyle.push_back({"fill", "none"});

                writer.Line(p1, p2, streetStyle);
            }
        }
        // draw bus lines
        for(size_t i = 0; i + 1 < plan.size(); i++){
            auto fromStop = DBusSystem->StopByID(plan[i].DStopID);
            auto toStop = DBusSystem->StopByID(plan[i+1].DStopID);
            if(!fromStop || !toStop){
                continue;
            }
            std::string busColor = plan[i].DRouteName.empty() ? busColor1 : busColor0;
            TAttributes busStyle;
            busStyle.push_back({"stroke", busColor});
            busStyle.push_back({"stroke-width", std::to_string(busStroke)});
            busStyle.push_back({"fill","none"});
            if(!plan[i].DRouteName.empty() && plan[i].DRouteName == plan[i+1].DRouteName){
                auto path = DBusSystem->PathByStopIDs(plan[i].DStopID, plan[i+1].DStopID);
                if(path && path->NodeCount() >= 2){
                    for(size_t j = 0; j+1<path->NodeCount(); j++){
                        auto n1 = DStreetMap->NodeByID(path->GetNodeID(j));
                        auto n2 = DStreetMap->NodeByID(path->GetNodeID(j+1));
                        if(!n1 || !n2){
                            continue;
                        }
                        auto l1 = n1->Location();
                        auto l2 = n2->Location();
                        SSVGPoint p1 = toPoint(l1.DLongitude, l1.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
                        SSVGPoint p2 = toPoint(l2.DLongitude, l2.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
                        writer.Line(p1, p2, busStyle);
                    }
                }
            }
            else{
                auto n1 = DStreetMap->NodeByID(fromStop->NodeID());
                auto n2 = DStreetMap->NodeByID(toStop->NodeID());
                if(!n1 || !n2){
                    continue;
                }
                auto l1 = n1->Location();
                auto l2 = n2->Location();
                SSVGPoint p1 = toPoint(l1.DLongitude, l1.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
                SSVGPoint p2 = toPoint(l2.DLongitude, l2.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
                writer.Line(p1, p2, busStyle);
            }
            
        }

        // draw stop circles
        for(size_t i = 0; i<plan.size(); i++){
            auto stop = DBusSystem->StopByID(plan[i].DStopID);
            if(!stop){
                continue;
            }
           
            auto node = nodePoint(stop->NodeID());
            if(!node){
                continue;
            }
            auto loc = node->Location();
            auto pt = toPoint(loc.DLongitude, loc.DLatitude, minLon, maxLat, lonRange, latRange, margin, drawW, drawH);
    
                auto stop = DBusSystem->StopByID(plan[i].DStopID);
                if(i==0){
                    drawCircle(writer, pt, srcRadius, srcColor);
                }
                else if(i+1 == plan.size()){
                    drawCircle(writer, pt, dstRadius, dstColor);
                }
                else{
                    drawCircle(writer, pt, stopRadius, plan[i].DRouteName.empty() ? busColor1 : busColor0);
                }
        

            
            std::string labelColor = std::any_cast<std::string>(cfg->GetOption(CSVGTripPlanWriter::LabelColor));
            int labelSize = std::any_cast<int>(cfg->GetOption(CSVGTripPlanWriter::LabelSize));
            std::string text = "<text x='" + std::to_string(pt.DX) + "' y='" + std::to_string(pt.DY - 10) + "' fill='" + labelColor + "' font-size='" + std::to_string(labelSize) + "'>" + stop->Description() + "</text>\n";

            std::vector<char> tbuf(text.begin(), text.end());
            sink->Write(tbuf);
        }
        std::string svgTail = "</svg>\n";
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

