# check dependencies
# find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)

# =============================================================================================================
# add imgui lib
set(IMGUI_LIB imgui)
set(IMGUI_SRC_DIR          ${THIRD_PARTY_DIR}//imgui)
set(IMGUI_INCLUDE_DIR      ${IMGUI_SRC_DIR}) # all good, dont worry
set(IMGUI_BACK_SRC_DIR     ${IMGUI_SRC_DIR}/backends)
set(IMGUI_BACK_INCLUDE_DIR ${IMGUI_INCLUDE_DIR}/backends)

add_library(${IMGUI_LIB} STATIC)

target_sources(${IMGUI_LIB}
    PRIVATE
        ${IMGUI_SRC_DIR}/imgui.cpp
        ${IMGUI_SRC_DIR}/imgui_demo.cpp
        ${IMGUI_SRC_DIR}/imgui_draw.cpp
        ${IMGUI_SRC_DIR}/imgui_tables.cpp
        ${IMGUI_SRC_DIR}/imgui_widgets.cpp
        ${IMGUI_BACK_SRC_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_BACK_SRC_DIR}/imgui_impl_opengl3.cpp
)

target_include_directories(${IMGUI_LIB}
    PUBLIC
        ${IMGUI_INCLUDE_DIR}
        ${IMGUI_BACK_INCLUDE_DIR}
)

# =============================================================================================================
# add glad lib
set(GLAD_LIB glad)
set(GLAD_SRC_DIR          ${THIRD_PARTY_DIR}/glad)
set(GLAD_INCLUDE_DIR      ${GLAD_SRC_DIR}) # all good, dont worry

add_library(${GLAD_LIB} STATIC)

target_sources(${GLAD_LIB}
    PRIVATE
        ${GLAD_SRC_DIR}/glad.c
)

target_include_directories(${GLAD_LIB}
    PUBLIC
        ${GLAD_INCLUDE_DIR}
)

# =============================================================================================================

set(OCTREE_VIZ_LIB octree_viz)
add_library(${OCTREE_VIZ_LIB})

set(OCTREE_VIZ_SRC_DIR ${OCTREE_SRC_DIR}/dump_3d)
set(OCTREE_VIZ_SRC
    ${OCTREE_VIZ_SRC_DIR}/octree_vizualization.cppm
)

target_sources(${OCTREE_VIZ_LIB}
    PUBLIC
        FILE_SET
            CXX_MODULES
        TYPE
            CXX_MODULES
        FILES
            ${OCTREE_VIZ_SRC}
)

target_include_directories(${OCTREE_VIZ_LIB}
    PRIVATE
        ${GLOBAL_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
)

target_link_libraries(${OCTREE_VIZ_LIB}
    PRIVATE
        ${OCTREE_LIB}
        ${GLAD_LIB}
        ${IMGUI_LIB}
        glfw
        # OpenGL::GL
)

target_debug_options(${OCTREE_VIZ_LIB})

# defined macro DUMP_3D 
set(3D_DUMP_MACRO DUMP_3D)

foreach(target ${TARGETS_USING_2D_DUMP})
    target_compile_definitions(${target}
        PRIVATE
            ${3D_DUMP}
    )

    target_link_libraries(${target}
        PRIVATE
            ${OCTREE_VIZ_LIB}
    )
endforeach()