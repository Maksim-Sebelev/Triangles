cmake_minimum_required(VERSION 3.28) # 3.28 for modules

project(Triangles
    LANGUAGES CXX
    VERSION 2.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================================================
# functions to add some debug options and sanitizers to target in Debug mode

function(target_debug_options target)
    if (MSVC) # правда что ли?
        set(DEBUG_FLAGS
            /W4
            /Wx
        )

        target_compile_options(${target}
            PRIVATE
                $<$<CONFIG:Debug>:${DEBUG_FLAGS}>
        )    

    else() # exptect g++ or clang++
        # some saitizers for debug version
        set(DEBUG_SANITIZERS
            -fsanitize=address
            -fsanitize=alignment
            -fsanitize=bool
            -fsanitize=bounds
            -fsanitize=enum
            -fsanitize=float-cast-overflow
            -fsanitize=undefined
            -fsanitize=unreachable
            -fsanitize=vla-bound
            -fsanitize=vptr
            -fsanitize=float-divide-by-zero
            -fsanitize=integer-divide-by-zero
            -fsanitize=leak
            -fsanitize=nonnull-attribute
            -fsanitize=null
            -fsanitize=return
            -fsanitize=returns-nonnull-attribute
            -fsanitize=shift
            -fsanitize=signed-integer-overflow
            # -fsanitize=object-size
        )

        set(DEBUG_FLAGS
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Weffc++
            # experimental flags:
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wconversion
            -Wdouble-promotion
            -Wformat=2
            -Wimplicit-fallthrough
        )

        target_compile_options(${target}
            PRIVATE
                $<$<CONFIG:Debug>:
                    ${DEBUG_SANITIZERS}
                    ${DEBUG_FLAGS}
                >
        )

        target_compile_definitions(${target}
            PRIVATE
                $<$<CONFIG:Debug>:_DEBUG>
        )

        # linking sanitizers
        target_link_options(${target}
            PRIVATE
                $<$<CONFIG:Debug>:${DEBUG_SANITIZERS}>
        )

    endif()

endfunction(target_debug_options)

# =============================================================================================================
# set some variables for add_executale

# set some include directories
set(GLOBAL_INCLUDE_DIR           ${PROJECT_SOURCE_DIR}/include/global)
set(GENERATE_PROGRAM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/create_run_program_file) # for some cringe

set(MODULES_DIR                  ${PROJECT_SOURCE_DIR}/modules)

# =============================================================================================================

set(MATH_LIB math_lib)

set(MATH_MODULE_DIR ${MODULES_DIR}/math)

add_library(${MATH_LIB})

target_sources(${MATH_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE CXX_MODULES
        FILES
            ${MATH_MODULE_DIR}/compare/compare.cppm
            ${MATH_MODULE_DIR}/linear_algebra/vector.cppm
            ${MATH_MODULE_DIR}/linear_algebra/matrix.cppm
            ${MATH_MODULE_DIR}/linear_algebra/linear_systems.cppm
)

target_include_directories(${MATH_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for math_lib
target_debug_options(${MATH_LIB})

# =============================================================================================================

set(GEOMETRY_MODULES_DIR ${MODULES_DIR}/geometry)

# =============================================================================================================

set(GLOBAL_GEOMETRY_LIB _global_geometry)
add_library(${GLOBAL_GEOMETRY_LIB})

set(GLOBAL_GEOMETRY_MODULES_DIR ${GEOMETRY_MODULES_DIR}/global)

target_sources(${GLOBAL_GEOMETRY_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE     CXX_MODULES
        FILES
            ${GLOBAL_GEOMETRY_MODULES_DIR}/constants.cppm
            ${GLOBAL_GEOMETRY_MODULES_DIR}/relative_positions.cppm
)

target_include_directories(${GLOBAL_GEOMETRY_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for global_geometry_lib
target_debug_options(${GLOBAL_GEOMETRY_LIB})

# =============================================================================================================

set(HELP_GEOMETRY_LIB _help_geometry)
add_library(${HELP_GEOMETRY_LIB})

set(HELP_GEOMETRY_MODULES_DIR ${GEOMETRY_MODULES_DIR}/base_geometry_obj)

target_sources(${HELP_GEOMETRY_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE     CXX_MODULES
        FILES
            ${HELP_GEOMETRY_MODULES_DIR}/point.cppm
            ${HELP_GEOMETRY_MODULES_DIR}/line.cppm
            ${HELP_GEOMETRY_MODULES_DIR}/plain.cppm
)

target_include_directories(${HELP_GEOMETRY_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${HELP_GEOMETRY_LIB}
    PRIVATE
        ${MATH_LIB}
        ${GLOBAL_GEOMETRY_LIB}
)
# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for help_geomtery_lib
target_debug_options(${HELP_GEOMETRY_LIB})

# =============================================================================================================

set(TRIANGLES_LIB _triangles)
add_library(${TRIANGLES_LIB})

set(TRIANGLE_MODULES_DIR ${GEOMETRY_MODULES_DIR}/triangle)

target_sources(${TRIANGLES_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE     CXX_MODULES
        FILES ${TRIANGLE_MODULES_DIR}/triangle.cppm
)

target_include_directories(${TRIANGLES_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${TRIANGLES_LIB}
    PRIVATE
        ${MATH_LIB}
        ${GLOBAL_GEOMETRY_LIB}

    PUBLIC
        ${HELP_GEOMETRY_LIB}
)
# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for triangels_lib
target_debug_options(${TRIANGLES_LIB})

# =============================================================================================================

set(OCTREE_LIB _octree)
add_library(${OCTREE_LIB})

set(OCTREE_MODULES_DIR ${GEOMETRY_MODULES_DIR}/tree)

target_sources(${OCTREE_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE     CXX_MODULES
        FILES ${OCTREE_MODULES_DIR}/octree.cppm
)

target_include_directories(${OCTREE_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${OCTREE_LIB}
    PRIVATE
        ${MATH_LIB}
        ${GLOBAL_GEOMETRY_LIB}

    PUBLIC
        ${HELP_GEOMETRY_LIB}
        ${TRIANGLES_LIB}
)
# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for octree_lib
target_debug_options(${OCTREE_LIB})

# =============================================================================================================

set(TEST_LIB _test)
add_library(${TEST_LIB})

set(TEST_LIB_SRC_DIR ${MODULES_DIR}/test)

set(TEST_LIB_SRC
    ${TEST_LIB_SRC_DIR}/global/test_files.cppm
    ${TEST_LIB_SRC_DIR}/flag_parse/flag_parser.cppm
    ${TEST_LIB_SRC_DIR}/read_input/read_test_data.cppm
    ${TEST_LIB_SRC_DIR}/read_input/read_test_answer.cppm
    ${TEST_LIB_SRC_DIR}/read_input/get_test_data.cppm
    ${TEST_LIB_SRC_DIR}/read_input/input_stream.cppm
    ${TEST_LIB_SRC_DIR}/parse_result/parse_test_result.cppm
    ${TEST_LIB_SRC_DIR}/execute_test/execute_test.cppm
)

target_sources(${TEST_LIB}
    PUBLIC
        FILE_SET CXX_MODULES
        TYPE     CXX_MODULES
        FILES ${TEST_LIB_SRC}
)

target_include_directories(${TEST_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${TEST_LIB}
    PUBLIC
        ${OCTREE_LIB}
)

# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for test_lib
target_debug_options(${TEST_LIB})

# =============================================================================================================

# set name main (and the only one) executable file of this project (float coordainate type)
set(TRIANGLES_EXE           run_triangles                                    )
set(TRIANGLES_MAIN_SOURCE   ${PROJECT_SOURCE_DIR}/src/run_float_triangles.cpp)

add_executable(${TRIANGLES_EXE}
               ${TRIANGLES_MAIN_SOURCE}
)

# include all headers
target_include_directories(${TRIANGLES_EXE}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
        ${GENERATE_PROGRAM_INCLUDE_DIR}
)

target_link_libraries(${TRIANGLES_EXE}
    PRIVATE
        ${TEST_LIB}
)

# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for run_trinagles
target_debug_options(${TRIANGLES_EXE})

# =============================================================================================================
# for fun adding a double coordinate type triangles
set(DOUBLE_TRIANGLES_EXE           run_double_triangles                              )
set(DOUBLE_TRIANGLES_MAIN_SOURCE   ${PROJECT_SOURCE_DIR}/src/run_double_triangles.cpp)

add_executable(${DOUBLE_TRIANGLES_EXE}
            ${DOUBLE_TRIANGLES_MAIN_SOURCE}
)

# include all headers
target_include_directories(${DOUBLE_TRIANGLES_EXE}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
        ${GENERATE_PROGRAM_INCLUDE_DIR}
)

target_link_libraries(${DOUBLE_TRIANGLES_EXE}
    PRIVATE
        ${TEST_LIB}
)

# NOT default cmake functions
# this is my function, defined in 14 line of this `CMakeLists.txt` file
# set some compile options for run_double_trinagles
target_debug_options(${DOUBLE_TRIANGLES_EXE})
# =============================================================================================================

# =============================================================================================================
# add custom targets 'rebuild'

set(REBUILD_TARGET rebuild)

add_custom_target(${REBUILD_TARGET}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Rebuilding project"
)

# =============================================================================================================
# TODO: compile logger as external project (ExternalProject_Add)

# add logger if defined USE_LOGGER flag
if (USE_LOGGER)
    message("-- Adding logger")

    # костыль, но так надо чтобы не ждать логгера по полгода и без запары с exterbal project (я 2 раза пытался его настроить, оба раза проиграл)
    set(MAIN_PROJECT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE Release)

    set(LOGGER_PROJECT_DIR ${CMAKE_SOURCE_DIR}/third-party/logger/Src)
    add_subdirectory(${LOGGER_PROJECT_DIR})

    function(add_logger some_target)
        target_link_libraries(${some_target} 
            PRIVATE
                logger
        )
        target_compile_definitions(${some_target}
            PRIVATE
                USE_LOGGER
        )
    endfunction(add_logger)

    set(TARGETS_USING_LOGGER
        ${TRIANGLES_EXE}
        ${DOUBLE_TRIANGLES_EXE}
        ${HELP_GEOMETRY_LIB}
        ${TRIANGLES_LIB}
        ${OCTREE_LIB}
        ${TEST_LIB}
        ${MATH_LIB}
        ${ALL_UNIT_TESTS_TARGET}
    )

    foreach(target_that_use_logger ${TARGETS_USING_LOGGER})
        add_logger(${target_that_use_logger})
    endforeach()

    set(CMAKE_BUILD_TYPE ${MAIN_PROJECT_BUILD_TYPE})

endif()

# =============================================================================================================

if (TREE_GRAPHIC_DUMP)
    
endif()
# =============================================================================================================
# =============================================================================================================
# =============================================================================================================

# TESTS (unit and e2e)
enable_testing()

# =============================================================================================================
# UNIT-TESTS

find_package(GTest REQUIRED)
include(GoogleTest)
# WHAT DOU YOU THINK ABOUT this:
# if (NOT find_package(gtest))
# <fetchcontnent installation>
# endif()

# =============================================================================================================

# function to genererate unit-test
# IF SECOND AND/OR THIRD ARGS ARE/IS LIST(-S), DONT FORGET ABOUT QUOTES

# target_unit_test
function(target_unit_test unit_test_target_name unit_test_src modules_library)

    add_executable(${unit_test_target_name} EXCLUDE_FROM_ALL
                   ${unit_test_src}
    )

    target_link_libraries(${unit_test_target_name}
        PRIVATE
            ${modules_library}
            GTest::gtest_main
    )

    target_include_directories(${unit_test_target_name}
        PRIVATE
            ${GLOBAL_INCLUDE_DIR}
    )

    # NOT default cmake function
    # MY function for add compile options to executable
    target_debug_options(${unit_test_target_name})
endfunction(target_unit_test)

# =============================================================================================================

# creating unit-tests

# =============================================================================================================

# set name of target to build all unit-tests
set(BUILD_ALL_UNIT_TESTS_TARGET build_tests)

# set dir with unit-tests
set(UNIT_TEST_DIR ${PROJECT_SOURCE_DIR}/../tests/unit-tests)

# =============================================================================================================

# MATH UNIT TESTS

# =============================================================================================================

# create floating point numbers operations unit test

set(COMPARE_UNIT_TEST_EXE compare_unit_test                          )
set(COMPARE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_compare.cpp)

target_unit_test(
    "${COMPARE_UNIT_TEST_EXE}"
    "${COMPARE_UNIT_TEST_SRC}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create vector unit test

set(VECTOR_UNIT_TEST_EXE vector_unit_test                          )
set(VECTOR_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_vector.cpp)

target_unit_test(
    "${VECTOR_UNIT_TEST_EXE}"
    "${VECTOR_UNIT_TEST_SRC}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create matrix unit test

set(MATRIX_UNIT_TEST_EXE matrix_unit_test                          )
set(MATRIX_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_matrix.cpp)

target_unit_test(
    "${MATRIX_UNIT_TEST_EXE}"
    "${MATRIX_UNIT_TEST_SRC}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create linear systems unit test

set(LIN_SYS_UNIT_TEST_EXE linear_systems_unit_test                         )
set(LIN_SYS_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_linear_system.cpp)

target_unit_test(
    "${LIN_SYS_UNIT_TEST_EXE}"
    "${LIN_SYS_UNIT_TEST_SRC}"
    "${MATH_LIB}"
)

# =============================================================================================================

set(MATH_UNIT_TESTS
    ${COMPARE_UNIT_TEST_EXE}
    ${VECTOR_UNIT_TEST_EXE}
    ${MATRIX_UNIT_TEST_EXE}
    ${LIN_SYS_UNIT_TEST_EXE}
)

set(MATH_UNIT_TESTS_TARGET math_unit_tests)

add_custom_target(${MATH_UNIT_TESTS_TARGET}
    DEPENDS ${MATH_UNIT_TESTS}
)

# =============================================================================================================
# create point unit test

set(POINT_UNIT_TEST_EXE point_unit_test                              )
set(POINT_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_point.cpp)

target_unit_test(
    "${POINT_UNIT_TEST_EXE}"
    "${POINT_UNIT_TEST_SRC}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================
# create line unit test

set(LINE_UNIT_TEST_EXE line_unit_test                              )
set(LINE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_line.cpp)

target_unit_test(
    "${LINE_UNIT_TEST_EXE}"
    "${LINE_UNIT_TEST_SRC}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================

set(PLAIN_UNIT_TEST_EXE plain_unit_test                              )
set(PLAIN_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_plain.cpp)

target_unit_test(
    "${PLAIN_UNIT_TEST_EXE}"
    "${PLAIN_UNIT_TEST_SRC}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================
# create triangles unit test

set(TRIANGLES_UNIT_TEST_EXE triangles_unit_test                              )
set(TRIANGLES_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_triangles.cpp)

target_unit_test(
    "${TRIANGLES_UNIT_TEST_EXE}"
    "${TRIANGLES_UNIT_TEST_SRC}"
    "${TRIANGLES_LIB}"
)

# =============================================================================================================
# create octree unit test

set(OCTREE_UNIT_TEST_EXE octree_unit_test                              )
set(OCTREE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_octree.cpp)

target_unit_test(
    "${OCTREE_UNIT_TEST_EXE}"
    "${OCTREE_UNIT_TEST_SRC}"
    "${OCTREE_LIB}"
)

# =============================================================================================================

set(GEOMETRY_UNIT_TESTS
    ${POINT_UNIT_TEST_EXE}
    ${LINE_UNIT_TEST_EXE}
    ${PLAIN_UNIT_TEST_EXE}
    ${TRIANGLES_UNIT_TEST_EXE}
    ${OCTREE_UNIT_TEST_EXE}
)

set(GEOMETRY_UNIT_TESTS_TARGET geometry_unit_tests)

add_custom_target(${GEOMETRY_UNIT_TESTS_TARGET}
    DEPENDS ${GEOMETRY_UNIT_TESTS}
)

# =============================================================================================================
# create read answer unit test

set(READ_ANS_UNIT_TEST_EXE read_ans_unit_test                             )
set(READ_ANS_UNIT_TEST_SRC ${UNIT_TEST_DIR}/test/unit_test_read_answer.cpp)

target_unit_test(
    "${READ_ANS_UNIT_TEST_EXE}"
    "${READ_ANS_UNIT_TEST_SRC}"
    "${TEST_LIB}"
)

# =============================================================================================================
# create read test data unit test

set(READ_TEST_DATA_UNIT_TEST_EXE read_test_data_unit_test                          )
set(READ_TEST_DATA_UNIT_TEST_SRC ${UNIT_TEST_DIR}/test/unit_test_read_test_data.cpp)

target_unit_test(
    "${READ_TEST_DATA_UNIT_TEST_EXE}"
    "${READ_TEST_DATA_UNIT_TEST_SRC}"
    "${TEST_LIB}"
)

# =============================================================================================================
# create flag parser unit test

set(FLAG_PARSER_UNIT_TEST_EXE flag_parser_unit_test                          )
set(FLAG_PARSER_UNIT_TEST_SRC ${UNIT_TEST_DIR}/test/unit_test_flag_parser.cpp)

target_unit_test(
    "${FLAG_PARSER_UNIT_TEST_EXE}"
    "${FLAG_PARSER_UNIT_TEST_SRC}"
    "${TEST_LIB}"
)

# =============================================================================================================
# create get test data unit test

set(GET_TEST_DATA_UNIT_TEST_EXE get_test_data_unit_test                          )
set(GET_TEST_DATA_UNIT_TEST_SRC ${UNIT_TEST_DIR}/test/unit_test_get_test_data.cpp)

target_unit_test(
    "${GET_TEST_DATA_UNIT_TEST_EXE}"
    "${GET_TEST_DATA_UNIT_TEST_SRC}"
    "${TEST_LIB}"
)

# =============================================================================================================

set(TEST_DATA_UNIT_TESTS
    ${READ_ANS_UNIT_TEST_EXE}
    ${READ_TEST_DATA_UNIT_TEST_EXE}
    ${FLAG_PARSER_UNIT_TEST_EXE}
    ${GET_TEST_DATA_UNIT_TEST_EXE}
)

set(TEST_DATA_UNIT_TESTS_TARGET     test_data_unit_tests    )

add_custom_target(${TEST_DATA_UNIT_TESTS_TARGET}
    DEPENDS ${TEST_DATA_UNIT_TESTS}
)

# =============================================================================================================

set(ALL_UNIT_TESTS_TARGET
    ${TEST_DATA_UNIT_TESTS}
    ${MATH_UNIT_TESTS}
    ${GEOMETRY_UNIT_TESTS}
)

# create target to compile all unit tests
add_custom_target(${BUILD_ALL_UNIT_TESTS_TARGET}
    DEPENDS ${ALL_UNIT_TESTS_TARGET}
)

# =============================================================================================================

include(CTest)

# function for adding tests (not unit) to some executable files
function(target_e2e_test test_executable test_dir ans_dir)

    file(GLOB test_files "${test_dir}/*.dat") # save all tests -files in variable 'test_files'

    # get quantity of tests
    list(LENGTH test_files num_tests)

    # check that at least 1 test was found
    if(${num_tests} EQUAL 0)
        message(WARNING "No .dat files found in ${test_dir}")
        return()
    endif()

    # add tests
    foreach(test_file ${test_files})

        # get name of answer file
        get_filename_component(test_file_name "${test_file}" NAME_WE) # NAME_WE = without extension
        set(answer_file "${ans_dir}/${test_file_name}.ans")

        # create test_name
        set(test_name ${test_executable}_${test_file_name})

        if (VERBOSE_OUTPUT)
            set(TEST_COMMAND ./${test_executable} --verbose --input_stream=files ${test_file} ${answer_file})
        else()
            set(TEST_COMMAND ./${test_executable} --input_stream=files ${test_file} ${answer_file})
        endif()

        # add test
        add_test(
            NAME ${test_name}
            # execute test
        
            COMMAND ${TEST_COMMAND}
            # working directory - dircetory with executable file
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

    endforeach()
endfunction(target_e2e_test)

# =============================================================================================================

# set the directory with e2e tests for run_triangles
set(TRIANGLES_TEST_DIR ${PROJECT_SOURCE_DIR}/../tests/e2e)

# set the directories with e2e test data for run_triangles
set(TRIANGLES_TEST_DAT_DIR ${TRIANGLES_TEST_DIR}/dat)

# set the directories with e2e test answers for run_triangles
set(TRIANGLES_TEST_ANS_DIR ${TRIANGLES_TEST_DIR}/ans)

# add e2e tests to float triangles
target_e2e_test(${TRIANGLES_EXE}
               ${TRIANGLES_TEST_DAT_DIR}
               ${TRIANGLES_TEST_ANS_DIR}
)

# add e2e tests to long double triangles
target_e2e_test(${DOUBLE_TRIANGLES_EXE}
               ${TRIANGLES_TEST_DAT_DIR}
               ${TRIANGLES_TEST_ANS_DIR}
)

# =============================================================================================================

foreach(unit_test_target ${ALL_UNIT_TESTS_TARGET})
    gtest_discover_tests(${unit_test_target})
endforeach()

# =============================================================================================================
