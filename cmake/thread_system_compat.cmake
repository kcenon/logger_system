# thread_system currently enables std::jthread with a directory definition.
# Its public thread_base/lifecycle_controller layout depends on this flag, so
# consumers of a source-built library must inherit the same ABI selection.
function(logger_propagate_thread_system_abi target)
    get_target_property(aliased_target ${target} ALIASED_TARGET)
    if(aliased_target)
        set(target ${aliased_target})
    endif()

    get_target_property(imported ${target} IMPORTED)
    if(imported)
        return()
    endif()

    get_target_property(source_dir ${target} SOURCE_DIR)
    get_directory_property(definitions DIRECTORY "${source_dir}" COMPILE_DEFINITIONS)
    if("USE_STD_JTHREAD" IN_LIST definitions)
        target_compile_definitions(${target} INTERFACE USE_STD_JTHREAD)
    endif()
endfunction()
