add_custom_target(iterator)
add_dependencies(examples iterator)
add_dependencies(_destruct iterator)
configure_file(pyiterator.py pyiterator.py COPYONLY)

set (pydestruct_examples_iterator_files
pyiterator.py
)
