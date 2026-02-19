#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    std::queue<SXMLEntity> DEntityQueue;
    bool ParsingDone = false;
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src): DSource(src), DParserInitialized(false), DImplementation(std::make_unique<SImplementation>()){
    DParser = XML_ParserCreate(nullptr);
    if(DParser){
        DParserInitialized = true;
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, StartElement, EndElement);
        XML_SetCharacterDataHandler(DParser, CharData);
    }
}

CXMLReader::~CXMLReader(){
    if(DParserInitialized){
        XML_ParserFree(DParser);
        DParser = nullptr;
    }
}

bool CXMLReader::End() const{
    if(!DImplementation->DEntityQueue.empty()){
        return false;
    }
    if(DSource){
        return DSource->End();
    }
    else{
        return true;
    }
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    while(DImplementation->DEntityQueue.empty()){
        if(!DSource || DSource->End()){
            return false;
        }
        
        std::vector<char> buffer(512);
        size_t bytesRead = 0;

        char ch;
        while (bytesRead < buffer.size() && DSource->Get(ch)){
            buffer[bytesRead++] = ch;
        }

        bool isFinal = (bytesRead == 0 || DSource->End());
        if(XML_Parse(DParser, buffer.data(), static_cast<int>(bytesRead), isFinal)==XML_STATUS_ERROR){
            if(!DImplementation->DEntityQueue.empty()){
                break;
            }
            return false;
        }

        if(bytesRead == 0 && DImplementation->DEntityQueue.empty()){
            return false;
        }
    }

    entity = DImplementation->DEntityQueue.front();
    DImplementation->DEntityQueue.pop();    
        

    if(skipcdata && entity.DType == SXMLEntity::EType::CharData){
        return ReadEntity(entity, skipcdata);
    }

    return true;
} 


void CXMLReader::StartElement(void *userData, const XML_Char *name, const XML_Char **atts){
    auto reader = static_cast<CXMLReader *>(userData);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    if(name){
        entity.DNameData = name;
    }
    else{
        entity.DNameData = "";
    }


    if(atts){
        for(int i = 0; atts[i]; i+=2){
        entity.SetAttribute(atts[i], atts[i+1]);
    }
    }

    reader->DImplementation->DEntityQueue.push(entity);
}

void CXMLReader::EndElement(void *userData, const XML_Char *name){
    auto reader = static_cast<CXMLReader *>(userData);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::EndElement;
    if(name){
        entity.DNameData = name;
    }
    else{
        entity.DNameData = "";
    }

    reader->DImplementation->DEntityQueue.push(entity);
}

void CXMLReader::CharData(void *userData, const XML_Char *s, int len){
    if(len <= 0) return;
    auto reader = static_cast<CXMLReader *>(userData);



    if(!reader->DImplementation->DEntityQueue.empty()&&reader->DImplementation->DEntityQueue.back().DType == SXMLEntity::EType::CharData){
        reader->DImplementation->DEntityQueue.back().DNameData.append(s, len);
    }
    else{
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::CharData;
        entity.DNameData.assign(s, len);
        reader->DImplementation->DEntityQueue.push(entity);
    }
}
