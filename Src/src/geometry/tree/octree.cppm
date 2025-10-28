module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <limits>
#include <array>
#include <memory>

#include "global.hpp"
#include "custom_console_output.hpp"

import compare;
import constants;

#if defined(USE_LOGGER)
import logger;
#endif /* defined(USE_LOGGER) */

#if defined(DUMP_2D)
#include <filesystem>
#endif /* defined(DUMP_2D) */

//----------------------------------------------------------------------------------------------------------------------------

export module octree;

//----------------------------------------------------------------------------------------------------------------------------

export import point;
export import triangle;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class octree_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
        "in octree_t as template type excpect only floating-point type");

    private:
        struct bounding_box_t;
        struct node_t;

    using point_t    = point_t   <coordinate_t>;
    using triangle_t = triangle_t<coordinate_t>;

    using triangle_and_box = std::pair<triangle_t, bounding_box_t>;

    static constexpr size_t       children_quant       = 8;

    private:
        size_t                        triangles_quantity_                                      ;
        std::vector<triangle_and_box> triangles_and_boxes_                                     ;
        std::vector<bool>             numbers_of_triangles_that_have_intersections_with_others_;
        std::unique_ptr<node_t>       root_                                                    ;
        coordinate_t                  min_bbox_side_length_                                    ;
        bool                          bad_triangles_                                           ;

        void                          update_min_bbox_side_length_in_ctor       (const bounding_box_t& new_triangle_box);
    
        void                          update_all_space_box_in_ctor              (bounding_box_t& all_space_box, const bounding_box_t& new_triangle_box);
        void                          made_from_box_a_square                    (bounding_box_t& bbox);

        void                          build_tree                                (size_t node_number_as_parents_child = 0);

        bool                          dont_need_to_split_node                   (const node_t* node) const;
        void                          split_node                                (node_t* node);

        void                          empty_child_action                        (node_t* node, size_t child_it);
        void                          child_contain_triangle_bbox_action        (node_t* node, size_t triangle_number, size_t triangle_it, size_t child_it, bool& is_child_empty);
        void                          remove_i_triangle_from_node               (node_t* node, size_t triangle_it);
        void                          add_triangle_to_i_node_child              (node_t* node, size_t triangle_number, size_t child_it);

        std::array<bounding_box_t,
                   children_quant>    split_bbox                                (const bounding_box_t& bbox);
     
        void                          find_intersections_helper                 (const node_t* node);
        void                          find_intersections_with_children          (const node_t* node);
        void                          find_intersections_with_children_helper   (const node_t* node, const node_t* child);
        void                          find_intersections_in_node                (const node_t* node);
        void                          parse_result_of_triagles_intersection     (size_t triangle_1_index, size_t triangle_2_index);
        void                          intersect_2_nodes_triangles               (const node_t* node_1, const node_t* node_2);

        ON_LOGGER(
        void dump(node_t* node, size_t h = 0)const;
        ) /* ON_LOGGER */

        ON_2D_DUMP(
        void        make_dump                   (std::ofstream& dot)                                          const;
        void        node_dump                   (std::ofstream& dot, const node_t* node, size_t num_as_child) const;
        void        create_edges                (std::ofstream& dot, const node_t* node)                      const;
        std::string get_node_color              (size_t num_as_child)                                         const;
        std::string get_node_data               (const node_t* node, size_t num_as_child)                     const;
        void        create_dir                  (const std::string& dir)                                      const;
        std::string get_png_file_path           (std::string_view img_name, const std::string& img_dir)       const;
        std::string get_dot_file_path           (std::string_view dot_name, const std::string& dot_out_dir)   const;
        void       create_image_from_dot        (std::string_view dot_file, std::string_view img_file)        const;
        void       failed_open_dot_file_message (std::string_view img_name)                                   const;
        ) /* ON_2D_DUMP */

    public:
        octree_t(const std::vector<triangle_t>& triangles, bool bad_triangles = false);

        void                find_intersections();                              
        std::vector<size_t> get_good_triangles()                               const;
        ON_LOGGER(
        void                dump              ()                               const;
        ) /* ON_LOGGER */
        ON_2D_DUMP(
        void                dump_2d           (std::string_view img_name = "") const;
        ) /* ON_2D_DUMP */

        ON_3D_DUMP(
        // TODO: 3d dump :)
        void                dump_3d           ()                               const;
        ) /* ON_3D_DUMP */
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octree_t<coordinate_t>::octree_t(const std::vector<triangle_t>& triangles, bool bad_triangles) :
triangles_quantity_                                      (triangles.size()          ),
triangles_and_boxes_                                     (triangles_quantity_       ),
numbers_of_triangles_that_have_intersections_with_others_(triangles_quantity_, false),
bad_triangles_                                           (bad_triangles             )
{
    if (triangles_quantity_ == 0)
        return;

    bounding_box_t all_space_box; // infinity size by default ctor

    for (size_t i = 0; i < triangles_quantity_; i++)
    {
        const triangle_t     i_triangle        = triangles[i];
        const bounding_box_t box_of_i_triangle = bounding_box_t(triangles[i]);

        triangles_and_boxes_[i].first  = i_triangle;
        triangles_and_boxes_[i].second = box_of_i_triangle;

        update_all_space_box_in_ctor(all_space_box, box_of_i_triangle);
    }

    made_from_box_a_square(all_space_box);

    root_ = std::make_unique<node_t>(all_space_box);

    for (size_t i = 0; i < triangles_quantity_; i++)
        root_->numbers_of_this_node_triangles.push_back(i);

    split_node(root_.get());
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::split_node(node_t* node)
{
    msg_assert(node, "nullptr node no expect here");

    if (dont_need_to_split_node(node))
    {
        node->is_leaf = true;
        return;
    }

    std::array<bounding_box_t, children_quant> children_bboxes = split_bbox(node->bbox);    

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        node->children[child_it] = std::make_unique<node_t>(children_bboxes[child_it]);
        bool is_empty_child = true;
    
        for (size_t triangles_it = 0; triangles_it < node->numbers_of_this_node_triangles.size();)
        {
            const size_t         triangle_number = node->numbers_of_this_node_triangles[triangles_it];
            const bounding_box_t triangle_bbox   = triangles_and_boxes_[triangle_number].second;

            if (!triangle_bbox.is_inside(children_bboxes[child_it]))
            {
                ++triangles_it;
                continue;
            }

            child_contain_triangle_bbox_action(node, triangle_number, triangles_it, child_it, is_empty_child);
        }
        
        if (is_empty_child)
        {
            empty_child_action(node, child_it);
            continue;
        }

        node->is_leaf = false;
    
        split_node(node->children[child_it].get());
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::child_contain_triangle_bbox_action(node_t* node, size_t triangle_number, size_t triangle_it, size_t child_it, bool& is_child_empty)
{
    msg_assert(node, "nullptr node no expect here");
    msg_assert(child_it < children_quant, "here expect only correct child_it value (0-7)");
    remove_i_triangle_from_node(node, triangle_it);
    add_triangle_to_i_node_child(node, triangle_number, child_it);
    is_child_empty = false;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::empty_child_action(node_t* node, size_t child_it)
{
    msg_assert(node, "nullptr node no expect here");
    msg_assert(child_it < children_quant, "here expect only correct child_it value (0-7)");
    msg_assert(node->children[child_it], child_it << " - child is nullptr");

    // no triangles in - its leaf => no memory allocation
    node->children[child_it].reset();
}


//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::remove_i_triangle_from_node(node_t* node, size_t triangle_it)
{
    msg_assert(node, "nullptr node no expect here");
    node->numbers_of_this_node_triangles.erase(node->numbers_of_this_node_triangles.begin() + static_cast<long>(triangle_it));
}

//----------------------------------------------------------------------------------------------------------------------------
template <typename coordinate_t>
void
octree_t<coordinate_t>::add_triangle_to_i_node_child(node_t* node, size_t triangle_number, size_t child_it)
{
    msg_assert(node, "nullptr node no expect here");
    node->children[child_it]->numbers_of_this_node_triangles.push_back(triangle_number);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::dont_need_to_split_node(const node_t* node) const
{
    msg_assert(node, "nullptr node no expect here");
    msg_assert(node->bbox.is_valid(), "bad node bbox");

    return
        (node->bbox.get_side_length() <= min_bbox_side_length_) or
        (node->node_have_not_greater_1_triangle());
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::array<typename octree_t<coordinate_t>::bounding_box_t, octree_t<coordinate_t>::children_quant>
octree_t<coordinate_t>::split_bbox(const bounding_box_t& bbox)
{
    static_assert(children_quant == 8, "this function made split in 8 bboxes");

    const point_t lbp = bbox.lbp;
    const point_t rtp = bbox.rtp;

    const coordinate_t lbp_x = lbp.get_x();
    const coordinate_t lbp_y = lbp.get_y();
    const coordinate_t lbp_z = lbp.get_z();

    const coordinate_t rtp_x = rtp.get_x();
    const coordinate_t rtp_y = rtp.get_y();
    const coordinate_t rtp_z = rtp.get_z();

    const coordinate_t x_center = (lbp_x + rtp_x) / 2;
    const coordinate_t y_center = (lbp_y + rtp_y) / 2;
    const coordinate_t z_center = (lbp_z + rtp_z) / 2;

    std::array<bounding_box_t, children_quant> new_bboxes;

    new_bboxes[0] = bounding_box_t(lbp                                  , point_t(x_center, y_center, z_center));
    new_bboxes[1] = bounding_box_t(point_t(x_center, lbp_y   , lbp_z   ), point_t(rtp_x   , y_center, z_center));
    new_bboxes[2] = bounding_box_t(point_t(lbp_x   , y_center, lbp_z   ), point_t(x_center, rtp_y   , z_center));
    new_bboxes[3] = bounding_box_t(point_t(x_center, y_center, lbp_z   ), point_t(rtp_x   , rtp_y   , z_center));
    new_bboxes[4] = bounding_box_t(point_t(lbp_x   , lbp_y   , z_center), point_t(x_center, y_center, rtp_z   ));
    new_bboxes[5] = bounding_box_t(point_t(x_center, lbp_y   , z_center), point_t(rtp_x   , y_center, rtp_z   ));
    new_bboxes[6] = bounding_box_t(point_t(lbp_x   , y_center, z_center), point_t(x_center, rtp_y   , rtp_z   ));
    new_bboxes[7] = bounding_box_t(point_t(x_center, y_center, z_center), rtp                                  );

    msg_assert(new_bboxes[0].is_valid(), "0 new bbox is invalid");
    msg_assert(new_bboxes[1].is_valid(), "1 new bbox is invalid");
    msg_assert(new_bboxes[2].is_valid(), "2 new bbox is invalid");
    msg_assert(new_bboxes[3].is_valid(), "3 new bbox is invalid");
    msg_assert(new_bboxes[4].is_valid(), "4 new bbox is invalid");
    msg_assert(new_bboxes[5].is_valid(), "5 new bbox is invalid");
    msg_assert(new_bboxes[6].is_valid(), "6 new bbox is invalid"); // СПАСИБО МОЙ САМЫЙ ЛЮБИМЫЙ msg_assert, ты спас меня от ужасных вещей
    msg_assert(new_bboxes[7].is_valid(), "7 new bbox is invalid");

    return new_bboxes;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::update_all_space_box_in_ctor(bounding_box_t& all_space_box, const bounding_box_t& new_triangle_box)
{
    const point_t& this_bottom_left_point =    all_space_box.lbp;
    const point_t& this_top_right_point   =    all_space_box.rtp; 

    const point_t& new_bottom_left_point  = new_triangle_box.lbp;
    const point_t& new_top_right_point    = new_triangle_box.rtp;

    all_space_box.lbp = point_t
    {
        std::min(this_bottom_left_point.get_x(), new_bottom_left_point.get_x()),
        std::min(this_bottom_left_point.get_y(), new_bottom_left_point.get_y()),
        std::min(this_bottom_left_point.get_z(), new_bottom_left_point.get_z()),
    };

    all_space_box.rtp = point_t
    {
        std::max(this_top_right_point.get_x(), new_top_right_point.get_x()),
        std::max(this_top_right_point.get_y(), new_top_right_point.get_y()),
        std::max(this_top_right_point.get_z(), new_top_right_point.get_z()),
    };


}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::update_min_bbox_side_length_in_ctor(const bounding_box_t& new_triangle_box)
{
    const point_t& lbp = new_triangle_box.lbp;
    const point_t& rtp = new_triangle_box.rtp;

    const coordinate_t x_triangle_box_side = rtp.get_x() - lbp.get_x(); msg_assert(x_triangle_box_side >= 0, "x side length less 0");
    const coordinate_t y_triangle_box_side = rtp.get_y() - lbp.get_y(); msg_assert(y_triangle_box_side >= 0, "y side length less 0");
    const coordinate_t z_triangle_box_side = rtp.get_z() - lbp.get_z(); msg_assert(z_triangle_box_side >= 0, "z side length less 0");

    min_bbox_side_length_ = std::min(x_triangle_box_side,
                            std::min(y_triangle_box_side,
                                     z_triangle_box_side));
}
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::made_from_box_a_square(bounding_box_t& bbox)
{
    point_t      lb       = bbox.lbp;
    point_t      tr       = bbox.rtp;

    coordinate_t lb_x     = lb.get_x();
    coordinate_t lb_y     = lb.get_y();
    coordinate_t lb_z     = lb.get_z();
    coordinate_t tr_x     = tr.get_x();
    coordinate_t tr_y     = tr.get_y();
    coordinate_t tr_z     = tr.get_z();

    coordinate_t x_center = (lb_x + tr_x) / 2;
    coordinate_t y_center = (lb_y + tr_y) / 2;
    coordinate_t z_center = (lb_z + tr_z) / 2;

    coordinate_t half_max_box_side_length = std::max(tr_x - lb_x,
                                            std::max(tr_y - lb_y,
                                                     tr_z - lb_z)
                                                    ) / static_cast<coordinate_t>(2);

    msg_assert(tr_x >= lb_x, "invariant of this class");
    msg_assert(tr_y >= lb_y, "invariant of this class");
    msg_assert(tr_z >= lb_z, "invariant of this class");

    bbox.lbp = point_t
    (
        x_center - half_max_box_side_length - static_cast<coordinate_t>(1), // add 1 to made box a lot bigger to contain all triangles 
        y_center - half_max_box_side_length - static_cast<coordinate_t>(1),
        z_center - half_max_box_side_length - static_cast<coordinate_t>(1)
    );

    bbox.rtp = point_t
    (
        x_center + half_max_box_side_length + static_cast<coordinate_t>(1),
        y_center + half_max_box_side_length + static_cast<coordinate_t>(1),
        z_center + half_max_box_side_length + static_cast<coordinate_t>(1)
    );
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<size_t>
octree_t<coordinate_t>::get_good_triangles() const
{
    const size_t        triangles_quant        = triangles_and_boxes_.size();
    std::vector<size_t> good_triangles_numbers;

    for (size_t triangles_it = 0; triangles_it < triangles_quant; ++triangles_it)
    {
        if (!numbers_of_triangles_that_have_intersections_with_others_[triangles_it]) continue;

        good_triangles_numbers.push_back(triangles_it);
    }

    return good_triangles_numbers;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::find_intersections()
{
    find_intersections_helper(root_.get());
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::find_intersections_helper(const node_t* node)
{
    msg_assert(node, "node is nullptr");

    find_intersections_in_node(node);

    if (node->is_leaf) return;

    find_intersections_with_children(node);

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get(); 

        if (!child_ptr /*or child_ptr->is_leaf*/) continue;

        find_intersections_helper(child_ptr);
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::find_intersections_with_children(const node_t* node)
{
    msg_assert(node, "node is nullptr");

    if (node->is_leaf) return;

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get(); 

        if (!child_ptr /* or child_ptr->is_leaf*/) continue;

        find_intersections_with_children_helper(node, child_ptr);
    }
}

/*
    про копипасту цикла в предыдущей и следующей функциях:
    это необходимо, чтобы избежать проверки в
    find_intersections_with_children_helper
    на node == child
    (а вообще можно вызвать find_intersections_with_children_helper(node, node)
    в find_intersections_with_children для простоты, но там будут лишние сравнения и сравнение треугольников с самими собой)
*/

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::find_intersections_with_children_helper(const node_t* node, const node_t* child)
{
    msg_assert(node ,       "node is nullptr");
    msg_assert(child, "child node is nullptr");
    msg_assert(node != child, "node is equal to child");
    msg_assert(!node->is_leaf, "expect no leaf child");

    intersect_2_nodes_triangles(node, child);

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = child->children[child_it].get(); 

        if (!child_ptr) continue;

        find_intersections_with_children_helper(node, child_ptr);
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::find_intersections_in_node(const node_t* node)
{
    msg_assert(node, "node is nullptr");

    const size_t node_triangles_quant = node->get_node_triangles_quant();

    for (size_t triangles_it_1 = 0; triangles_it_1 + 1 < node_triangles_quant; ++triangles_it_1)
    {
        for (size_t triangles_it_2 = triangles_it_1 + 1; triangles_it_2 < node_triangles_quant; ++triangles_it_2)
        {
            const size_t triangle_1_index = node->numbers_of_this_node_triangles[triangles_it_1];
            const size_t triangle_2_index = node->numbers_of_this_node_triangles[triangles_it_2];

            parse_result_of_triagles_intersection(triangle_1_index, triangle_2_index);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::parse_result_of_triagles_intersection(size_t triangle_1_index, size_t triangle_2_index)
{
    msg_assert(triangle_1_index != triangle_2_index, "try to intersect triangle with himself");

    const bool is_both_good = numbers_of_triangles_that_have_intersections_with_others_[triangle_1_index] and
                              numbers_of_triangles_that_have_intersections_with_others_[triangle_2_index];

    if (is_both_good) return; // dont check intersection if both already are good

    const triangle_t triagnle_1 = triangles_and_boxes_[triangle_1_index].first;
    const triangle_t triagnle_2 = triangles_and_boxes_[triangle_2_index].first;

    const bool are_intersected = triagnle_1.is_intersect_with_another_triangle(triagnle_2, bad_triangles_);

    if (!are_intersected) return;

    numbers_of_triangles_that_have_intersections_with_others_[triangle_1_index] = true;
    numbers_of_triangles_that_have_intersections_with_others_[triangle_2_index] = true;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::intersect_2_nodes_triangles(const node_t* node_1, const node_t* node_2)
{
    msg_assert(node_1, "node_1 is nullptr");
    msg_assert(node_2, "node_2 is nullptr");

    msg_assert(node_1 != node_2, "expect difference noder");
    
    const size_t node_1_triangles_quant = node_1->get_node_triangles_quant();
    const size_t node_2_triangles_quant = node_2->get_node_triangles_quant();

    for (size_t node_1_triangles_it = 0; node_1_triangles_it < node_1_triangles_quant; ++node_1_triangles_it)
    {
        for (size_t node_2_triangles_it = 0; node_2_triangles_it < node_2_triangles_quant; ++node_2_triangles_it)
        {
            const size_t triangle_1_index = node_1->numbers_of_this_node_triangles[node_1_triangles_it];
            const size_t triangle_2_index = node_2->numbers_of_this_node_triangles[node_2_triangles_it];

            parse_result_of_triagles_intersection(triangle_1_index, triangle_2_index);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------

// BOUNDING_BOX_T
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
struct
octree_t<coordinate_t>::bounding_box_t
{
    point_t lbp; // left bottom point
    point_t rtp; // right top point

    bounding_box_t();
    bounding_box_t(const triangle_t& triangle);
    bounding_box_t(const point_t   & bottom_left_point,
                   const point_t   &   top_right_point);

    bool         is_valid       ()                                  const;

    bool         is_intersect   (const bounding_box_t& another_box) const;
    bool         is_inside      (const bounding_box_t& another_box) const;
    bool         is_contain     (const bounding_box_t& another_box) const;

    coordinate_t get_side_length()                                  const;

    ON_LOGGER(
    std::string  get_dump(std::string_view name = "")               const;
    )
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octree_t<coordinate_t>::bounding_box_t::bounding_box_t() :
lbp(std::numeric_limits<coordinate_t>::infinity(),
    std::numeric_limits<coordinate_t>::infinity(),
    std::numeric_limits<coordinate_t>::infinity()),

rtp(-std::numeric_limits<coordinate_t>::infinity(),
    -std::numeric_limits<coordinate_t>::infinity(),
    -std::numeric_limits<coordinate_t>::infinity())
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octree_t<coordinate_t>::bounding_box_t::bounding_box_t(const point_t& bottom_left_point,
                                                      const point_t&   top_right_point) :
lbp(bottom_left_point),
rtp(  top_right_point)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octree_t<coordinate_t>::bounding_box_t::bounding_box_t(const triangle_t& triangle)
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

    *this = bounding_box_t(bottom_left_point,
                            top_right_point);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::bounding_box_t::is_valid() const
{
    bool is_points_valid = lbp.is_valid() and
                           rtp.is_valid();
    if (!is_points_valid)
        return false;

    coordinate_t len_x = rtp.get_x() - lbp.get_x();
    coordinate_t len_y = rtp.get_y() - lbp.get_y();
    coordinate_t len_z = rtp.get_z() - lbp.get_z();

    bool is_square = (Math::Compare::compare(len_x, len_y)) and
                     (Math::Compare::compare(len_x, len_z)) and // len_x = len_y = len_z >= 0
                     (len_x >= 0);

    return is_square;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::bounding_box_t::is_intersect(const bounding_box_t& box) const
{
    msg_assert(is_valid()    , "invalid box");
    msg_assert(box.is_valid(), "invalid box");

    return
            lbp.is_between_2_points(box.lbp, box.rtp) or
            rtp.is_between_2_points(box.lbp, box.rtp) or
        box.lbp.is_between_2_points(    lbp,     rtp) or
        box.rtp.is_between_2_points(    lbp,     rtp);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::bounding_box_t::is_inside(const bounding_box_t& box) const
{
    return
        lbp.is_between_2_points(box.lbp, box.rtp) and
        rtp.is_between_2_points(box.lbp, box.rtp);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::bounding_box_t::is_contain(const bounding_box_t& box) const
{
    return
        box.lbp.is_between_2_points(lbp, rtp) and
        box.rtp.is_between_2_points(lbp, rtp);
}

//----------------------------------------------------------------------------------------------------------------------------


template <typename coordinate_t>
coordinate_t
octree_t<coordinate_t>::bounding_box_t::get_side_length() const
{
    msg_assert(is_valid(), "try to get side in invalid bbox.\n"
                            "here expect only tree-octants boxes, not triangles boxes");

    return rtp.get_x() - lbp.get_x();
}

//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
octree_t<coordinate_t>::bounding_box_t::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    dump << "BBOX " << name
         << "\n{\n\t"
         << lbp.get_dump("lb") << "\n\t"
         << rtp.get_dump("rt"  ) << "\n" 
         << "}";

    return dump.str();
}
) /* ON_LOGGER */

//----------------------------------------------------------------------------------------------------------------------------

// NODE_T
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
struct octree_t<coordinate_t>::node_t
{
    bool                                                is_leaf                       ;
    bounding_box_t                                      bbox                          ;
    std::vector<size_t>                                 numbers_of_this_node_triangles;
    std::array<std::unique_ptr<node_t>,
               octree_t<coordinate_t>::children_quant> children                       ;

    node_t(const bounding_box_t& bbox);

    bool        node_have_not_greater_1_triangle()   const;
    size_t      get_node_triangles_quant        ()   const;

    ON_LOGGER(
    std::string get_dump(std::string_view name = "") const;
    ) /* ON_LOGGER */
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
octree_t<coordinate_t>::node_t::node_t(const bounding_box_t& bbox) :
is_leaf(true),
bbox   (bbox)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
octree_t<coordinate_t>::node_t::node_have_not_greater_1_triangle() const
{
    return
        (numbers_of_this_node_triangles.size() <= 1);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
octree_t<coordinate_t>::node_t::get_node_triangles_quant() const
{
    return numbers_of_this_node_triangles.size();
}

//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
octree_t<coordinate_t>::node_t::get_dump(std::string_view name) const
{
    std::ostringstream dump;

    dump << "NODE " << name
         << "\n{\n\t"
         << std::boolalpha << "is leaf = " << is_leaf << "\n\t"
         << "triangle quat = " << get_node_triangles_quant() << "\n\t"
         << "nums of triagnles:\n\t\t";

    for (size_t triagnles_nums: numbers_of_this_node_triangles)
        dump << triagnles_nums << " ";


    dump << "\n\tchildren ptrs:\n\t\t";

    for (size_t children_it = 0; children_it < octree_t<coordinate_t>::children_quant; ++children_it)
        dump << children[children_it].get();    
    
    dump << "}";

    return dump.str();
}
) /* ON_LOGGER */


// DUMP
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

// dump in logfile
ON_LOGGER(
template <typename coordinate_t>
void
octree_t<coordinate_t>::dump() const
{
    for (size_t i = 0; i < triangles_quantity_; i++)
    {
        glog.set_color(((i%2) ? LogColor::Green : LogColor::Red));
        std::ostringstream s;
        s << i;
        glog.log(triangles_and_boxes_[i].first.get_dump(s.str().c_str()));
    }

    glog.set_color(LogColor::Green);
    glog.log_in_line_begin();
    dump(root_.get());
    glog.log_in_line_end();
}
) /* ON_LOGGER */

//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
void
octree_t<coordinate_t>::dump(node_t* node, size_t h) const
{
    glog.log_in_line("( ", "h{", h, "} ");
    for (size_t i = 0; i < node->numbers_of_this_node_triangles.size(); i++)
        glog.log_in_line(node->numbers_of_this_node_triangles[i], " ");

    for (size_t i = 0; i < 8; i++)
    {
        if (!node->children[i]) continue;

        dump(node->children[i].get(), h + 1);                
    }

    glog.log_in_line(") ");
}
) /* ON_LOGGER */

// image dump (2d dump)
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_2D_DUMP(
template <typename coordinate_t>
void
octree_t<coordinate_t>::dump_2d(std::string_view img_name) const
{
    static constexpr std::string dot_out_dir = "dot-out/";
    static constexpr std::string img_dir     = "../2d-dump/";

    create_dir(dot_out_dir);
    create_dir(img_dir    );
 
    std::string dot_file = get_dot_file_path(img_name, dot_out_dir);
    std::string img_file = get_png_file_path(img_name, img_dir    );

    std::ofstream dot(dot_file);

    if (!dot.is_open())
        return failed_open_dot_file_message(img_name);;

    make_dump(dot);

    create_image_from_dot(dot_file, img_file);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::make_dump(std::ofstream& dot) const
{
    dot << "digraph G{\nrankdir=TB\ngraph [bgcolor=\"#000000\"];\n";

    constexpr size_t root_number = 8; // так надо
    node_dump(dot, root_.get(), root_number);

    dot << "edge[color=\"#373737\"];\n";
    create_edges(dot, root_.get());

    dot << "}" << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::node_dump(std::ofstream& dot, const node_t* node, size_t num_as_child) const
{
    msg_assert(node, "nullptr is no expect here");
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);

    std::string node_color = get_node_color(num_as_child);
    std::string node_data  = get_node_data(node, num_as_child);

    dot << "node" << node
       << "[shape=Mrecord, style=filled, fillcolor=\"" << node_color << "\", "
          "label=\"{ " << node_data << "}\", "
          "color = \"#777777\"];\n";

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get();
        if (!child_ptr) continue;
        node_dump(dot, child_ptr, child_it);
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_edges(std::ofstream& dot, const node_t* node) const
{
    msg_assert(node, "nullptr is not excpect here");

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get();
        if (!child_ptr) continue;
        dot << "node" << node << "->node" << child_ptr << "\n";
        create_edges(dot, child_ptr); 
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_node_color(size_t num_as_child) const
{
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);
    
    static_assert(children_quant == 8, "here is using children quant as 8");
    switch (num_as_child)
    {
        case 0: return "#1cb9ff";
        case 1: return "#00ca2c";
        case 2: return "#f31807";
        case 3: return "#FFFACD";
        case 4: return "#CD5C5C";
        case 5: return "#CDFC5C";
        case 6: return "#CFFC5C";
        case 7: return "#FF0080";
        case 8: return "#00ca2f";;
        default: builtin_unreachable_wrapper("here expect only number: 0 <= number <= 8 (8 is for root node)");
    }

    builtin_unreachable_wrapper("we must return in switch");
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_node_data(const node_t* node, size_t num_as_child) const
{
    msg_assert(node, "nullptr is no expect here");
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);
    
    static_assert(children_quant == 8, "here is using children quant as 8");

    std::ostringstream node_data;

    if (num_as_child == 8) node_data << "root";    
    else                   node_data << "child:\\n" << num_as_child;

    static constexpr size_t max_triangle_per_line = 7;

    const size_t node_triangles_quant = node->get_node_triangles_quant();

    if (node_triangles_quant == 0)
    {
        node_data << " | no triangles";
        goto bbox_dump;
    }

    node_data << " | triangles:";

    for (size_t triangles_it = 0, triangles_ie = node->get_node_triangles_quant(); triangles_it < triangles_ie; ++triangles_it)
    {
        if (triangles_it % max_triangle_per_line == 0 )
            node_data << "\\n";
        else
            node_data << " ";
        node_data << node->numbers_of_this_node_triangles[triangles_it];
    }

    bbox_dump:

    node_data << "| bbox:\\n"
                 "(" << node->bbox.lbp.get_x() << ", " << node->bbox.lbp.get_y() << ", " << node->bbox.lbp.get_z() << ")\\n"
                 "(" << node->bbox.rtp.get_x() << ", " << node->bbox.rtp.get_y() << ", " << node->bbox.rtp.get_z() << ")";

    return node_data.str();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_dir(const std::string& dir) const
{
    const bool dir_already_exists =  std::filesystem::exists(dir);

    if (dir_already_exists)
        return;

    std::filesystem::create_directories(dir);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_png_file_path(std::string_view img_name, const std::string& img_dir) const
{
    std::string name(img_name);
    if (name == "")
    {
        static size_t default_name_it = 0;
        ++default_name_it;
        std::ostringstream default_name;
        default_name << "octree" << default_name_it;
        name = default_name.str();
    }

    return img_dir + name + ".png";
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_dot_file_path(std::string_view dot_name, const std::string& dot_out_dir) const
{
    std::string name(dot_name);
    if (name == "")
    {
        static size_t default_name_it = 0;
        ++default_name_it;
        std::ostringstream default_name;
        default_name << "octree" << default_name_it;
        name = default_name.str();
    }

    return dot_out_dir + name + ".dot";
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_image_from_dot(std::string_view dot_file, std::string_view img_file) const
{
    std::ostringstream dot_command;
    dot_command << "dot -Tpng " << dot_file << " > " << img_file;
    int sys_result = std::system(dot_command.str().c_str());

    if (sys_result == EXIT_SUCCESS) return;

    std::ostringstream check_dot_command;
    check_dot_command << "which dot";
    const std::string check_dot = check_dot_command.str();

    std::cerr << RED BOLD << "Failed create\n" RESET_CONSOLE_OUT WHITE "`"
              << img_file << "'\n" RED BOLD "from\n" RESET_CONSOLE_OUT WHITE "`"
              << dot_file << "'\n\n" RED
                "Possibles reason: command " WHITE BOLD "`dot' " RESET_CONSOLE_OUT RED "didnt exists.\n"
                 RESET_CONSOLE_OUT WHITE "Check this command output: " BOLD << check_dot << RESET_CONSOLE_OUT "\n";

    sys_result = std::system(check_dot.c_str());

    if (sys_result != EXIT_SUCCESS)
    {
        std::cout << std::endl;
        return;
    }

    std::cout << WHITE "Hmm, dot was found, so, i have no idea why creating image was failed :(\n"
                 RESET_CONSOLE_OUT << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::failed_open_dot_file_message(std::string_view img_name) const
{
    std::cerr << RED BOLD << "Failed open: " RESET_CONSOLE_OUT WHITE "`"
              << img_name << "'" RESET_CONSOLE_OUT << std::endl;
}

) /* ON_3D_DUMP */

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
