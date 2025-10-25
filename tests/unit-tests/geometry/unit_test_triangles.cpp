#include <gtest/gtest.h>

import triangle;

using namespace Geometry;

using fpoint = point_t<float>;
using dpoint = point_t<long double>;

using ftriag = triangle_t<float>;
using dtriag = triangle_t<long double>;

TEST(TesingTriangleFloat, CtorArray)
{
    triangle_t<float> t({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f});

    EXPECT_FLOAT_EQ(t.get_a_x(), 1.0f);
    EXPECT_FLOAT_EQ(t.get_a_y(), 2.0f);
    EXPECT_FLOAT_EQ(t.get_a_z(), 3.0f);
    EXPECT_FLOAT_EQ(t.get_b_x(), 4.0f);
    EXPECT_FLOAT_EQ(t.get_b_y(), 5.0f);
    EXPECT_FLOAT_EQ(t.get_b_z(), 6.0f);
    EXPECT_FLOAT_EQ(t.get_c_x(), 7.0f);
    EXPECT_FLOAT_EQ(t.get_c_y(), 8.0f);
    EXPECT_FLOAT_EQ(t.get_c_z(), 9.0f);
}

TEST(TestingTriangleDouble, CtorArray)
{
    triangle_t<long double    > t({1.0L, 2.0L, 3.0L, 4.0L, 5.0L, 6.0L, 7.0L, 8.0L, 9.0L});

    EXPECT_FLOAT_EQ(t.get_a_x(), 1.0);
    EXPECT_FLOAT_EQ(t.get_a_y(), 2.0);
    EXPECT_FLOAT_EQ(t.get_a_z(), 3.0);
    EXPECT_FLOAT_EQ(t.get_b_x(), 4.0);
    EXPECT_FLOAT_EQ(t.get_b_y(), 5.0);
    EXPECT_FLOAT_EQ(t.get_b_z(), 6.0);
    EXPECT_FLOAT_EQ(t.get_c_x(), 7.0);
    EXPECT_FLOAT_EQ(t.get_c_y(), 8.0);
    EXPECT_FLOAT_EQ(t.get_c_z(), 9.0);
}


TEST(TesingTriangleFloat, IntersectionsOfTrianglesInDifferentPlains)
{
    fpoint a, b, c, d, e, f;
    ftriag t1, t2;

    // test 1
    a = fpoint(0.0f, 0.0f, 0.0f);
    b = fpoint(1.0f, 1.0f, 1.0f);
    c = fpoint(1.0f, 1.0f, 0.0f);
 
    d = fpoint(1.0f, 0.0f, 0.0f);
    e = fpoint(0.0f, 1.0f, 0.0f);
    f = fpoint(1.74f, 1.36f, 0.5f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 2
    d = fpoint(1.36f, -0.5f, 2.0f);
    e = fpoint(0.4f, 1.5f, 0.5f);
    f = fpoint(-0.37f, 0.26f, 0.5f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 3
    a = fpoint(0.6f, 1.43f, -0.33f);
    b = fpoint(1.17f, 0.93f, 2.04f);
    c = fpoint(1.66f, 0.66f, 0.8f);

    d = fpoint(1.36f, -0.5f, 2.0f);
    e = fpoint(0.4f, 1.5f, 0.4f);
    f = fpoint(-0.37f, 0.26f, 0.5f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 4
    a = fpoint(0.3107f,-4.74856f,-0.33f);
    b = fpoint(2.46779f,2.17252f,2.04f);
    c = fpoint(0.7641f,0.01955f,1.75283f);

    d = fpoint(0.72246f,1.92511f,0.0f);
    e = fpoint(3.76171f,2.4891f,0.0f);
    f = fpoint(1.9091f,2.43072f,3.5f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 5
    a = fpoint(0.3107f,-4.74856f,-0.33f);
    b = fpoint(2.40775f,2.77007f,2.04f);
    c = fpoint(0.7641f,0.01955f,1.75283f);

    d = fpoint(0.72246f,1.92511f,0.0f);
    e = fpoint(3.76171f,2.4891f,0.0f);
    f = fpoint(1.9091f,2.43072f,3.5f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 6
    a = fpoint(-4.37871f,-2.49585f,1.66934f);
    b = fpoint(2.79069f,-3.30025f,1.17701f);
    c = fpoint(-3.88008f,2.38f,1.67737f);

    d = fpoint(-4.27335f,1.81777f,0.0f);
    e = fpoint(-3.20461f,-0.62398f,2.85218f);
    f = fpoint(-1.59365f,2.12206f,0.0f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 7
    a = fpoint(-4.37871f,-2.49585f,1.66934f);
    b = fpoint(2.79069f,-3.30025f,1.17701f);
    c = fpoint(-2.12369f,1.4715f,1.67737f);
    d = fpoint(-5.16794f,-0.45839f,0.0f);
    e = a;
    f = fpoint(-1.59365f,2.12206f,0.0f);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));
}
