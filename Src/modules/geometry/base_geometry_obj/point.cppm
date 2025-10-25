module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <iostream>
#include <limits>

#include "global.hpp"
#include "custom_console_output.hpp"

import constants;

import compare;

#if defined(USE_LOGGER)
import logger;
#endif /*define(USE_LOGGER)*/

#if defined(TREE_GRAPHIC_DUMP)
#define ON_GRAPHIC_DUMP(...) __VA_ARGS__
#else /* defined(TREE_GRAPHIC_DUMP) */
#define ON_GRAPHIC_DUMP(...)
#endif /* defined(TREE_GRAPHIC_DUMP) */

//----------------------------------------------------------------------------------------------------------------------------

export module point;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

namespace Detail
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
class point_t
{
    static_assert(std::is_floating_point_v<coordinate_t>,
                "In point_t as template type excpect only floating point type.");

    private:
        coordinate_t x_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();
        coordinate_t y_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();
        coordinate_t z_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();

        void try_to_compare_invalid_point     () const;
        void try_to_compare_with_invalid_point() const;

    public:
        point_t() = default;
        point_t(coordinate_t x, coordinate_t y, coordinate_t z);
        point_t(const std::array<coordinate_t, dimension_of_space>& array_of_coordinates);

        coordinate_t get_x          ()                                     const;
        coordinate_t get_y          ()                                     const;
        coordinate_t get_z          ()                                     const;

        bool         is_valid                  ()                                     const;
    
        bool         compare_with_another_point(const point_t& compare_point)         const;
        bool         is_collenear(const point_t& p1, const point_t& p2) const;
        bool         is_between_2_points       (const point_t& p1, const point_t& p2) const;
        
        void         set_x                     (const coordinate_t& value);
        void         set_y                     (const coordinate_t& value);
        void         set_z                     (const coordinate_t& value);
        
        void         made_invalid              ();

        ON_LOGGER(
        std::string get_dump                   (std::string_view name)                const;
        )
        ON_GRAPHIC_DUMP(
        void        get_graphic_dump           ()                                     const;
        )
};

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>::point_t(coordinate_t x, coordinate_t y, coordinate_t z) :
x_coordinate_(x),
y_coordinate_(y),
z_coordinate_(z)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
point_t<coordinate_t>::point_t(const std::array<coordinate_t, dimension_of_space>& array_of_coordinates) :
x_coordinate_(array_of_coordinates[0]),
y_coordinate_(array_of_coordinates[1]),
z_coordinate_(array_of_coordinates[2])
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
point_t<coordinate_t>::get_x() const
{
    return x_coordinate_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
point_t<coordinate_t>::get_y() const
{
    return y_coordinate_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
coordinate_t
point_t<coordinate_t>::get_z() const
{
    return z_coordinate_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
point_t<coordinate_t>::compare_with_another_point(const point_t& compare_point) const
{
    if (!is_valid())
    {
        try_to_compare_invalid_point();
        return false;
    }

    if (!compare_point.is_valid())
    {
        try_to_compare_with_invalid_point();
        return false;
    }

    return
        Math::Compare::compare<coordinate_t>(x_coordinate_, compare_point.x_coordinate_) &&
        Math::Compare::compare<coordinate_t>(y_coordinate_, compare_point.y_coordinate_) &&
        Math::Compare::compare<coordinate_t>(z_coordinate_, compare_point.z_coordinate_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
point_t<coordinate_t>::is_valid() const
{
    return (!std::isnan(x_coordinate_)) &&
           (!std::isnan(y_coordinate_)) &&
           (!std::isnan(z_coordinate_));
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
point_t<coordinate_t>::is_collenear(const point_t& p1, const point_t& p2) const
{
    coordinate_t x_21_diff = p1.get_x() - get_x();
    coordinate_t x_31_diff = p2.get_x() - get_x();

    coordinate_t y_21_diff = p1.get_y() - get_y();
    coordinate_t y_31_diff = p2.get_y() - get_y();

    if (!Math::Compare::compare<coordinate_t>(x_21_diff * y_31_diff, 
                               y_21_diff * x_31_diff
    )) return false;

    coordinate_t z_21_diff = p1.get_z() - get_z();
    coordinate_t z_31_diff = p2.get_z() - get_z();

    if (!Math::Compare::compare<coordinate_t>(x_21_diff * z_31_diff, 
                               z_21_diff * x_31_diff
    )) return false;

    return Math::Compare::compare<coordinate_t>(y_21_diff * z_31_diff, 
                                 z_21_diff * y_31_diff);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
bool
point_t<coordinate_t>::is_between_2_points(const point_t& p1, const point_t& p2) const
{
    return
        ((x_coordinate_ - p1.x_coordinate_) * (x_coordinate_ - p2.x_coordinate_) <= (coordinate_t) (0 + 1e-4)) &&
        ((y_coordinate_ - p1.y_coordinate_) * (y_coordinate_ - p2.y_coordinate_) <= (coordinate_t) (0 + 1e-4)) &&
        ((z_coordinate_ - p1.z_coordinate_) * (z_coordinate_ - p2.z_coordinate_) <= (coordinate_t) (0 + 1e-4));
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::made_invalid()
{
    x_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();
    y_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();
    z_coordinate_ = std::numeric_limits<coordinate_t>::quiet_NaN();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::set_x(const coordinate_t& value)
{
    x_coordinate_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::set_y(const coordinate_t& value)
{
    y_coordinate_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::set_z(const coordinate_t& value)
{
    z_coordinate_ = value;
}

//----------------------------------------------------------------------------------------------------------------------------

//private
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::try_to_compare_invalid_point() const
{
    std::cerr << YELLOW "Warning: Try to compare invalid point." << std::endl
              << "Result of this compare is " << RED BOLD << "false" RESET_CONSOLE_OUT << std::endl; 
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
point_t<coordinate_t>::try_to_compare_with_invalid_point() const
{
    std::cerr << YELLOW "Warning: Try to compare with invalid point." << std::endl
              << "Result of this compare is " << RED BOLD << "false" RESET_CONSOLE_OUT << std::endl; 
}

// dump
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_LOGGER(
template <typename coordinate_t>
std::string
point_t<coordinate_t>::get_dump(std::string_view name) const
{
    std::ostringstream point_dump;

    point_dump << "POINT "
               << name << "("  << x_coordinate_
                       << ", " << y_coordinate_
                       << ", " << z_coordinate_
                       << ")";

    return point_dump.str();
}
)

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */
} /* namespace Detail */

// TREE_GRAPHIC_DUMP
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

ON_GRAPHIC_DUMP(


)