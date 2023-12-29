
function(mktart_dir TARGET_NAME DIR)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DIR}
    )
endfunction()

function(copy_target_files TARGET_NAME DIR TARGETS)
    mktart_dir(${TARGET_NAME} ${DIR})
    list(LENGTH ${TARGETS} COUNT)
    set(i 1)

    foreach (T ${${TARGETS}})
        set(ni ${i})
        math(EXPR i "${i}+1")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND echo [${ni}/${COUNT}] Copying ${T}...
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${T}> ${DIR}
        )
    endforeach ()
endfunction()