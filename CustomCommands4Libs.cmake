# #############################################################################
# Custom copy of shared libraries:
# Possible copy hooks are: PRE_BUILD | PRE_LINK | POST_BUILD
# #############################################################################
function(CopyLibToBin)
    add_custom_command(
            TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}lib${PROJECT_NAME}.so
                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endfunction()
