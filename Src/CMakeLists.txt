cmake_minimum_required(VERSION 3.28) # 3.28 for modules

project(Triangles
    LANGUAGES C CXX
    VERSION 2.1
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SETTINGS_DIR ${PROJECT_SOURCE_DIR}/cmake)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(${CMAKE_SETTINGS_DIR}/debug.cmake)

# =============================================================================================================

# set some include directories
set(INCLUDE_DIR                  ${PROJECT_SOURCE_DIR}/include)
set(GLOBAL_INCLUDE_DIR           ${INCLUDE_DIR}/global)
set(GENERATE_PROGRAM_INCLUDE_DIR ${INCLUDE_DIR}/create_run_program_file) # for some cringe

# set some sources directories
set(SRC_DIR                      ${PROJECT_SOURCE_DIR}/src)
set(THIRD_PARTY_DIR              ${PROJECT_SOURCE_DIR}/third-party)

# =============================================================================================================

set(MATH_LIB math)

set(MATH_MODULE_DIR ${SRC_DIR}/math)

add_library(${MATH_LIB})

target_sources(${MATH_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
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
# function defined in Src/cmake/debug.cmake
# set some compile options for math_lib
target_hard_debug_options(${MATH_LIB})

# =============================================================================================================

set(GEOMETRY_SRC_DIR ${SRC_DIR}/geometry)

# =============================================================================================================

set(GLOBAL_GEOMETRY_LIB global_geometry)
add_library(${GLOBAL_GEOMETRY_LIB})

set(GLOBAL_GEOMETRY_SRC_DIR ${GEOMETRY_SRC_DIR}/global)

target_sources(${GLOBAL_GEOMETRY_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${GLOBAL_GEOMETRY_SRC_DIR}/constants.cppm
            ${GLOBAL_GEOMETRY_SRC_DIR}/relative_positions.cppm
)

target_include_directories(${GLOBAL_GEOMETRY_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for global_geometry_lib
target_hard_debug_options(${GLOBAL_GEOMETRY_LIB})

# =============================================================================================================

set(HELP_GEOMETRY_LIB help_geometry)
add_library(${HELP_GEOMETRY_LIB})

set(HELP_GEOMETRY_SRC_DIR ${GEOMETRY_SRC_DIR}/base_geometry_obj)

target_sources(${HELP_GEOMETRY_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${HELP_GEOMETRY_SRC_DIR}/point.cppm
            ${HELP_GEOMETRY_SRC_DIR}/line.cppm
            ${HELP_GEOMETRY_SRC_DIR}/plain.cppm
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
# function defined in Src/cmake/debug.cmake
# set some compile options for help_geomtery_lib
target_hard_debug_options(${HELP_GEOMETRY_LIB})

# =============================================================================================================

set(TRIANGLES_LIB triangles)
add_library(${TRIANGLES_LIB})

set(TRIANGLE_SRC_DIR ${GEOMETRY_SRC_DIR}/triangle)

target_sources(${TRIANGLES_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${TRIANGLE_SRC_DIR}/triangle.cppm
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
# function defined in Src/cmake/debug.cmake
# set some compile options for triangels_lib
target_hard_debug_options(${TRIANGLES_LIB})

# =============================================================================================================

set(OCTREE_LIB octree)
add_library(${OCTREE_LIB})

set(OCTREE_SRC_DIR ${GEOMETRY_SRC_DIR}/tree)

target_sources(${OCTREE_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${OCTREE_SRC_DIR}/octree.cppm
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
# function defined in Src/cmake/debug.cmake
# set some compile options for octree_lib
target_hard_debug_options(${OCTREE_LIB})

# =============================================================================================================

set(READ_INPUT_LIB read_input)
add_library(${READ_INPUT_LIB})

set(READ_INPUT_LIB_SRC_DIR ${SRC_DIR}/read_input)

set(READ_INPUT_SRC
    ${READ_INPUT_LIB_SRC_DIR}/read_input_data.cppm
)

target_sources(${READ_INPUT_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${READ_INPUT_SRC}
)

target_include_directories(${READ_INPUT_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${READ_INPUT_LIB}
    PUBLIC
        ${TRIANGLES_LIB}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for test_lib
target_hard_debug_options(${READ_INPUT_LIB})

# =============================================================================================================

set(FLAGS_PARSER_LIB flags_parser)
add_library(${FLAGS_PARSER_LIB})

set(FLAGS_PARSER_LIB_SRC_DIR ${SRC_DIR}/flags_parser)

set(FLAGS_PARSER_SRC
    ${FLAGS_PARSER_LIB_SRC_DIR}/flags_parser.cppm
)

target_sources(${FLAGS_PARSER_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${FLAGS_PARSER_SRC}
)

target_include_directories(${FLAGS_PARSER_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${FLAGS_PARSER_LIB}
    PUBLIC
        ${TRIANGLES_LIB}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for test_lib
target_hard_debug_options(${FLAGS_PARSER_LIB})

# =============================================================================================================

set(TRIANGLES_INTERSECTIONS_LIB find_triangles_intersections)
add_library(${TRIANGLES_INTERSECTIONS_LIB})

set(TRIANGLES_INTERSECTIONS_LIB_SRC_DIR ${SRC_DIR}/run_triangles_intersection)

set(TRIANGLES_INTERSECTIONS_SRC
    ${TRIANGLES_INTERSECTIONS_LIB_SRC_DIR}/run_triangles_intersection.cppm
)

target_sources(${TRIANGLES_INTERSECTIONS_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${TRIANGLES_INTERSECTIONS_SRC}
)

target_include_directories(${TRIANGLES_INTERSECTIONS_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
)

target_link_libraries(${TRIANGLES_INTERSECTIONS_LIB}
    PUBLIC
        ${TRIANGLES_LIB}
        ${OCTREE_LIB}
        ${FLAGS_PARSER_LIB}
        ${READ_INPUT_LIB}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for test_lib
target_hard_debug_options(${TRIANGLES_INTERSECTIONS_LIB})

# =============================================================================================================

set(MAIN_SRC_DIR ${SRC_DIR}/main)

# =============================================================================================================

# set name main (and the only one) executable file of this project (float coordainate type)
set(TRIANGLES_EXE           run_triangles                          )
set(TRIANGLES_MAIN_SOURCE   ${MAIN_SRC_DIR}/run_float_triangles.cpp)

add_executable(${TRIANGLES_EXE}
               ${TRIANGLES_MAIN_SOURCE}
)

# include all headers
target_include_directories(${TRIANGLES_EXE}
    PRIVATE
        ${GENERATE_PROGRAM_INCLUDE_DIR}
)

target_link_libraries(${TRIANGLES_EXE}
    PRIVATE
        ${READ_INPUT_LIB}
        ${FLAGS_PARSER_LIB}
        ${TRIANGLES_INTERSECTIONS_LIB}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for run_trinagles
target_hard_debug_options(${TRIANGLES_EXE})

# =============================================================================================================
# for fun adding a double coordinate type triangles
set(DOUBLE_TRIANGLES_EXE           run_double_triangles                         )
set(DOUBLE_TRIANGLES_MAIN_SOURCE   ${MAIN_SRC_DIR}/run_long_double_triangles.cpp)

add_executable(${DOUBLE_TRIANGLES_EXE}
            ${DOUBLE_TRIANGLES_MAIN_SOURCE}
)

# include all headers
target_include_directories(${DOUBLE_TRIANGLES_EXE}
    PRIVATE
        ${GENERATE_PROGRAM_INCLUDE_DIR}
)

target_link_libraries(${DOUBLE_TRIANGLES_EXE}
    PRIVATE
        ${READ_INPUT_LIB}
        ${FLAGS_PARSER_LIB}
        ${TRIANGLES_INTERSECTIONS_LIB}
)

# NOT default cmake functions
# function defined in Src/cmake/debug.cmake
# set some compile options for run_double_trinagles
target_hard_debug_options(${DOUBLE_TRIANGLES_EXE})
# =============================================================================================================

# =============================================================================================================
# add custom targets 'rebuild'
set(REBUILD_TARGET rebuild)

add_custom_target(${REBUILD_TARGET}
    COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}
    COMMENT "Rebuilding project"
)

# =============================================================================================================

# add logger if defined USE_LOGGER flag
if (USE_LOGGER)
    set(TARGETS_USING_LOGGER
        ${TRIANGLES_EXE}
        ${DOUBLE_TRIANGLES_EXE}
        ${HELP_GEOMETRY_LIB}
        ${TRIANGLES_LIB}
        ${OCTREE_LIB}
        ${READ_INPUT_LIB}
        ${MATH_LIB}
        ${ALL_UNIT_TESTS_TARGET}
    )

    include(${CMAKE_SETTINGS_DIR}/logger.cmake)

endif()

# =============================================================================================================

# add 2d dump
if (TREE_2D_DUMP)
    set(TARGETS_USING_2D_DUMP
        ${OCTREE_LIB}
        ${TRIANGLES_INTERSECTIONS_LIB}
    )
    include(${CMAKE_SETTINGS_DIR}/tree-2d-dump.cmake)
endif()

# =============================================================================================================

# add 3d dump
if (TREE_3D_DUMP)
    set(TARGETS_USING_3D_DUMP
        ${OCTREE_LIB}
        ${TRIANGLES_INTERSECTIONS_LIB}
    )
    include(${CMAKE_SETTINGS_DIR}/tree-3d-dump.cmake)
endif()

# =============================================================================================================
# =============================================================================================================
# =============================================================================================================

# TESTS (unit and e2e)

set(CMAKE_TESTS_DIR ${CMAKE_SETTINGS_DIR}/tests)

include(${CMAKE_TESTS_DIR}/global.cmake)

# =============================================================================================================

# UNIT-TESTS
include(${CMAKE_TESTS_DIR}/unit-tests.cmake)

# =============================================================================================================

# set name of target to build all unit-tests
set(BUILD_ALL_UNIT_TESTS_TARGET "build-tests")

# set dir with unit-tests
set(UNIT_TEST_DIR ${PROJECT_SOURCE_DIR}/../tests/unit-tests)

set(ALL_UNIT_TESTS_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIR})

# =============================================================================================================

# MATH UNIT TESTS

# =============================================================================================================

# create floating point numbers operations unit test

set(COMPARE_UNIT_TEST_EXE compare_unit_test                          )
set(COMPARE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_compare.cpp)

target_unit_test(
    "${COMPARE_UNIT_TEST_EXE}"
    "${COMPARE_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create vector unit test

set(VECTOR_UNIT_TEST_EXE vector_unit_test                          )
set(VECTOR_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_vector.cpp)

target_unit_test(
    "${VECTOR_UNIT_TEST_EXE}"
    "${VECTOR_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create matrix unit test

set(MATRIX_UNIT_TEST_EXE matrix_unit_test                          )
set(MATRIX_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_matrix.cpp)

target_unit_test(
    "${MATRIX_UNIT_TEST_EXE}"
    "${MATRIX_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${MATH_LIB}"
)

# =============================================================================================================
# create linear systems unit test

set(LIN_SYS_UNIT_TEST_EXE linear_systems_unit_test                         )
set(LIN_SYS_UNIT_TEST_SRC ${UNIT_TEST_DIR}/math/unit_test_linear_system.cpp)

target_unit_test(
    "${LIN_SYS_UNIT_TEST_EXE}"
    "${LIN_SYS_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
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
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================
# create line unit test

set(LINE_UNIT_TEST_EXE line_unit_test                              )
set(LINE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_line.cpp)

target_unit_test(
    "${LINE_UNIT_TEST_EXE}"
    "${LINE_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================

set(PLAIN_UNIT_TEST_EXE plain_unit_test                              )
set(PLAIN_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_plain.cpp)

target_unit_test(
    "${PLAIN_UNIT_TEST_EXE}"
    "${PLAIN_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${HELP_GEOMETRY_LIB}"
)

# =============================================================================================================
# create triangles unit test

set(TRIANGLES_UNIT_TEST_EXE triangles_unit_test                              )
set(TRIANGLES_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_triangles.cpp)

target_unit_test(
    "${TRIANGLES_UNIT_TEST_EXE}"
    "${TRIANGLES_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${TRIANGLES_LIB}"
)

# =============================================================================================================
# create octree unit test

set(OCTREE_UNIT_TEST_EXE octree_unit_test                              )
set(OCTREE_UNIT_TEST_SRC ${UNIT_TEST_DIR}/geometry/unit_test_octree.cpp)

target_unit_test(
    "${OCTREE_UNIT_TEST_EXE}"
    "${OCTREE_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
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
# create read test data unit test

set(READ_INPUT_DATA_UNIT_TEST_EXE read_test_data_unit_test                          )
set(READ_INPUT_DATA_UNIT_TEST_SRC ${UNIT_TEST_DIR}/read_input_data/unit_test_read_input_data.cpp)

target_unit_test(
    "${READ_INPUT_DATA_UNIT_TEST_EXE}"
    "${READ_INPUT_DATA_UNIT_TEST_SRC}"
    "${ALL_UNIT_TESTS_INCLUDE_DIRS}"
    "${READ_INPUT_LIB}"
)

set(READ_INPUT_UNIT_TESTS
    ${FLAG_PARSER_UNIT_TEST_EXE}
)

# =============================================================================================================

set(ALL_UNIT_TESTS_TARGET
    ${READ_INPUT_UNIT_TESTS}
    ${MATH_UNIT_TESTS}
    ${GEOMETRY_UNIT_TESTS}
)

# create target to compile all unit tests
add_custom_target(${BUILD_ALL_UNIT_TESTS_TARGET}
    DEPENDS
        ${TRIANGLES_EXE}
        ${DOUBLE_TRIANGLES_EXE}
        ${ALL_UNIT_TESTS_TARGET}
)

# =============================================================================================================

# set the directory with e2e tests for run_triangles
set(TRIANGLES_TEST_DIR ${PROJECT_SOURCE_DIR}/../tests/e2e)

# set the directories with e2e test data for run_triangles
set(TRIANGLES_TEST_DAT_DIR ${TRIANGLES_TEST_DIR}/dat)

# set the directories with e2e test answers for run_triangles
set(TRIANGLES_TEST_ANS_DIR ${TRIANGLES_TEST_DIR}/ans)

include(${CMAKE_TESTS_DIR}/e2e-tests.cmake)

# add e2e tests to float triangles
target_e2e_test(${PROJECT_BINARY_DIR}/${TRIANGLES_EXE}
                ${PYTHON_RUN_TEST_SCRIPT}
                ${TRIANGLES_TEST_DAT_DIR}
                ${TRIANGLES_TEST_ANS_DIR}
)

# add e2e tests to long double triangles
target_e2e_test(${PROJECT_BINARY_DIR}/${DOUBLE_TRIANGLES_EXE}
                ${PYTHON_RUN_TEST_SCRIPT}
                ${TRIANGLES_TEST_DAT_DIR}
                ${TRIANGLES_TEST_ANS_DIR}
)

# =============================================================================================================

add_gtests_to_ctest("${ALL_UNIT_TESTS_TARGET}")

# =============================================================================================================
