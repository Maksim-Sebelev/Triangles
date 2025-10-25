#include <gtest/gtest.h>

#include <cmath>
#include <array>

import point;

using namespace Geometry::Detail;

using fpoint = point_t<float>;
using dpoint = point_t<double>;

TEST(point_float, testing_default_ctor)
{
    fpoint p1;
    fpoint p2;

    EXPECT_TRUE(std::isnan(p1.get_x()));
    EXPECT_TRUE(std::isnan(p1.get_y()));
    EXPECT_TRUE(std::isnan(p1.get_z()));

    EXPECT_FALSE(p1.is_valid());

    EXPECT_FALSE(p1.compare_with_another_point(p2));
}

TEST(point_float, testing_array_ctor)
{
    fpoint p1((std::array<float, 3>){1.0f, 2.0f, 3.0f});
    fpoint p2 = p1;

    fpoint p3((std::array<float, 3>){1.1f, 2.0f, 3.0f});
    fpoint p4((std::array<float, 3>){1.0f, 2.1f, 3.0f});
    fpoint p5((std::array<float, 3>){1.0f, 2.0f, 3.1f});

    EXPECT_FLOAT_EQ(p1.get_x(), 1.0f);
    EXPECT_FLOAT_EQ(p1.get_y(), 2.0f);
    EXPECT_FLOAT_EQ(p1.get_z(), 3.0f);

    EXPECT_TRUE(p1.is_valid());

    EXPECT_TRUE(p1.compare_with_another_point(p2));

    EXPECT_FALSE(p1.compare_with_another_point(p3));
    EXPECT_FALSE(p1.compare_with_another_point(p4));
    EXPECT_FALSE(p1.compare_with_another_point(p5));
}

TEST(point_float, testing_coordinate_ctor)
{
    fpoint p1(1.0f, 2.0f, 3.0f);
    fpoint p2 = p1;

    fpoint p3(1.1f, 2.0f, 3.0f);
    fpoint p4(1.0f, 2.1f, 3.0f);
    fpoint p5(1.0f, 2.0f, 3.1f);

    EXPECT_FLOAT_EQ(p1.get_x(), 1.0f);
    EXPECT_FLOAT_EQ(p1.get_y(), 2.0f);
    EXPECT_FLOAT_EQ(p1.get_z(), 3.0f);

    EXPECT_TRUE(p1.is_valid());

    EXPECT_TRUE(p1.compare_with_another_point(p2));

    EXPECT_FALSE(p1.compare_with_another_point(p3));
    EXPECT_FALSE(p1.compare_with_another_point(p4));
    EXPECT_FALSE(p1.compare_with_another_point(p5));
}


TEST(point_double, testing_deault_ctor)
{
    dpoint p1;
    dpoint p2;

    EXPECT_TRUE(std::isnan(p1.get_x()));
    EXPECT_TRUE(std::isnan(p1.get_y()));
    EXPECT_TRUE(std::isnan(p1.get_z()));

    EXPECT_FALSE(p1.is_valid());

    EXPECT_FALSE(p1.compare_with_another_point(p2));
}

TEST(point_double, testing_array_ctor)
{
    dpoint p1((std::array<double, 3>){1.0, 2.0, 3.0});
    dpoint p2 = p1;

    dpoint p3((std::array<double, 3>){1.1, 2.0, 3.0});
    dpoint p4((std::array<double, 3>){1.0, 2.1, 3.0});
    dpoint p5((std::array<double, 3>){1.0, 2.0, 3.1});

    EXPECT_FLOAT_EQ(p1.get_x(), 1.0);
    EXPECT_FLOAT_EQ(p1.get_y(), 2.0);
    EXPECT_FLOAT_EQ(p1.get_z(), 3.0);

    EXPECT_TRUE(p1.is_valid());

    EXPECT_TRUE(p1.compare_with_another_point(p2));

    EXPECT_FALSE(p1.compare_with_another_point(p3));
    EXPECT_FALSE(p1.compare_with_another_point(p4));
    EXPECT_FALSE(p1.compare_with_another_point(p5));
}

TEST(point_double, testing_coordinate_ctor)
{
    dpoint p1(1.0, 2.0, 3.0);
    dpoint p2 = p1;

    dpoint p3(1.1, 2.0, 3.0);
    dpoint p4(1.0, 2.1, 3.0);
    dpoint p5(1.0, 2.0, 3.1);

    EXPECT_FLOAT_EQ(p1.get_x(), 1.0);
    EXPECT_FLOAT_EQ(p1.get_y(), 2.0);
    EXPECT_FLOAT_EQ(p1.get_z(), 3.0);

    EXPECT_TRUE(p1.is_valid());

    EXPECT_TRUE(p1.compare_with_another_point(p2));

    EXPECT_FALSE(p1.compare_with_another_point(p3));
    EXPECT_FALSE(p1.compare_with_another_point(p4));
    EXPECT_FALSE(p1.compare_with_another_point(p5));
}

