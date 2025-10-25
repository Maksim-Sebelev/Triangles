module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <cassert>
#include <vector>

#include "global.hpp"

import triangle        ;
import read_test_data  ;
import read_test_answer;
import flags_parser    ;
import test_files      ;
import input_stream    ;

#if defined(USE_LOGGER)
import logger;

#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module get_test_data;

//---------------------------------------------------------------------------------------------------------------

export namespace InputData
{

//---------------------------------------------------------------------------------------------------------------

using namespace Detail;

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
struct test_data_t
{
    using triangles_vector = std::vector<Geometry::triangle_t<coordinate_t>>;

    private:
        test_input_t<coordinate_t> test_input_                        ;
        test_answer_t              test_answer_                       ;
        bool                       print_result_and_dont_check_answer_;
        bool                       verbose_output_                    ;
        bool                       bad_triangles_                     ;

    public:
        test_data_t(                                bool verbose_output, bool bad_triangles); // ctor for stdin
        test_data_t(const test_files_t& test_files, bool verbose_output, bool bad_triangles); // ctor for files 

        triangles_vector                   get_triangles                     ()         const;
        size_t                             get_good_triangles_quantity       ()         const;
        size_t                             get_number_of_i_good_triangle     (size_t i) const;
        
        size_t                             get_triangles_quantity            ()         const;
        Geometry::triangle_t<coordinate_t> get_i_triangle                    (size_t i) const;
        std::vector<size_t>                get_good_triangles                ()         const;

        bool                               print_result_and_dont_check_answer()         const;
        bool                               verbose_output                    ()         const;
        bool                               bad_triangles                     ()         const;
};

//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
test_data_t<coordinate_t> get_test_data(int argc, char* argv[])
{
    FlagsParsing::flags_parser parsing_flags_result(argc, argv);
    const input_stream                type_of_input_stream = parsing_flags_result.get_input_stream();
    const bool                        verbose_output       = parsing_flags_result.verbose_output  ();
    const bool                        bad_triangles        = parsing_flags_result.bad_triangles   ();

    msg_assert(type_of_input_stream != input_stream::invalid_input_stream,
                "invalid input strem no expect here");

    switch (type_of_input_stream)
    {
        case input_stream::standart_input:
            return test_data_t<coordinate_t>(verbose_output, bad_triangles);

        case input_stream::dat_file_stream:
        {
            test_files_t test_files = parsing_flags_result.get_test_files();
            return test_data_t<coordinate_t>(test_files, verbose_output, bad_triangles);
        }

        case input_stream::invalid_input_stream: builtin_unreachable_wrapper("program wait here only correct input_stream");
        default:                                 builtin_unreachable_wrapper("undefined input stream. maybe you forgot to add some new value in switch");
    }

    builtin_unreachable_wrapper("we must return in switch-case");
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor for reading from files
template <typename coordinate_t>
test_data_t<coordinate_t>::test_data_t(const test_files_t& test_files, bool verbose_output, bool bad_triangles) :
test_input_                        (test_files.  test_file),
test_answer_                       (test_files.answer_file),
print_result_and_dont_check_answer_(false)                 ,
verbose_output_                    (verbose_output)        ,
bad_triangles_                     (bad_triangles)
{}

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from stdin
template <typename coordinate_t>
test_data_t<coordinate_t>::test_data_t(bool verbose_output, bool bad_triangles) :
test_input_                        ()              ,
test_answer_                       ()              ,
print_result_and_dont_check_answer_(true)          ,
verbose_output_                    (verbose_output),
bad_triangles_                     (bad_triangles)
{}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<Geometry::triangle_t<coordinate_t>>
test_data_t<coordinate_t>::get_triangles() const
{
    return test_input_.get_triangles();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::vector<size_t>
test_data_t<coordinate_t>::get_good_triangles() const
{
    return test_answer_.get_triangles();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
test_data_t<coordinate_t>::get_triangles_quantity() const
{
    return test_input_.get_triangles_quantity();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
Geometry::triangle_t<coordinate_t>
test_data_t<coordinate_t>::get_i_triangle(size_t i) const
{
    return test_input_.get_i_triangle(i);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
test_data_t<coordinate_t>::get_good_triangles_quantity() const
{
    return test_answer_.get_good_triangles_quantity();
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
size_t
test_data_t<coordinate_t>::get_number_of_i_good_triangle(size_t i) const
{
    return test_answer_.get_number_of_i_good_triangle(i);
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
test_data_t<coordinate_t>::print_result_and_dont_check_answer() const
{
    return print_result_and_dont_check_answer_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
test_data_t<coordinate_t>::verbose_output() const
{
    return verbose_output_;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
test_data_t<coordinate_t>::bad_triangles() const
{
    return bad_triangles_;
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace InputData */

//---------------------------------------------------------------------------------------------------------------
