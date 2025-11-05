# expect TARGETS_USING_LOGGER variable before include

message("-- Adding logger")

set(LOGGER_PROJECT_DIR ${THIRD_PARTY_DIR}/logger/Src)
add_subdirectory(${LOGGER_PROJECT_DIR}/modules-based)

function(add_logger some_target)
    target_link_libraries(${some_target} 
        PRIVATE
            pineaplog_mb
    )
    target_compile_definitions(${some_target}
        PRIVATE
            USE_LOGGER
    )
endfunction(add_logger)

foreach(target_that_use_logger ${TARGETS_USING_LOGGER})
    add_logger(${target_that_use_logger})
endforeach()
