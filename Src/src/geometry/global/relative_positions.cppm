module;

//----------------------------------------------------------------------------------------------------------------------------

#include "global.hpp" // for reletive_position_t output operator

#if defined(USE_LOGGER) or not defined(NDEBUG)
#include <iostream> // for reletive_position_t output operator
#include <fstream>
#endif /* defined(USE_LOGGER) */

//----------------------------------------------------------------------------------------------------------------------------

export module relative_positions;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Geometry
{
    /*
        relative positions of:
            -point
            -segment
            -line
            -plain
            -triangles
    */
    enum class relative_position_t
    {
        NO_INTERSECTION,
        /*
            dont exist al least 1 point, that contain both object
        */
    
        CONTAIN_OR_EQUAL,
        /*
            what does CONTAIN_OR_EQUAL mean?:
                -point is equal to another point
                -point on segment
                -point on line
                -point on plain
                -point on triangle
                -segment contains point
                -segment is parallel to segment and have intersection with it
                -segment on line
                -segment on plain
                -line contains point
                -line contains segment
                -line is equal to anoter line
                -line on plain
                -plain contains point
                -plain contains segment
                -plain constains line
                -plain is equal to another plain
                -plain contains triangle
                -triangle contains point
                -triangle contains segment
                -triangle contain another triangle or is equal to him (equal <=> all vertices are equal)
        */
    
        NORMAL_INTERSECTION,
        /*
            what does NORMAL_INTERSECTION mean?:
                -segment has 1 general point with another segment and is not parallel to him
                -segment has 1 general point with line
                -segment has 1 general point with plain
                -line has 1 general point with segment
                -line has 1 general point with another line
                -line has general point with plain
                -plain has 1 general point with segment
                -plain has 1 general point with line
                -plain intersect another plain wit a line
                -triangle have intersection with line
                -triangle have intersection with segment,  but exists a lot 1 point of segment, that is not in triangle
                -triange have intersection with plain, but not all triangle is on plain
        */
    };

//----------------------------------------------------------------------------------------------------------------------------

    #if defined(USE_LOGGER) or not defined(NDEBUG)
    // no inline, because its .cppm
    // :)
    std::ostream& operator<<(std::ostream& os, const relative_position_t& rp)
    {
        switch (rp)
        {
            case relative_position_t::NO_INTERSECTION    : return os << "NO_INTERSECTION"    ;
            case relative_position_t::CONTAIN_OR_EQUAL   : return os << "CONTAIN_OR_EQUAL"   ;
            case relative_position_t::NORMAL_INTERSECTION: return os << "NORMAL_INTERSECTION";
            default: builtin_unreachable_wrapper("undef relative_position_t value. "
                                                    "maybe you forgot to add parse new value in this switch");
        }
        builtin_unreachable_wrapper("we must return in switch");
    }

    #endif /* defined(USE_LOGGER) or not defined(NDEBUG) */

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
