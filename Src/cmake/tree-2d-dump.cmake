# check dependencies
find_program(DOT_COMMAND
    NAMES dot dot.exe
    DOC "Graphviz dot executable"
)

if (NOT DOT_COMMAND)
    message(FATAL_ERROR "Graphviz NOT find. You can use program anyway, just do not use option -DTREE_2D_DAMP")
endif()

set(2D_DUMP_MACRO DUMP_2D)

set(DOT_OUT_DIR ${PROJECT_BINARY_DIR}/dot-out)
set(IMG_OUT_DIR ${PROJECT_BINARY_DIR}/../2d-dump)


# give paths for dot-out and images dir
target_compile_definitions(${OCTREE_LIB}
    PRIVATE
        ${2D_DUMP_MACRO}
        DOT_OUT_DIR="${DOT_OUT_DIR}/"
        IMG_OUT_DIR="${IMG_OUT_DIR}/"
)

foreach(target ${TARGETS_USING_3D_DUMP})
    target_compile_definitions(${target} # this dump made in Src/modules/test/execute_test/execute_test.cppm
        PUBLIC
            ${2D_DUMP_MACRO}
    )
endforeach()


set(DOT_CLEAN_TARGET dot-clean)

add_custom_target(${DOT_CLEAN_TARGET}
    COMMAND ${CMAKE_COMMAND} -E clean_directory "${DOT_OUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E clean_directory "${IMG_OUT_DIR}"
    COMMENT "remove graphviz out dirs"
)
