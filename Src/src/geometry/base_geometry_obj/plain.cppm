module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <iostream>

#include "global.hpp"
#include "custom_console_output.hpp"

import constants;

import compare;
import matrix;

#if defined(USE_LOGGER)
import pineaplog;
#endif /*defined(USE_LOGGER)*/ 

//----------------------------------------------------------------------------------------------------------------------------

export module plain;

//----------------------------------------------------------------------------------------------------------------------------

export import point;
export import line;
export import relative_positions;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class plain_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
                "In plain_t as template type excpect only floating point type.");

    using coeff_t = coordinate_t;
    static_assert(std::is_same_v<coeff_t, coordinate_t>,
                  "this types must be equals");

    using point_t      =                      point_t     <coordinate_t>;
    using line_t       =                      line_t      <coordinate_t>; 
    using matrix_2x2_t = Math::LinearAlgebra::matrix_2x2_t<coordinate_t>;

    private:
        // ax + by + cz + d = 0
        coeff_t a_;
        coeff_t b_;
        coeff_t c_;
        coeff_t d_;

        point_t get_second_ref_point_for_intersection_of_plain(const plain_t& plain, const point_t& first_ref_point)                        const;

        void    det_1_not_null_in_intersection_of_plains      (const plain_t& plain, const coordinate_t& det1, line_t& intersection_line)   const;
        void    det_2_not_null_in_intersection_of_plains      (const plain_t& plain, const coordinate_t& det2, line_t& intersection_line)   const;
        void    det_3_not_null_in_intersection_of_plains      (const plain_t& plain, const coordinate_t& det3, line_t& intersection_line)   const;

    public:
        plain_t() = default;
        plain_t(const coeff_t& a, const coeff_t& b, const coeff_t& c, const coeff_t& d);
        plain_t(const point_t& a, const point_t& b, const point_t& c);

        coeff_t             get_a                                               ()                                                          const;
        coeff_t             get_b                                               ()                                                          const;
        coeff_t             get_c                                               ()                                                          const;
        coeff_t             get_d                                               ()                                                          const;

        bool                compare_with_another_plain                          (const plain_t<coordinate_t>& plain)                        const;
        bool                is_parallel_and_not_equal_to_another_plain          (const plain_t<coordinate_t>& plain)                        const;

        relative_position_t get_intersection_with_another_plain                 (const plain_t& plain, line_t& intersection_line = line_t())const;

        ON_LOGGER(
        std::string         get_dump                                            (std::string_view name)                                     const;
        )

};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
plain_t<coordinate_t>::plain_t(const coeff_t& a, const coeff_t& b, const coeff_t& c, const coeff_t& d) :
a_(a),
b_(b),
c_(c),
d_(d)
{}

//----------------------------------------------------------------------------------------------------------------------------


template <typename coordinate_t>
plain_t<coordinate_t>::plain_t(const point_t& a, const point_t& b, const point_t& c) :
//y₁(z₂ - z₃) + y₂(z₃ - z₁) + y₃(z₁ - z₂)
a_(
a.get_y() * (b.get_z() - c.get_z()) +
b.get_y() * (c.get_z() - a.get_z()) +
c.get_y() * (a.get_z() - b.get_z())
),
// b = x₁(z₃ - z₂) + x₂(z₁ - z₃) + x₃(z₂ - z₁)]
b_(
a.get_x() * (c.get_z() - b.get_z()) +
b.get_x() * (a.get_z() - c.get_z()) +
c.get_x() * (b.get_z() - a.get_z())
),
// c = x₁(y₂ - y₃) + x₂(y₃ - y₁) + x₃(y₁ - y₂)
c_(
a.get_x() * (b.get_y() - c.get_y()) +
b.get_x() * (c.get_y() - a.get_y()) +
c.get_x() * (a.get_y() - b.get_y())
),
// d = x₁(y₃z₂ - y₂z₃) + x₂(y₁z₃ - y₃z₁) + x₃(y₂z₁ - y₁z₂)]
d_(
a.get_x() * (c.get_y() * b.get_z() - b.get_y() * c.get_z()) +
b.get_x() * (a.get_y() * c.get_z() - c.get_y() * a.get_z()) +
c.get_x() * (b.get_y() * a.get_z() - a.get_y() * b.get_z())
)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
plain_t<coordinate_t>::compare_with_another_plain(const plain_t& plain) const
{
    return 
        Math::Compare::compare<coordinate_t>(a_ * plain.b_, plain.a_ * b_, static_cast<coordinate_t>(1e-1)) and
        Math::Compare::compare<coordinate_t>(a_ * plain.c_, plain.a_ * c_, static_cast<coordinate_t>(1e-1)) and
        Math::Compare::compare<coordinate_t>(a_ * plain.d_, plain.a_ * d_, static_cast<coordinate_t>(1e-1)) and
        Math::Compare::compare<coordinate_t>(b_ * plain.c_, plain.b_ * c_, static_cast<coordinate_t>(1e-1)) and
        Math::Compare::compare<coordinate_t>(b_ * plain.d_, plain.b_ * d_, static_cast<coordinate_t>(1e-1)) and
        Math::Compare::compare<coordinate_t>(c_ * plain.d_, plain.c_ * d_, static_cast<coordinate_t>(1e-1));
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
plain_t<coordinate_t>::get_intersection_with_another_plain(const plain_t& plain, line_t& intersection_line) const
{
    coordinate_t det1 = matrix_2x2_t(      a_,       b_,
                                     plain.a_, plain.b_).get_determinate();

    if (!Math::Compare::compare_with_null(det1))
    {
        det_1_not_null_in_intersection_of_plains(plain, det1, intersection_line);
        return relative_position_t::NORMAL_INTERSECTION;
    }

    coordinate_t det2 = matrix_2x2_t(      a_,       c_,
                                     plain.a_, plain.c_).get_determinate();

    if (!Math::Compare::compare_with_null(det2))
    {
        det_2_not_null_in_intersection_of_plains(plain, det2, intersection_line);
        return relative_position_t::NORMAL_INTERSECTION;
    }

    coordinate_t det3 = matrix_2x2_t(      b_,       c_,
                                     plain.b_, plain.c_).get_determinate();

    if (!Math::Compare::compare_with_null(det1))
    {
        det_3_not_null_in_intersection_of_plains(plain, det3, intersection_line);
        return relative_position_t::NORMAL_INTERSECTION;
    }

    intersection_line.made_invalid();

    if (compare_with_another_plain(plain))
        return relative_position_t::CONTAIN_OR_EQUAL;

    return relative_position_t::NO_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
plain_t<coordinate_t>::get_a() const
{
    return a_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
plain_t<coordinate_t>::get_b() const
{
    return b_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
plain_t<coordinate_t>::get_c() const
{
    return c_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
plain_t<coordinate_t>::get_d() const
{
    return d_;
}

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
plain_t<coordinate_t>::det_1_not_null_in_intersection_of_plains(const plain_t& plain, const coordinate_t& det1, line_t& intersection_line) const
{
    coordinate_t x = (b_ * plain.d_ - plain.b_ * d_) / det1;
    coordinate_t y = (plain.a_ * d_ - a_ * plain.d_) / det1;
    coordinate_t z = 0;

    point_t ref_point_1(x, y, z);
    point_t ref_point_2 = get_second_ref_point_for_intersection_of_plain(plain, ref_point_1);
    
    intersection_line = line_t(ref_point_1, ref_point_2);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
plain_t<coordinate_t>::det_2_not_null_in_intersection_of_plains(const plain_t& plain, const coordinate_t& det2, line_t& intersection_line) const
{
    coordinate_t x = (c_ * plain.d_ - plain.c_ * d_) / det2;
    coordinate_t y = 0;
    coordinate_t z = (plain.a_ * d_ - a_ * plain.d_) / det2;

    point_t ref_point_1(x, y, z);
    point_t ref_point_2 = get_second_ref_point_for_intersection_of_plain(plain, ref_point_1);
    
    intersection_line = line_t(ref_point_1, ref_point_2);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
plain_t<coordinate_t>::det_3_not_null_in_intersection_of_plains(const plain_t& plain, const coordinate_t& det3, line_t& intersection_line) const
{
    coordinate_t x = 0;
    coordinate_t y = (c_ * plain.d_ - plain.c_ * d_) / det3;
    coordinate_t z = (plain.b_ * d_ - b_ * plain.d_) / det3;

    point_t ref_point_1(x, y, z);
    point_t ref_point_2 = get_second_ref_point_for_intersection_of_plain(plain, ref_point_1);
    
    intersection_line = line_t(ref_point_1, ref_point_2);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
plain_t<coordinate_t>::get_second_ref_point_for_intersection_of_plain(const plain_t& plain, const point_t& first_ref_point) const
{
    coordinate_t vx = matrix_2x2_t(b_, plain.b_,
                                   c_, plain.c_).get_determinate();

    coordinate_t vy = matrix_2x2_t(c_, plain.c_,
                                   a_, plain.a_).get_determinate();

    coordinate_t vz = matrix_2x2_t(a_, plain.a_,
                                   b_, plain.b_).get_determinate();

    return point_t(
        first_ref_point.get_x() + vx,
        first_ref_point.get_y() + vy,
        first_ref_point.get_z() + vz
    );
}


// dump
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
plain_t<coordinate_t>::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    std::ostringstream f;
    
    bool is_a_0 = Math::Compare::compare_with_null(a_);
    bool is_b_0 = Math::Compare::compare_with_null(b_);
    bool is_c_0 = Math::Compare::compare_with_null(c_);
    bool is_d_0 = Math::Compare::compare_with_null(d_);

    bool are_all_coordinate_0 = is_a_0 and
                                is_b_0 and
                                is_c_0 and
                                is_d_0;

    if (!is_a_0)
        f << a_ << "x ";

    if (!is_b_0)
        f << ((b_ > 0) ? "+ " : "") << b_ << "y ";
        
    if (!is_c_0)
        f << ((c_ > 0) ? "+ " : "") << c_ << "z ";

    if (!is_d_0)
        f << ((d_ > 0) ? "+ " : "") << d_;

    f << ((are_all_coordinate_0) ? "all R^3" : " = 0");

    dump << "PLAIN "
         << name << "\n{" 
                    "\n\tA = " << a_ <<
                    "\n\tB = " << b_ <<
                    "\n\tC = " << c_ <<
                    "\n\tD = " << d_ <<
                    "\n\t" << f.str() <<
                    "\n}";

    return dump.str();
}
)

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
