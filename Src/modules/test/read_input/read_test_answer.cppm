module;

//---------------------------------------------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include "global.hpp"

#if defined(_DEBUG)
#include "custom_console_output.hpp"
#endif // defined(_DEBUG)

#if defined(USE_LOGGER)
import logger;

#endif // defined(USE_LOGGER)

//---------------------------------------------------------------------------------------------------------------

export module read_test_answer;

//---------------------------------------------------------------------------------------------------------------

export namespace InputData
{

//---------------------------------------------------------------------------------------------------------------

struct test_answer_t
{
    public:
        size_t              get_good_triangles_quantity  (        ) const;
        size_t              get_number_of_i_good_triangle(size_t i) const;
        std::vector<size_t> get_triangles                (        ) const;
        ON_DEBUG(
        void                dump                         (        ) const;
        )

        /*
        ctor for reading from stdin
        made nothing, but needed for 
        initialization test_data_t
        made nothing, because when input from stdin
        you check test result yourself.
        in that case answer is in stdout 
        */
        test_answer_t() = default;                   // ctor for reading from stdin
        test_answer_t(std::string_view answer_file); // reading from .dat file

    private:
        size_t              good_triangles_quantity_;
        std::vector<size_t> good_triangles_numbers_ ;
        std::string         answer_file_            ;
        std::ifstream       file_                   ;

        // struct methods for reading from .ans file
        void open_answer_file           ();
        void read_test_answer_from_ans  ();
        void close_answer_file          ();
    
        [[noreturn]]
        void failed_open_answer_file    ();

        [[noreturn]]
        void failed_read_answer_from_ans();

        [[noreturn]]
        void empty_answer_file          ();
};

//---------------------------------------------------------------------------------------------------------------

// ctor for reading from .ans file
test_answer_t::test_answer_t(std::string_view answer_file) :
good_triangles_numbers_(           ),
answer_file_           (answer_file)
{
    open_answer_file         ();
    read_test_answer_from_ans();
    close_answer_file        ();
}

//---------------------------------------------------------------------------------------------------------------

// public methodes
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

std::vector<size_t> test_answer_t::get_triangles() const
{
    return good_triangles_numbers_;
}

//---------------------------------------------------------------------------------------------------------------

size_t test_answer_t::get_good_triangles_quantity() const
{
    return good_triangles_quantity_;
}

//---------------------------------------------------------------------------------------------------------------

size_t test_answer_t::get_number_of_i_good_triangle(size_t i) const
{
    return good_triangles_numbers_[i];
}

//---------------------------------------------------------------------------------------------------------------

ON_DEBUG(
void test_answer_t::dump() const
{
    std::cout << "=========== TEST DATA DUMP ==========="               << std::endl
                                                                        << std::endl;

    std::cout << "Reading asnwer from \"" BOLD WHITE << answer_file_
                                                                        << RESET_CONSOLE_OUT "\""
                                                                        << std::endl
                                                                        << std::endl;

    std::cout << "Good triangles quatity = " BOLD GREEN << good_triangles_quantity_
                                                                        << RESET_CONSOLE_OUT
                                                                        << std::endl
                                                                        << std::endl;

    for (size_t i = 0; i < good_triangles_quantity_; i++)
        std::cout << "good_triangles[" << BOLD WHITE << i << "] = " RED
                  << good_triangles_numbers_[i] << RESET_CONSOLE_OUT << std::endl;

    std::cout << std::endl 
              << "========= TEST DATA DUMP END ========="               << std::endl
                                                                        << std::endl;
}
) // ON_DEBUG

// private methods
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

void test_answer_t::open_answer_file()
{
    file_.open(answer_file_);
    
    if (!file_.is_open())
        failed_open_answer_file();
}

//---------------------------------------------------------------------------------------------------------------

void test_answer_t::read_test_answer_from_ans()
{
    while (true)
    {
        size_t   tmp_triangle_number = 0;
        file_ >> tmp_triangle_number;

        if (file_.fail()) break;

        good_triangles_numbers_.push_back(tmp_triangle_number);
    }

    good_triangles_quantity_ = good_triangles_numbers_.size();
}

//---------------------------------------------------------------------------------------------------------------

void test_answer_t::close_answer_file()
{
    file_.close();
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void test_answer_t::failed_open_answer_file()
{
    std::cerr << "Failed open '" << answer_file_ << "'"
              << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

[[noreturn]]
void test_answer_t::failed_read_answer_from_ans()
{
    std::cerr << "Failed read answer from '" << answer_file_
              << "'" << std::endl;

    exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace InputData */

//---------------------------------------------------------------------------------------------------------------
