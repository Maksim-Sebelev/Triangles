module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <limits>

#include "constants.hpp"

#include "global.hpp"
#include "custom_console_output.hpp"


#if defined(USE_LOGGER)
import logger;
#endif /* defined(USE_LOGGER) */

//----------------------------------------------------------------------------------------------------------------------------

export module octotree;

//----------------------------------------------------------------------------------------------------------------------------

export import point;
export import triangle;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

using namespace Detail;

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class octo_tree_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
        "in octo_tree_t as template type excpect only floating-point type");

    private:
        struct bounding_box_t;

    using point_t    = point_t   <coordinate_t>;
    using triangle_t = triangle_t<coordinate_t>;

    struct triangle_number_box
    {
        triangle_t     triangle;
        bounding_box_t box     ;
        size_t         number  ;
    };

    static constexpr size_t       children_quant = 8;
    static constexpr coordinate_t min_box_size   = static_cast<coordinate_t>(1.0);

    private:
        std::vector<triangle_number_box> triangles_numbers_boxes_;
        std::vector<bool>                triangles_that_have_intersections_with_others_;
        // std::vector

        octo_tree_t*   parent_                  ;
        octo_tree_t*   children_[children_quant];
        bounding_box_t box_                     ;

        octo_tree_t(const octo_tree_t* parent);

        void update_main_box_in_ctor(const bounding_box_t& new_triangle_box);

        void made_from_box_a_square ();

        void build_tree             (size_t node_number_as_parents_child, const octo_tree_t* parent);

    public:
        octo_tree_t(const std::vector<triangle_t> triangles);
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octo_tree_t<coordinate_t>::octo_tree_t(const std::vector<triangle_t> triangles) :
parent_(nullptr)
{
    const size_t triangles_quant = triangles.size();

    if (triangles_quant == 0)
        return;

    triangles_numbers_boxes_                      .reserve(triangles_quant);
    triangles_that_have_intersections_with_others_.reserve(triangles_quant);

    for (size_t i = 0; i < triangles_quant; i++)
    {
        const triangle_t     i_triangle        = triangles[i];
        const bounding_box_t box_of_i_triangle = bounding_box_t(triangles[i]);
    
        triangles_numbers_boxes_[i].triangle  = i_triangle;
        triangles_numbers_boxes_[i].box       = box_of_i_triangle;
        triangles_numbers_boxes_[i].number    = i;

        update_main_box_in_ctor(box_of_i_triangle);
    }

    made_from_box_a_square();

    build_tree(nullptr);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octo_tree_t<coordinate_t>::build_tree(size_t node_number_as_parents_child, const octo_tree_t* parent)
{
    msg_assert(box_.is_valid(), "inavalid box");

    switch (node_number_as_parents_child)
    {
        

    }

}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octo_tree_t<coordinate_t>::update_main_box_in_ctor(const bounding_box_t& new_triangle_box)
{
    point_t new_bl_point =
    {
        std::min(box_.get_bottom_left().get_x_coordinate(), new_triangle_box.get_bottom_left().get_x_coordinate()),
        std::min(box_.get_bottom_left().get_y_coordinate(), new_triangle_box.get_bottom_left().get_y_coordinate()),
        std::min(box_.get_bottom_left().get_z_coordinate(), new_triangle_box.get_bottom_left().get_z_coordinate()),
    };

    point_t new_tp_point =
    {
        std::max(box_.get_top_right().get_x_coordinate(), new_triangle_box.get_top_right().get_x_coordinate()),
        std::max(box_.get_top_right().get_y_coordinate(), new_triangle_box.get_top_right().get_y_coordinate()),
        std::max(box_.get_top_right().get_z_coordinate(), new_triangle_box.get_top_right().get_z_coordinate()),
    };

    box_.set_bottom_left(new_bl_point);
    box_.set_top_right  (new_tp_point);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octo_tree_t<coordinate_t>::made_from_box_a_square()
{
    point_t      lb       = box_.get_bottom_left();
    point_t      tr       = box_.get_top_right  ();

    coordinate_t lb_x     = lb.get_x();
    coordinate_t lb_y     = lb.get_y();
    coordinate_t lb_z     = lb.get_z();
    coordinate_t tr_x     = tr.get_x();
    coordinate_t tr_y     = tr.get_y();
    coordinate_t tr_z     = tr.get_z();

    coordinate_t x_center = (lb_x() + tr_x()) / 2;
    coordinate_t y_center = (lb_y() + tr_y()) / 2;
    coordinate_t z_center = (lb_z() + tr_z()) / 2;

    coordinate_t half_max_box_side_length = std::max(tr_x - lb_x,
                                                tr_y - lb_y,
                                                tr_z - lb_z) / 2;

    lb = point_t
    (
        x_center - half_max_box_side_length,
        y_center - half_max_box_side_length,
        z_center - half_max_box_side_length
    );

    tr = point_t
    (
        x_center + half_max_box_side_length,
        y_center + half_max_box_side_length,
        z_center + half_max_box_side_length
    );

    box_ = bounding_box_t(lb, tr);
}

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
struct
octo_tree_t<coordinate_t>::bounding_box_t
{
    private:
        point_t bottom_left_point_;
        point_t   top_right_point_;

    public:
        bounding_box_t();
        bounding_box_t(const triangle_t& triangle);
        bounding_box_t(const point_t& bottom_left_point,
                       const point_t&   top_right_point);

        bool    is_valid         ()                                  const;

        bool    is_intersect     (const bounding_box_t& another_box) const;
        bool    is_inside        (const bounding_box_t& another_box) const;
        bool    is_contain       (const bounding_box_t& another_box) const;

        point_t get_bottom_left  ()                                  const;
        point_t get_top_right    ()                                  const;

        void    set_bottom_left  (const point_t& value);
        void    set_top_right    (const point_t& value);
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octo_tree_t<coordinate_t>::bounding_box_t::bounding_box_t() :
bottom_left_point_(std::numeric_limits<coordinate_t>::infinity(),
                   std::numeric_limits<coordinate_t>::infinity(),
                   std::numeric_limits<coordinate_t>::infinity()),

  top_right_point_(-std::numeric_limits<coordinate_t>::infinity(),
                   -std::numeric_limits<coordinate_t>::infinity(),
                   -std::numeric_limits<coordinate_t>::infinity())
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octo_tree_t<coordinate_t>::bounding_box_t::bounding_box_t(const point_t& bottom_left_point,
                                                          const point_t&   top_right_point) :
bottom_left_point_(bottom_left_point),
  top_right_point_(top_right_point)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octo_tree_t<coordinate_t>::bounding_box_t::bounding_box_t(const triangle_t& triangle)
{
    coordinate_t x_coordinates[triangle_vertices_quant] =
    {
        triangle.get_a_x(),
        triangle.get_b_x(),
        triangle.get_c_x(),
    };

    coordinate_t y_coordinates[triangle_vertices_quant] =
    {
        triangle.get_a_y(),
        triangle.get_b_y(),
        triangle.get_c_y(),
    };

    coordinate_t z_coordinates[triangle_vertices_quant] =
    {
        triangle.get_a_z(),
        triangle.get_b_z(),
        triangle.get_c_z(),
    };

    coordinate_t min_x_coordinate = *(std::min_element(x_coordinates, x_coordinates + triangle_vertices_quant));
    coordinate_t min_y_coordinate = *(std::min_element(y_coordinates, y_coordinates + triangle_vertices_quant));
    coordinate_t min_z_coordinate = *(std::min_element(z_coordinates, z_coordinates + triangle_vertices_quant));

    coordinate_t max_x_coordinate = *(std::max_element(x_coordinates, x_coordinates + triangle_vertices_quant));
    coordinate_t max_y_coordinate = *(std::max_element(y_coordinates, y_coordinates + triangle_vertices_quant));
    coordinate_t max_z_coordinate = *(std::max_element(z_coordinates, z_coordinates + triangle_vertices_quant));

    point_t bottom_left_point = point_t(min_x_coordinate,
                                        min_y_coordinate,
                                        min_z_coordinate);

    point_t top_right_point   = point_t(max_x_coordinate,
                                        max_y_coordinate,
                                        max_z_coordinate);

    return bounding_box_t(bottom_left_point,
                            top_right_point);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octo_tree_t<coordinate_t>::bounding_box_t::is_valid() const
{
    bool is_points_valid = bottom_left_point_.is_valid() &&
                             top_right_point_.is_valid();
    if (!is_points_valid)
        return false;

    coordinate_t len_x = top_right_point_.get_x_coordinate() - bottom_left_point_.get_x_coordinate();
    coordinate_t len_y = top_right_point_.get_y_coordinate() - bottom_left_point_.get_y_coordinate();
    coordinate_t len_z = top_right_point_.get_z_coordinate() - bottom_left_point_.get_z_coordinate();

    bool is_square = (len_x == len_y) &&
                     (len_x == len_z) &&
                     (len_x >= 0);

    return is_square;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octo_tree_t<coordinate_t>::bounding_box_t::is_intersect(const bounding_box_t& box) const
{
    msg_assert(is_valid()    , "invalid box");
    msg_assert(box.is_valid(), "invalid box");

    return
            bottom_left_point_.is_between_2_points(box.bottom_left_point_, box.top_right_point_) ||
              top_right_point_.is_between_2_points(box.bottom_left_point_, box.top_right_point_) ||
        box.bottom_left_point_.is_between_2_points(    bottom_left_point_,     top_right_point_) ||
        box.  top_right_point_.is_between_2_points(    bottom_left_point_,     top_right_point_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octo_tree_t<coordinate_t>::bounding_box_t::is_inside(const bounding_box_t& box) const
{
    msg_assert(is_valid()    , "invalid box");
    msg_assert(box.is_valid(), "invalid box");

    return
        bottom_left_point_.is_between_2_points(box.bottom_left_point_, box.top_right_point_) &&
          top_right_point_.is_between_2_points(box.bottom_left_point_, box.top_right_point_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octo_tree_t<coordinate_t>::bounding_box_t::is_contain(const bounding_box_t& box) const
{
    msg_assert(is_valid()    , "invalid box");
    msg_assert(box.is_valid(), "invalid box");

    return
        box.bottom_left_point_.is_between_2_points(bottom_left_point_, top_right_point_) &&
        box.  top_right_point_.is_between_2_points(bottom_left_point_, top_right_point_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
octo_tree_t<coordinate_t>::bounding_box_t::get_bottom_left() const
{
    return bottom_left_point_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>
octo_tree_t<coordinate_t>::bounding_box_t::get_top_right() const
{
    return top_right_point_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octo_tree_t<coordinate_t>::bounding_box_t::set_bottom_left(const point_t& value)
{
    bottom_left_point_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octo_tree_t<coordinate_t>::bounding_box_t::set_top_right(const point_t& value)
{
    top_right_point_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
