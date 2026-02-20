#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DWayTag = "way";


    struct SNode: public CStreetMap::SNode{
        const std::string DNodeIDAttr = "id";
        const std::string DNodeLatAttr = "lat";
        const std::string DNodeLonAttr = "lon";
        TNodeID DID;
        SLocation DLocation;
        // std::unordered_map<std::string,std::string> DAttributes;
        std::vector<std::pair<std::string,std::string>> DAttributes;

        SNode(const SXMLEntity &entity){
            auto NodeID = std::stoull(entity.AttributeValue(DNodeIDAttr));
            auto NodeLat = std::stod(entity.AttributeValue(DNodeLatAttr));
            auto NodeLon = std::stod(entity.AttributeValue(DNodeLonAttr));
            DID = NodeID;
            DLocation = SLocation{NodeLat,NodeLon};
            
        }
        ~SNode(){

        }

        TNodeID ID() const noexcept override{
            return DID;
        }
        
        SLocation Location() const noexcept override{
            return DLocation;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();

        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if (index < DAttributes.size()){
                return DAttributes[index].first;
            }
            // auto it = DAttributes.begin();
            // std::advance(it, index);
            // return it->first;
            return "";

        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            // return DAttributes.find(key) != DAttributes.end();
            for (const auto &attr : DAttributes){
                if (attr.first == key){
                    return true;
                }
            }
            return false;

        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            // auto it = DAttributes.find(key);
            // return it != DAttributes.end() ? it->second : "";
            for (const auto &attr : DAttributes){
                if (attr.first == key){
                    return attr.second;
                }
            }
            return "";

        }
        
    };

    struct SWay: public CStreetMap::SWay{
        TWayID DID; // way id
        std::vector<TNodeID> DNodeIDs; // ids of nodes in this way
        // std::unordered_map<std::string, std::string> DAttributes;
        std::vector<std::pair<std::string,std::string>> DAttributes;
        SWay(const SXMLEntity &entity){
            DID = std::stoull(entity.AttributeValue("id"));
        }
        ~SWay(){

        }

        TWayID ID() const noexcept override{
            return DID;

        }
        
        std::size_t NodeCount() const noexcept override{
            return DNodeIDs.size();

        }
        
        TNodeID GetNodeID(std::size_t index) const noexcept override{
            if (index < DNodeIDs.size()){
                return DNodeIDs[index];
            }
            return 0;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();

        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index <DAttributes.size()){
                return DAttributes[index].first;

            }
            return "";


        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            for (const auto &attr : DAttributes){
                if(attr.first == key){
                    return true;
                }
            }
            return false;

        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            for (const auto &attr : DAttributes){
                if (attr.first == key){
                    return attr.second;
                }
            }
            return "";

        }
        
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    std::vector<std::shared_ptr<SWay>> DWaysByIndex;
    std::unordered_map<TWayID, std::shared_ptr<SWay>> DWaysByID;

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

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        if(!FindStartTag(src,DOSMTag)){
            return false;
        }
        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NewNode = std::make_shared<SNode>(TempEntity);
                SXMLEntity Child;
                while(src->ReadEntity(Child)){
                    if(Child.DType == SXMLEntity::EType::StartElement && Child.DNameData == "tag"){
                        // NewNode->DAttributes[Child.AttributeValue("k")] = Child.AttributeValue("v");
                        NewNode->DAttributes.push_back({Child.AttributeValue("k"), Child.AttributeValue("v")});
                        FindEndTag(src, "tag");
                    }
                    else if(Child.DType == SXMLEntity::EType::EndElement && Child.DNameData == DNodeTag){
                        break;
                    }
                }
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NewNode->ID()] = NewNode;
                // FindEndTag(src,DNodeTag);
            }
            else if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                auto NewWay = std::make_shared<SWay>(TempEntity);
                SXMLEntity Child;
                while(src->ReadEntity(Child)){
                    if(Child.DType==SXMLEntity::EType::StartElement){
                        if(Child.DNameData == "nd"){
                            NewWay->DNodeIDs.push_back(std::stoull(Child.AttributeValue("ref")));
                            FindEndTag(src, "nd");
                        }
                        else if(Child.DNameData == "tag"){
                            // NewWay->DAttributes[Child.AttributeValue("k")] = Child.AttributeValue("v");
                            NewWay->DAttributes.push_back({Child.AttributeValue("k"), Child.AttributeValue("v")});
                            FindEndTag(src, "tag");
                        }
                    }
                    else if(Child.DType == SXMLEntity::EType::EndElement && Child.DNameData == DWayTag){
                        break;
                    }
                }

                DWaysByIndex.push_back(NewWay);
                DWaysByID[NewWay->ID()] = NewWay;
            }
            else if(TempEntity.DType == SXMLEntity::EType::EndElement &&TempEntity.DNameData == DOSMTag){
                break;
            }
        }
        return true;

    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOSM(src);

    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if(index < DNodesByIndex.size()){
            return DNodesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto Search = DNodesByID.find(id);
        if(Search != DNodesByID.end()){
            return Search->second;
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if (index < DWaysByIndex.size()){
            return DWaysByIndex[index];
        }
        return nullptr;
        
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        auto Search = DWaysByID.find(id);
        if(Search != DWaysByID.end()){
            return Search->second;
        }
        return nullptr;
        
    }

};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}

