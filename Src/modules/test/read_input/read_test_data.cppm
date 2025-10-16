module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>

#include "custom_console_output.hpp"
#include "global.hpp"

import triangle;

#if defined (USE_LOGGER)
#include <source_location>
import logger;
#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module read_test_data;

//---------------------------------------------------------------------------------------------------------------

export
template <typename coordinate_t>
class test_input_t
{
    public:
        size_t                                get_triangles_quantity()         const;
        triangle_t<coordinate_t>              get_i_triangle        (size_t i) const;
        std::vector<triangle_t<coordinate_t>> get_triangles         ()         const;

        test_input_t();                           // ctor for reading from stdin
        test_input_t(std::string_view test_file); // ctor for reading from .dat file

        // dump in logfile
        ON_LOGGER(
        void dump();
        )

    private:
        size_t                                triangles_quant_ ;
        std::vector<triangle_t<coordinate_t>> triangles_       ;
        std::ifstream                         file_            ;
        std::string                           test_file_       ;

        bool                     is_index_out_of_range                             (size_t index) const;
        [[noreturn]]
        void                     index_out_of_range                                (size_t index) const;

        // struct methods for reading from stdin
        void                     read_triangles_quant_from_stdin                   ();
        void                     read_all_triangles_from_stdin                     ();

        triangle_t<coordinate_t> read_triangle_from_stdin                          (size_t number_of_triangle);

        [[noreturn]]
        void                     failed_read_triangle_from_stdin                   (size_t number_of_bad_triangle, size_t number_of_bad_cordinate);
        [[noreturn]]             
        void                     failed_read_cache_size_from_stdin                 ();
        [[noreturn]]             
        void                     negative_or_too_big_triangles_quant_from_stdin    ();
        [[noreturn]]             
        void                     negative_or_too_big_input_size_from_stdin         ();
        [[noreturn]]             
        void                     failed_read_triangles_quant_from_stdin            ();
        [[noreturn]]             
        void                     no_input_triangles_from_stdin                     ();


        // struct methods for reading tests from .dat file ans answers from .ans file
        void                     open_test_file                                    ();
        void                     read_triangles_quant_from_dat                     ();
        void                     read_all_triangles_from_dat                       ();
        void                     close_test_file                                   ();

        triangle_t<coordinate_t> read_triangle_from_dat                            (size_t number_of_triangle);

        [[noreturn]]
        void                     failed_open_test_file                             ();
        [[noreturn]]
        void                     failed_read_cache_size_from_dat                   ();
        [[noreturn]]
        void                     negative_or_too_big_triangles_quant_from_dat      ();
        [[noreturn]]
        void                     negative_or_too_big_input_size_from_dat           ();
        [[noreturn]]
        void                     failed_read_triangles_quant_from_dat              ();
        [[noreturn]]
        void                     failed_read_i_triangle_from_dat                   (size_t number_of_bad_triangle, size_t number_of_bad_cordinate);
        [[noreturn]]             
        void                     no_input_triangles_from_dat                       ();

};


//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

//ctor for reading from stdin
// no arguments
template <typename coordinate_t>
test_input_t<coordinate_t>::test_input_t()
{
    read_triangles_quant_from_stdin();
    read_all_triangles_from_stdin  ();
}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from .dat
// arg = name of .dat file
template <typename coordinate_t>
test_input_t<coordinate_t>::test_input_t(std::string_view test_file) :
test_file_(test_file)
{
    open_test_file               ();
    read_triangles_quant_from_dat();
    read_all_triangles_from_dat  ();
    close_test_file              ();
}

//---------------------------------------------------------------------------------------------------------------

// public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
test_input_t<coordinate_t>::get_triangles_quantity() const
{
    return triangles_quant_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
triangle_t<coordinate_t>
test_input_t<coordinate_t>::get_i_triangle(size_t i) const
{
    if (is_index_out_of_range(i))
        index_out_of_range(i);

    return triangles_[i];
}

// reading from stdin
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
test_input_t<coordinate_t>::read_triangles_quant_from_stdin()
{
    signed long long int signed_triangles_quant_ = 0;

    std::cin >> signed_triangles_quant_;

    if (std::cin.fail())
        failed_read_triangles_quant_from_stdin(); // exit 1

    if (signed_triangles_quant_ < 0)
        negative_or_too_big_triangles_quant_from_stdin(); // exit 1

    if (signed_triangles_quant_ == 0)
        no_input_triangles_from_stdin(); // exit 1

    triangles_quant_ = static_cast<size_t> (signed_triangles_quant_); // here we are sure, that signed_triangles_quant is > 0
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
triangle_t<coordinate_t>
test_input_t<coordinate_t>::read_triangle_from_stdin(size_t number_of_triangle)
{
    static const size_t coordinates_number = 9;

    std::array<coordinate_t, coordinates_number> coordinates;

    for (size_t i = 0; i < coordinates_number; i++)
    {
        std::cin >> coordinates[i];

        if (!(std::cin.fail())) continue;

        failed_read_triangle_from_stdin(number_of_triangle, i);
    }

    return triangle_t<coordinate_t>(coordinates);   
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
void
test_input_t<coordinate_t>::read_all_triangles_from_stdin()
{
    triangles_.resize(triangles_quant_);

    for (size_t i = 0; i < triangles_quant_; i++)
        triangles_[i] = read_triangle_from_stdin(i);
}

//---------------------------------------------------------------------------------------------------------------

// reading from .dat file
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void test_input_t<coordinate_t>::open_test_file()
{
    file_.open(std::string(test_file_));

    if (!(file_.fail())) return;

    failed_open_test_file(); // exit 1
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
test_input_t<coordinate_t>::read_triangles_quant_from_dat()
{
    signed long long int signed_triangles_quant_ = 0;

    file_ >> signed_triangles_quant_;

    if (file_.fail())
        failed_read_triangles_quant_from_dat(); // exit 1

    if (signed_triangles_quant_ < 0)
        negative_or_too_big_triangles_quant_from_dat(); // exit 1

    if (signed_triangles_quant_ == 0)
        no_input_triangles_from_dat(); // exit 1

    triangles_quant_ = static_cast<size_t> (signed_triangles_quant_); // here we are sure, that signed_triangles_quant is > 0
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
void
test_input_t<coordinate_t>::read_all_triangles_from_dat()
{
    triangles_.resize(triangles_quant_);

    for (size_t i = 0; i < triangles_quant_; i++)
        triangles_[i] = read_triangle_from_dat(i);
}

//---------------------------------------------------------------------------------------------------------------

template <typename  coordinate_t>
triangle_t<coordinate_t>
test_input_t<coordinate_t>::read_triangle_from_dat(size_t num_of_triangle)
{
    static const size_t coordinates_number = 9;

    std::array<coordinate_t, coordinates_number> coordinates;

    for (size_t i = 0; i < coordinates_number; i++)
    {
        file_ >> coordinates[i];

        if (!(file_.fail())) continue;

        failed_read_i_triangle_from_dat(num_of_triangle, i);
    }

    return triangle_t<coordinate_t>(coordinates);   
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
test_input_t<coordinate_t>::close_test_file()
{
    file_.close();
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
test_input_t<coordinate_t>::is_index_out_of_range(size_t index) const
{
    return (index >= triangles_quant_);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::index_out_of_range(size_t index) const
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
test_input_t<coordinate_t>::negative_or_too_big_triangles_quant_from_stdin()
{
    std::cerr << RED "Triangles quant is negative or too big. We can`t parse it." RESET_CONSOLE_OUT << std::endl
    << "failed read triangles quant from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::negative_or_too_big_input_size_from_stdin()
{
    std::cerr << "Input size is negative or too big. We can`t parse it." << std::endl
    << "failed read input size from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::failed_read_triangles_quant_from_stdin()
{
    std::cerr << "Failed read triangles quant from stdin." << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::failed_read_triangle_from_stdin(size_t number_of_bad_triangle, size_t number_of_bad_cordinate)
{
    std::cerr << RED "failed read " BOLD << number_of_bad_cordinate + 1 << RESET_CONSOLE_OUT RED " coordinate of " BOLD
              << number_of_bad_triangle + 1 << RESET_CONSOLE_OUT RED " triangle, from stdin" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::no_input_triangles_from_stdin()
{
    std::cerr << RED "0 input triangles was given from stdin." << std::endl << WHITE
              << "I love you, because you give me chance, to relax :) " "\u2764" RESET_CONSOLE_OUT
              << std::endl;

    exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::failed_open_test_file()
{
    std::cerr << RED "Failed open" WHITE BOLD " '" << test_file_ << "'" RESET_CONSOLE_OUT
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::negative_or_too_big_triangles_quant_from_dat()
{
    std::cerr << RED "Triangles quant is negative or too big. We can`t parse it." << std::endl
              << "failed read triangles quant from" WHITE BOLD " `" << test_file_ << "'" RESET_CONSOLE_OUT << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::failed_read_triangles_quant_from_dat()
{
    std::cerr << RED "Failed read triangles quant from dat: `" WHITE BOLD
              << test_file_ << RED "'" RESET_CONSOLE_OUT << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::failed_read_i_triangle_from_dat(size_t number_of_bad_triangle, size_t number_of_bad_cordinate)
{
    std::cerr << RED "failed read " BOLD << number_of_bad_cordinate + 1 << RESET_CONSOLE_OUT RED " coordinate of " BOLD
              << number_of_bad_triangle + 1 << RESET_CONSOLE_OUT RED " triangle, from "  << WHITE ITALIC "`"
              << test_file_ << "'" RESET_CONSOLE_OUT << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
[[noreturn]]
void
test_input_t<coordinate_t>::no_input_triangles_from_dat()
{
    std::cerr << RED "0 input triangles in " << WHITE ITALIC "`"
              << test_file_ << "'" RESET_CONSOLE_OUT << std::endl
              << WHITE "I love you, because you give me chance, to relax :) " "\u2764" RESET_CONSOLE_OUT
              << std::endl;

    exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------------------------------------------
