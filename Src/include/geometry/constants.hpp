#pragma once

//----------------------------------------------------------------------------------------------------------------------------

#include <cstddef>

//----------------------------------------------------------------------------------------------------------------------------

namespace Geometry
{
    namespace Detail
    {
        constexpr size_t dimension_of_space = 3;
        static_assert(dimension_of_space == 3, "in this projects we`re working in R^3");

        constexpr size_t triangle_vertices_quant = 3;
        static_assert(triangle_vertices_quant == 3, ":)");

        constexpr size_t triangle_sides_quant = 3;
        static_assert(triangle_vertices_quant == 3, ":)");
    } /* namespace Detail */
} /* namespace Geomaetry */

//----------------------------------------------------------------------------------------------------------------------------
