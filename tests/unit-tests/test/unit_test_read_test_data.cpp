#include <gtest/gtest.h>

import triangle;
import read_test_data;

using namespace InputData::Detail;

#define WAY_FROM_BUILD_TO_THIS_TEST_DATA_DIR "../tests/e2e/dat/"

#define create_test_data_float(n) std::string_view dat_##n(WAY_FROM_BUILD_TO_THIS_TEST_DATA_DIR #n ".dat");\
test_input_t<float> td##n(dat_##n)

#define create_test_data_double(n) std::string_view dat_##n(WAY_FROM_BUILD_TO_THIS_TEST_DATA_DIR #n ".dat");\
test_input_t<double> td##n(dat_##n)


#define creat_coordinate_of_float_triangle_tests(n_test, n_triag, c1, c2, c3, c4, c5, c6, c7, c8, c9)\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_a_x(), c1);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_a_y(), c2);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_a_z(), c3);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_b_x(), c4);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_b_y(), c5);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_b_z(), c6);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_c_x(), c7);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_c_y(), c8);\
EXPECT_FLOAT_EQ(td##n_test.get_i_triangle(n_triag).get_c_z(), c9)

#define creat_coordinate_of_double_triangle_tests(n_test, n_triag, c1, c2, c3, c4, c5, c6, c7, c8, c9)\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_a_x(), c1);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_a_y(), c2);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_a_z(), c3);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_b_x(), c4);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_b_y(), c5);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_b_z(), c6);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_c_x(), c7);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_c_y(), c8);\
EXPECT_DOUBLE_EQ(td##n_test.get_i_triangle(n_triag).get_c_z(), c9)

TEST(ReadTestDataFloat, Dat1)
{
    create_test_data_float(1);
    
    EXPECT_EQ(td1.get_triangles_quantity(), 2);

    creat_coordinate_of_float_triangle_tests(1, 0,
        0, 0, 0,
        1, 1,1,
        1,1,0
    );

    creat_coordinate_of_float_triangle_tests(1, 1,
        1, 0, 0,
        0, 1, 0,
        1.74, 1.36, 0.5
    );

}

TEST(ReadTestDataFloat, Dat2)
{
    create_test_data_float(2);

    EXPECT_EQ(td2.get_triangles_quantity(), 0);
}

TEST(ReadTestDataFloat, Dat3)
{
    create_test_data_float(3);

    EXPECT_EQ(td3.get_triangles_quantity(), 4);

    creat_coordinate_of_float_triangle_tests(3, 0,
        0.3107, -4.74856, -0.33,
        2.46779, 2.17252, 2.04,
        0.7641, 0.01955, 1.75283
    );


    creat_coordinate_of_float_triangle_tests(3, 1,
        0.72246, 1.92511, 0,
        3.76171, 2.4891, 0,
        1.9091, 2.43072, 3.5
    );

    creat_coordinate_of_float_triangle_tests(3, 2,
        0.3107, -4.74856, -0.33,
        2.40775, 2.77007, 2.04,
        0.7641, 0.01955, 1.75283
    );

    creat_coordinate_of_float_triangle_tests(3, 3,
        -4.37871, -2.49585, 1.66934,
        2.79069, -3.30025, 1.17701,
        -3.88008, 2.38, 1.67737
    );
}

