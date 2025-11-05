module;

//---------------------------------------------------------------------------------------------------------------

#include <getopt.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <cstdlib>

#include "global.hpp"
#include "custom_console_output.hpp"

#if defined(USE_LOGGER)
import pineaplog;
#endif /* defined(USE_LOGGER) */

//---------------------------------------------------------------------------------------------------------------

export module flags_parser;

//---------------------------------------------------------------------------------------------------------------

void set_getopt_args_default_values();

//---------------------------------------------------------------------------------------------------------------

constexpr option long_options[] =
{
    {"help"         , no_argument      , 0, 'h'},
    {"bad_triangles", no_argument      , 0, 'b'},
    {"color"        , no_argument      , 0, 'c'},
    {""             , 0                , 0,  0 }, /* just for safety */
};

//---------------------------------------------------------------------------------------------------------------

export namespace FlagsParsing
{

//---------------------------------------------------------------------------------------------------------------

struct program_options_t
{
    unsigned char color_output  : 1 = false;
    unsigned char bad_triangles : 1 = false;
};

//---------------------------------------------------------------------------------------------------------------

class flags_parser
{
    public:
        program_options_t get_program_options() const;

        flags_parser() = delete;
        flags_parser(int argc, char* argv[]);

    private:
        program_options_t program_options_;

        [[noreturn]]        
        void parse_flag_help          () const;
        void parse_color_flag         ();
        void parse_bad_triangles_flag ();
        void parse_not_a_flag         (const char* argument); // maybe test file name, maybe invalid option

        [[noreturn]]
        void undefined_option         (const char* argument) const;
};

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

// class public methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

flags_parser::flags_parser(int argc, char* argv[]) :
program_options_()
{
    static constexpr int undefined_option_key = -1;

    for (int options_iterator = 1; options_iterator < argc; options_iterator++)
    {
        int option = getopt_long(argc, argv, "hcb", long_options, nullptr);
    
        switch (option)
        {
            case 'h'                 : parse_flag_help         ();                       continue;
            case 'b'                 : parse_bad_triangles_flag();                       continue;
            case 'c'                 : parse_color_flag        ();                       continue;
            case undefined_option_key: undefined_option        (argv[options_iterator]); continue;
            default                  : undefined_option        (argv[options_iterator]); continue;
        }
    }

    set_getopt_args_default_values();
}

//---------------------------------------------------------------------------------------------------------------

program_options_t flags_parser::get_program_options() const
{
    return program_options_;
}

// class private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void flags_parser::parse_flag_help() const
{
    std::cout <<
    BOLD
    "There are all flags and parametrs:\n"
    RESET_CONSOLE_OUT
    "-h, --help\n"
    "-b, --bad_triangles\n"
    "-c, --color\b"
    "\n"
    "What does it mean?\n"
    "-h, --help\n"
    "these flags show information about all flags and their functions.\n"
    "\n"
    "-b, --bad_triangles\n"
    "use it, if you are not sure, that all your triangles are valid.\n"
    "this option add correct check of bad triangles intersections.\n"
    "without this option program think, that all triangles are valid.\n"
    "\n"
    "-c, --color\n"
    "just add color to program output.\n"
    "\n"
    "So, that was all, what I know about flags in this program.\n"
    "Good luck, I love you " HEART
    "\n";

    exit(EXIT_SUCCESS); // good exit :)
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_color_flag()
{
    program_options_.color_output = true;
}

//---------------------------------------------------------------------------------------------------------------

void flags_parser::parse_bad_triangles_flag()
{
    program_options_.bad_triangles = true;
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

} /* namespace FlagsParsing */

//---------------------------------------------------------------------------------------------------------------

void set_getopt_args_default_values()
{
    optarg = nullptr;
    optind = 1;
}

//---------------------------------------------------------------------------------------------------------------

