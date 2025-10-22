#if ! defined(COORDINATE_TYPE)
static_assert(false, "COORDINATE_TYPE not defined");
#endif /* ! defined(COORDINATE_TYPE) */

#include <cstdlib>

#include "global.hpp"

import get_test_data;
import execute_test;
import parse_test_result;

#if defined(_DEBUG)
#include <iostream>
#include "custom_console_output.hpp"
#endif /*defined(_DEBUG)*/

int main(int argc, char* argv[])
{
    InputData  ::test_data_t     <COORDINATE_TYPE> test_data      = InputData  ::get_test_data    <COORDINATE_TYPE>(argc          , argv     );
    RunProgram ::program_result_t<COORDINATE_TYPE> program_result                                                  (test_data                );
    ParseResult::test_result_t                     test_result    = ParseResult::parse_test_result<COORDINATE_TYPE>(program_result, test_data);

    switch (test_result)
    {
        case ParseResult::test_result_t::TEST_PASSED:       return EXIT_SUCCESS;
        case ParseResult::test_result_t::TEST_FAILED:       return EXIT_FAILURE;
        case ParseResult::test_result_t::DONT_CHECK_RESULT: return EXIT_SUCCESS;
        default: builtin_unreachable_wrapper("undefined test result type. maybe you forgot to add new value in switch");
    }

    builtin_unreachable_wrapper("we must return in switch");
}