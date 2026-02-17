#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    XML_Parser DParser;
    std::queue< SXMLEntity > DQueue;
    std::shared_ptr< CDataSource > DSource;

    static void ExpatStartElement(void *data, const XML_Char *name, const XML_Char **attrs){
        SImplementation *This = (SImplementation *)data;
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::StartElement;
        for(int Index = 0; attrs[Index]; Index += 2){
            TempEntity.DAttributes.push_back({attrs[Index],attrs[Index+1]});
        }
        This->DQueue.push(TempEntity);
    }

    static void ExpatEndElement(void *data, const XML_Char *name){
        SImplementation *This = (SImplementation *)data;
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::EndElement;
        This->DQueue.push(TempEntity);
    }

    static void ExpatCharacterData(void *data, const XML_Char *s, int len){
        SImplementation *This = (SImplementation *)data;
        SXMLEntity TempEntity;
        TempEntity.DNameData = std::string(s,len);
        TempEntity.DType = SXMLEntity::EType::CharData;
        This->DQueue.push(TempEntity);
    }

    SImplementation(std::shared_ptr< CDataSource > src){
        DSource = src;
        DParser = XML_ParserCreate(nullptr);
        XML_SetStartElementHandler(DParser,ExpatStartElement);
        XML_SetEndElementHandler(DParser,ExpatEndElement);
        XML_SetCharacterDataHandler(DParser,ExpatCharacterData);
        XML_SetUserData(DParser,(void *)this);

    }

    ~SImplementation(){

    }

    bool End() const{
        return DQueue.empty() && DSource->End();
    }

    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        while(DQueue.empty()){
            std::vector<char> Buffer(512);
            if(DSource->Read(Buffer,Buffer.size())){
                XML_Parse(DParser,Buffer.data(),Buffer.size(),DSource->End());
            }
            else{
                return false;
            }
        }
        while(skipcdata && !DQueue.empty() && DQueue.front().DType == SXMLEntity::EType::CharData){
            DQueue.pop();
        }
        if(DQueue.empty()){
            return false;
        }
        entity = DQueue.front();
        DQueue.pop();
        return true;
        
    }
    

};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique< SImplementation >(src);
}

CXMLReader::~CXMLReader(){

}

bool CXMLReader::End() const{
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity,skipcdata);
}
