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
#include <source_location>
#endif // defined(USE_LOGGER)


//---------------------------------------------------------------------------------------------------------------

export module parse_test_result;

//---------------------------------------------------------------------------------------------------------------

export enum class test_result_t
{
    DONT_CHECK_RESULT = 0,
    TEST_PASSED          ,
    TEST_FAILED          ,
};

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void print_good_triangles            (const program_result_t<coordinate_t>& program_out);
void bad_good_triangles_quant_message(size_t program_good_triangles_quant, size_t correct_good_traignles_quant);
void bad_i_good_triangle_message     (size_t i, size_t program_i_triangle_number, size_t correct_i_triangle_number);
void tets_passed_message             ();
void test_failed_message             ();

//---------------------------------------------------------------------------------------------------------------

export
template <typename coordinate_t>
test_result_t parse_test_result(const program_result_t<coordinate_t>& program_result, const test_data_t<coordinate_t>& test_data)
{
    if (test_data.print_result_and_dont_check_answer())
    {
        print_good_triangles(program_result);
        return test_result_t::DONT_CHECK_RESULT;
    }

    const size_t program_good_triangles_quant = program_result.get_good_triangles_quantity();
    const size_t correct_good_traignles_quant = test_data     .get_good_triangles_quantity();

    if (program_good_triangles_quant != correct_good_traignles_quant)
    {
        bad_good_triangles_quant_message(program_good_triangles_quant, correct_good_traignles_quant);
        test_failed_message();
        return test_result_t::TEST_FAILED;
    }

    bool was_bad_triangle = false;

    for (size_t i = 0; i < correct_good_traignles_quant; i++)
    {
        const size_t program_i_good_triangle_number = program_result.get_number_of_i_good_triangle(i);
        const size_t correct_i_good_triangle_number = test_data     .get_number_of_i_good_triangle(i);

        if (program_i_good_triangle_number == correct_i_good_triangle_number) continue;

        was_bad_triangle = true;
        bad_i_good_triangle_message(i, program_i_good_triangle_number, correct_i_good_triangle_number);
    }

    if (was_bad_triangle)
    {
        test_failed_message();
        return test_result_t::TEST_FAILED;
    }

    tets_passed_message();

    return test_result_t::TEST_PASSED;
}

//---------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void print_good_triangles(const program_result_t<coordinate_t>& program_out)
{
    const size_t good_triangles_quant = program_out.get_good_triangles_quantity();

    msg_assert(good_triangles_quant != 1, "it's impossible, that exists only 1 good triagnles (minumum 2)");

    if (good_triangles_quant == 0)
    {
        std::cout << RED "No triangles, that have intersections with others." << std:: endl;
        return;
    }

    std::cout << GREEN "The numbers of triangles that have intersections with others (" YELLOW BOLD
              << good_triangles_quant << RESET_CONSOLE_OUT GREEN << "):" BOLD << std::endl;

    for (size_t i = 0; i < good_triangles_quant; i++)
        std::cout << program_out.get_number_of_i_good_triangle(i) << std::endl;

    std::cout << RESET_CONSOLE_OUT;
}

//---------------------------------------------------------------------------------------------------------------


void bad_good_triangles_quant_message(size_t program_good_triangles_quant, size_t correct_good_traignles_quant)
{
    std::cout << RED "False good triangles qunatiny:"                                              << std::endl
              << "Program out  : " BOLD << program_good_triangles_quant << RESET_CONSOLE_OUT GREEN << std::endl
              << "Correct value: " BOLD << correct_good_traignles_quant << RESET_CONSOLE_OUT       << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void bad_i_good_triangle_message(size_t i, size_t program_i_triangle_number, size_t correct_i_triangle_number)
{
    std::cout << RED "False number of " BOLD << i << RESET_CONSOLE_OUT RED << " good_triangle:" << std::endl
              << "Program out  : " BOLD << program_i_triangle_number << RESET_CONSOLE_OUT GREEN << std::endl
              << "Correct value: " BOLD << correct_i_triangle_number << RESET_CONSOLE_OUT       << std::endl << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void test_failed_message()
{
    std::cout << std::endl << RED BOLD "TEST FAILED" RESET_CONSOLE_OUT << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void tets_passed_message()
{
    std::cout << std::endl << GREEN BOLD "TEST PASSED" RESET_CONSOLE_OUT << std::endl;
}

//---------------------------------------------------------------------------------------------------------------
