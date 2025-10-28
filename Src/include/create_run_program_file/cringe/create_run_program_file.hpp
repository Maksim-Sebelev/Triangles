/*
before include this file:
#define COORDINATE_TYPE <some floating point type>

the full usage example:
// some .cpp:
#define COORDINATE_TYPE float
#include "cringe/create_run_porgram_file.hpp"

// or:
#define COORDINATE_TYPE long double
#include "cringe/create_run_porgram_file.hpp"
*/

#if not defined(COORDINATE_TYPE)
static_assert(false, "COORDINATE_TYPE not defined");
#endif /* ! defined(COORDINATE_TYPE) */

#include <cstdlib> // for EXIT_SUCCESS macro

import flags_parser               ;
import read_input_data            ;
import find_triangle_intersections;

int main(int argc, char* argv[])
{
    const FlagsParsing::flags_parser                                 flags_parsing_result                                           (argc,       argv           );
    const FlagsParsing::program_options_t                            program_options      = flags_parsing_result.get_program_options(                           );
    const InputData   ::input_data_t               <COORDINATE_TYPE> input_data                                                                                  ;
          RunProgram  ::find_triangle_intersections<COORDINATE_TYPE>                                                                (input_data, program_options);

    return EXIT_SUCCESS;
}