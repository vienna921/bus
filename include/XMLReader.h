#ifndef XMLREADER_H
#define XMLREADER_H

#include <memory>
#include "XMLEntity.h"
#include "DataSource.h"
#include "expat.h"

class CXMLReader{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;

        std::shared_ptr<CDataSource> DSource;

        XML_Parser DParser;
        bool DParserInitialized;

    public:
        //Constructor for XML reader, src specifies the data source
        CXMLReader(std::shared_ptr< CDataSource > src);
        //Destructor for XML Reader
        ~CXMLReader();
        // Returns true if all entities have been read from the XML
        bool End() const;
        // Returns true if the entity is successfully read if skipcdata
        // is true only element type entities will be returned
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);

        static void StartElement(void *userData, const XML_Char *name, const XML_Char **atts);
        static void EndElement(void *userData, const XML_Char *name);
        static void CharData(void *userData, const XML_Char *s, int len);
};

#endif
