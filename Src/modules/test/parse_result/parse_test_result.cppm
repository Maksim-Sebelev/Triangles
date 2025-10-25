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
void print_good_triangles                               (const RunProgram::program_result_t<coordinate_t>& program_out,                bool verbose_output);
void bad_good_triangles_quant_message                   (size_t program_good_triangles_quant, size_t correct_good_traignles_quant,     bool verbose_output);
void bad_i_good_triangle_message                        (size_t i, size_t program_i_triangle_number, size_t correct_i_triangle_number, bool verbose_output);
void tets_passed_message                                (                                                                              bool verbose_output);
void test_failed_message                                (                                                                              bool verbose_output);
void no_input_message                                   (                                                                              bool verbose_output);
void print_difference_between_program_and_correct_answer(const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles);
void print_find_triangles_information_good_triangles    (const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles);
void print_not_found_good_triangles                     (const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles);

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

    const std::vector<size_t> correct_good_triangles = test_data     .get_good_triangles();
    const std::vector<size_t> program_good_triangles = program_result.get_triangles     ();

    const size_t correct_good_traignles_quant = test_data     .get_good_triangles_quantity();
    const size_t program_good_triangles_quant = program_result.get_good_triangles_quantity();

    const bool is_good_triangles_quant_bad = (correct_good_traignles_quant !=
                                              program_good_triangles_quant);

    bool was_bad_triangle = is_good_triangles_quant_bad;

    if (was_bad_triangle)
        goto skip_cycle_because_false_triangles_quant;

    for (size_t i = 0; i < correct_good_traignles_quant; i++)
    {
        const size_t program_i_good_triangle_number = program_good_triangles[i];
        const size_t correct_i_good_triangle_number = correct_good_triangles[i];

        if (program_i_good_triangle_number == correct_i_good_triangle_number)
            continue;

        was_bad_triangle = true;
        bad_i_good_triangle_message(i, program_i_good_triangle_number, correct_i_good_triangle_number, verbose_output);
    }

    if (was_bad_triangle)
    {
        skip_cycle_because_false_triangles_quant:

        if (verbose_output)
            print_difference_between_program_and_correct_answer(program_good_triangles, correct_good_triangles);

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

void print_difference_between_program_and_correct_answer(const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles)
{
    print_find_triangles_information_good_triangles(program_good_triangles, correct_good_triangles);
    print_not_found_good_triangles(program_good_triangles, correct_good_triangles);
}

//---------------------------------------------------------------------------------------------------------------

void print_find_triangles_information_good_triangles(const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles)
{
    std::cout << WHITE "Find triangles:\n" BOLD;

    bool was_at_least_1_triangle_correct_find = false;

    for (size_t program_good_triangles_number: program_good_triangles)
    {
        bool is_correct_find = (std::find(correct_good_triangles.begin(),
                                          correct_good_triangles.end  (),
                                          program_good_triangles_number
                                        ) != correct_good_triangles.end());

        was_at_least_1_triangle_correct_find = is_correct_find;

        std::cout << ((is_correct_find) ? GREEN : RED)
                  << program_good_triangles_number << " ";
    }

    if (!was_at_least_1_triangle_correct_find)
    {
        const size_t correct_good_trianlges_quant = correct_good_triangles.size();

        std::cout << ((correct_good_trianlges_quant > 0) ? RED : GREEN)
                  << "No one good triangle was found.";
    }

    std::cout << RESET_CONSOLE_OUT << std::endl;
}

//---------------------------------------------------------------------------------------------------------------

void print_not_found_good_triangles(const std::vector<size_t>& program_good_triangles, const std::vector<size_t>& correct_good_triangles)
{
    std::cout << "\n" WHITE "Not find triangles:\n" BOLD RED;

    bool was_all_triangles_not_found = true;

    for (size_t correct_good_triangles_number: correct_good_triangles)
    {
        bool is_find = (std::find(program_good_triangles.begin(),
                                  program_good_triangles.end  (),
                                  correct_good_triangles_number
                                ) != program_good_triangles.end());

        if (is_find) continue;

        was_all_triangles_not_found = false;
        std::cout << correct_good_triangles_number << " ";
    }

    if (was_all_triangles_not_found)
        std::cout << GREEN "All good triangles was found!";

    std::cout << RESET_CONSOLE_OUT << std::endl;
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
