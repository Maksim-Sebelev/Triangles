module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <array>

#include "global.hpp"

import line;
import plain;
import constants;
import relative_positions;

#if defined(USE_LOGGER)
import logger;
#endif // defined(USE_LOGGER)

//----------------------------------------------------------------------------------------------------------------------------

export module triangle;

//----------------------------------------------------------------------------------------------------------------------------

export import point;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class triangle_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
                  "In triangle_t as template type excpect only floating point type.");

    // is it cringe??
    using point_t   = point_t  <coordinate_t>;
    using line_t    = line_t   <coordinate_t>;
    using plain_t   = plain_t  <coordinate_t>;
    using side_t    = segment_t<coordinate_t>;
    using segment_t = segment_t<coordinate_t>;

    static_assert(std::is_same_v<segment_t, side_t>,
                  "this types must be equal");

    private:
        point_t a_vertex_;
        point_t b_vertex_;
        point_t c_vertex_;

        bool                is_intersect_with_another_triangle_on_same_plain       (const triangle_t& triangle)                          const;
        bool                is_intersect_with_another_triangle_on_different_plains (const triangle_t& triangle,
                                                                                    const plain_t& this_triangle_plain,
                                                                                    const plain_t& another_triangle_plain)               const;

        relative_position_t get_intersection_of_line_and_triangle_on_the_same_plain(const line_t& line, const side_t& side,
                                                                                    segment_t& intersection_segment)                     const;

        relative_position_t get_intersect_with_another_triangle_on_same_plain      (const triangle_t& triangle)                          const;
        relative_position_t get_intersect_with_another_triangle_on_different_plains(const triangle_t& triangle)                          const;

        relative_position_t get_intersection_with_line_on_the_same_plain           (const line_t& line, segment_t& intersection_segment) const;
        relative_position_t get_intersection_with_segment_on_the_same_plain        (const segment_t& segment,
                                                                                    segment_t& intersection_segment)                     const;

        bool                are_intersect_line_intersections_with_triangles        (const triangle_t& triangle, const line_t& line)      const;

        side_t              get_i_side                                             (size_t i)                                            const;

        plain_t             get_triangle_plain                                     ()                                                    const;

        bool                is_point                                               ()                                                    const;
        bool                is_line                                                (line_t& line)                                        const;

        bool                intersect_if_degenerate_triangles                      (const triangle_t& triangle, bool& intersect_result)  const;

        bool                is_point_in_triangle                                   (const point_t& point)                                const;

    public:
        triangle_t                                     () = default;
        triangle_t                                     (const point_t& a,
                                                        const point_t& b,
                                                        const point_t& c);

        triangle_t                                     (std::array<coordinate_t,
                                                        triangle_vertices_quant * dimension_of_space>
                                                        array_of_coordinates);

        coordinate_t get_a_x                           ()                               const;
        coordinate_t get_a_y                           ()                               const;
        coordinate_t get_a_z                           ()                               const;
        coordinate_t get_b_x                           ()                               const;
        coordinate_t get_b_y                           ()                               const;
        coordinate_t get_b_z                           ()                               const;
        coordinate_t get_c_x                           ()                               const;
        coordinate_t get_c_y                           ()                               const;
        coordinate_t get_c_z                           ()                               const;

        bool         is_valid                          ()                               const;

        bool         is_intersect_with_another_triangle(const triangle_t& triangle,
                                                        bool parse_degenerate_triangles
                                                             = false)                   const;

        ON_LOGGER(
        std::string  get_dump                          (std::string_view name)          const;
        )
};

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
triangle_t<coordinate_t>::triangle_t(const point_t& a, const point_t& b, const point_t& c) :
a_vertex_(a),
b_vertex_(b),
c_vertex_(c)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
triangle_t<coordinate_t>::triangle_t(std::array<coordinate_t, triangle_vertices_quant * dimension_of_space> array_of_coordinates) :
a_vertex_(array_of_coordinates[0], array_of_coordinates[1], array_of_coordinates[2]),
b_vertex_(array_of_coordinates[3], array_of_coordinates[4], array_of_coordinates[5]),
c_vertex_(array_of_coordinates[6], array_of_coordinates[7], array_of_coordinates[8])
{}

// public methods
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_valid() const
{
    if (!(
        a_vertex_.get_x().is_valid() &&
        a_vertex_.get_y().is_valid() &&
        a_vertex_.get_z().is_valid() &&
        b_vertex_.get_x().is_valid() &&
        b_vertex_.get_y().is_valid() &&
        b_vertex_.get_z().is_valid() &&
        c_vertex_.get_x().is_valid() &&
        c_vertex_.get_y().is_valid() &&
        c_vertex_.get_z().is_valid()
    )) return false;

    return
        !(a_vertex_.is_collenear(b_vertex_, c_vertex_));
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_a_x() const
{
    return a_vertex_.get_x();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_a_y() const
{
    return a_vertex_.get_y();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_a_z() const
{
    return a_vertex_.get_z();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_b_x() const
{
    return b_vertex_.get_x();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_b_y() const
{
    return b_vertex_.get_y();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_b_z() const
{
    return b_vertex_.get_z();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_c_x() const
{
    return c_vertex_.get_x();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_c_y() const
{
    return c_vertex_.get_y();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
triangle_t<coordinate_t>::get_c_z() const
{
    return c_vertex_.get_z();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_intersect_with_another_triangle(const triangle_t<coordinate_t>& triangle, bool parse_degenerate_triangles) const
{
    if (parse_degenerate_triangles)
    {
        bool intersect_triangles_result = false;
        if (intersect_if_degenerate_triangles(triangle, intersect_triangles_result))
            return intersect_triangles_result;
    }

    const plain_t    this_triangle_plain =          get_triangle_plain();
    const plain_t another_triangle_plain = triangle.get_triangle_plain();

    if (this_triangle_plain.compare_with_another_plain(another_triangle_plain))
        return is_intersect_with_another_triangle_on_same_plain(triangle);

    return
        is_intersect_with_another_triangle_on_different_plains(triangle,
                                                               this_triangle_plain,
                                                               another_triangle_plain);
}

// private methods
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_intersect_with_another_triangle_on_same_plain(const triangle_t& triangle) const
{
    side_t this_triangle_sides    [triangle_sides_quant];
    side_t another_triangle_sides [triangle_sides_quant];

    for (size_t i = 0; i < triangle_sides_quant; i++)
    {
           this_triangle_sides[i] =          get_i_side(i);
        another_triangle_sides[i] = triangle.get_i_side(i);
    }

    for (size_t i = 0; i < triangle_sides_quant; i++)
    {
        for (size_t j = 0; j < triangle_sides_quant; j++)
        {
            relative_position_t relative_position_of_triangles_i_and_j_sides =
                this_triangle_sides[i].get_intersection_type_with_another_segment(another_triangle_sides[j]);

            if (relative_position_of_triangles_i_and_j_sides ==
                relative_position_t::NO_INTERSECTION
            ) continue;

            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_intersect_with_another_triangle_on_different_plains(const triangle_t& triangle,
                                                                                 const    plain_t& this_triangle_plain,
                                                                                 const plain_t& another_triangle_plain) const
{
    msg_assert(!this_triangle_plain.compare_with_another_plain(another_triangle_plain), "in this functions we are parsing situation with not-equal plains");

    line_t              intersection_line;
    relative_position_t relative_position_of_triangles_plains = this_triangle_plain.get_intersection_with_another_plain(another_triangle_plain, intersection_line);

    if (relative_position_of_triangles_plains == relative_position_t::NO_INTERSECTION)
        return false; // plains are parallel

    return are_intersect_line_intersections_with_triangles(triangle, intersection_line);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::are_intersect_line_intersections_with_triangles(const triangle_t& triangle, const line_t& line) const
{
    segment_t intersection_segment_1;
    segment_t intersection_segment_2;

    return
        (                       get_intersection_with_line_on_the_same_plain        (line, intersection_segment_1) != relative_position_t::NO_INTERSECTION) &&
        (triangle              .get_intersection_with_line_on_the_same_plain        (line, intersection_segment_2) != relative_position_t::NO_INTERSECTION) &&
        (intersection_segment_1.get_intersection_type_with_another_collinear_segment(      intersection_segment_2) != relative_position_t::NO_INTERSECTION);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
segment_t<coordinate_t>
triangle_t<coordinate_t>::get_i_side(size_t i) const
{
    best_optimisation_ever_with_goto:
    switch (i)
    {
        case 0: return side_t(a_vertex_, b_vertex_);
        case 1: return side_t(b_vertex_, c_vertex_);
        case 2: return side_t(c_vertex_, a_vertex_);
        default:
            i = i % triangle_sides_quant;
            goto best_optimisation_ever_with_goto;
            // if 'i' have correct value, we dont need to calc i%3 :)
    }

    builtin_unreachable_wrapper("we must return in switch");
    return side_t();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
triangle_t<coordinate_t>::get_intersection_with_line_on_the_same_plain(const line_t& line, segment_t& intersection_segment) const
{
    side_t              sides                              [triangle_sides_quant];
    point_t             intersection_points                [triangle_sides_quant];
    relative_position_t relative_postions_of_sides_and_line[triangle_sides_quant];

    for (size_t i = 0; i < triangle_sides_quant; i++)
    {
        sides                              [i] = get_i_side(i);
        relative_postions_of_sides_and_line[i] = line.get_intersection_with_segment(sides[i], intersection_points[i]);
    }

    for (size_t i = 0; i < triangle_vertices_quant; i++)
    {
        if (relative_postions_of_sides_and_line[i] != relative_position_t::CONTAIN_OR_EQUAL)
            continue;;

        intersection_segment = segment_t(sides[i]);
        return relative_position_t::NORMAL_INTERSECTION;
    }

    for (size_t i = 0; i < triangle_vertices_quant; i++)
    {
        size_t i1 = i;
        size_t i2 = (i + 1) % triangle_vertices_quant;

        if (relative_postions_of_sides_and_line[i1] != relative_position_t::NORMAL_INTERSECTION ||
            relative_postions_of_sides_and_line[i2] != relative_position_t::NORMAL_INTERSECTION
        ) continue;

        size_t i3 = (i + 2) % triangle_vertices_quant;
        
        if (!intersection_points[i1].compare_with_another_point(intersection_points[i2]))
        {
            intersection_segment = segment_t(intersection_points[i1], intersection_points[i2]);
            return relative_position_t::NORMAL_INTERSECTION;
        }

        if (relative_postions_of_sides_and_line[i3] != relative_position_t::NORMAL_INTERSECTION)
        {
            intersection_segment = segment_t(intersection_points[i1], intersection_points[i1]); // intersection_segment - is a point
            return relative_position_t::NORMAL_INTERSECTION;
        }

        intersection_segment = segment_t(intersection_points[i1], intersection_points[i3]);
        return relative_position_t::NORMAL_INTERSECTION;
    }

    intersection_segment.made_invalid();
    return relative_position_t::NO_INTERSECTION;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
relative_position_t
triangle_t<coordinate_t>::get_intersection_with_segment_on_the_same_plain(const segment_t& segment, segment_t& intersection_segment) const
{
    const line_t line(segment);

    segment_t intersection_segment_of_line_and_trianle;

    relative_position_t relative_position_of_segment_line_and_triangle = 
        get_intersection_with_line_on_the_same_plain(line, intersection_segment_of_line_and_trianle);

    msg_assert(relative_position_of_segment_line_and_triangle != relative_position_t::CONTAIN_OR_EQUAL, 
        "triangle and segment can only have no intersection or have normal intersection"
    );

    if (relative_position_of_segment_line_and_triangle ==
        relative_position_t::NO_INTERSECTION
    ) return relative_position_t::NO_INTERSECTION;

    return
        segment.get_intersection_with_another_collinear_segment(intersection_segment_of_line_and_trianle,
                                                                intersection_segment);                                                            
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
plain_t<coordinate_t>
triangle_t<coordinate_t>::get_triangle_plain() const
{
    return plain_t(a_vertex_,
                   b_vertex_,
                   c_vertex_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_point() const
{
    return
        a_vertex_.compare_with_another_point(b_vertex_) &&
        a_vertex_.compare_with_another_point(c_vertex_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::is_line(line_t& line) const
{
    const bool is_ab_line = (!a_vertex_.compare_with_another_point(b_vertex_)           ) &&
                      ( c_vertex_.is_collenear              (a_vertex_, b_vertex_));
                    
    if (is_ab_line)
    {
        line = line_t(a_vertex_, b_vertex_);
        return true;
    }

    const bool is_ac_line = (!a_vertex_.compare_with_another_point(c_vertex_)          )  &&
                            ( b_vertex_.is_collenear              (a_vertex_, c_vertex_));
    if (is_ac_line)
    {
        line = line_t(a_vertex_, c_vertex_);
        return true;
    }

    line.made_invalid();
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
triangle_t<coordinate_t>::intersect_if_degenerate_triangles(const triangle_t& triangle, bool& intersect_result) const
{
    const bool is_this_point    =          is_point();
    const bool is_another_point = triangle.is_point();

    if (is_this_point && is_another_point)
    {
        intersect_result = a_vertex_.compare_with_another_point(triangle.a_vertex_);
        return true;
    }

    line_t another_line;
    const bool is_another_line = triangle.is_line(another_line);

    if (is_this_point && is_another_line)
    {
        intersect_result = another_line.is_point_on_line(a_vertex_);
        return true;
    }

    line_t this_line;
    const bool is_this_line = is_line(this_line);

    if (is_this_line && is_another_point)
    {
        intersect_result = this_line.is_point_on_line(triangle.a_vertex_);
        return true;
    }

    if (is_this_line && is_another_line)
    {
        intersect_result = (this_line.get_intersection_type_with_another_line(another_line) !=
                            relative_position_t::NO_INTERSECTION);
        return true;
    }

    return false;
} 


// DUMP
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
triangle_t<coordinate_t>::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    dump << "TRIANGLE " << name << "\n{\n\t"
         << a_vertex_.get_dump("A") << "\n\t"
         << b_vertex_.get_dump("B") << "\n\t"
         << c_vertex_.get_dump("C") << "\n"
         << get_triangle_plain().get_dump("of triangle")
         << "\n}\n";

    return dump.str();
}
)

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
