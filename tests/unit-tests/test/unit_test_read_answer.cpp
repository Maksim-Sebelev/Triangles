#include <gtest/gtest.h>

#include <string>

import read_test_answer;
using namespace InputData;

#define WAY_FROM_BUILD_TO_THIS_TEST_DATA_DIR "../tests/e2e/ans/"

#define create_test_answer(n) std::string_view ans_##n(WAY_FROM_BUILD_TO_THIS_TEST_DATA_DIR #n ".ans"); \
test_answer_t test_answer_##n(ans_##n)

TEST(ReadAnswerTest, FilesStream_1)
{
    create_test_answer(1);

    EXPECT_EQ(test_answer_1.get_good_triangles_quantity(), 2);

    EXPECT_EQ(test_answer_1.get_number_of_i_good_triangle(0), 0);
    EXPECT_EQ(test_answer_1.get_number_of_i_good_triangle(1), 1);
}


TEST(ReadAnswerTest, FilesStream_2)
{
    create_test_answer(2);
    EXPECT_EQ(test_answer_2.get_good_triangles_quantity(), 0);
}


TEST(ReadAnswerTest, FilesStream_3)
{
    create_test_answer(3);

    EXPECT_EQ(test_answer_3.get_good_triangles_quantity  ( ), 3);

    EXPECT_EQ(test_answer_3.get_number_of_i_good_triangle(0), 0);
    EXPECT_EQ(test_answer_3.get_number_of_i_good_triangle(1), 1);
    EXPECT_EQ(test_answer_3.get_number_of_i_good_triangle(2), 2);

}


TEST(ReadAnswerTest, FilesStream_4)
{
    create_test_answer(4);

    EXPECT_EQ(test_answer_4.get_good_triangles_quantity  ( ), 2);

    EXPECT_EQ(test_answer_4.get_number_of_i_good_triangle(0), 0);
    EXPECT_EQ(test_answer_4.get_number_of_i_good_triangle(1), 1);
}
