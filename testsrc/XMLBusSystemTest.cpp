#include <gtest/gtest.h>
#include "XMLBusSystem.h"
#include "StringDataSource.h"
// checks if <stops> is parsed correctly
TEST(XMLBusSystemTest, StopParsing){
    // creates in memory XML string
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    // wraps string to XML parser
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    // constructs bus system (parsing happens in constructor)
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    // 2 stops parsed
    ASSERT_EQ(BusSystem.StopCount(),2);
    // stop at index 0 must exist
    EXPECT_NE(BusSystem.StopByIndex(0),nullptr);
    EXPECT_NE(BusSystem.StopByIndex(1),nullptr);
    // lookup by ID 
    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),321);
    EXPECT_EQ(StopObj->Description(),"First");
    StopObj = BusSystem.StopByID(2);
    ASSERT_NE(StopObj,nullptr);
    // checks if attributes parsed correctly
    EXPECT_EQ(StopObj->ID(),2);
    EXPECT_EQ(StopObj->NodeID(),311);
    EXPECT_EQ(StopObj->Description(),"second");

}
// checks <routes> section parsing is correct
TEST(XMLBusSystemTest, RouteParsing){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                "        <stop id=\"1\"/>\n"
                                                                "        <stop id=\"2\"/>\n"
                                                                "    </route>\n"
                                                                "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);   
    // checks exactly 1 route parsed
    EXPECT_EQ(BusSystem.RouteCount(), 1);
    // get first route
    auto route = BusSystem.RouteByIndex(0);
    ASSERT_NE(route, nullptr);
    // route name must match
    EXPECT_EQ(route->Name(), "A");
    // route has 2 stops
    EXPECT_EQ(route->StopCount(), 2);
    // route must match XML order
    EXPECT_EQ(route->GetStopID(0), 1);
    EXPECT_EQ(route->GetStopID(1), 2);

    auto routeByName = BusSystem.RouteByName("A");
    ASSERT_NE(routeByName, nullptr);
    EXPECT_EQ(routeByName->Name(), "A");
    // non-existing route must return nullptr
    EXPECT_EQ(BusSystem.RouteByName("B"),nullptr);
}
// makes sure path XML is parsed correctly
TEST(XMLBusSystemTest, PathParsing){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);
    
    EXPECT_EQ(BusSystem.RouteCount(),0);
    // pass node IDs
    auto path = BusSystem.PathByStopIDs(321, 311);
    ASSERT_NE(path, nullptr);
    EXPECT_EQ(path->StartNodeID(), 321);
    EXPECT_EQ(path->EndNodeID(), 311);
    EXPECT_EQ(path->NodeCount(), 3);
    EXPECT_EQ(path->GetNodeID(0), 321);
    EXPECT_EQ(path->GetNodeID(1), 315);
    EXPECT_EQ(path->GetNodeID(2), 311);

    EXPECT_EQ(BusSystem.PathByStopIDs(999, 1000), nullptr);
}
// looks for failure cases
TEST(XMLBusSystemTest, LookupFailure){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);
    // return nullptr if failed
    EXPECT_EQ(BusSystem.StopByID(999), nullptr);
    EXPECT_EQ(BusSystem.StopByIndex(5), nullptr);
    EXPECT_EQ(BusSystem.RouteByIndex(10), nullptr);
    EXPECT_EQ(BusSystem.RouteByName("nonexisting"), nullptr);
}
// no stops = no routes
TEST(XMLBusSystemTest, EmptySystem){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);
    // contains nothing but system should load
    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
}
// stops section exists but empty
TEST(XMLBusSystemTest, ZeroStops){
    auto BusRouteSource = std::make_shared<CStringDataSource>( "<bussystem>\n"
                                                                "<stops>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
}
// wrong closing tags
// constructor detect </bussystem not found>
TEST(XMLBusSystemTest, InvalidXML){
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "</stops>\n"
                                                                "</bus>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
}