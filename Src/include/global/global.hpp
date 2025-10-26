#pragma once

//---------------------------------------------------------------------------------------------------------------

#if defined(_DEBUG)

#include <iostream>                  // for msg_assert
#include "custom_console_output.hpp" // for msg_ assert
#include <cstdlib>                   // for EXIT_SUCCESS macro

#define ON_DEBUG(...) __VA_ARGS__
#define OFF_DEBUG(...)

#define builtin_unreachable_wrapper(debug_message) msg_assert(false, debug_message)

#define msg_assert(bool_expression, message) do {       \
if (!(bool_expression)) {                                \
    std::cerr << RED BOLD "assertation failed:\n"         \
                 RESET_CONSOLE_OUT WHITE                   \
              << #bool_expression << "\n"                   \
              << __FILE__ <<   ":" << __LINE__  <<           \
              " [" << __func__ << "]\n" RESET_CONSOLE_OUT RED \
              "message:\n" BOLD                                \
              << message << RESET_CONSOLE_OUT << std::endl;     \
    exit(EXIT_FAILURE);                                          \
}} while (0)

#else /* defined(_DEBUG) */

#define ON_DEBUG(...)
#define OFF_DEBUG(...) __VA_ARGS__

#define builtin_unreachable_wrapper(debug_message) __builtin_unreachable()

#define msg_assert(bool_expression, message)

#endif /* defined(_DEBUG) */

//---------------------------------------------------------------------------------------------------------------

#if defined(USE_LOGGER)

#define ON_LOGGER(...) __VA_ARGS__
#include <sstream>
#include <string> 

#else /* defined(USE_LOGGER) */

#define ON_LOGGER(...)

#endif /* defined(USE_LOGGER) */

//---------------------------------------------------------------------------------------------------------------

#if defined(DUMP_2D)
#define ON_2D_DUMP(...) __VA_ARGS__
#include <iostream>
#include <string>
#include <sstream>
#else /* defined(DUMP_2D) */
#define ON_2D_DUMP(...)
#endif /* defined(DUMP_2D) */

//---------------------------------------------------------------------------------------------------------------

#if defined(DUMP_3D)
#define ON_3D_DUMP(...) __VA_ARGS__
#else
#define ON_3D_DUMP(...)
#endif /* defined(DUMP_3D) */

//---------------------------------------------------------------------------------------------------------------
