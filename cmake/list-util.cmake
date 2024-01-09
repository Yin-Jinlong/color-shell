
function(add_list_suffix LIST SUFFIX OUT)
    foreach (I ${${LIST}})
        list(APPEND R ${SUFFIX}${I})
    endforeach ()
    set(${OUT} ${R} PARENT_SCOPE)
endfunction()
