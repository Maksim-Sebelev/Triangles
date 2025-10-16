#include <gtest/gtest.h>

import matrix;

using f_vector_2 = vector_2_t<float>; 
using d_vector_2 = vector_2_t<double>; 

using f_matrix_2x2 = matrix_2x2_t<float>;
using d_matrix_2x2 = matrix_2x2_t<double>; 

TEST(MathMatrixtesting, Float_ByAllElementsCtor)
{
    f_matrix_2x2 m1(1.0f, 2.0f, 3.0f, 4.0f);
    // f_matrix_2x2 m2(0.5f, );
    EXPECT_FLOAT_EQ(m1.get_determinate(), -2.0f);
    
    
}

TEST(MathMatrixtesting, Float_ByColumnCtor)
{
    f_vector_2 c1(1.0f, 2.0f);
    f_vector_2 c2(3.0f, 4.0f);

    f_matrix_2x2 m(c1, c2);

    EXPECT_FLOAT_EQ(m.get_determinate(), -2.0f);
}

TEST(MathMatrixtesting, Double_ByAllElementsCtor)
{
    d_matrix_2x2 m1(1.0f, 2.0f, 3.0f, 4.0f);
    // f_matrix_2x2 m2(0.5f, );
    EXPECT_FLOAT_EQ(m1.get_determinate(), -2.0f);
    
    
}

TEST(MathMatrixtesting, Double_ByColumnCtor)
{
    d_vector_2 c1(1.0f, 2.0f);
    d_vector_2 c2(3.0f, 4.0f);

    d_matrix_2x2 m(c1, c2);

    EXPECT_FLOAT_EQ(m.get_determinate(), -2.0f);
}
