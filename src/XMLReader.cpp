#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src): DSource(src), DParserInitialized(false){
    DParser = XML_ParserCreate(nullptr);
    if(DParser){
        DParserInitialized = true;
        XML_SetUserData(DParser, this);
    }
}

CXMLReader::~CXMLReader(){
    if(DParserInitialized){
        XML_ParserFree(DParser);
        DParser = nullptr;
    }
}

bool CXMLReader::End() const{
    if(!DSource){
        return true;
    }
    return DSource->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    if(!DParserInitialized || !DSource){
        return false;
    }

    std::vector<char> buffer(512);

    int bytesRead = DSource->Read(buffer, sizeof(buffer));
    if(bytesRead<0){
        return false;
    }
    if(XML_Parse(DParser, buffer.data(), bytesRead, DSource->End())==XML_STATUS_ERROR){
        return false;
    }

    return bytesRead > 0;
}
