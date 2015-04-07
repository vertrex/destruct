add_custom_target(serialization)
add_dependencies(examples serialization)
add_dependencies(_destruct serialization)
configure_file(pyserial.py pyserial.py COPYONLY)

set (pydestruct_examples_iterator_files
pyserial.py
)
