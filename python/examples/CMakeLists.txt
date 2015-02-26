add_custom_target(examples)
add_dependencies(_destruct examples)

add_subdirectory(iterator)
add_subdirectory(inheritance)

if (UNIX)
add_subdirectory(test)
add_subdirectory(embedding)
#add_subdirectory(swig)
add_subdirectory(registry)
add_subdirectory(workerpool)
add_subdirectory(loader)
endif (UNIX)
