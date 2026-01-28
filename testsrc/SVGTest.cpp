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
    //ASSERT_NE(DContext, nullptr);
    //std::string output = DOutput.JoinOutput();
    //EXPECT_NE(output.find("<?xml"), std::string::npos);
    //EXPECT_NE(output.find("<svg"), std::string::npos);

    //svg_return_t des = destroy(DContext);
    //EXPECT_EQ(des, SVG_OK);
}

// --- INVALID INPUT TESTS ---
TEST_F(SVGTest, NullContextFunctions){
    //EXPECT_EQ(svg_circle(nullptr, nullptr, 0, nullptr), SVG_ERR_NULL);
    //EXPECT_EQ(svg_rect(nullptr, nullptr, nullptr, nullptr), SVG_ERR_NULL);
    //EXPECT_EQ(svg_line(nullptr, nullptr, nullptr, nullptr), SVG_ERR_NULL);
}

// --- DRAWING TESTS ---
TEST_F(SVGTest, Circle){
    //svg_point_t center = {};
}

TEST_F(SVGTest, Rectangle){

}

TEST_F(SVGTest, Line){

}

// --- GROUPING TEST ---
TEST_F(SVGTest, Grouping){

}

// --- EDGE CASES ---
TEST_F(SVGTest, ZeroDimensions){

}

TEST_F(SVGTest, NullPointPointer){

}

TEST_F(SVGTest, CreateEdgeCases){

}

TEST_F(SVGTest, DestroyEdgeCases){

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

}