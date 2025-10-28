module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>

#include "custom_console_output.hpp"
#include "global.hpp"

import triangle;

#if defined (USE_LOGGER)
import logger;
#endif /* defined(USE_LOGGER) */

//---------------------------------------------------------------------------------------------------------------

export module read_input_data;

//---------------------------------------------------------------------------------------------------------------

export namespace InputData
{

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class input_data_t
{
    public:
        size_t                                                 get_triangles_quantity()         const;
        Geometry::triangle_t<coordinate_t>                     get_i_triangle        (size_t i) const;
        const std::vector<Geometry::triangle_t<coordinate_t>>& get_triangles         ()         const;

        input_data_t();

        ON_LOGGER(
        void dump(); // dump in logfile
        )

    private:
        size_t                                          triangles_quant_ ;
        std::vector<Geometry::triangle_t<coordinate_t>> triangles_       ;

        bool                               is_index_out_of_range                  (size_t index) const;
        [[noreturn]]
        void                               index_out_of_range                     (size_t index) const;

        // struct methods for reading from stdin
        void                               read_triangles_quant                   ();
        void                               read_all_triangles                     ();

        Geometry::triangle_t<coordinate_t> read_triangle                          (size_t number_of_triangle);

        [[noreturn]]
        void                               failed_read_triangle                   (size_t number_of_bad_triangle, size_t number_of_bad_cordinate);
        [[noreturn]]             
        void                               failed_read_cache_size                 ();
        [[noreturn]]             
        void                               negative_or_too_big_triangles_quant    ();
        [[noreturn]]             
        void                               negative_or_too_big_input_size         ();
        [[noreturn]]             
        void                               failed_read_triangles_quant            ();
};


//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

//ctor for reading from stdin
// no arguments
template <typename coordinate_t>
input_data_t<coordinate_t>::input_data_t()
{
    read_triangles_quant();
    read_all_triangles  ();
}

//---------------------------------------------------------------------------------------------------------------

// public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
input_data_t<coordinate_t>::get_triangles_quantity() const
{
    return triangles_quant_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
Geometry::triangle_t<coordinate_t>
input_data_t<coordinate_t>::get_i_triangle(size_t i) const
{
    if (is_index_out_of_range(i))
        index_out_of_range(i);

    return triangles_[i];
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
const std::vector<Geometry::triangle_t<coordinate_t>>&
input_data_t<coordinate_t>::get_triangles() const
{
    return triangles_;
}

// reading from stdin
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
input_data_t<coordinate_t>::read_triangles_quant()
{
    signed long long int signed_triangles_quant_ = 0;

    std::cin >> signed_triangles_quant_;

    if (std::cin.fail())
        failed_read_triangles_quant(); // exit 1

    if (signed_triangles_quant_ < 0)
        negative_or_too_big_triangles_quant(); // exit 1

    triangles_quant_ = static_cast<size_t> (signed_triangles_quant_); // here we are sure, that signed_triangles_quant is > 0
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
Geometry::triangle_t<coordinate_t>
input_data_t<coordinate_t>::read_triangle(size_t number_of_triangle)
{
    static const size_t coordinates_number = 9;

    std::array<coordinate_t, coordinates_number> coordinates;

    for (size_t i = 0; i < coordinates_number; i++)
    {
        std::cin >> coordinates[i];

        if (!(std::cin.fail())) continue;

        failed_read_triangle(number_of_triangle, i);
    }

    return Geometry::triangle_t<coordinate_t>(coordinates);   
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
void
input_data_t<coordinate_t>::read_all_triangles()
{
    triangles_.resize(triangles_quant_);

    for (size_t i = 0; i < triangles_quant_; i++)
        triangles_[i] = read_triangle(i);
}

//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
input_data_t<coordinate_t>::is_index_out_of_range(size_t index) const
{
    return (index >= triangles_quant_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
input_data_t<coordinate_t>::index_out_of_range(size_t index) const
{
    std::cerr << "trying to get no exists triangle in input." << std::endl
    << "your index   = " << index            << std::endl
    << "triang quant = " << triangles_quant_ << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
input_data_t<coordinate_t>::negative_or_too_big_triangles_quant()
{
    std::cerr << RED "Triangles quant is negative or too big. We can`t parse it." RESET_CONSOLE_OUT << std::endl
    << "failed read triangles quant from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
input_data_t<coordinate_t>::negative_or_too_big_input_size()
{
    std::cerr << "Input size is negative or too big. We can`t parse it." << std::endl
    << "failed read input size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
input_data_t<coordinate_t>::failed_read_triangles_quant()
{
    std::cerr << "Failed read triangles quant from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
input_data_t<coordinate_t>::failed_read_triangle(size_t number_of_bad_triangle, size_t number_of_bad_cordinate)
{
    std::cerr << RED "failed read " BOLD << number_of_bad_cordinate + 1 << RESET_CONSOLE_OUT RED " coordinate of " BOLD
              << number_of_bad_triangle + 1 << RESET_CONSOLE_OUT RED " triangle, from stdin" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace InputData */

//---------------------------------------------------------------------------------------------------------------
