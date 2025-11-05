module;

//---------------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <vector>
#include <cassert>
#include <iostream>
#include <cstdlib>

#include "global.hpp"
#include "custom_console_output.hpp"

import flags_parser;
import read_input_data;
import triangle;
import octree;

ON_3D_DUMP(
import octree_viz;
)

//---------------------------------------------------------------------------------------------------------------

export module find_triangle_intersections;

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<size_t> get_good_triangles          (const std::vector<Geometry::triangle_t<coordinate_t>>& triangles,
                                                 const bool& bad_triangles);

void                print_good_triangles_numbers(const std::vector<size_t>& good_triangles_numbers, const bool& color_output);
void                no_input_message            (                                                   const bool& color_output);

//---------------------------------------------------------------------------------------------------------------

export namespace RunProgram
{
    template <typename coordinate_t>
    void find_triangle_intersections(const InputData::input_data_t<coordinate_t>& input_data, const FlagsParsing::program_options_t& program_options)
    {
        const bool   color_output    = static_cast<bool>(program_options.color_output);
        const size_t triangles_quant = input_data     .get_triangles_quantity();

        if (triangles_quant == 0)
            return no_input_message(color_output);

        const bool                                            bad_triangles          = static_cast<bool>(program_options.bad_triangles);
        const std::vector<Geometry::triangle_t<coordinate_t>> triangles              =                   input_data     .get_triangles();

        const std::vector<size_t>                             good_triangles_numbers = get_good_triangles(triangles, bad_triangles);

        return print_good_triangles_numbers(good_triangles_numbers, color_output);
    }
} /* namespace RunProgram */

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<size_t>
get_good_triangles(const std::vector<Geometry::triangle_t<coordinate_t>>& triangles, const bool& bad_triangles)
{
    Geometry::octree_t tree(triangles, bad_triangles);
    tree.find_intersections();

    ON_LOGGER (tree.dump   ());
    ON_2D_DUMP(tree.dump_2d());
    ON_3D_DUMP(Geometry::OctreeViz::dump_3d());

    return tree.get_good_triangles();
}

//---------------------------------------------------------------------------------------------------------------

void print_good_triangles_numbers(const std::vector<size_t>& good_triangles_numbers, const bool& color_output)
{
    if (good_triangles_numbers.size() == 0)
    {
        if (!color_output) return;
        std::cout << RED "No triangles, that have intersections with other." RESET_CONSOLE_OUT
                  << std::endl;
        return;
    }

    if (color_output)
        std::cout << GREEN "Numbers of triangles, that have intersections with other:\n";

    for (size_t good_triangle_num: good_triangles_numbers)
        std::cout << good_triangle_num << "\n";

    if (color_output)
        std::cout << RESET_CONSOLE_OUT;

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void no_input_message(const bool& color_output)
{
    std::cout << ((color_output) ? RED   : "") << "No input triangles.\n"
              << ((color_output) ? WHITE : "") << "I love you, because you give me chance, to relax "
              << ((color_output) ? RED   : "") << HEART
              << ((color_output) ? RESET_CONSOLE_OUT : "")
              << std::endl;
}

//---------------------------------------------------------------------------------------------------------------
