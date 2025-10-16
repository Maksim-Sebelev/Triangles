module;

//---------------------------------------------------------------------------------------------------------------

#include <string>

#if defined (USE_LOGGER)
import logger;
#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module test_files;

//---------------------------------------------------------------------------------------------------------------

export
struct test_files_t
{
    std::string test_file  ;
    std::string answer_file;

    test_files_t(std::string_view test_file, std::string_view answer_file);
};

//---------------------------------------------------------------------------------------------------------------

test_files_t::test_files_t(std::string_view test_file, std::string_view answer_file) :
test_file(test_file), answer_file(answer_file)
{}

//---------------------------------------------------------------------------------------------------------------
