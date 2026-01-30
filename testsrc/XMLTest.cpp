#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReaderTest, SimpleTest){
    std::string xml = R"(<tagname attr1="val1"></tagname>)";
    std::shared_ptr<CDataSource> source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tagname");
    EXPECT_TRUE(entity.AttributeExists("attr1"));
    EXPECT_EQ(entity.AttributeValue("attr1"), "val1");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tagname");
    EXPECT_TRUE(reader.End());
}

TEST(XMLReaderTest, ElementTest){
    std::string xml = "<parent><child>text</child></parent>";
    std::shared_ptr<CDataSource> source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "parent");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "child");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "text");
    
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "child");
    
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "parent");

    EXPECT_TRUE(reader.End());
}

TEST(XMLReaderTest, CDataTest){
    std::string xml = "<tag>THIS IS THE CDATA</tag>";
    std::shared_ptr<CDataSource> source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "THIS IS THE CDATA");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);

    EXPECT_TRUE(reader.End());
}

TEST(XMLReaderTest, LongCDataTest){
    std::string longText(600, 'x');
    std::string xml = "<tag>" + longText + "</tag>";
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData.size(), 600);
    for(std::size_t i=0; i<entity.DNameData.size(); i++){
        EXPECT_EQ(entity.DNameData[i], 'x');
    }

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);

    EXPECT_TRUE(reader.End());
}

TEST(XMLReaderTest, SpecialCharacterTest){
    std::string xml = "<tag>&lt;&gt;&amp;&quot;&apos;</tag>";
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "<>&\"'");

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_TRUE(reader.End());
}

TEST(XMLReaderTest, InvalidXMLTest){
    std::string xml = "<tag><unclosed></tag>";
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    ASSERT_TRUE(reader.ReadEntity(entity));

    EXPECT_FALSE(reader.ReadEntity(entity));
}

TEST(XMLReaderTest, LongCharDataCrosses512Boundary){
    std::string longText(1024, 'A');
    std::string xml = "<tag>" + longText + "</tag>";
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(src);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData.size(), 1024);
    for(std::size_t i=0; i<entity.DNameData.size(); i++){
        EXPECT_EQ(entity.DNameData[i], 'A');
    }

    ASSERT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_TRUE(reader.End());
}
