#include <gtest/gtest.h>

import linear_systems;

using fv_2 = vector_2_t<float>; 
using dv_2 = vector_2_t<double>; 

using fm_2x2 = matrix_2x2_t<float>;
using dm_2x2 = matrix_2x2_t<double>; 

using fls_2x2 = linear_system_2x2_t<float>;
using dls_2x2 = linear_system_2x2_t<double>;

TEST(LinearSystemsTesting, Float_InfSolutions)
{
    fv_2 b(0.0f, 0.0f);
    fm_2x2 A(0.0f, 0.0f, 0.0f, 0.0f);
    fls_2x2 l(A, b);
    fv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::INF_SOLUTIONS);
    EXPECT_FALSE(x.is_valid());
}

TEST(LinearSystemsTesting, Float_NoSolutions)
{
    fv_2 b(1.0f, 2.0f);
    fm_2x2 A(1.0f, 0.0f, 1.0f, 0.0f);
    fls_2x2 l(A, b);
    fv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::NO_SOLUTIONS);
    EXPECT_FALSE(x.is_valid());
}

/*
    x + 2y = 3
    17x - 18y = -1
*/

TEST(LinearSystemsTesting, Float_OneSolution)
{
    fv_2 b(3.0f, -1.0f);
    fm_2x2 A(1.0f, 2.0f, 17.0f, -18.0f);
    fls_2x2 l(A, b);
    fv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::ONE_SOLUTION);
    EXPECT_TRUE(x.is_valid());

    EXPECT_FLOAT_EQ(x.get_a1(), 1.0f);
    EXPECT_FLOAT_EQ(x.get_a2(), 1.0f);
}

TEST(LinearSystemsTesting, Double_InfSolutions)
{
    dv_2 b(0.0, 0.0);
    dm_2x2 A(0.0, 0.0, 0.0, 0.0);
    dls_2x2 l(A, b);
    dv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::INF_SOLUTIONS);
    EXPECT_FALSE(x.is_valid());
}

TEST(LinearSystemsTesting, Double_NoSolutions)
{
    dv_2 b(1.0, 2.0);
    dm_2x2 A(1.0, 0.0, 1.0, 0.0);
    dls_2x2 l(A, b);
    dv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::NO_SOLUTIONS);
    EXPECT_FALSE(x.is_valid());
}


TEST(LinearSystemsTesting, Double_OneSolution)
{
    dv_2 b(3.0, -1.0);
    dm_2x2 A(1.0, 2.0, 17.0, -18.0);
    dls_2x2 l(A, b);
    dv_2 x;

    EXPECT_EQ(l.get_solution(x), square_linear_system_solution_t::ONE_SOLUTION);
    EXPECT_TRUE(x.is_valid());

    EXPECT_FLOAT_EQ(x.get_a1(), 1.0);
    EXPECT_FLOAT_EQ(x.get_a2(), 1.0);
}