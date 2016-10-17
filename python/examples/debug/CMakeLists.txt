add_custom_target(debug)
add_dependencies(examples debug)
add_dependencies(_destruct debug)
configure_file(debug.py ../bin/debug.py COPYONLY)

set(pydestruct_examples_debug_files
debug.py
)
