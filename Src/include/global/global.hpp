#pragma once

//---------------------------------------------------------------------------------------------------------------

#if defined(NDEBUG) and (defined(_DEBUG) or defined(DEBUG))
static_assert(false, "bad release-debug macro combo");
#endif /* defined(NDEBUG) and (defined(_DEBUG) or defined(DEBUG)) */

//---------------------------------------------------------------------------------------------------------------

#if not defined(NDEBUG)

#include <iostream>                  // for msg_assert
#include <cstdlib>                   // for EXIT_SUCCESS macro

#include "custom_console_output.hpp"

#define ON_DEBUG(...) __VA_ARGS__
#define OFF_DEBUG(...)

#define builtin_unreachable_wrapper(debug_message) msg_assert(false, "__builtin_unreachable() reaached\n" << debug_message)

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

#else /* not defined(NDEBUG) */

#define ON_DEBUG(...)
#define OFF_DEBUG(...) __VA_ARGS__

#define builtin_unreachable_wrapper(debug_message) __builtin_unreachable()

#define msg_assert(bool_expression, message)

#endif /* not defined(NDEBUG) */

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
