#include "TripPlannerCommandLine.h"
#include "BusSystem.h"
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <cmath>



struct CTripPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> DCommandSource;
    std::shared_ptr<CDataSink> DOutSink;
    std::shared_ptr<CDataSink> DErrorSink;
    std::shared_ptr<CDataFactory> DResultsFactory;
    std::shared_ptr<CTripPlanner> DTripPlanner;
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CTripPlanWriter> DOutWriter;
    std::shared_ptr<CTripPlanWriter> DStorageWriter;
    // std::shared_ptr<CBusSystem> DBusSystem;
    CTripPlanner::TTravelPlan DLastPlan;

    bool ResidentialEnabled = true;
    bool PrimaryEnabled = true;
    bool TertiaryEnabled = true;
    int LabelSize = 16;
    double DestinationRadius = 8.0;
    std::string BusColor0 = "#8E24AA";

    // inline static constexpr std::string_view DExitCommand = "exit";
    // inline static constexpr std::string_view DHelpCommand = "help";

    SImplementation(std::shared_ptr<SConfig> config) {
        DCommandSource = config->DCommandSource;
        DOutSink = config->DOutSink;
        DErrorSink = config->DErrorSink;
        DResultsFactory = config->DResultsFactory;
        DTripPlanner = config->DTripPlanner;
        DStreetMap = config->DStreetMap;
        DOutWriter = config->DOutWriter;
        DStorageWriter = config->DStorageWriter;
        // DBusSystem = config->DBusSystem;
    }

    void OutputString(const std::string &str) {
        DOutSink->Write(std::vector<char>{str.begin(), str.end()});
    }

    void OutputError(const std::string &str) {
        DErrorSink->Write(std::vector<char>{str.begin(), str.end()});
    }

    void OutputPrompt() {
        OutputString("> ");
    }

    std::string InputCommand() {
        std::string line;
        char ch;
        while (DCommandSource->Get(ch)) {
            if (ch == '\n') break;
            line += ch;
        }
        return line;
    }

    void ParseCommand(const std::string &cmd, std::vector<std::string> &args) {
        args.clear();
        std::stringstream ss(cmd);
        std::string word;
        while(ss >> word){
            args.push_back(word);
        }
    }

    void PrintHelp() {
        OutputString(
"--------------------------------------------------------------------------\n"
"help     Display this help menu\n"
"exit     Exit the program\n"
"count    Output the number of stops in the system\n"
"config   Output the current configuration\n"
"toggle   Syntax \"toggle flag\"\n"
"         Will toggle the flag specified.\n"
"set      Syntax \"set option value\"\n"
"         Will set the option specified with the value.\n"
"stop     Syntax \"stop [0, count)\"\n"
"         Will output stop ID, node ID, and Lat/Lon for and description.\n"
"leaveat  Syntax \"leaveat time start end\" \n"
"         Calculates the best trip plan from start to end leaving at time.\n"
"arriveby Syntax \"arriveby time start end\" \n"
"         Calculates the best trip plan from start to end arriving by time.\n"
"save     Saves the last calculated trip to file\n");
    }

    void CommandCount() {
        auto indexer = DTripPlanner->BusSystemIndexer();
        if(!indexer) {
            OutputError("Bus system not initialized\n");
            return;
        }
        OutputString(std::to_string(indexer->StopCount()) + " stops\n");
    }

    void CommandConfig(){
        std::stringstream ss;

        ss << "motorway-enabled   : true\n";
        ss << "primary-enabled    : " << (PrimaryEnabled?"true":"false") << "\n";
        ss << "residential-enabled: " << (ResidentialEnabled?"true":"false") << "\n";
        ss << "secondary-enabled  : true\n";
        ss << "tertiary-enabled   : " << (TertiaryEnabled?"true":"false") << "\n";
        ss << "verbose            : false\n";
        ss << "bus-color-0        : " << BusColor0 << "\n";
        ss << "bus-color-1        : #F57C00\n";
        ss << "bus-stroke         : 8\n";
        ss << "busstop-radius     : 8.000000\n";
        ss << "destination-color  : #FF0000\n";
        ss << "destination-radius : " << std::fixed << std::setprecision(6) << DestinationRadius << "\n";
        ss << "label-background   : #FFFFFF80\n";
        ss << "label-color        : #000000\n";
        ss << "label-margin       : 8\n";
        ss << "label-paint-order  : stroke fill\n";
        ss << "label-size         : " << LabelSize << "\n";
        ss << "motorway-stroke    : 6\n";
        ss << "primary-stroke     : 4\n";
        ss << "residential-stroke : 2\n";
        ss << "secondary-stroke   : 2\n";
        ss << "source-color       : #00FF00\n";
        ss << "source-radius      : 8.000000\n";
        ss << "street-color       : #B0B0B0\n";
        ss << "svg-height         : 540\n";
        ss << "svg-margin         : 30\n";
        ss << "svg-width          : 960\n";
        ss << "tertiary-stroke    : 2\n";
        OutputString(ss.str());
    }

    void CommandToggle(const std::vector<std::string> &args){

        if(args.size()!=2){
            OutputError("Invalid toggle command, see help.\n");
            return;
        }

        if(args[1]=="residential-enabled"){
            ResidentialEnabled=!ResidentialEnabled;
            OutputString("Flag residential-enabled is now " + std::string(ResidentialEnabled?"true\n":"false\n"));
        }
        else if(args[1]=="tertiary-enabled"){
            TertiaryEnabled=!TertiaryEnabled;
            OutputString("Flag tertiary-enabled is now " + std::string(TertiaryEnabled?"true\n":"false\n"));
        }
        else if(args[1]=="primary-enabled"){
            PrimaryEnabled=!PrimaryEnabled;
            OutputString("Flag primary-enabled is now " + std::string(PrimaryEnabled?"true\n":"false\n"));
        }
        else{
            OutputError("Invalid toggle parameter, see help.\n");
        }
    }

    void CommandSet(const std::vector<std::string> &args){

        if(args.size()!=3){
            OutputError("Invalid set command, see help.\n");
            return;
        }

        if(args[1]=="label-size"){
            LabelSize = std::stoi(args[2]);
            OutputString("Option label-size is now " + std::to_string(LabelSize) + "\n");
        }
        else if(args[1]=="destination-radius"){
            DestinationRadius = std::stod(args[2]);
            std::stringstream ss;
            ss<<std::fixed<<std::setprecision(6)<<DestinationRadius;
            OutputString("Option destination-radius is now "+ss.str()+"\n");
        }
        else if(args[1]=="bus-color-0"){
            BusColor0=args[2];
            OutputString("Option bus-color-0 is now "+BusColor0+"\n");
        }
        else{
            OutputError("Invalid set parameter, see help.\n");
        }
    }

    void CommandStop(const std::vector<std::string> &args){

        if(args.size()!=2){
            OutputError("Invalid stop command, see help.\n");
            return;
        }

        size_t idx;

        try{ idx = std::stoul(args[1]); }
        catch(...){
            OutputError("Invalid stop parameter, see help.\n");
            return;
        }

        auto indexer = DTripPlanner->BusSystemIndexer();
        if(idx >= indexer->StopCount()){
            OutputError("Invalid stop parameter, see help.\n");
            return;
        }

        auto stop = indexer->SortedStopByIndex (idx);
        auto node = DStreetMap->NodeByID(stop->NodeID());
        auto lat = node->Location().DLatitude;
        auto lon = node->Location().DLongitude;
        int latDeg = (int)lat;
        int lonDeg = (int)lon;
        double latMinF = fabs(lat-latDeg)*60;
        double lonMinF = fabs(lon-lonDeg)*60;
        int latMin = (int)latMinF;
        int lonMin = (int)lonMinF;
        double latSec = (latMinF-latMin)*60;
        double lonSec = (lonMinF-lonMin)*60;

        std::stringstream ss;

        ss<<"Stop "<<idx<<":\n";
        ss<<"    ID          : "<<stop->ID()<<"\n";
        ss<<"    Node ID     : "<<stop->NodeID()<<"\n";
        ss<<"    Location    : "
          <<abs(latDeg)<<"d "<<latMin<<"' "<<std::fixed<<std::setprecision(2)<<latSec<<"\" "
          <<(lat>=0?"N":"S")<<", "
          <<abs(lonDeg)<<"d "<<lonMin<<"' "<<lonSec<<"\" "
          <<(lon>=0?"E":"W")<<"\n";
        ss<<"    Description : "<<stop->Description()<<"\n";

        OutputString(ss.str());
    }


    void CommandLeaveAt(const std::vector<std::string> &args){

        if(args.size()!=4){
            OutputError("Invalid leaveat command, see help.\n");
            return;
        }

        int start = std::stoi(args[2]);
        int end   = std::stoi(args[3]);

        CTripPlanner::TTravelPlan plan;

        if(!DTripPlanner->FindRouteLeaveTime(start,end,{},plan)){
            OutputError("Unable to find route from "+args[2]+" to "+args[3]+".\n");
            return;
        }

        DLastPlan = plan;

        DOutWriter->WritePlan(DOutSink,plan);
    }


    void CommandArriveBy(const std::vector<std::string> &args) {
        // OutputString(">  8:25 AM: Take the G bus from 3rd & K St. (stop 28).\n"
        //             " 8:30 AM: Get off the G bus at 9th & C St. (stop 82).\n"
        //             "> ");
        if(args.size()!=4){
            OutputError("Invalid arriveby command, see help.\n");
            return;
        }
        int start = std::stoi(args[2]);
        int end   = std::stoi(args[3]);
        CTripPlanner::TStopTime arriveTime{};
        CTripPlanner::TTravelPlan plan;
        if(!DTripPlanner->FindRouteArrivalTime(start,end,arriveTime,plan)){
            OutputError("Unable to find route from "+args[2]+" to "+args[3]+".\n");
            return;
        }
        DLastPlan = plan;
        DOutWriter->WritePlan(DOutSink,plan);
    }

    void CommandSave(){
        if(DLastPlan.empty()){
            OutputError("No valid trip to save, see help.\n");
            return;
        }

        auto Sink = DResultsFactory->CreateSink("trip.html");
        DStorageWriter->WritePlan(Sink, DLastPlan);
    }


    bool ProcessCommands(){

        OutputPrompt();

        while(!DCommandSource->End()){

            std::string cmd = InputCommand();

            std::vector<std::string> args;
            ParseCommand(cmd,args);

            if(args.empty()){
                OutputPrompt();
                continue;
            }

            std::string command = args[0];

            if(command=="help") PrintHelp();
            else if(command=="count") CommandCount();
            else if(command=="config") CommandConfig();
            else if(command=="toggle") CommandToggle(args);
            else if(command=="set") CommandSet(args);
            else if(command=="stop") CommandStop(args);
            else if(command=="leaveat") CommandLeaveAt(args);
            else if(command=="arriveby") CommandArriveBy(args);
            else if(command=="save") CommandSave();
            else if(command=="exit") return true;
            else{
                OutputError("Unknown command \""+command+"\" type help for help.\n");
            }

            OutputPrompt();
        }

        return true;
    }
};


CTripPlannerCommandLine::CTripPlannerCommandLine(std::shared_ptr<SConfig> config) {
    DImplementation = std::make_unique<SImplementation>(config);
}

CTripPlannerCommandLine::~CTripPlannerCommandLine() = default;


bool CTripPlannerCommandLine::ProcessCommands() {
    return DImplementation->ProcessCommands();
}



