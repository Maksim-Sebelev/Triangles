module;

//---------------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <vector>
#include <cassert>
#include <iostream>
#include <cstdlib>

#include "global.hpp"
#include "custom_console_output.hpp"

import triangle;
import octree;
import get_test_data;


//---------------------------------------------------------------------------------------------------------------

export module execute_test;

//---------------------------------------------------------------------------------------------------------------

export namespace RunProgram
{

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class program_result_t
{
    private: /* what about comment like this? i think its comfortable to visually separate variables from methods*/
        //--------------------- varibles -----------------------------------
        bool                no_input_ = false;

        size_t              good_triangles_quant_     ;
        std::vector<size_t> numbers_of_good_triangles_;

        //--------------------- methods ------------------------------------
        void                no_input_action      ();

        bool                is_index_out_of_range(size_t i) const;
        [[noreturn]]
        void                index_out_of_range   (size_t i) const;

        //--------------------------------------------------------

    public:        
        //--------------------- methods -----------------------------------

        program_result_t(const InputData::test_data_t<coordinate_t>& test_data);

        bool                no_input                     ()         const;

        size_t              get_good_triangles_quantity  ()         const;
        size_t              get_number_of_i_good_triangle(size_t i) const;

        std::vector<size_t> get_triangles                ()         const;

        //--------------------------------------------------------
};

// public
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
template <typename coordinate_t>
program_result_t<coordinate_t>::program_result_t(const InputData::test_data_t<coordinate_t>& test_data)
{
    const size_t triangles_quant = test_data.get_triangles_quantity();

    if (triangles_quant == 0)
    {
        no_input_action();
        return;
    }

    const bool bad_triangles = test_data.bad_triangles();

    Geometry::octree_t tree(test_data.get_triangles(), bad_triangles);

    tree.find_intersections();

    ON_LOGGER(tree.dump());

    numbers_of_good_triangles_ = tree                      .get_good_triangles();
    good_triangles_quant_      = numbers_of_good_triangles_.size              ();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<size_t>
program_result_t<coordinate_t>::get_triangles() const
{
    return numbers_of_good_triangles_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
program_result_t<coordinate_t>::get_good_triangles_quantity() const
{
    assert(good_triangles_quant_ == numbers_of_good_triangles_.size());

    return good_triangles_quant_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
program_result_t<coordinate_t>::get_number_of_i_good_triangle(size_t i) const
{
    if (is_index_out_of_range(i))
        index_out_of_range(i); // exit 1

    return numbers_of_good_triangles_[i];
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
program_result_t<coordinate_t>::no_input() const
{
    return no_input_;
}

// private
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
program_result_t<coordinate_t>::is_index_out_of_range(size_t i) const
{
    return (i >= good_triangles_quant_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
program_result_t<coordinate_t>::index_out_of_range(size_t i) const
{
    std::cerr << RED "Try to get no exits good triangle:\n" WHITE
              << "Your index          : " BOLD RED   << i
              << "\n" RESET_CONSOLE_OUT WHITE
              << "Good triangles quant: " BOLD GREEN << good_triangles_quant_
              << RESET_CONSOLE_OUT
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
program_result_t<coordinate_t>::no_input_action()
{
    no_input_             = true;
    good_triangles_quant_ = 0   ;
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace RunProgram */

//---------------------------------------------------------------------------------------------------------------
