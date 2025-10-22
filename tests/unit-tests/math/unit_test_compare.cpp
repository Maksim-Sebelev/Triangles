#include <gtest/gtest.h>

//---------------------------------------------------------------------------------------------------------------

import compare;

using namespace Math::Compare;

//---------------------------------------------------------------------------------------------------------------

TEST(MathTesing, EqualOfDoubles)
{
    EXPECT_TRUE(compare<double>(1.0, 1.0, 1.0    ));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.1    ));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.01   ));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.001  ));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.0001 ));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.00001));
    EXPECT_TRUE(compare<double>(1.0, 1.0, 0.0    ));

    EXPECT_FALSE(compare<double>(-1.19823, +17.983264, 0.0    ));
    EXPECT_FALSE(compare<double>(-1.19823, +17.983264, 5.123  ));
    EXPECT_FALSE(compare<double>(-1.19823, +17.983264, 10.2138));

    EXPECT_TRUE( compare<double>(-1.19823, +17.983264, 20.0   ));
    EXPECT_TRUE( compare<double>(28346.32847, 28346.32857, 0.001));
    EXPECT_FALSE(compare<double>(28346.32847, 38346.32857, 0.001));
}

//---------------------------------------------------------------------------------------------------------------

TEST(MathTesting, EqualODoubleToNull)
{
    EXPECT_FALSE(compare_with_null<double>(1.0));
    EXPECT_FALSE(compare_with_null<double>(0.1));
    EXPECT_FALSE(compare_with_null<double>(0.01));
    EXPECT_FALSE(compare_with_null<double>(0.001));
    EXPECT_TRUE (compare_with_null<double>(0.000001));

    EXPECT_TRUE (compare_with_null<double>(1.0, 1.0));
    EXPECT_FALSE(compare_with_null<double>(1.0, 0.1));
    EXPECT_FALSE(compare_with_null<double>(1.0, 0.5));
    EXPECT_TRUE (compare_with_null<double>(1.0, 100.0));

    EXPECT_TRUE(compare_with_null<double>(0.0, 0.0));
    EXPECT_TRUE(compare_with_null<double>(0.001, 0.001));
    EXPECT_TRUE(compare_with_null<double>(-0.0001, 0.001));
    EXPECT_TRUE(compare_with_null<double>(-0.0000001));
}

// ---------------------------------------------------------------------------------------------------------------

TEST(MathTesting, EqualOfFloats)
{
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 1.0f    ));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.1f    ));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.01f   ));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.001f  ));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.0001f ));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.00001f));
    EXPECT_TRUE (compare<float>(1.0f, 1.0f, 0.0f    ));

    EXPECT_FALSE(compare<float>(-1.19823f, +17.983264f, 0.0f    ));
    EXPECT_FALSE(compare<float>(-1.19823f, +17.983264f, 5.123f  ));
    EXPECT_FALSE(compare<float>(-1.19823f, +17.983264f, 10.2138f));
    EXPECT_TRUE (compare<float>(-1.19823f, +17.983264f, 20.0f   ));

    EXPECT_TRUE (compare<float>(28346.32847f, 28346.32857f, 0.001f));
    EXPECT_FALSE(compare<float>(28346.32847f, 38346.32857f, 0.001f));
}

// ---------------------------------------------------------------------------------------------------------------

TEST(MathTesting, EqualOfFloatToNull)
{
    EXPECT_FALSE(compare_with_null<float>(1.0f));
    EXPECT_FALSE(compare_with_null<float>(0.1f));
    EXPECT_FALSE(compare_with_null<float>(0.01f));
    EXPECT_TRUE (compare_with_null<float>(0.000001f));

    EXPECT_TRUE (compare_with_null<float>(1.0f, 1.0f));
    EXPECT_FALSE(compare_with_null<float>(1.0f, 0.1f));
    EXPECT_FALSE(compare_with_null<float>(1.0f, 0.5f));
    EXPECT_TRUE (compare_with_null<float>(1.0f, 100.0f));

    EXPECT_TRUE(compare_with_null<float>(0.0f, 0.0f));
    EXPECT_TRUE(compare_with_null<float>(0.001f, 0.001f));
    EXPECT_TRUE(compare_with_null<float>(-0.0001f, 0.001f));
    EXPECT_TRUE(compare_with_null<float>(-0.0000001f));

}

// ---------------------------------------------------------------------------------------------------------------
