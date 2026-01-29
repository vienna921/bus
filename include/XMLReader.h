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
        CXMLReader(std::shared_ptr< CDataSource > src);
        ~CXMLReader();
        
        bool End() const;
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif
