#include <gtest/gtest.h>

#include <cmath>

import vector;
using namespace Math::LinearAlgebra;

using f_vec_2 = vector_2_t<float>;
using d_vec_2 = vector_2_t<double>;

TEST(MathVectorTesing, FloatVectorSelfOperations)
{
    f_vec_2 v(1.0f, 1.0f);

    // EXPECT_TRUE(v.compare(v));

    EXPECT_FLOAT_EQ(v.get_a1(), 1.0f);
    EXPECT_FLOAT_EQ(v.get_a2(), 1.0f);

    EXPECT_FLOAT_EQ(v.get_square_module(), 2.0f);
    EXPECT_FLOAT_EQ(v.get_module(), std::sqrt(2.0f));

    v.set_a1(3.0f);
    v.set_a2(4.0f);

    EXPECT_FLOAT_EQ(v.get_a1(), 3.0f);
    EXPECT_FLOAT_EQ(v.get_a2(), 4.0f);

    EXPECT_FLOAT_EQ(v.get_square_module(), 25.0f);
    EXPECT_FLOAT_EQ(v.get_module(), 5.0f);

    v.mul_by_number(-8.0f);

    EXPECT_FLOAT_EQ(v.get_a1(), -24.0f);
    EXPECT_FLOAT_EQ(v.get_a2(), -32.0f);

    EXPECT_FLOAT_EQ(v.get_square_module(), 1600.0f);
    EXPECT_FLOAT_EQ(v.get_module(), 40.0f);
}


TEST(MathVectorTesting, FloatVectorOperationsWithAnotherVectrs)
{
    f_vec_2 v1(1.0f, -1.0f);
    f_vec_2 v2(-1.0f, -1.0f);

    // EXPECT_TRUE (v1.compare(v1));
    // EXPECT_TRUE (v2.compare(v2));
    // EXPECT_FALSE(v1.compare(v2));
    // EXPECT_FALSE(v2.compare(v1));


    EXPECT_FLOAT_EQ(v1.get_mul_by_another_vector(v2), 0.0f);
    EXPECT_FLOAT_EQ(v2.get_mul_by_another_vector(v1), 0.0f);


    v2 = v1.get_mul_by_number(2910.2005f);

    EXPECT_FLOAT_EQ(v2.get_a1(), 2910.2005f);
    EXPECT_FLOAT_EQ(v2.get_a2(), -2910.2005f);
}
