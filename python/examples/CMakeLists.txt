add_custom_target(examples)
add_dependencies(_destruct examples)

add_subdirectory(loader)
add_subdirectory(test) #must use loader, and be updated 
add_subdirectory(iterator)#protocol tester
add_subdirectory(inheritance)
add_subdirectory(workerpool)
add_subdirectory(serialization)
add_subdirectory(debug)

if (UNIX)
add_subdirectory(embedding)
#add_subdirectory(swig)
endif (UNIX)
add_subdirectory(registry)
add_subdirectory(device)
add_subdirectory(performance)
add_subdirectory(memdevice)
