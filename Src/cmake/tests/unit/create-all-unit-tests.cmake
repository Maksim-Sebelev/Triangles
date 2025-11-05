# =============================================================================================================

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

# =============================================================================================================

add_gtests_to_ctest("${ALL_UNIT_TESTS_TARGET}")

# =============================================================================================================
