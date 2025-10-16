module;

//---------------------------------------------------------------------------------------------------------------

#include <getopt.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "global.hpp"
#include "custom_console_output.hpp"

export import test_files;
export import input_stream;

#if defined(USE_LOGGER)
import logger;
#include <source_location>
#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module flags_parser;

//---------------------------------------------------------------------------------------------------------------

export class flags_parser
{
    public:
        // class public methods
        input_stream   get_input_stream                  () const;
        test_files_t   get_test_files                    () const;
        bool           print_result_and_dont_check_answer() const;

        // ctor
        flags_parser(int argc, char* argv[]);
       ~flags_parser();
    private:
        // class private variables
        input_stream  input_stream_                      ;
        test_files_t  test_files_                        ;
        bool          print_result_and_dont_check_answer_;

        struct are_parametrs_already_defined
        {
            bool is_define_input_stream = false;
            bool is_define_test_file    = false;
            bool is_define_answer_file  = false;
        };

        are_parametrs_already_defined are_parametrs_already_defined_;

        // class private methods
        void         parse_flag_input_stream                         ();
        [[noreturn]]        
        void         parse_flag_help                                 ();
        void         parse_not_a_flag                                (const char* argument); // maybe test file name, maybe invalid option

        void         define_input_stream                             ();
        void         define_test_file                                ();
        void         define_answer_file                              ();

        input_stream get_type_of_input_stream                        ();
        
        std::string  get_file_extension                              (const std::string& filename );
        bool         is_string_test_file_name                        (const std::string& test_file);
        bool         is_string_answer_file_name                      (const std::string& test_file);

        [[noreturn]]
        void         invalid_type_of_input_stream                    () const;
        [[noreturn]]
        void         try_to_get_test_files_when_input_stream_is_stdin() const;
        [[noreturn]]
        void         redefine_input_stream                           () const;
        [[noreturn]]
        void         redefine_test_file                              () const;
        [[noreturn]]
        void         redefine_answer_file                            () const;
        [[noreturn]]
        void         define_test_file_before_input_stream            () const;
        [[noreturn]]
        void         define_answer_file_before_input_stream          () const;
        [[noreturn]]
        void         undefined_option                                (const char* argument) const;
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// почему бы не хранить это в global scope, если у меня модули?
const option long_options[] =
{
    {"input_stream", required_argument, 0, 'i'},
    {"help"        , no_argument      , 0, 'h'},
    {""            , 0                , 0,  0 }, // just for safety
};

//---------------------------------------------------------------------------------------------------------------

const std::pair<input_stream, std::string> input_stream_flag_values[] =
{
    {input_stream::standart_input      , "stdin"},
    {input_stream::dat_file_stream     , "files"},
    {input_stream::invalid_input_stream, ""      },
};

//---------------------------------------------------------------------------------------------------------------

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// ctor
flags_parser::flags_parser(int argc, char* argv[]) :
input_stream_                      (input_stream::standart_input), // default value of input_stream_
test_files_                        ("", "")                      , // we don`t know files before parsing args
print_result_and_dont_check_answer_(true)                          // in default we get answer in stdin
{
    for (int options_iterator = 1; options_iterator < argc; options_iterator++)
    {
        int option = getopt_long(argc, argv, "i::h", long_options, nullptr);
    
        switch (option)
        {
            case 'i': parse_flag_input_stream();                          continue;
            case 'h': parse_flag_help        ();                          continue;
            case -1 : parse_not_a_flag       (argv[options_iterator]); continue;
            default : builtin_unreachable_wrapper("all unknown options must be pase in <case -1:>");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------

flags_parser::~flags_parser()
{
    // set getopt args to default values
    optarg = nullptr;
    optind = 1;
}

//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

input_stream flags_parser::get_input_stream() const
{
    return input_stream_;
}

//---------------------------------------------------------------------------------------------------------------

test_files_t flags_parser::get_test_files() const
{
    if (input_stream_ != input_stream::dat_file_stream)
        try_to_get_test_files_when_input_stream_is_stdin(); // exit 1
    
    return test_files_;
};

//---------------------------------------------------------------------------------------------------------------

bool flags_parser::print_result_and_dont_check_answer() const
{
    return print_result_and_dont_check_answer_;
}

//---------------------------------------------------------------------------------------------------------------

// class private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_flag_input_stream()
{
    input_stream_ = get_type_of_input_stream();

    if (input_stream_ == input_stream::dat_file_stream)
        print_result_and_dont_check_answer_ = false;

    if (input_stream_ == input_stream::invalid_input_stream)
        invalid_type_of_input_stream(); // exit 1

    define_input_stream();
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_not_a_flag(const char* argument)
{
    assert(argument);

    if (is_string_test_file_name(argument))
    {
        if (!are_parametrs_already_defined_.is_define_input_stream)
            define_test_file_before_input_stream(); // exit 1

        test_files_.test_file = argument;
        define_test_file();

        return;
    }

    if (is_string_answer_file_name(argument))
    {
        if (!are_parametrs_already_defined_.is_define_input_stream)
            define_answer_file_before_input_stream(); // exit 1

        test_files_.answer_file = argument;
        define_answer_file();

        return;
    }

    undefined_option(argument); // exit 1
}

//---------------------------------------------------------------------------------------------------------------


[[noreturn]]
void flags_parser::parse_flag_help()
{
    std::cout <<
    BOLD
    "There are all flags and parametrs:\n"
    RESET_CONSOLE_OUT
    "-h, --help\n"
    "-i=<some_value>, --input_stream=<some_value>\n"
    "*.dat, *.ans\n"
    "\n"
    "What does it mean?\n"
    "-h, --help\n"
    "these flags show information about all flags and their functions.\n"
    "\n"
    "-i=<some_value>, --input_stream=<some_value>\n"
    "these options change format of test.\n"
    "<some_value> has onlu 2 values:\n"
    "'stdin' and 'file'\n"
    "if youre using\n"
    "--input_stream=stdin\n"
    "program will read test data from stdin\n"
    "if youre using\n"
    "--input_stream=file\n"
    "program will read test data from file\n"
    "what is that file?\n"
    "any arg, that look like '*.dat' will be input stream for a programm\n"
    "also '*.ans' will be input stream for answer at the test\n"
    "\n"
    "ATTENTION:\n"
    "You can`t choose a .dat and .ans file if your input stream is file\n"
    "Default value of input stream is stdin\n"
    "(you can don`t indicate explicitly stdin), but if you need files - use -i=file\n"
    "You can`t choose more than 1 .dat and 1 .ans file\n"
    "You can`t choose input stream more 1 time\n"
    "\n"
    "some examples:\n"
    "correct:\n"
    "./program --input_stream=file test.dat test.ans\n"
    "\n"
    "INCORRECT (no files needed for stdin):\n"
    "./program --input_stream=stdin test.dat test.ans\n"
    "\n"
    "correct (no options => stdin):\n"
    "./program\n"
    "\n"
    "INCORRECT (no -i=file => program wait stdin, not files):\n"
    "./program test.dat test.ans\n"
    "\n"
    "\n"
    "So, that was all, what I know about flags in this program.\n"
    "Good luck, I love you :)\n";

    exit(EXIT_SUCCESS); // good exit :)
}

//---------------------------------------------------------------------------------------------------------------

input_stream flags_parser::get_type_of_input_stream()
{
    assert(optarg);

    for (auto input_stream_type: input_stream_flag_values)
    {
        if (input_stream_type.second.compare(optarg) == 0)
            return input_stream_type.first;        
    }

    return input_stream::invalid_input_stream;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::define_input_stream()
{
    if (are_parametrs_already_defined_.is_define_input_stream)
        redefine_input_stream(); // exit 1

    are_parametrs_already_defined_.is_define_input_stream = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::define_test_file()
{
    if (are_parametrs_already_defined_.is_define_test_file)
        redefine_test_file();

    are_parametrs_already_defined_.is_define_test_file = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::define_answer_file()
{
    if (are_parametrs_already_defined_.is_define_answer_file)
        redefine_answer_file();

    are_parametrs_already_defined_.is_define_answer_file = true;
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::invalid_type_of_input_stream() const
{
    assert(optarg);

    std::cerr << "undefined type of input stream '" << optarg << "'."       << std::endl
              << "Usage: '--input_stream=stdin' or '--input_stream=files'." << std::endl
              << "For more information use option '--help' or '-h'."        << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::try_to_get_test_files_when_input_stream_is_stdin() const
{
    std::cerr << "Try to get test_files, but in they is nothing," << std::endl
              << "because test data was reading from stdin."      << std::endl;

    exit(EXIT_FAILURE);
}


//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::redefine_input_stream() const
{
    std::cerr << "redefinition of input stream."     << std::endl
             << "I don`t know, what i must to do :(" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::redefine_test_file() const
{
    std::cerr << "redefinition of test file (.dat)."  << std::endl
              << "I don`t know, what i need to do :(" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::redefine_answer_file() const
{
    std::cerr << "redefinition of answer file (.ans)." << std::endl
              << "I don`t know, what i need to do :("  << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::undefined_option(const char* argument) const
{
    assert(argument);
    std::cerr << "Undefined option '" << argument << "'" << std::endl
              << "I don`t know, what i need to do :("  << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::define_test_file_before_input_stream() const
{
    std::cerr << "Test file was defined before input stream." << std::endl
              << "logical error."                             << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::define_answer_file_before_input_stream() const
{
    std::cerr << "Answer file was defined before input stream." << std::endl
              << "logical error."                               << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

// return empty string if file didn`t have an extension
std::string flags_parser::get_file_extension(const std::string& filename)
{
    const size_t dot_pos = filename.rfind('.');

    if ((dot_pos != std::string::npos) && // dot is found
        (dot_pos + 1 < filename.size()))  // dot isn`t last symbol
    
        return filename.substr(dot_pos); // return ".ext"

    return ""; // return empty string, because this file has not an extension
}

//---------------------------------------------------------------------------------------------------------------

#define check_extension(file, needed_extension)  \
std::string extension = get_file_extension(file); \
return  (                                          \
        (extension != "") &&                        \
        (extension == needed_extension)              \
        );                                            \

//---------------------------------------------------------------------------------------------------------------

bool flags_parser::is_string_test_file_name(const std::string& test_file)
{
    check_extension(test_file, ".dat");
}

//---------------------------------------------------------------------------------------------------------------

bool flags_parser::is_string_answer_file_name(const std::string& answer_file)
{
    check_extension(answer_file, ".ans");
}

//---------------------------------------------------------------------------------------------------------------

#undef check_extension

//---------------------------------------------------------------------------------------------------------------

