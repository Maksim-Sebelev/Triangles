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
    ON_DEBUG(
    std::cout << GREEN BOLD "Running:\n" << VIOLET ITALIC;
    for (int i = 0; i < argc; i++)
        std::cout << argv[i] << " ";

    std::cout << RESET_CONSOLE_OUT << std::endl << std::endl;
    )

    test_data_t     <COORDINATE_TYPE> test_data      = get_test_data    <COORDINATE_TYPE>(argc          , argv     );
    program_result_t<COORDINATE_TYPE> program_result                                     (test_data                );
    test_result_t                     test_result    = parse_test_result<COORDINATE_TYPE>(program_result, test_data);

    if (test_result == test_result_t::TEST_FAILED)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
