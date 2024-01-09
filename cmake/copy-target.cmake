
function(copy_target_files NAME DIR)
    add_custom_command(TARGET build-${NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${NAME}> ${DIR}
    )
endfunction()

function(copy_target_to T TF)
    add_custom_command(TARGET ${T} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${TF}> ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()
