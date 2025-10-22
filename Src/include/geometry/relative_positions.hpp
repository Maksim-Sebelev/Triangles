#pragma once

//----------------------------------------------------------------------------------------------------------------------------

#include "global.hpp"


#if defined(USE_LOGGER)
#include <iostream>
#endif /* defined(USE_LOGGER) */

//----------------------------------------------------------------------------------------------------------------------------

namespace Geometry
{

//----------------------------------------------------------------------------------------------------------------------------

namespace Detail
{
    /*
        relative positions of:
            -point
            -segment
            -line
            -plain
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
                -point on plaine
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
        */
    };

    ON_LOGGER(
    inline std::ostream& operator<<(std::ostream& os, const relative_position_t& rp)
    {
        switch (rp)
        {
            case relative_position_t::NO_INTERSECTION    : return os << "NO_INTERSECTION"    ;
            case relative_position_t::CONTAIN_OR_EQUAL   : return os << "CONTAIN_OR_EQUAL"   ;
            case relative_position_t::NORMAL_INTERSECTION: return os << "NORMAL_INTERSECTION";
            default: builtin_unreachable_wrapper("undef relative_position_t value. maybe you forgot to add parse new value in this switch");
        }
        return os << "UNDEFINED_RELATIVE_POSITION_TYPE";
    }
    )

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace Detail */
} /* namespace Geometry */

//----------------------------------------------------------------------------------------------------------------------------
