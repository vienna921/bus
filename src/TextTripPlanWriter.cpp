#include "TextTripPlanWriter.h"
struct SimpleConfig : public CTripPlanWriter::SConfig{
    bool VerboseEnabled = false;
    bool FlagEnabled(std::string_view flag) const override{
        if(flag == CTextTripPlanWriter::Verbose){
            return VerboseEnabled;
        }
        return false;
    }

    void EnableFlag(std::string_view flag) override{
        if(flag == CTextTripPlanWriter::Verbose){
            VerboseEnabled = true;
        }
    }

    void DisableFlag(std::string_view flag) override{
        if(flag == CTextTripPlanWriter::Verbose){
            VerboseEnabled = false;
        }
    }
    
    std::any GetOption(std::string_view) const override{
        return std::any();
    }

    std::unordered_set<std::string> ValidFlags() const override{
        return {};
    }

    EOptionType GetOptionType(std::string_view) const override{
        return EOptionType::None;
    }
    
    void SetOption(std::string_view, int) override{}
    void SetOption(std::string_view, double) override{}
    void SetOption(std::string_view, const std::string &) override{}
    void ClearOption(std::string_view) override{}
    std::unordered_set<std::string> ValidOptions() const override{
        return{};
    }
};

struct CTextTripPlanWriter::SImplementation{
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;

    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = bussystem;
        DConfig = std::make_shared<SimpleConfig>();
    }

    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if(!sink){
            return false;
        }
        for(size_t i = 0; i<plan.size(); i++){
            auto Step = plan[i];
            auto Stop = DBusSystem->StopByID(Step.DStopID);
            std::string StopName = Stop->Description();

            int Hour = Step.DTime.hours().count();
            int Minute = Step.DTime.minutes().count() %60;
            
            std::string AMPM = "AM";
            if(Hour >= 12){
                AMPM = "PM";
            }
            if(Hour > 12){
                Hour -= 12;
            }
            if(Hour == 0){
                Hour = 12;
            }

            std::string TimeString = std::to_string(Hour) + ":";
            if(Minute<10){
                TimeString += "0";
            }
            TimeString += std::to_string(Minute) + " " + AMPM;
           
            if(DConfig->FlagEnabled(CTextTripPlanWriter::Verbose) && i>0 && Step.DRouteName == plan[i-1].DRouteName){
                std::string StayLine = " " + TimeString + ": Stay on the " + Step.DRouteName + " bus at " + StopName + " (stop " + std::to_string(Step.DStopID) + ").\n";
                std::vector<char> StayOutput(StayLine.begin(), StayLine.end());
                sink->Write(StayOutput);
                
            }
            std::string Line;
        
            if (i>0 && Step.DRouteName != "" && plan[i-1].DRouteName != Step.DRouteName){
                auto Prev = plan[i-1];
                std::string TransferLine = "        : Get off the " + Prev.DRouteName + " bus at " + StopName + " (stop " + std::to_string(Step.DStopID) + ") and wait for the " + Step.DRouteName + " bus.\n";
                std::vector<char> TransferOutput(TransferLine.begin(), TransferLine.end());
                sink->Write(TransferOutput);
            }
            if(i == 0 || Step.DRouteName != plan[i-1].DRouteName){
                if(Step.DRouteName !=""){
                    Line = " " + TimeString + ": Take the " + Step.DRouteName + " bus from " + StopName + " (stop " + std::to_string(Step.DStopID) + ").\n";
                }
                else if(i>0){
                    auto Prev = plan[i-1];
                    Line = " " + TimeString + ": Get off the " + Prev.DRouteName + " bus at " + StopName + " (stop " + std::to_string(Step.DStopID) + ").\n"; 
                }
            }
            else if(Step.DRouteName == "" && i>0){
                auto Prev = plan[i-1];
                Line = " " + TimeString + ": Get off the " + Prev.DRouteName + " bus at " + StopName + " (stop " + std::to_string(Step.DStopID) + ").\n"; 
            }
            if(!Line.empty()){
                std::vector<char> Output(Line.begin(), Line.end());
                sink->Write(Output);
            }
            
        }
        return true;
    }
};



CTextTripPlanWriter::CTextTripPlanWriter(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CTextTripPlanWriter::~CTextTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CTextTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CTextTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

