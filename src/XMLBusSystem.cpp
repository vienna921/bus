#include "XMLBusSystem.h"
// vector container = fast index lookup
#include <vector>
// unordered map = fast ID lookup
#include <unordered_map>
#include <iostream>
using std::cout;
using std::endl;
// PIMPL pattern where SImplementation hides the real data + logi
struct CXMLBusSystem::SImplementation{
    const std::string DBusSystemTag = "bussystem";
    const std::string DStopsTag = "stops";
    const std::string DStopTag = "stop";
    const std::string DStopIDAttr = "id";
    const std::string DStopNodeAttr = "node";
    const std::string DStopDescAttr = "description";
    const std::string DRoutesTag = "routes";
    const std::string DRouteTag = "route";
    const std::string DRouteNameAttr = "name";
    const std::string DPathTag = "path";
    const std::string DPathSourceAttr = "source";
    const std::string DPathDestAttr = "destination";
    const std::string DNodeTag = "node";
    const std::string DNodeIDAttr = "id";

    
    // implements abstract interface
    struct SStop : public CBusSystem::SStop{
        TStopID DID = 0;;
        CStreetMap::TNodeID DNodeID = 0;
        std::string DDescription;
        // constructor
        SStop(TStopID id, CStreetMap::TNodeID nodeid, const std::string &description){
            DID = id;
            DNodeID = nodeid;
            DDescription = description;
        }
        // destructor
        ~SStop(){};
        // Bus System Stop member functions
        // Returns the stop id of the stop
        TStopID ID() const noexcept override{
            return DID;
        }
        // Returns the node id of the bus stop
        CStreetMap::TNodeID NodeID() const noexcept override{
            return DNodeID;  
        }

        std::string Description() const noexcept override{
            return DDescription;
        }

        std::string Description(const std::string &description) noexcept override{
            DDescription = description;
            return DDescription;
        }
    };  
    
    struct SRoute : public CBusSystem::SRoute{
        std::string DName;
        std::vector<CBusSystem::TStopID> DStopsIDs;

        SRoute(const std::string &name = "") : DName(name){

        }
        ~SRoute() override {

        }

        std::string Name() const noexcept override{
            return DName;
        }
        std::size_t StopCount() const noexcept override{
            return DStopsIDs.size();
        }
        CBusSystem::TStopID GetStopID(std::size_t index) const noexcept override{
            if(index < DStopsIDs.size()){
                return DStopsIDs[index];
            }
            return CBusSystem::InvalidStopID;
        }
    };

    struct SPath : public CBusSystem::SPath{
        CStreetMap::TNodeID DStartNodeID = 0;
        CStreetMap::TNodeID DEndNodeID = 0;
        std::vector<CStreetMap::TNodeID> DNodes;

        //fix this
        SPath() = default;
        ~SPath() override {

        }

        CStreetMap::TNodeID StartNodeID() const noexcept override{
            return DStartNodeID;
        }
        CStreetMap::TNodeID EndNodeID() const noexcept override{
            return DEndNodeID;
        }
        std::size_t NodeCount() const noexcept override{
            return DNodes.size();
        }
        CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept override {
            if(index < DNodes.size()) {
                return DNodes[index];
            }
            return CStreetMap::InvalidNodeID;
        }
    };

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<SStop> > DStopsByIndex;
    std::unordered_map<TStopID,std::shared_ptr<SStop> > DStopsByID;
    std::vector<std::shared_ptr<SRoute> > DRoutesByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRoute>> DRoutesByName;
    std::vector<std::shared_ptr<SPath>> DPaths;

    void ParseStop(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &stop){
        // stoull mean string to unsigned long long
        TStopID StopID = std::stoull(stop.AttributeValue(DStopIDAttr));
        CStreetMap::TNodeID NodeID = std::stoull(stop.AttributeValue(DStopNodeAttr));
        // creates stop object
        auto NewStop = std::make_shared<SStop>(StopID, NodeID, stop.AttributeValue(DStopDescAttr));
        // stores for index lookup
        DStopsByIndex.push_back(NewStop);
        cout<<"DStopsByIndex "<<DStopsByIndex.size()<<endl;
        // stores for ID lookup
        DStopsByID[StopID] = NewStop;
        FindEndTag(systemsource,DStopTag);
    }

    void ParseStops(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;
        // keep reading until we find an end element name stops
        do{
            // systemsource->ReasEntity(TempEntity, true) = reads next XML piece and fill TempEntity
            // if reading next XML element fails, exit function
            if(!systemsource->ReadEntity(TempEntity,true)){
                return;
            }
            cout<<int(TempEntity.DType)<<" '"<<TempEntity.DNameData<<"'"<<endl;
            // if we found <stop>, call ParseStop
            if((TempEntity.DType == SXMLEntity::EType::StartElement) &&(TempEntity.DNameData == DStopTag)){
                ParseStop(systemsource,TempEntity);
            }

        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DStopsTag));
    }

    void ParseRoute(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &routeEntity){
        auto NewRoute = std::make_shared<SRoute>();
        NewRoute->DName = routeEntity.AttributeValue(DRouteNameAttr);

        // Parse stops inside route
        SXMLEntity StopEntity;
        do{
            if(!systemsource->ReadEntity(StopEntity, true)){
                break;
            }
            if((StopEntity.DType == SXMLEntity::EType::StartElement) && (StopEntity.DNameData == DStopTag)){
                TStopID StopID = std::stoull(StopEntity.AttributeValue(DStopIDAttr));
                NewRoute->DStopsIDs.push_back(StopID);
            }
        }while((StopEntity.DType != SXMLEntity::EType::EndElement) || (StopEntity.DNameData != DRouteTag));
        // store the route
        DRoutesByIndex.push_back(NewRoute);
        DRoutesByName[NewRoute->DName] = NewRoute;
        
    }

    void ParseRoutes(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;
        do{
            if(!systemsource->ReadEntity(TempEntity, true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DRouteTag)){
                ParseRoute(systemsource, TempEntity);
            }
        }while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DRoutesTag));
    }

    void ParsePath(std::shared_ptr<CXMLReader> pathsource,const SXMLEntity &pathEntity){
        auto NewPath = std::make_shared<SPath>();

        //read source and destination node IDs
        NewPath->DStartNodeID = std::stoull(pathEntity.AttributeValue(DPathSourceAttr));
        NewPath->DEndNodeID = std::stoull(pathEntity.AttributeValue(DPathDestAttr));

        // read node elements
        SXMLEntity NodeEntity;
        do{
            if(!pathsource->ReadEntity(NodeEntity, true)){
                break;
            }
            if(NodeEntity.DType == SXMLEntity::EType::StartElement && NodeEntity.DNameData == DNodeTag){
                CStreetMap::TNodeID nodeID = std::stoull(NodeEntity.AttributeValue(DNodeIDAttr));
                NewPath->DNodes.push_back(nodeID);
            }
        }while(!(NodeEntity.DType == SXMLEntity::EType::EndElement && NodeEntity.DNameData == DPathTag));
        
        DPaths.push_back(NewPath);
    }
    void ParsePaths(std::shared_ptr<CXMLReader> pathsource){
        SXMLEntity TempEntity;

        // find <paths> tag
        if(!FindStartTag(pathsource, "paths")){
            return;
        }
        do{
            if(!pathsource->ReadEntity(TempEntity, true)){
                break;
            }
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DPathTag){
                ParsePath(pathsource, TempEntity);
            }
        }while(!(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == "paths"));
    }

    void ParseBusSystem(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;
        if(!FindStartTag(systemsource,DBusSystemTag)){
            cout<<"Start tag bussystem not found"<<endl;
            return;
        }
        if(!FindStartTag(systemsource,DStopsTag)){
            cout<<"Start tag stop not found"<<endl;
            return;
        }
        ParseStops(systemsource);

    }

    SImplementation(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
        SXMLEntity Entity;
        // reads<bussystem>
        if(!systemsource->ReadEntity(Entity, true) || Entity.DType != SXMLEntity::EType::StartElement || Entity.DNameData != DBusSystemTag){
            return;
        }
        bool IsEndBusSystemFound = false;
        // loops until </bussystem>
        while(systemsource->ReadEntity(Entity, true)){
            if(Entity.DType == SXMLEntity::EType::StartElement){
                if(Entity.DNameData == DStopsTag){
                    // Parse <stops>
                    ParseStops(systemsource);
                }
                else if(Entity.DNameData == DRoutesTag){
                    // Parse <routes>
                    ParseRoutes(systemsource);
                }
            }
            // if </bussystem> never found then invalid xml
            else if(Entity.DType == SXMLEntity::EType::EndElement && Entity.DNameData == DBusSystemTag){
                IsEndBusSystemFound = true;
                break;
            }
        }
        if(!IsEndBusSystemFound){
            DStopsByIndex.clear();
            DStopsByID.clear();
            DRoutesByIndex.clear();
            DRoutesByName.clear();
            return;
        }
        ParsePaths(pathsource);
    }

    // returns the number of stops in the system
    std::size_t StopCount() const noexcept{
        return DStopsByIndex.size();
    }

    // returns the number of routes in the system
    std::size_t RouteCount() const noexcept{
        return DRoutesByIndex.size();
    }
    
    // returns the SStop specified by the index, nullptr is returns if index is greater than equal to StopCount()
    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept{
        if(index < DStopsByIndex.size()){
            return DStopsByIndex[index];
        }
        return nullptr;
    }
    
    // returns the SStop specified by the stop id, nullptr is returned if id is not in the stops
    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept{
        auto it = DStopsByID.find(id);
        if(it != DStopsByID.end()){
            return it->second;
        }
        return nullptr;
    }
    
    // returns the SRoute specified by the index, nullptr is returned if index is greater than equal to RouteCount()
    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept{
        if(index < DRoutesByIndex.size()){
            return DRoutesByIndex[index];
        }
        return nullptr;
    }
    
    //returns the SRoute specified by the name, nullptr is returned if name is not in the routes
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept{
        auto it = DRoutesByName.find(name);
        if(it != DRoutesByName.end()){
            return it->second;
        }
        return nullptr;
    }
    
    // returns the SPath that connects the two stops, nullptr is returned if path doesn't exist
    std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept{
        // rewrite
        for(auto &path : DPaths){
            if(path->StartNodeID() == start && path->EndNodeID() == end){
                return path;
            }
        }
        return nullptr;
    }
    
};

CXMLBusSystem::CXMLBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
    DImplementation = std::make_unique<SImplementation>(systemsource,pathsource);
}
    
CXMLBusSystem::~CXMLBusSystem(){

}
    
std::size_t CXMLBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
}
    
std::size_t CXMLBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByID(TStopID id) const noexcept{
    return DImplementation->StopByID(id);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByName(const std::string &name) const noexcept{
    return DImplementation->RouteByName(name);
}

std::shared_ptr<CBusSystem::SPath> CXMLBusSystem::PathByStopIDs(TStopID start, TStopID end) const noexcept{
    return DImplementation->PathByStopIDs(start, end);
}
