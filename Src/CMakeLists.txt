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

if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
include(${CMAKE_TESTS_DIR}/unit/unit-tests.cmake)
include(${CMAKE_TESTS_DIR}/unit/create-all-unit-tests.cmake)

# =============================================================================================================

# set name of target to build all unit-tests
set(BUILD_ALL_UNIT_TESTS_TARGET "build-tests")

# =============================================================================================================

# create target to compile all unit tests
add_custom_target(${BUILD_ALL_UNIT_TESTS_TARGET}
    DEPENDS
        ${TRIANGLES_EXE}
        ${DOUBLE_TRIANGLES_EXE}
        ${ALL_UNIT_TESTS_TARGET}
)

# =============================================================================================================

# create all e2e tests
include(${CMAKE_TESTS_DIR}/e2e/create-all-e2e-tests.cmake)

# =============================================================================================================
