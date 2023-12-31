
function(copy_target_files TARGET_NAME TARGET DIR)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DIR}
            COMMAND echo Copying ${TARGET}...
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${TARGET}> ${DIR}
    )
endfunction()

function(copy_target_to T TF)
    add_custom_command(TARGET ${T} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${TF}> ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()
