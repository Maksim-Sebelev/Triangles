module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <cassert>

import compare;

//----------------------------------------------------------------------------------------------------------------------------

export module linear_systems;

//----------------------------------------------------------------------------------------------------------------------------

export import matrix;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Math
{

//----------------------------------------------------------------------------------------------------------------------------

namespace LinearAlgebra
{

//----------------------------------------------------------------------------------------------------------------------------

enum class square_linear_system_solution_t : char
{
    NO_SOLUTIONS ,
    ONE_SOLUTION ,
    INF_SOLUTIONS,
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
class linear_system_2x2_t
{
    static_assert(std::is_floating_point_v<number_t>,
        "maybe yoy dont need to linear system with integer coeffs??");

    using vector_2_t   = vector_2_t  <number_t>;
    using matrix_2x2_t = matrix_2x2_t<number_t>; 

    private:
        matrix_2x2_t A_;
        vector_2_t   b_;

    public:
        linear_system_2x2_t(const matrix_2x2_t& A, const vector_2_t& b);
    
        square_linear_system_solution_t get_solution     (vector_2_t& solution) const;
        square_linear_system_solution_t get_solution_type()                     const;
};

//---------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
linear_system_2x2_t<number_t>::linear_system_2x2_t(const matrix_2x2_t& A, const vector_2_t& b) :
A_(A),
b_(b)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
square_linear_system_solution_t
linear_system_2x2_t<number_t>::get_solution_type() const
{
    number_t detA   = A_.get_determinate();
    number_t deltaX = matrix_2x2_t(b_               , A_.get_column_2()).get_determinate();
    number_t deltaY = matrix_2x2_t(A_.get_column_1(), b_               ).get_determinate();

    if (Math::Compare::compare_with_null<number_t>(detA))
    {
        if (Math::Compare::compare_with_null<number_t>(deltaX) &&
            Math::Compare::compare_with_null<number_t>(deltaY)
        ) return square_linear_system_solution_t::INF_SOLUTIONS;

        return square_linear_system_solution_t::NO_SOLUTIONS;
    }

    return square_linear_system_solution_t::ONE_SOLUTION;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
square_linear_system_solution_t
linear_system_2x2_t<number_t>::get_solution(vector_2_t& solution) const
{
    number_t detA   = A_.get_determinate();
    number_t deltaX = matrix_2x2_t(b_               , A_.get_column_2()).get_determinate();
    number_t deltaY = matrix_2x2_t(A_.get_column_1(), b_               ).get_determinate();

    if (Math::Compare::compare_with_null<number_t>(detA))
    {
        solution.set_a1(std::numeric_limits<number_t>::quiet_NaN());
        solution.set_a2(std::numeric_limits<number_t>::quiet_NaN());

        if (Math::Compare::compare_with_null<number_t>(deltaX) &&
            Math::Compare::compare_with_null<number_t>(deltaY)
        ) return square_linear_system_solution_t::INF_SOLUTIONS;

        return square_linear_system_solution_t::NO_SOLUTIONS;
    }

    number_t x1 = deltaX / detA; assert(!std::isnan(x1));
    number_t x2 = deltaY / detA; assert(!std::isnan(x2));

    solution.set_a1(x1);
    solution.set_a2(x2);

    return square_linear_system_solution_t::ONE_SOLUTION;
}

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace LinearAlgebra */
} /* namespace Math */

//----------------------------------------------------------------------------------------------------------------------------
