
# set the directory with e2e tests for run_triangles
set(TRIANGLES_TEST_DIR ${PROJECT_SOURCE_DIR}/../tests/e2e)

# set the directories with e2e test data for run_triangles
set(TRIANGLES_TEST_DAT_DIR ${TRIANGLES_TEST_DIR}/dat)

# set the directories with e2e test answers for run_triangles
set(TRIANGLES_TEST_ANS_DIR ${TRIANGLES_TEST_DIR}/ans)

include(${CMAKE_TESTS_DIR}/e2e/e2e-tests.cmake)

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
