#include <gtest/gtest.h>
#include "SVGWriter.h"
#include "StringDataSink.h"

TEST(SVGWriterTest, CreateDestroyTest){

}

TEST(SVGWriterTest, CircleTest){
    //example: <circle cx="50" cy="50" r="10" fill="red" />
    auto sink = std::make_shared<CStringDataSink>();
    CSVGWriter writer(sink, 100, 100);
    TAttributes style = {{"fill", "red"}, {"stroke", "black"}};
    writer.Circle({50, 50}, 10, style);
    std::string expectedOutput = "<circle cx=\"50\" cy=\"50\" r=\"10\" fill=\"red\" stroke=\"black\"/>\n";
    EXPECT_EQ(sink->String(), expectedOutput);

}


TEST(SVGWriterTest, RectangleTest){
    //example: <rect x="10" y="20" width="50" height="30" fill="blue" stroke="black"/>

    auto sink = std::make_shared<CStringDataSink>();
    TAttributes style ={{"fill", "blue"}, {"stroke", "black"}};
    CSVGWriter writer(sink, 100, 100);
    writer.Rectange({10, 20}, {50, 30}, style);
    std::string expectedOutput = "<rect x=\"10\" y=\"20\" width=\"50\" height=\"30\" fill=\"blue\" stroke=\"black\"/>\n";
    EXPECT_EQ(sink->String(), expectedOutput);

}

TEST(SVGWriterTest, LineTest){
    //example: <line x1="0" y1="0" x2="100" y2="100" stroke="black" stroke-width="2"/>
    auto sink = std::make_shared<CStringDataSink>();
    TAttributes style = {{"stroke","black"},{"stroke-width","2"}};
    CSVGWriter writer(sink, 100, 100);
    writer.Line({0, 0}, {100, 100}, style);
    std::string expectedOutput = "<line x1=\"0\" y1=\"0\" x2=\"100\" y2=\"100\" stroke=\"black\" stroke-width=\"2\"/>\n";
    EXPECT_EQ(sink->String(), expectedOutput);
}

TEST(SVGWriterTest, SimplePathTest){
    //example: <path d="M 10 10 L 20 20 L 30 15" stroke="black" fill="none"/>
    auto sink = std::make_shared<CStringDataSink>();
    TAttributes style = {{"stroke", "black"}, {"fill", "none"}};
    std::vector<SSVGPoint> points = {{10, 10}, {20, 20}, {30, 15}};
    CSVGWriter writer(sink, 200, 200);
    writer.SimplePath(points, style);
    std::string expectedOutput = "<path d=\"M 10 10 L 20 20 L 30 15\" stroke=\"black\" fill=\"none\"/>\n";
    EXPECT_EQ(sink->String(), expectedOutput);

}

TEST(SVGWriterTest, GroupTest){
    //example: <g fill="red" stroke="black">
    auto sink = std::make_shared<CStringDataSink>();
    TAttributes style = {{"fill", "red"}, {"stroke", "black"}};
    CSVGWriter writer(sink, 100, 100);
    writer.GroupBegin(style);
    writer.GroupEnd();
    std::string expectedOutput = "<g fill=\"red\" stroke=\"black\">\n</g>\n";
    EXPECT_EQ(sink->String(), expectedOutput);

}

class CFailingSink : public CDataSink{
    public:
        int DValidCalls = 0;
        virtual ~CFailingSink(){};
        bool Put(const char &ch) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }

        bool Write(const std::vector<char> &buf) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }
};

TEST(SVGWriterTest, ErrorTests){
    auto sink = std::make_shared<CFailingSink>();
    CSVGWriter writer(sink, 100, 100);
    TAttributes style = {{"fill", "red"}};
    sink->DValidCalls = 0; // this forces sink to fail
    EXPECT_FALSE(writer.Circle({50,50}, 10, style)); // checks that circle returns false
}
