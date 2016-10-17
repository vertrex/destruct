add_custom_target(pyinheritance)
add_dependencies(examples pyinheritance)
add_dependencies(_destruct pyinheritance)
configure_file(pyinherit.py ../bin/pyinherit.py COPYONLY)

set (pydestruct_examples_inheritance_files
pyinherit.py
)
