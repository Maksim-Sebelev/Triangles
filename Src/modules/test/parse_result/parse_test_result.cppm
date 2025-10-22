module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <vector>

#include "global.hpp"
#include "custom_console_output.hpp"

import triangle;
import get_test_data;
import execute_test;

#if defined (USE_LOGGER)
import logger;
#endif // defined(USE_LOGGER)


//---------------------------------------------------------------------------------------------------------------

export module parse_test_result;

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void print_good_triangles            (const RunProgram::program_result_t<coordinate_t>& program_out,                bool verbose_output);
void bad_good_triangles_quant_message(size_t program_good_triangles_quant, size_t correct_good_traignles_quant,     bool verbose_output);
void bad_i_good_triangle_message     (size_t i, size_t program_i_triangle_number, size_t correct_i_triangle_number, bool verbose_output);
void tets_passed_message             (                                                                              bool verbose_output);
void test_failed_message             (                                                                              bool verbose_output);
void no_input_message                (                                                                              bool verbose_output);

//---------------------------------------------------------------------------------------------------------------

export namespace ParseResult
{

//---------------------------------------------------------------------------------------------------------------

enum class test_result_t
{
    DONT_CHECK_RESULT = 0,
    TEST_PASSED          ,
    TEST_FAILED          ,
};

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
test_result_t parse_test_result(const RunProgram::program_result_t<coordinate_t>& program_result, const InputData::test_data_t<coordinate_t>& test_data)
{
    const bool verbose_output = test_data.verbose_output();

    if (program_result.no_input())
    {
        no_input_message(verbose_output);
        return test_result_t::TEST_PASSED;
    }

    if (test_data.print_result_and_dont_check_answer())
    {
        print_good_triangles(program_result, verbose_output);
        return test_result_t::DONT_CHECK_RESULT;
    }

    const size_t correct_good_traignles_quant = test_data     .get_good_triangles_quantity();
    const size_t program_good_triangles_quant = program_result.get_good_triangles_quantity();

    if (program_good_triangles_quant != correct_good_traignles_quant)
    {
        bad_good_triangles_quant_message(program_good_triangles_quant, correct_good_traignles_quant, verbose_output);
        test_failed_message(verbose_output);
        return test_result_t::TEST_FAILED;
    }

    bool was_bad_triangle = false;

    for (size_t i = 0; i < correct_good_traignles_quant; i++)
    {
        const size_t program_i_good_triangle_number = program_result.get_number_of_i_good_triangle(i);
        const size_t correct_i_good_triangle_number = test_data     .get_number_of_i_good_triangle(i);

        if (program_i_good_triangle_number == correct_i_good_triangle_number) continue;

        was_bad_triangle = true;
        bad_i_good_triangle_message(i, program_i_good_triangle_number, correct_i_good_triangle_number, verbose_output);
    }

    if (was_bad_triangle)
    {
        test_failed_message(verbose_output);
        return test_result_t::TEST_FAILED;
    }

    tets_passed_message(verbose_output);

    return test_result_t::TEST_PASSED;
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace ParseResult */

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void print_good_triangles(const RunProgram::program_result_t<coordinate_t>& program_out, bool verbose_output)
{
    const size_t good_triangles_quant = program_out.get_good_triangles_quantity();

    msg_assert(good_triangles_quant != 1, "it's impossible, that exists only 1 good triagnles (minumum 2)");

    if (good_triangles_quant == 0)
    {
        if (!verbose_output) return;

        std::cout << GREEN
                  << "No triangles, that have intersections with others."
                  << RESET_CONSOLE_OUT
                  << std:: endl;

        return;
    }

    if (verbose_output)
        std::cout << GREEN "The numbers of triangles that have intersections with others (" YELLOW BOLD
                  << good_triangles_quant 
                  << RESET_CONSOLE_OUT GREEN "):" BOLD << std::endl;

    for (size_t i = 0; i < good_triangles_quant; i++)
        std::cout << program_out.get_number_of_i_good_triangle(i) << std::endl;

    
    if (!verbose_output) return;

    std::cout << RESET_CONSOLE_OUT;
}

//---------------------------------------------------------------------------------------------------------------


void bad_good_triangles_quant_message(size_t program_good_triangles_quant, size_t correct_good_traignles_quant, bool verbose_output)
{
    std::cout << ((verbose_output) ? RED : "")
              <<  "False good triangles quantity:\n"
              << "Program out  : " << ((verbose_output) ? BOLD : "") << program_good_triangles_quant
              << ((verbose_output) ? RESET_CONSOLE_OUT GREEN : "")   << "\n"
              << "Correct value: " << ((verbose_output) ? BOLD : "") << correct_good_traignles_quant << ((verbose_output) ? RESET_CONSOLE_OUT : "")
              << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void bad_i_good_triangle_message(size_t i, size_t program_i_triangle_number, size_t correct_i_triangle_number, bool verbose_output)
{
    std::cout << ((verbose_output) ? RED : "")
              << "False number of " << ((verbose_output) ? BOLD : "")
              << i << ((verbose_output) ? RESET_CONSOLE_OUT RED : "") << " good_triangle:\n"
              << "Program out  : " << ((verbose_output) ? BOLD : "")
              << program_i_triangle_number << ((verbose_output) ? RESET_CONSOLE_OUT GREEN : "") << "\n"
              << "Correct value: " << ((verbose_output) ? BOLD : "")
              << correct_i_triangle_number << ((verbose_output) ? RESET_CONSOLE_OUT : "")
              << "\n" << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void no_input_message(bool verbose_output)
{
    static constexpr std::string_view heart_for_good_tester_of_this_program = "\u2764";

    std::cout << ((verbose_output) ? RED   : "") << "No input triangles.\n"
              << ((verbose_output) ? WHITE : "") << "I love you, because you give me chance, to relax "
              << ((verbose_output) ? RED   : "") << heart_for_good_tester_of_this_program << ((verbose_output) ? RESET_CONSOLE_OUT : "")
              << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void test_failed_message(bool verbose_output)
{
    std::cout << ((verbose_output) ? BOLD RED : "") << "\nTEST FAILED" << ((verbose_output) ? RESET_CONSOLE_OUT : "") << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void tets_passed_message(bool verbose_output)
{
    std::cout << ((verbose_output) ? BOLD GREEN : "") << "\nTEST PASSED" << ((verbose_output) ? RESET_CONSOLE_OUT : "") << std::endl;
}

//---------------------------------------------------------------------------------------------------------------
