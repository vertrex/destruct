add_custom_target(iterator)
add_dependencies(examples iterator)
add_dependencies(_destruct iterator)
configure_file(debug.py debug.py COPYONLY)

set (pydestruct_examples_debug_files
debug.py
)
