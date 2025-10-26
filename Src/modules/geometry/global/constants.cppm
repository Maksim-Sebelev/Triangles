module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cstddef>

//----------------------------------------------------------------------------------------------------------------------------

export module constants;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{
    constexpr size_t dimension_of_space = 3;
    static_assert(dimension_of_space == 3, "in this projects we`re working in R^3");

    constexpr size_t triangle_vertices_quant = 3;
    static_assert(triangle_vertices_quant == 3, ":)");

    constexpr size_t triangle_sides_quant = 3;
    static_assert(triangle_vertices_quant == 3, ":)");
} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
