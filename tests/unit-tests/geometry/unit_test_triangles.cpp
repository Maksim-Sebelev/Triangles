#include <gtest/gtest.h>

import triangle;

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
    triangle_t<long double    > t({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});

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
    a = fpoint(0, 0, 0);
    b = fpoint(1, 1, 1);
    c = fpoint(1, 1, 0);
 
    d = fpoint(1, 0, 0);
    e = fpoint(0, 1, 0);
    f = fpoint(1.74, 1.36, 0.5);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 2
    d = fpoint(1.36, -0.5, 2);
    e = fpoint(0.4, 1.5, 0.5);
    f = fpoint(-0.37, 0.26, 0.5);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 3
    a = fpoint(0.6, 1.43, -0.33);
    b = fpoint(1.17, 0.93, 2.04);
    c = fpoint(1.66, 0.66, 0.8);

    d = fpoint(1.36, -0.5, 2);
    e = fpoint(0.4, 1.5, 0.4);
    f = fpoint(-0.37, 0.26, 0.5);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 4
    a = fpoint(0.3107,-4.74856,-0.33);
    b = fpoint(2.46779,2.17252,2.04);
    c = fpoint(0.7641,0.01955,1.75283);

    d = fpoint(0.72246,1.92511,0);
    e = fpoint(3.76171,2.4891,0);
    f = fpoint(1.9091,2.43072,3.5);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 5
    a = fpoint(0.3107,-4.74856,-0.33);
    b = fpoint(2.40775,2.77007,2.04);
    c = fpoint(0.7641,0.01955,1.75283);

    d = fpoint(0.72246,1.92511,0);
    e = fpoint(3.76171,2.4891,0);
    f = fpoint(1.9091,2.43072,3.5);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 6
    a = fpoint(-4.37871,-2.49585,1.66934) ;
    b = fpoint(2.79069,-3.30025,1.17701);
    c = fpoint(-3.88008,2.38,1.67737);

    d = fpoint(-4.27335,1.81777,0);
    e = fpoint(-3.20461,-0.62398,2.85218);
    f = fpoint(-1.59365,2.12206,0);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 7
    a = fpoint(-4.37871,-2.49585,1.66934);
    b = fpoint(2.79069,-3.30025,1.17701);
    c = fpoint(-2.12369,1.4715,1.67737);
    d = fpoint(-5.16794,-0.45839,0);
    e = a;
    f = fpoint(-1.59365,2.12206,0);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));
}

TEST(TesingTriangleFloat, IntersectionsOfTrianglesInSamePlain)
{
    fpoint a, b, c, d, e, f;
    ftriag t1, t2;

    // test 1
    a = fpoint(0, 0, 0);
    b = fpoint(1, 1, 0);
    c = fpoint(-1, 1, 0);
 
    d = fpoint(1, 0.5, 0);
    e = fpoint(1, 1.5, 0);
    f = fpoint(-1, 1.5, 0);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 2
    a = fpoint(0, 0, 1);
    b = fpoint(1, 1, 1);
    c = fpoint(-1, 1, 1);
 
    d = fpoint(1, 0.5, 0);
    e = fpoint(1, 1.5, 0);
    f = fpoint(-1, 1.5, 0);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 3
    a = fpoint(1, 1, 1);
    b = fpoint(0, 0, 0);
    c = fpoint(1, -1, 1);
 
    d = fpoint(0, 0.5, 0);
    e = fpoint(1, 1.5, 1);
    f = fpoint(1, -0.5, 1);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 4
    a = fpoint(1.36674,0.73458,0.80515);
    b = fpoint(-0.4726,0.3774,0.78554);
    c = fpoint(0.64906,-1.18033,1.40528);

    d = fpoint(1.87, 1.52, 0.57);
    e = fpoint(0.21, 0.86, 0.67);
    f = fpoint(-1.57, -0.52, 1.01);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

    // test 5
    a = fpoint(1.36674,0.73458,0.80515);
    b = fpoint(-0.4726,0.3774,0.78554);
    c = fpoint(0.64906,-1.18033,1.40528);

    d = fpoint(1.87, 1.52, 0.57);
    e = fpoint(0.21, 0.86, 0.67);
    f = fpoint(-1.14, 2.36, 0.06);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_FALSE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_FALSE(t2.is_intersect_with_another_triangle(t1));

    // test 6
    a = fpoint(-2,-6,5);
    b = fpoint(-1.20485,-1.99208,2.76152);
    c = fpoint(-1.63645,1.64677,1.40528);

    d = fpoint(0.63, 8.38, -2.9);
    e = fpoint(-1.84, -7.25, 5.46);
    f = fpoint(-3.1, -1.63, 3.71);

    t1 = ftriag(a, b, c);
    t2 = ftriag(d, e, f);

    EXPECT_TRUE(t1.is_intersect_with_another_triangle(t2));
    EXPECT_TRUE(t2.is_intersect_with_another_triangle(t1));

}
