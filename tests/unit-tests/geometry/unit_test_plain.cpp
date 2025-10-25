#include <gtest/gtest.h>


import plain;

using namespace Geometry::Detail;

using fpoint = point_t<float>;
using dpoint = point_t<double>;

using fline = line_t<float>;
using dline = line_t<double>;

using fplain = plain_t<float >;
using dplain = plain_t<double>;

TEST(TestingFloatPlain, Ctor)
{
    fpoint p1(0.0f, 0.0f, 0.0f);
    fpoint p2(1.0f, 1.0f, 1.0f);
    fpoint p3(1.0f, 1.0f, 0.0f);

    fplain pl1(p1, p2, p3);
    EXPECT_FLOAT_EQ(pl1.get_a(), -1.0f);
    EXPECT_FLOAT_EQ(pl1.get_b(), 1.0f);
    EXPECT_FLOAT_EQ(pl1.get_c(), 0.0f);
    EXPECT_FLOAT_EQ(pl1.get_d(), 0.0f);

    EXPECT_TRUE(pl1.compare_with_another_plain(pl1));

    fpoint p4(1.0f, 0.0f, 0.0f);
    fpoint p5(0.0f, 1.0f, 0.0f);
    fpoint p6(0.0f, 0.0f, 0.0f);
    
    fplain pl2(p4, p5, p6);
    EXPECT_FLOAT_EQ(pl2.get_a(), 0.0f);
    EXPECT_FLOAT_EQ(pl2.get_b(), 0.0f);
    EXPECT_FLOAT_EQ(pl2.get_c(), 1.0f);
    EXPECT_FLOAT_EQ(pl2.get_d(), 0.0f);

    EXPECT_TRUE(pl2.compare_with_another_plain(pl2));

    EXPECT_FALSE(pl1.compare_with_another_plain(pl2));
    EXPECT_FALSE(pl2.compare_with_another_plain(pl1));


    // Плоскость XZ (y = 0)
    fpoint p7(1.0f, 0.0f, 0.0f);
    fpoint p8(0.0f, 0.0f, 1.0f);
    fpoint p9(0.0f, 0.0f, 0.0f);
    
    fplain pl3(p7, p8, p9);
    fplain pl3_equal(0.0f, 1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(pl3.compare_with_another_plain(pl3_equal));
    EXPECT_TRUE(pl3_equal.compare_with_another_plain(pl3));

    EXPECT_FALSE(pl3.compare_with_another_plain(pl1));
    EXPECT_FALSE(pl1.compare_with_another_plain(pl3));

    EXPECT_FALSE(pl3.compare_with_another_plain(pl2));
    EXPECT_FALSE(pl2.compare_with_another_plain(pl3));

    // Плоскость YZ (x = 0)
    fpoint p10(0.0f, 1.0f, 0.0f);
    fpoint p11(0.0f, 0.0f, 1.0f);
    fpoint p12(0.0f, 0.0f, 0.0f);
    
    fplain pl4(p10, p11, p12);
    fplain pl4_equal(1.0f, 0.0f, 0.0f, 0.0f);
    
    EXPECT_TRUE(pl4.compare_with_another_plain(pl4_equal));
    EXPECT_TRUE(pl4_equal.compare_with_another_plain(pl4));

    // Наклонная плоскость x + y + z - 3 = 0
    fpoint p13(1.0f, 1.0f, 1.0f);
    fpoint p14(2.0f, 1.0f, 0.0f);
    fpoint p15(1.0f, 2.0f, 0.0f);
    
    fplain pl5(p13, p14, p15);
    fplain pl5_equal(1.0f, 1.0f, 1.0f, -3.0f);
    EXPECT_TRUE(pl5.compare_with_another_plain(pl5_equal));
    EXPECT_TRUE(pl5_equal.compare_with_another_plain(pl5));

    // Вертикальная плоскость x - y = 0
    fpoint p16(0.0f, 0.0f, 0.0f);
    fpoint p17(1.0f, 1.0f, 0.0f);
    fpoint p18(1.0f, 1.0f, 1.0f);
    
    fplain pl6(p16, p17, p18);
    fplain pl6_equal(1.0f, -1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(pl6.compare_with_another_plain(pl6_equal));
    EXPECT_TRUE(pl6_equal.compare_with_another_plain(pl6));


    // Плоскость 2x + 3y - z + 5 = 0
    fpoint p19(0.0f, 0.0f, 5.0f);
    fpoint p20(1.0f, 0.0f, 7.0f);
    fpoint p21(0.0f, 1.0f, 8.0f);
    
    fplain pl7(p19, p20, p21);
    fplain pl7_equal(2.0f, 3.0f, -1.0f, 5.0f);
    EXPECT_TRUE(pl7.compare_with_another_plain(pl7_equal));
    EXPECT_TRUE(pl7_equal.compare_with_another_plain(pl7));

    // Плоскость параллельная OZ: x + 2y - 1 = 0
    fpoint p22(1.0f, 0.0f, 0.0f);
    fpoint p23(0.0f, 0.5f, 0.0f);
    fpoint p24(1.0f, 0.0f, 1.0f);
    
    fplain pl8(p22, p23, p24);
    fplain pl8_equal(1.0f, 2.0f, 0.0f, -1.0f);
    EXPECT_TRUE(pl8.compare_with_another_plain(pl8_equal));
    EXPECT_TRUE(pl8_equal.compare_with_another_plain(pl8));

    fpoint p25(1.36674f,0.73458f,0.80515f);
    fpoint p26(-0.4726f,0.3774f,0.78554f);
    fpoint p27(0.64906f,-1.18033f,1.40528f);

    fpoint p28(1.87f, 1.52f, 0.57f);
    fpoint p29(0.21f, 0.86f, 0.67f);
    fpoint p30(-1.57f, -0.52f, 1.01f);

    fplain pl9(p25, p26, p27);
    fplain pl10(p28, p29, p30);

    EXPECT_TRUE(pl9.compare_with_another_plain(pl10));
    EXPECT_TRUE(pl10.compare_with_another_plain(pl9));

    EXPECT_FALSE(pl8.compare_with_another_plain(pl7));
    EXPECT_FALSE(pl8.compare_with_another_plain(pl6));
    EXPECT_FALSE(pl8.compare_with_another_plain(pl5));
    EXPECT_FALSE(pl8.compare_with_another_plain(pl4));
    EXPECT_FALSE(pl8.compare_with_another_plain(pl3));

    EXPECT_FALSE(pl5.compare_with_another_plain(pl2));
    EXPECT_FALSE(pl5.compare_with_another_plain(pl4));
    EXPECT_FALSE(pl5.compare_with_another_plain(pl7));

}

TEST(TestingFloatPlain, IntersectionsOfPlain)
{
    fline il;

    fpoint p1(0.0f, 0.0f, 0.0f);
    fpoint p2(1.0f, 1.0f, 1.0f);
    fpoint p3(1.0f, 1.0f, 0.0f);

    fplain pl1(p1, p2, p3);

    fpoint p4(1.0f, 0.0f, 0.0f);
    fpoint p5(0.0f, 1.0f, 0.0f);
    fpoint p6(0.0f, 0.0f, 0.0f);
    
    fplain pl2(p4, p5, p6);

    fpoint il2_ref_point_1(0.0f, 0.0f, 0.0f);
    fpoint il2_ref_point_2(1.0f, 1.0f, 0.0f);

    fline il12(il2_ref_point_1, il2_ref_point_2);

    EXPECT_EQ(pl1.get_intersection_with_another_plain(pl2, il), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(il12.compare_with_another_line(il));

    // Тест 1: Параллельные плоскости (z = 0 и z = 1)
    fpoint p7(0.0f, 0.0f, 0.0f);
    fpoint p8(1.0f, 0.0f, 0.0f);
    fpoint p9(0.0f, 1.0f, 0.0f);
    fplain pl3(p7, p8, p9);  // z = 0

    fpoint p10(0.0f, 0.0f, 1.0f);
    fpoint p11(1.0f, 0.0f, 1.0f);
    fpoint p12(0.0f, 1.0f, 1.0f);
    fplain pl4(p10, p11, p12);  // z = 1

    fline il_parallel;
    EXPECT_EQ(pl3.get_intersection_with_another_plain(pl4, il_parallel), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(il_parallel.is_valid());

    // Тест 2: Совпадающие плоскости (x + y + z - 3 = 0)
    fpoint p13(1.0f, 1.0f, 1.0f);
    fpoint p14(2.0f, 1.0f, 0.0f);
    fpoint p15(1.0f, 2.0f, 0.0f);
    fplain pl5(p13, p14, p15);  // x + y + z - 3 = 0

    fpoint p16(3.0f, 0.0f, 0.0f);
    fpoint p17(0.0f, 3.0f, 0.0f);
    fpoint p18(0.0f, 0.0f, 3.0f);
    fplain pl6(p16, p17, p18);  // тоже x + y + z - 3 = 0

    fline il_same;
    EXPECT_EQ(pl5.get_intersection_with_another_plain(pl6, il_same), relative_position_t::CONTAIN_OR_EQUAL);
    EXPECT_FALSE(il_same.is_valid());

    // Тест 3: Наклонные пересекающиеся плоскости
    fpoint p19(0.0f, 0.0f, 0.0f);
    fpoint p20(1.0f, 0.0f, 0.0f);
    fpoint p21(0.0f, 1.0f, 0.0f);
    fplain pl7(p19, p20, p21);  // z = 0

    fpoint p22(0.0f, 0.0f, 0.0f);
    fpoint p23(0.0f, 1.0f, 0.0f);
    fpoint p24(0.0f, 0.0f, 1.0f);
    fplain pl8(p22, p23, p24);  // x = 0

    fpoint il_ref_point_1(0.0f, 0.0f, 0.0f);
    fpoint il_ref_point_2(0.0f, 1.0f, 0.0f);
    fline il_expected(il_ref_point_1, il_ref_point_2);  // Прямая x=0, z=0

    fline il_intersect;
    EXPECT_EQ(pl7.get_intersection_with_another_plain(pl8, il_intersect), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(il_expected.compare_with_another_line(il_intersect));

    // Тест 4: Еще один случай пересечения (плоскости под углом)
    fpoint p25(1.0f, 0.0f, 0.0f);
    fpoint p26(0.0f, 1.0f, 0.0f);
    fpoint p27(0.0f, 0.0f, 1.0f);
    fplain pl9(p25, p26, p27);  // x + y + z - 1 = 0

    // я съел деда
    // как вы здесь очутились вообще?
    // здесь просто еще куча unit тестов для плоскостей
    // неужели вы так фанатично любите свою работу, что забрели даже сюда??
    // на самом деле, если вы это читаете, то я восхищен вами как преподавателем/ментором
    // без сарказма :)

    fpoint p28(0.0f, 0.0f, 0.0f);
    fpoint p29(1.0f, 0.0f, 0.0f);
    fpoint p30(0.0f, 0.0f, 1.0f);
    fplain pl10(p28, p29, p30);  // y = 0

    fpoint il_ref2_point_1(0.0f, 0.0f, 1.0f);
    fpoint il_ref2_point_2(1.0f, 0.0f, 0.0f);
    fline il_expected2(il_ref2_point_1, il_ref2_point_2);  // Прямая пересечения

    fline il_intersect2;
    EXPECT_EQ(pl9.get_intersection_with_another_plain(pl10, il_intersect2), relative_position_t::NORMAL_INTERSECTION);
    EXPECT_TRUE(il_expected2.compare_with_another_line(il_intersect2));

    // Тест 5: Параллельные наклонные плоскости
    fpoint p31(0.0f, 0.0f, 0.0f);
    fpoint p32(1.0f, 0.0f, 1.0f);
    fpoint p33(0.0f, 1.0f, 1.0f);
    fplain pl11(p31, p32, p33);  // x + y - z = 0

    fpoint p34(0.0f, 0.0f, 1.0f);
    fpoint p35(1.0f, 0.0f, 2.0f);
    fpoint p36(0.0f, 1.0f, 2.0f);
    fplain pl12(p34, p35, p36);  // x + y - z + 1 = 0

    fline il_parallel2;
    EXPECT_EQ(pl11.get_intersection_with_another_plain(pl12, il_parallel2), relative_position_t::NO_INTERSECTION);
    EXPECT_FALSE(il_parallel2.is_valid());
}
