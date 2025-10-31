# expect TARGETS_USING_LOGGER variable before include

message("-- Adding logger")

# TODO: add logger as external project (ExternalProject_Add)
# костыль, но так надо чтобы не ждать логгера по полгода и без запары с exterbal project (я 2 раза пытался его настроить, оба раза проиграл)

set(MAIN_PROJECT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)

set(LOGGER_PROJECT_DIR ${THIRD_PARTY_DIR}/logger/Src)
add_subdirectory(${LOGGER_PROJECT_DIR})

function(add_logger some_target)
    target_link_libraries(${some_target} 
        PRIVATE
            logger
    )
    target_compile_definitions(${some_target}
        PRIVATE
            USE_LOGGER
    )
endfunction(add_logger)

foreach(target_that_use_logger ${TARGETS_USING_LOGGER})
    add_logger(${target_that_use_logger})
endforeach()

set(CMAKE_BUILD_TYPE ${MAIN_PROJECT_BUILD_TYPE})
