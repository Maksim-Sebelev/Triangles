#include <gtest/gtest.h>

import triangle;
import get_test_data;

using namespace InputData;

TEST(TestingGetTestData, Float_FilStream_1)
{
    int argc = 4;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*) "--input_stream=files",
        (char*) "../tests/e2e/ans/1.ans",
        (char*) "../tests/e2e/dat/1.dat",
    };

    test_data_t<float> td = get_test_data<float>(argc, argv);

    EXPECT_EQ(td.get_triangles_quantity(), 2);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_a_x(), 0.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_a_y(), 0.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_a_z(), 0.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_b_x(), 1.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_b_y(), 1.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_b_z(), 1.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_c_x(), 1.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_c_y(), 1.0);
    EXPECT_FLOAT_EQ(td.get_i_triangle(0).get_c_z(), 0.0);

    EXPECT_EQ(td.get_good_triangles_quantity(), 2);
    EXPECT_EQ(td.get_number_of_i_good_triangle(0), 0);
    EXPECT_EQ(td.get_number_of_i_good_triangle(1), 1);

    EXPECT_EQ(td.print_result_and_dont_check_answer(), false);
}

