#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

/*
wha needs to be supported:
map level
- NodeCount()
- WayCount()
- NodeByIndex()
- NodeByID()
- WayByIndex()
- WayByID()

node level:
- ID()
- Location()
- AttributeCount()
- GetAttributeKey()
- HasAttribute()
- GetAttribute()

way level:
- ID()
- NodeCount()
- GetNodeID()
- AttributeCount()
- GetAttributeKey()
- HasAttribute()
- GetAttribute()

*/

TEST(OpenStreetMapTest, SimpleTest){
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    ASSERT_EQ(OpenStreetMap.NodeCount(), 2);
    auto Node = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),1);
    auto Location = CStreetMap::SLocation{38.5,-121.7};
    EXPECT_EQ(Node->Location(),Location);
    ASSERT_EQ(OpenStreetMap.WayCount(), 1);
    EXPECT_NE(OpenStreetMap.NodeByID(1), nullptr); //checks valid id
    EXPECT_EQ(OpenStreetMap.NodeByID(999), nullptr);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(10), nullptr); // should return null ptr 
    EXPECT_EQ(OpenStreetMap.WayByID(999),nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(10),nullptr);

}

TEST(OpenStreetMapTest, NodeAttributesTest){
    auto OSMSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\">\n"
        "   <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
        "       <tag k=\"amenity\" v=\"school\"/>\n"
        "       <tag k=\"name\" v=\"UC Davis\"/>\n"
        "   </node>\n"
        "</osm>"
    );
    auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto Node = OpenStreetMap.NodeByID(1);
    ASSERT_NE(Node, nullptr);
    ASSERT_EQ(Node->AttributeCount(),2);
    EXPECT_TRUE(Node->HasAttribute("amenity"));
    EXPECT_EQ(Node->GetAttribute("amenity"),"school");
    EXPECT_EQ(Node->GetAttribute("name"),"UC Davis");
    EXPECT_FALSE(Node->HasAttribute("fake"));
    EXPECT_EQ(Node->GetAttribute("fake"),"");
    EXPECT_EQ(Node->GetAttributeKey(0), "amenity");
    EXPECT_EQ(Node->GetAttributeKey(1), "name");
    EXPECT_EQ(Node->GetAttributeKey(5), "");

}

TEST(OpenStreetMapTest, NodeCountTest){
    auto OSMSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\">\n"
        "   <node id=\"1\" lat=\"10.0\" lon=\"20.0\"/>\n"
        "   <node id=\"2\" lat=\"15.0\" lon=\"25.0\"/>\n"
        "   <node id=\"3\" lat=\"30.0\" lon=\"35.0\"/>\n"
        "</osm>"
    );
    auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(), 3);
    

}

TEST(OpenStreetMapTest, WayParsingTest){
    auto OSMSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\">\n"
        "   <node id=\"1\" lat=\"10.0\" lon=\"20.0\"/>\n"
        "   <node id=\"2\" lat=\"15.0\" lon=\"25.0\"/>\n"
        "   <way id=\"2\">\n"
        "       <nd ref=\"1\"/>\n"
        "       <nd ref=\"2\"/>\n"
        "   </way>\n"
        "</osm>"
    );
    auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    ASSERT_EQ(OpenStreetMap.WayCount(),1);
    auto way = OpenStreetMap.WayByIndex(0);
    ASSERT_NE(way, nullptr);
    EXPECT_EQ(way->NodeCount(),2);
    EXPECT_EQ(way->GetNodeID(0),1);
    EXPECT_EQ(way->GetNodeID(1),2);
    EXPECT_EQ(way->GetNodeID(10),0);
    EXPECT_EQ(way->ID(), 2);


}

TEST(OpenStreetMapTest, WayAttributesTest){
    auto OSMSource = std::make_shared<CStringDataSource>(
        "<osm version=\"0.6\">\n"
        "   <node id=\"1\" lat=\"0\" lon=\"0\"/>\n"
        "   <way id=\"500\">\n"
        "       <nd ref=\"1\"/>\n"
        "       <tag k=\"grape\" v=\"residential\"/>\n"
        "       <tag k=\"name\" v=\"apple\"/>\n"
        "   </way>\n"
        "</osm>"
    );
    auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);
    auto way = OpenStreetMap.WayByID(500);
    ASSERT_NE(way, nullptr);
    EXPECT_EQ(way->AttributeCount(),2);
    EXPECT_TRUE(way->HasAttribute("grape"));
    EXPECT_EQ(way->GetAttribute("grape"), "residential");
    EXPECT_TRUE(way->HasAttribute("name"));
    EXPECT_EQ(way->GetAttribute("name"), "apple");
    EXPECT_FALSE(way->HasAttribute("pineapple"));
    EXPECT_EQ(way->GetAttributeKey(0), "grape");
    EXPECT_EQ(way->GetAttributeKey(1), "name");
    EXPECT_EQ(way->GetAttributeKey(5), "");
}

TEST(OpenStreetMapTest, EmptyMapTest){
    auto OSMSource = std::make_shared<CStringDataSource>("<osm version=\"0.6\"></osm>");
    auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);
    EXPECT_EQ(OpenStreetMap.NodeCount(),0);
    EXPECT_EQ(OpenStreetMap.WayCount(),0);
    EXPECT_EQ(OpenStreetMap.NodeByIndex(0), nullptr);
    EXPECT_EQ(OpenStreetMap.WayByIndex(0), nullptr);

}