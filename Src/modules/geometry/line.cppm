module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>

//----------------------------------------------------------------------------------------------------------------------------

#include "global.hpp"
#include "constants.hpp"
#include "relative_positions.hpp"

import compare;
import linear_systems;

#if defined(USE_LOGGER)
import logger;
#include <sstream>
#include <string>
#endif // defined(USE_LOGGER)

//----------------------------------------------------------------------------------------------------------------------------

export module line;

//----------------------------------------------------------------------------------------------------------------------------

export import point;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

namespace Detail
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t> class line_t;
template <typename coordinate_t> class segment_t;

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class line_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
                "In line_t as template type excpect only floating point type.");

    using point_t                         =                      point_t            <coordinate_t>;
    using segment_t                       =                      segment_t          <coordinate_t>;

    using vector_2_t                      = Math::LinearAlgebra::vector_2_t         <coordinate_t>;
    using matrix_2x2_t                    = Math::LinearAlgebra::matrix_2x2_t       <coordinate_t>;
    using linear_system_2x2_t             = Math::LinearAlgebra::linear_system_2x2_t<coordinate_t>;

    using square_linear_system_solution_t = Math::LinearAlgebra::square_linear_system_solution_t;

    private:
        point_t ref_point_1_;
        point_t ref_point_2_;

        void set_intersection_point(point_t& intersection_point, const vector_2_t& solution) const;

    public:
        line_t() = default;
        line_t(const point_t& p1, const point_t& p2);
        line_t(const segment_t& segment);

        bool                is_valid                          ()                                                           const;

        point_t             get_first_ref_point               ()                                                           const;
        point_t             get_second_ref_point              ()                                                           const;

        bool                compare_with_another_line         (const line_t& compare_line)                                 const;
        bool                is_point_on_line                  (const point_t& point)                                       const;

        relative_position_t get_intersection_with_another_line(const line_t   & another_line, point_t& intersection_point) const;
        relative_position_t get_intersection_with_segment     (const segment_t& segment     , point_t& intersection_point) const;

        void                made_invalid                      ();

        ON_LOGGER(
        std::string         get_dump                          (std::string_view name)                                      const;
        )
};

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
line_t<coordinate_t>::line_t(const point_t& p1, const point_t& p2) :
ref_point_1_(p1),
ref_point_2_(p2)
{}


//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
line_t<coordinate_t>::line_t(const segment_t& segment) :
ref_point_1_(segment.get_begin_point()),
ref_point_2_(segment.get_end_point  ())
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
line_t<coordinate_t>::is_valid() const
{
    return (
        ref_point_1_.is_valid() &&
        ref_point_2_.is_valid() &&
        !ref_point_1_.compare_with_another_point(ref_point_2_)
    );
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
line_t<coordinate_t>::get_first_ref_point() const
{
    return ref_point_1_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
line_t<coordinate_t>::get_second_ref_point() const
{
    return ref_point_2_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
line_t<coordinate_t>::compare_with_another_line(const line_t& line) const
{
    const coordinate_t a11 =      ref_point_2_.get_x_coordinate() -      ref_point_1_.get_x_coordinate();
    const coordinate_t a12 = line.ref_point_1_.get_x_coordinate() - line.ref_point_2_.get_x_coordinate();

    const coordinate_t b1  = line.ref_point_1_.get_x_coordinate() -      ref_point_1_.get_x_coordinate();

    const coordinate_t a21 =      ref_point_2_.get_y_coordinate() -      ref_point_1_.get_y_coordinate();
    const coordinate_t a22 = line.ref_point_1_.get_y_coordinate() - line.ref_point_2_.get_y_coordinate();

    const coordinate_t b2  = line.ref_point_1_.get_y_coordinate() -      ref_point_1_.get_y_coordinate();

    const matrix_2x2_t        A(a11, a12, a21, a22);
    const vector_2_t          b(b1, b2);
    const linear_system_2x2_t linear_system(A, b);

    square_linear_system_solution_t solution_type = linear_system.get_solution_type();

    if (solution_type != square_linear_system_solution_t::INF_SOLUTIONS)
        return false;

    const coordinate_t a31 =        ref_point_2_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();
    const coordinate_t a32 = line.ref_point_1_.get_z_coordinate() - line  .ref_point_2_.get_z_coordinate();
    
    const coordinate_t b3  = line.ref_point_1_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();
    
    const matrix_2x2_t        A_2(a11, a12, a31, a32);
    const vector_2_t          b_2(b1, b3);

    const linear_system_2x2_t linear_system_2(A_2, b_2);

    solution_type = linear_system_2.get_solution_type();

    return
        (solution_type == square_linear_system_solution_t::INF_SOLUTIONS);
}


//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
line_t<coordinate_t>::get_intersection_with_another_line(const line_t& line, point_t& intersection_point) const
{
    const coordinate_t a11 =      ref_point_2_.get_x_coordinate() -      ref_point_1_.get_x_coordinate();
    const coordinate_t a12 = line.ref_point_1_.get_x_coordinate() - line.ref_point_2_.get_x_coordinate();

    const coordinate_t b1  = line.ref_point_1_.get_x_coordinate() -      ref_point_1_.get_x_coordinate();

    const coordinate_t a21 =      ref_point_2_.get_y_coordinate() -      ref_point_1_.get_y_coordinate();
    const coordinate_t a22 = line.ref_point_1_.get_y_coordinate() - line.ref_point_2_.get_y_coordinate();

    const coordinate_t b2  = line.ref_point_1_.get_y_coordinate() -      ref_point_1_.get_y_coordinate();

    const matrix_2x2_t        A(a11, a12, a21, a22);
    const vector_2_t          b(b1, b2);
    const linear_system_2x2_t linear_system(A, b);

    vector_2_t                        solution;
    square_linear_system_solution_t   solution_type = linear_system.get_solution(solution);

    switch (solution_type)
    {
        case square_linear_system_solution_t::NO_SOLUTIONS:
            intersection_point.made_invalid();
            return relative_position_t::NO_INTERSECTION;

        case square_linear_system_solution_t::INF_SOLUTIONS:
        {
            const coordinate_t a31 =      ref_point_2_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();
            const coordinate_t a32 = line.ref_point_1_.get_z_coordinate() - line.ref_point_2_.get_z_coordinate();
            
            const coordinate_t b3  = line.ref_point_1_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();
            
            const matrix_2x2_t        A_2(a11, a12, a31, a32);
            const vector_2_t          b_2(b1, b3);
            const linear_system_2x2_t linear_system2(A_2, b_2);

            solution_type = linear_system2.get_solution(solution);

            switch (solution_type)
            {
                case square_linear_system_solution_t::NO_SOLUTIONS:
                    intersection_point.made_invalid();
                    return relative_position_t::NO_INTERSECTION;
                
                case square_linear_system_solution_t::INF_SOLUTIONS:
                    intersection_point.made_invalid();
                    return relative_position_t::CONTAIN_OR_EQUAL;

                case square_linear_system_solution_t::ONE_SOLUTION:
                {
                    set_intersection_point(intersection_point, solution);
                    return relative_position_t::NORMAL_INTERSECTION;
                }

                default: builtin_unreachable_wrapper("undefined solution type. maybe you forgot to add new value of square_linear_system_solution_t");
            }

            builtin_unreachable_wrapper("we must return in switch");
        }
        case square_linear_system_solution_t::ONE_SOLUTION:
            break;

        default: builtin_unreachable_wrapper("undefined solution type. maybe you forgot to add new value of square_linear_system_solution_t");
    }

    const coordinate_t a31 =      ref_point_2_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();
    const coordinate_t a32 = line.ref_point_1_.get_z_coordinate() - line.ref_point_2_.get_z_coordinate();
            
    const coordinate_t b3  = line.ref_point_1_.get_z_coordinate() -      ref_point_1_.get_z_coordinate();

    vector_2_t third_equation(a31, a32);

    if (!Math::Compare::compare<coordinate_t>
        (third_equation.get_mul_by_another_vector(solution), b3, static_cast<coordinate_t>(1e-3)))
    {
        intersection_point.made_invalid();
        return relative_position_t::NO_INTERSECTION;
    }

    set_intersection_point(intersection_point, solution);

    return relative_position_t::NORMAL_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
line_t<coordinate_t>::get_intersection_with_segment(const segment_t& segment, point_t& intersection_point) const
{
    line_t line(segment);

    relative_position_t relative_posion_of_lines = get_intersection_with_another_line(line, intersection_point);

    switch (relative_posion_of_lines)
    {
        case relative_position_t::NO_INTERSECTION:     return relative_position_t::NO_INTERSECTION;
        case relative_position_t::CONTAIN_OR_EQUAL:    return relative_position_t::CONTAIN_OR_EQUAL;
        case relative_position_t::NORMAL_INTERSECTION: break;
        default: builtin_unreachable_wrapper("undef relative position type. maybe you forgot to add new value in switch");
    }

    if (intersection_point.is_between_2_points(segment.get_begin_point(), segment.get_end_point()))
        return relative_position_t::NORMAL_INTERSECTION;

    intersection_point.made_invalid();
    return relative_position_t::NO_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
line_t<coordinate_t>::set_intersection_point(point_t& intersection_point, const vector_2_t& solution) const
{
    coordinate_t solution_a1 = solution.get_a1();

    const coordinate_t solution_x = ref_point_1_.get_x_coordinate() +
                                    (ref_point_2_.get_x_coordinate() - ref_point_1_.get_x_coordinate()) *
                                     solution_a1;

    const coordinate_t solution_y = ref_point_1_.get_y_coordinate() +
                                   (ref_point_2_.get_y_coordinate() - ref_point_1_.get_y_coordinate()) *
                                     solution_a1;
    
    const coordinate_t solution_z = ref_point_1_.get_z_coordinate() +
                                   (ref_point_2_.get_z_coordinate() - ref_point_1_.get_z_coordinate()) *
                                     solution_a1;

    intersection_point.set_x(solution_x);
    intersection_point.set_y(solution_y);
    intersection_point.set_z(solution_z);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
line_t<coordinate_t>::is_point_on_line(const point_t& point) const
{
    return point.is_on_1_line_with_2_points(ref_point_1_, ref_point_2_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
line_t<coordinate_t>::made_invalid()
{
    ref_point_1_.made_invalid();
    ref_point_2_.made_invalid();
}

// line dump
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
line_t<coordinate_t>::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    dump << "LINE "
         << name << "\n{\n\t" 
                 << ref_point_1_.get_dump("P1") << "\n\t"
                 << ref_point_2_.get_dump("P2") << "\n}";

    return dump.str();
}
)


//----------------------------------------------------------------------------------------------------------------------------

// segment
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class segment_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
                "In segment_t as template type excpect only floating point type.");

    using point_t      =                      point_t     <coordinate_t>;
    using vector_2_t   = Math::LinearAlgebra::vector_2_t  <coordinate_t>;
    using matrix_2x2_t = Math::LinearAlgebra::matrix_2x2_t<coordinate_t>;

    private:
        point_t begin_point_;
        point_t   end_point_;

        // return true if intersection is alredy done
        bool set_collinear_segments_intersections_points(const point_t& set_point, segment_t& intersection_segment) const;

    public:
        segment_t                                                               () = default;
        segment_t                                                               (const point_t& begin_point,
                                                                                 const point_t& end_point);

        point_t             get_begin_point                                     ()                                const;
        point_t             get_end_point                                       ()                                const;

        bool                is_valid                                            ()                                const;
        bool                is_point                                            ()                                const;

        bool                is_point_on_segment                                 (const point_t& point)            const;
        bool                is_point_on_same_line_with_segment_on_segment       (const point_t& point)            const;
        relative_position_t get_intersection_with_another_segment               (const segment_t& segment,
                                                                                 point_t& intersection_point)     const;
        relative_position_t get_intersection_type_with_another_segment          (const segment_t& segment)        const;
        relative_position_t get_intersection_type_with_another_collinear_segment(const segment_t& segment)        const;
        relative_position_t get_intersection_with_another_collinear_segment     (const segment_t& segment, 
                                                                                 segment_t& intersection_segment) const;
        relative_position_t get_intersection_type_with_segment_on_the_same_plain(const segment_t& segment)        const;

        void                set_begin_point                                     (const point_t& value);
        void                set_end_point                                       (const point_t& value);
        void                made_invalid                                        ();

        ON_LOGGER(
        std::string         get_dump                                            (std::string_view name)           const;
        )
};

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
segment_t<coordinate_t>::segment_t(const point_t& begin_point, const point_t& end_point) :
begin_point_(begin_point),
end_point_  (end_point)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
segment_t<coordinate_t>::get_begin_point() const
{
    return begin_point_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
segment_t<coordinate_t>::get_end_point() const
{
    return end_point_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
segment_t<coordinate_t>::is_valid() const
{
    return begin_point_.is_valid() &&
           end_point_  .is_valid();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
segment_t<coordinate_t>::is_point() const
{
    return begin_point_.compare_with_another_point(end_point_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
segment_t<coordinate_t>::is_point_on_segment(const point_t& point) const
{
    return
        point.is_on_1_line_with_2_points(begin_point_, end_point_) &&
        point.is_between_2_points       (begin_point_, end_point_);
}
 
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
segment_t<coordinate_t>::is_point_on_same_line_with_segment_on_segment(const point_t& point) const
{
    return point.is_between_2_points(begin_point_,
                                       end_point_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
segment_t<coordinate_t>::get_intersection_with_another_segment(const segment_t& segment, point_t& intersection_point) const
{
    line_t line_1(*this);
    line_t line_2(segment);

    relative_position_t lines_relative_position = line_1.get_intersection_with_another_line(line_2, intersection_point);

    switch (lines_relative_position)
    {
        case relative_position_t::NO_INTERSECTION:
            return relative_position_t::NO_INTERSECTION;

        case relative_position_t::NORMAL_INTERSECTION:
        {
            bool flag =
                intersection_point.is_between_2_points(        begin_point_,         end_point_) &&
                intersection_point.is_between_2_points(segment.begin_point_, segment.end_point_);

            if (flag)
                return relative_position_t::NORMAL_INTERSECTION;

            intersection_point.made_invalid();
            return relative_position_t::NO_INTERSECTION;
        }

        case relative_position_t::CONTAIN_OR_EQUAL:
        {
            bool flag =
                begin_point_.is_between_2_points(segment.begin_point_, segment.end_point_) &&
                end_point_  .is_between_2_points(segment.begin_point_, segment.end_point_);

            if (flag)
                return relative_position_t::CONTAIN_OR_EQUAL;

            return relative_position_t::NO_INTERSECTION;
        }

        default: builtin_unreachable_wrapper("undefined relative_position_t type. maybe you forgot to add new value");
    }

    builtin_unreachable_wrapper("we must return in switch");
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
segment_t<coordinate_t>::get_intersection_type_with_another_segment(const segment_t& segment) const
{
    line_t line_1(*this);
    line_t line_2(segment);

    point_t intersection_point;

    relative_position_t lines_relative_position = line_1.get_intersection_with_another_line(line_2, intersection_point);

    switch (lines_relative_position)
    {
        case relative_position_t::NO_INTERSECTION:
            return relative_position_t::NO_INTERSECTION;

        case relative_position_t::NORMAL_INTERSECTION:
        {
            if (intersection_point.is_between_2_points(        begin_point_,         end_point_) &&
                intersection_point.is_between_2_points(segment.begin_point_, segment.end_point_)
            ) return relative_position_t::NORMAL_INTERSECTION;

            return relative_position_t::NO_INTERSECTION;
        }

        case relative_position_t::CONTAIN_OR_EQUAL:
            return get_intersection_type_with_another_collinear_segment(segment);

        default: builtin_unreachable_wrapper("undefined relative_position_t type. maybe you forgot to add new value");
    }

    builtin_unreachable_wrapper("we must return in switch");
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
segment_t<coordinate_t>::get_intersection_type_with_another_collinear_segment(const segment_t& segment) const
{

    bool are_segment_intersected = 
                begin_point_.is_between_2_points(segment.begin_point_, segment.end_point_) ||
                  end_point_.is_between_2_points(segment.begin_point_, segment.end_point_) ||
        segment.begin_point_.is_between_2_points(        begin_point_,         end_point_) ||
        segment.  end_point_.is_between_2_points(        begin_point_,         end_point_);

    if (are_segment_intersected) 
        return relative_position_t::CONTAIN_OR_EQUAL;

    return relative_position_t::NO_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------

#define set_needed_point_and_return_if_intersection_is_alredy_find(now_point, intersection_segment) do { \
if (set_collinear_segments_intersections_points(now_point, intersection_segment))                         \
    return relative_position_t::CONTAIN_OR_EQUAL;                                                          \
} while (0)

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
segment_t<coordinate_t>::get_intersection_with_another_collinear_segment(const segment_t& segment, segment_t& intersection_segment) const
{
    if (begin_point_.is_between_2_points(segment.begin_point_, segment.end_point_))
        intersection_segment.begin_point_ = begin_point_;

    if (end_point_.is_between_2_points(segment.begin_point_, segment.end_point_))
        set_needed_point_and_return_if_intersection_is_alredy_find(end_point_, intersection_segment);

    if (segment.begin_point_.is_between_2_points(begin_point_, end_point_))
        set_needed_point_and_return_if_intersection_is_alredy_find(segment.begin_point_, intersection_segment);

    if (segment.end_point_.is_between_2_points(begin_point_, end_point_))
        set_needed_point_and_return_if_intersection_is_alredy_find(segment.end_point_, intersection_segment);

    intersection_segment.made_invalid();
    return relative_position_t::NO_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------

#undef set_needed_point_and_return_if_intersection_is_alredy_find

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
segment_t<coordinate_t>::set_begin_point(const point_t& value)
{
    begin_point_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
segment_t<coordinate_t>::set_end_point(const point_t& value)
{
    end_point_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
segment_t<coordinate_t>::made_invalid()
{
    begin_point_.made_invalid();
      end_point_.made_invalid();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
segment_t<coordinate_t>::set_collinear_segments_intersections_points(const point_t& set_point, segment_t& intersection_segment) const
{
    if (intersection_segment.begin_point_.is_valid())
    {
        intersection_segment.end_point_ = set_point;
        return true;
    }

    intersection_segment.begin_point_ = set_point;
    return false;
}

// dump
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
segment_t<coordinate_t>::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    dump << "SEGMENT "
         << name << "\n{\n\t" 
         << begin_point_.get_dump("A") << ",\n\t"
         <<   end_point_.get_dump("B") << "\n}";

    return dump.str();
}
)

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Dateil */
} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
