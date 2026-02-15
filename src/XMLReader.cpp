#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    std::queue<SXMLEntity> DEntityQueue;
    bool ParsingDone = false;
};
// constructor
CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src): DSource(src), DParserInitialized(false), DImplementation(std::make_unique<SImplementation>()){
    // creates new XML parser
    DParser = XML_ParserCreate(nullptr);
    if(DParser){
        DParserInitialized = true;
        // lets callbacks access this CXMLReader object
        XML_SetUserData(DParser, this);
        //registers static functions as event handlers
        XML_SetElementHandler(DParser, StartElement, EndElement);
        XML_SetCharacterDataHandler(DParser, CharData);
    }
}
// destructor
CXMLReader::~CXMLReader(){
    if(DParserInitialized){
        XML_ParserFree(DParser);
        DParser = nullptr;
    }
}

bool CXMLReader::End() const{
    // if there is anything in entity queue, then we aren't done
    if(!DImplementation->DEntityQueue.empty()){
        return false;
    }
    //if data source exists, ask if data source is at the end
    if(DSource){
        return DSource->End();
    }
    else{
        return true;
    }
}

// returns true if there is a next parsed XML entity and false if not or failed
// parse more XML if needed
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    // if no parsed entities waiting, then read more data and parse it
    while(DImplementation->DEntityQueue.empty()){
        // if there is no source or nothing to read, exit
        if(!DSource || DSource->End()){
            return false;
        }
        //reads raw characters to buffer
        std::vector<char> buffer(1024);
        size_t bytesRead = 0;

        // reads one character at a time
        char ch;
        // stops if buffer is full or get() fails/ end of input
        while (bytesRead < buffer.size() && DSource->Get(ch)){
            buffer[bytesRead++] = ch;
        }
        // tells Expat that this is the last chunk of input
        bool isFinal = (bytesRead == 0 || DSource->End());
        // parse chunk
        // calls callbacks
        if(XML_Parse(DParser, buffer.data(), static_cast<int>(bytesRead), isFinal)==XML_STATUS_ERROR){
           // if the parsing fails but entites produced, return them, else hard failure
            if(!DImplementation->DEntityQueue.empty()){
                break;
            }
            return false;
        }
        // nothing read or produced then return
        if(bytesRead == 0 && DImplementation->DEntityQueue.empty()){
            return false;
        }
    }
    // queue is not empty
    // take the next parsed entity
    entity = DImplementation->DEntityQueue.front();
    // remove from queue
    DImplementation->DEntityQueue.pop();    
        
    // ignore CharData
    // fetcht eh next entity
    if(skipcdata && entity.DType == SXMLEntity::EType::CharData){
        return ReadEntity(entity, skipcdata);
    }

    return true;
} 

// expat calls start element whenever it sees a start tag <>
// userData points to CXMLReader object
// XML_Char *name is the tag name
// XML_Char **atts is the attribute list
void CXMLReader::StartElement(void *userData, const XML_Char *name, const XML_Char **atts){
    // recovers the CXMLReader objcet
    auto reader = static_cast<CXMLReader *>(userData);
    // create new XML entity
    SXMLEntity entity;
    // marked as start element
    entity.DType = SXMLEntity::EType::StartElement;
    //store tagged name
    if(name){
        entity.DNameData = name;
    }
    else{
        entity.DNameData = "";
    }

    //parse attributes

    if(atts){
        for(int i = 0; atts[i]; i+=2){
        entity.SetAttribute(atts[i], atts[i+1]);
    }
    }
    //push into queue
    reader->DImplementation->DEntityQueue.push(entity);
}

//expat calls when it sees end tag </tag>
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

// expat calls when it sees text content
void CXMLReader::CharData(void *userData, const XML_Char *s, int len){
    // ignore empty data
    if(len <= 0) return;
    // recover CXMLReader
    auto reader = static_cast<CXMLReader *>(userData);

    // if the last thing we pushed was a character data
    // merge adjacent character data
    if(!reader->DImplementation->DEntityQueue.empty()&&reader->DImplementation->DEntityQueue.back().DType == SXMLEntity::EType::CharData){
        reader->DImplementation->DEntityQueue.back().DNameData.append(s, len);
    }
    // if not create new CharData entity
    else{
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::CharData;
        entity.DNameData.assign(s, len);
        reader->DImplementation->DEntityQueue.push(entity);
    }
}
