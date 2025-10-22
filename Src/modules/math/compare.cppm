module;

//---------------------------------------------------------------------------------------------------------------

#include <cmath>
#include <type_traits>

#include "global.hpp"

#if defined(USE_LOGGER)
import logger;
#endif /* defined(USE_LOGGER) */

//---------------------------------------------------------------------------------------------------------------

export module compare;

//---------------------------------------------------------------------------------------------------------------

export namespace Math
{

//---------------------------------------------------------------------------------------------------------------

namespace Compare
{

//---------------------------------------------------------------------------------------------------------------

template <typename number_t>
bool
compare(number_t a, number_t b, 
        typename std::enable_if<std::is_floating_point<number_t>::value, number_t>::type tolerance = number_t(1e-4))
{
    msg_assert(tolerance >= 0, "what do you want in compare with tolerace < 0??");

    if constexpr (!std::is_floating_point_v<number_t>)
        return a == b;

    if (std::isnan(a) || std::isnan(b)) return false;
    if (std::isinf(a) || std::isinf(b)) return a == b;

    return std::abs(a - b) <= tolerance;
}

//---------------------------------------------------------------------------------------------------------------

template <typename number_t>
bool
compare_with_null(number_t a,
                 typename std::enable_if<std::is_floating_point<number_t>::value, number_t>::type tolerance = number_t(1e-6))
{
    msg_assert(tolerance >= 0, "what do you want in compare_with_null with tolerace < 0??");
    return compare<number_t>(a, number_t(0), tolerance);
}

//---------------------------------------------------------------------------------------------------------------

} /* namespace Compare */
} /* namespace Math */

//---------------------------------------------------------------------------------------------------------------
