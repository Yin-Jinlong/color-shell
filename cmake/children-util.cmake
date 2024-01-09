
function(add_children CHILDREN)
    foreach (V  ${ARGV})
        add_subdirectory(${V})
    endforeach ()
endfunction()
