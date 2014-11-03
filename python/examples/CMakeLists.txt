add_custom_target(examples)
add_subdirectory(iterator)
add_dependencies(_destruct examples)

add_subdirectory(test)
add_subdirectory(embedding)
#add_subdirectory(swig)

add_subdirectory(registry)

