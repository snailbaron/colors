add_executable(colors
    main.cpp
)
target_link_libraries(colors PRIVATE SDL2::SDL2 SDL2::SDL2main)

if(WIN32)
    add_custom_command(TARGET colors POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:colors> $<TARGET_FILE_DIR:colors>
        COMMAND_EXPAND_LISTS
    )
endif()