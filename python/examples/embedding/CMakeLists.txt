set (pydestruct_embedding_files
embedding.cpp
#pyembedding.py
)

configure_file(pyembdedding.py  pyembedding.py COPYONLY)

add_executable(embedding ${pydestruct_embedding_files})

include_directories("../../../")
include_directories("../../")
include_directories("../")
target_link_libraries(embedding destruct python2.7)
