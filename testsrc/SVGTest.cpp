#include "svg.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Helper structure to capture written SVG strings
struct STestOutput{
    std::vector<std::string> DLines;
    bool DDestroyed = false;

    std::string JoinOutput(){
        std::string Result;

        for (const auto& line : DLines){
            Result += line;
        }
        return Result;
    }
};

// Callback to capture SVG output
svg_return_t write_callback(svg_user_context_ptr user, const char* text){
    if(!user || !text){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    OutPtr->DLines.push_back(text);
    return SVG_OK;
}

// Cleanup callback (just returns OK for testing)
svg_return_t cleanup_callback(svg_user_context_ptr user){
    if(!user){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    if(OutPtr->DDestroyed){
        return SVG_ERR_STATE;
    }
    OutPtr->DDestroyed = true;
    return SVG_OK;
}

// --- TEST FIXTURE ---
class SVGTest : public ::testing::Test{
    protected:
        STestOutput DOutput;
        svg_context_ptr DContext = nullptr;

        void SetUp() override{
            DContext = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
            ASSERT_NE(DContext, nullptr);
        }

        void TearDown() override{
            if(DContext){
                svg_destroy(DContext);
                DContext = nullptr;
            }
        }
};

// --- BASIC CREATION TEST ---
TEST_F(SVGTest, CreateAndDestroy){
    EXPECT_FALSE(DOutput.DDestroyed);
    svg_return_t result = svg_destroy(DContext);
    EXPECT_EQ(result , SVG_OK);
    EXPECT_TRUE(DOutput.DDestroyed);
    DContext = nullptr;
}

// --- INVALID INPUT TESTS ---
TEST_F(SVGTest, NullContextFunctions){
    svg_point_t testPoint = {50, 50};
    svg_size_t testSize = {550, 324};
    const char* style = "stroke:black;fill:none";
    EXPECT_EQ(svg_circle(nullptr, &testPoint, 10, style), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(nullptr, &testPoint, &testSize, style), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(nullptr, &testPoint, &testPoint, style), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_begin(nullptr, style), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_end(nullptr), SVG_ERR_NULL);
    EXPECT_EQ(svg_destroy(nullptr), SVG_ERR_NULL);
}

// --- DRAWING TESTS ---
TEST_F(SVGTest, Circle){
    svg_point_t center = {50, 50};
    svg_real_t radius = 25;
    const char* style = "stroke:black;fill:none";

    svg_circle(DContext, &center, radius, style);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<circle"), std::string::npos);
}

TEST_F(SVGTest, Rectangle){
    svg_point_t top_left = {20, 40};
    svg_size_t size = {100, 32};
    const char* style = "stroke:green;fill:none";
    svg_rect(DContext, &top_left, &size, style);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<rectangle"), std::string::npos);
    
}

TEST_F(SVGTest, Line){
    svg_point_t start = {1, 5};
    svg_point_t end = {80, 65};
    const char* style = "stroke:blue;fill:none";
    svg_line(DContext, &start, &end, style);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<line"), std::string::npos);
}

// --- GROUPING TEST ---
TEST_F(SVGTest, Grouping){
    svg_point_t center = {100, 100};
    svg_real_t rad = 50;
    const char* style = "stroke:black;fill:none";
    svg_group_begin(DContext, "stroke:red;fill:none");
    svg_circle(DContext, &center, rad, style);
    svg_group_end(DContext);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<g"), std::string::npos);
    EXPECT_NE(output.find("<circle"), std::string::npos);
    EXPECT_NE(output.find("</g>"), std::string::npos);
}

// --- EDGE CASES ---
TEST_F(SVGTest, ZeroDimensions){
    // when width or height = 0
     svg_point_t testPoint = {50, 50};
     svg_size_t testSize = {550, 324};
     svg_point_t start = {1, 5};
     svg_point_t end = {80, 65};
     const char* style = "stroke:black;fill:none";
     svg_return_t result1 = svg_circle(DContext, &testPoint, 0, style);
     EXPECT_EQ(result1, SVG_OK);
     svg_return_t result2 = svg_rect(DContext, &testPoint, &testSize, style);
     EXPECT_EQ(result2, SVG_OK);
     svg_return_t result3 = svg_line(DContext, &start, &end, style);
     EXPECT_EQ(result3, SVG_OK);
     
}

TEST_F(SVGTest, NullPointPointer){
    // passing null pointer instead of pointer
    svg_size_t testSize = {550, 324};
    svg_point_t start = {1, 5};
    svg_point_t end = {80, 65};
    const char* style = "stroke:black;fill:none";
    svg_return_t result1 = svg_circle(DContext, nullptr,  50, style);
    EXPECT_EQ(result1, SVG_ERR_NULL);
    svg_return_t result2 = svg_rect(DContext, nullptr, &testSize, style);
    EXPECT_EQ(result2, SVG_ERR_NULL);
    svg_return_t result3 = svg_line(DContext, nullptr, &end, style);
    EXPECT_EQ(result3, SVG_ERR_NULL);
    svg_return_t result4 = svg_line(DContext, &start, nullptr, style);
    EXPECT_EQ(result4, SVG_ERR_NULL);
    
}

TEST_F(SVGTest, CreateEdgeCases){
    // testing weird inputs
    svg_context_ptr context1 = svg_create(write_callback, cleanup_callback, &DOutput,0 ,0);
    EXPECT_NE(context1, nullptr);
    svg_context_ptr context2 = svg_create(write_callback, cleanup_callback, &DOutput, 10000, 8000);
    EXPECT_NE(context2, nullptr);
    svg_context_ptr context3 = svg_create(nullptr, cleanup_callback, &DOutput, 100, 100);
    EXPECT_EQ(context3, nullptr);
    svg_context_ptr context4 = svg_create(write_callback, nullptr, &DOutput, 100, 100);
    EXPECT_EQ(context4, nullptr);
    
}

TEST_F(SVGTest, DestroyEdgeCases){
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    ASSERT_NE(context, nullptr);
    svg_return_t result1 = svg_destroy(context);
    EXPECT_EQ(result1, SVG_OK);
    context = nullptr;
    svg_return_t result2 = svg_destroy(context);
    EXPECT_EQ(result2, SVG_ERR_NULL);
    
}


// Callback to capture SVG output
svg_return_t write_error_callback(svg_user_context_ptr user, const char* text){
    int *FailureCount = (int *)user;
    if(*FailureCount){
        (*FailureCount)--;
        return SVG_OK;    
    }
    return SVG_ERR_IO;
}

TEST_F(SVGTest, IOErrorTest){
    int failsLeft = 0;
    svg_context_ptr context = svg_create(write_error_callback, cleanup_callback, &failsLeft, 100, 100);
    ASSERT_NE(context, nullptr);
    svg_point_t center = {100, 100};
    const char* style = "stroke:black;fill:none";
    svg_return_t result = svg_circle(context, &center, 10, style);
    EXPECT_EQ(result, SVG_ERR_IO);
    svg_destroy(context);
}
