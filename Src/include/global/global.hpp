#pragma once

//---------------------------------------------------------------------------------------------------------------

#if defined(_DEBUG)
#define ON_DEBUG(...) __VA_ARGS__
#define OFF_DEBUG(...)
    #include <iostream> // for msg_assert
    #include <cassert>  // for msg_assert
    #include "custom_console_output.hpp" // for msg_ assert
#else // _DEBUG
    #define ON_DEBUG(...)
    #define OFF_DEBUG(...) __VA_ARGS__
#endif // _DEBUG

//---------------------------------------------------------------------------------------------------------------

#define msg_assert(bool_expression, message) ON_DEBUG( do { \
if (!(bool_expression)) {                                    \
    std::cerr << RED BOLD "assertation failed:\n"             \
              << (message) << RESET_CONSOLE_OUT << std::endl;  \
    assert(false);                                              \
}} while (0))

//---------------------------------------------------------------------------------------------------------------

#define builtin_unreachable_wrapper(debug_message)  \
msg_assert(false, debug_message)                     \
OFF_DEBUG(__builtin_unreachable())

//---------------------------------------------------------------------------------------------------------------

#if defined(USE_LOGGER)
#define ON_LOGGER(...) __VA_ARGS__
import logger;
#include <source_location>
#else // USE_LOGGER
#define ON_LOGGER(...)
#endif // USE_LOGGER

//---------------------------------------------------------------------------------------------------------------
