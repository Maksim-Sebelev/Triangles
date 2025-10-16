module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <cassert>

#include "global.hpp"

import triangle        ;
import read_test_data  ;
import read_test_answer;
import flags_parser    ;
import test_files      ;
import input_stream    ;

#if defined(USE_LOGGER)
import logger;
#include <source_location>
#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module get_test_data;

//---------------------------------------------------------------------------------------------------------------

export
template <typename coordinate_t>
struct test_data_t
{
    private:
        test_input_t<coordinate_t> test_input_                        ;
        test_answer_t              test_answer_                       ;
        bool                       print_result_and_dont_check_answer_;

    public:
        test_data_t();                               // ctor for stdin
        test_data_t(const test_files_t& test_files); // ctor for files 

        size_t                   get_good_triangles_quantity       ()         const;
        size_t                   get_number_of_i_good_triangle     (size_t i) const;
        
        size_t                   get_triangles_quantity            ()         const;
        triangle_t<coordinate_t> get_i_triangle                    (size_t i) const;

        bool                     print_result_and_dont_check_answer()         const;
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

export
template <typename coordinate_t>
test_data_t<coordinate_t> get_test_data(int argc, char* argv[])
{
    flags_parser parsing_flags_result(argc, argv);
    input_stream type_of_input_stream = parsing_flags_result.get_input_stream();

    assert(type_of_input_stream != input_stream::invalid_input_stream);

    switch (type_of_input_stream)
    {
        case input_stream::standart_input:
            return test_data_t<coordinate_t>();

        case input_stream::dat_file_stream:
        {
            test_files_t test_files = parsing_flags_result.get_test_files();
            return test_data_t<coordinate_t>(test_files);
        }

        case input_stream::invalid_input_stream:
        default: builtin_unreachable_wrapper("program wait here only correct input_stream");
    }

    builtin_unreachable_wrapper("we must return in switch-case");
    return {};
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor for reading from files
template <typename coordinate_t>
test_data_t<coordinate_t>::test_data_t(const test_files_t& test_files) :
test_input_(test_files.test_file), test_answer_(test_files.answer_file), print_result_and_dont_check_answer_(false)
{}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from stdin
template <typename coordinate_t>
test_data_t<coordinate_t>::test_data_t() :
test_input_(), test_answer_(), print_result_and_dont_check_answer_(true)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t test_data_t<coordinate_t>::get_triangles_quantity() const
{
    return test_input_.get_triangles_quantity();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
triangle_t<coordinate_t> test_data_t<coordinate_t>::get_i_triangle(size_t i) const
{
    return test_input_.get_i_triangle(i);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t test_data_t<coordinate_t>::get_good_triangles_quantity() const
{
    return test_answer_.get_good_triangles_quantity();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t test_data_t<coordinate_t>::get_number_of_i_good_triangle(size_t i) const
{
    return test_answer_.get_number_of_i_good_triangle(i);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool test_data_t<coordinate_t>::print_result_and_dont_check_answer() const
{
    return print_result_and_dont_check_answer_;
}

//---------------------------------------------------------------------------------------------------------------
