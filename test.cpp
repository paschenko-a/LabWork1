#include <gtest/gtest.h>
#include "bmp_image.h"
#include <fstream>
#include <filesystem>

class BMPTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(BMPTest, LoadNonExistentFile) {
    BMP img;
    EXPECT_FALSE(img.Load("prizrak.bmp")); // Файла нет - должен быть false
}

TEST_F(BMPTest, MemoryUsageCalculation) {
    BMP img;
    img.Load("sample.bmp");
    if (img.IsLoaded()) {
        EXPECT_GT(img.MemoryUsage(), 0);
        size_t expected = img.Width() * img.Height() * 3; 
        EXPECT_GE(img.MemoryUsage(), expected); 
    }
}

TEST_F(BMPTest, RotateClockwiseLogic) {
    BMP img;
    ASSERT_TRUE(img.Load("sample.bmp"));
    
    auto rotated = img.RotateClockwise();
    EXPECT_EQ(img.Width(), rotated.Height());
    EXPECT_EQ(img.Height(), rotated.Width());
    
    EXPECT_TRUE(rotated.Save("test_clockwise.bmp"));
}

TEST_F(BMPTest, RotateCounterClockwiseLogic) {
    BMP img;
    ASSERT_TRUE(img.Load("sample.bmp"));
    
    auto rotated = img.RotateCounterClockwise();
    EXPECT_EQ(img.Width(), rotated.Height());
    EXPECT_EQ(img.Height(), rotated.Width());
    EXPECT_TRUE(rotated.Save("test_counter.bmp"));
}

TEST_F(BMPTest, GaussianFilterApplication) {
    BMP img;
    ASSERT_TRUE(img.Load("sample.bmp"));
    
    auto filtered = img.ApplyGaussianFilter();
    
    EXPECT_EQ(img.Width(), filtered.Width());
    EXPECT_EQ(img.Height(), filtered.Height());
}

TEST_F(BMPTest, GaussianOnRotated) {
    BMP img;
    img.Load("sample.bmp");
    auto rotated = img.RotateClockwise();
    auto final_img = rotated.ApplyGaussianFilter();
    
    EXPECT_TRUE(final_img.IsLoaded());
    EXPECT_TRUE(final_img.Save("test_gauss_rotated.bmp"));
}