#include <gtest/gtest.h>

import flags_parser;
import input_stream;

using namespace FlagsParsing;
using namespace InputData;


TEST(FlagParserTesting, FileStream_1)
{
    int argc = 4;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*) "--input_stream=files",
        (char*) "../tests/e2e/dat/1.dat",
        (char*) "../tests/e2e/ans/1.ans",
    };

    flags_parser parser(argc, argv);

    EXPECT_EQ(parser.get_input_stream(), input_stream::dat_file_stream);
    EXPECT_EQ(parser.get_test_files().test_file  , "../tests/e2e/dat/1.dat");
    EXPECT_EQ(parser.get_test_files().answer_file, "../tests/e2e/ans/1.ans");

    EXPECT_EQ(parser.print_result_and_dont_check_answer(), false);
}

TEST(FlagParserTesting, FileStream_2)
{
    int argc = 4;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*) "-ifiles",
        (char*) "../tests/e2e/ans/1.ans",
        (char*) "../tests/e2e/dat/1.dat",
    };

    flags_parser parser(argc, argv);

    EXPECT_EQ(parser.get_input_stream(), input_stream::dat_file_stream);
    EXPECT_EQ(parser.get_test_files().test_file  , "../tests/e2e/dat/1.dat");
    EXPECT_EQ(parser.get_test_files().answer_file, "../tests/e2e/ans/1.ans");

    EXPECT_EQ(parser.print_result_and_dont_check_answer(), false);
}


TEST(FlagParserTesting, StandartStream_1)
{
    int argc = 1;
    char* argv[] =
    {
        (char*)"as if executable file name",
    };

    flags_parser parser(argc, argv);

    EXPECT_EQ(parser.get_input_stream(), input_stream::standart_input);

    EXPECT_EQ(parser.print_result_and_dont_check_answer(), true);
}


TEST(FlagParserTesting, StandartStream_2)
{
    int argc = 1;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"--input_stream=stdin",
    };

    flags_parser parser(argc, argv);

    EXPECT_EQ(parser.get_input_stream(), input_stream::standart_input);

    EXPECT_EQ(parser.print_result_and_dont_check_answer(), true);
}


TEST(FlagParserTesting, StandartStream_)
{
    int argc = 1;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"-istdin",
    };

    flags_parser parser(argc, argv);

    EXPECT_EQ(parser.get_input_stream(), input_stream::standart_input);

    EXPECT_EQ(parser.print_result_and_dont_check_answer(), true);
}



TEST(FlagParserTesting, BadInputStream)
{
    int argc = 2;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"--input_stream=something_bad",
    };

    EXPECT_EXIT(
        flags_parser parser(argc, argv),
        ::testing::ExitedWithCode(EXIT_FAILURE),
        ".*"
    );
}

TEST(FlagParserTesting, RedefineInptStream)
{
    int argc = 3;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"--input_stream=files",
        (char*)"--input_stream=stdin",
    };

    EXPECT_EXIT(
        flags_parser parser(argc, argv),
        ::testing::ExitedWithCode(EXIT_FAILURE),
        ".*"
    );
}

TEST(FlagParserTesting, RedefineDatFile)
{
    int argc = 5;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"--input_stream=files",
        (char*)"1.dat",
        (char*)"1.ans",
        (char*)"2.dat",
    };

    EXPECT_EXIT(
        flags_parser parser(argc, argv),
        ::testing::ExitedWithCode(EXIT_FAILURE),
        ".*"
    );
}

TEST(FlagParserTesting, RedefineAnsFile)
{
    int argc = 4;
    char* argv[] =
    {
        (char*)"as if executable file name",
        (char*)"--input_stream=files",
        (char*)"1.ans",
        (char*)"2.ans",
    };

    EXPECT_EXIT(
        flags_parser parser(argc, argv),
        ::testing::ExitedWithCode(EXIT_FAILURE),
        ".*"
    );
}


TEST(FlagParserTesting, GetFileWhenInputStreamIsStdin)
{
    int argc = 1;
    char* argv[] =
    {
        (char*)"as if executable file name",
    };

    flags_parser parser(argc, argv);

    EXPECT_EXIT(
        parser.get_test_files(),
        ::testing::ExitedWithCode(EXIT_FAILURE),
        ".*"
    );
}
