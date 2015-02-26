set (pydestruct_embedding_files
embedding.cpp
#pyembedding.py
)

configure_file(pyembedding.py  pyembedding.py COPYONLY)

add_executable(embedding ${pydestruct_embedding_files})

include_directories("../../../")
include_directories("../../")
include_directories("../")
if (MSVC)
  target_link_libraries(embedding destruct ${PYTHON_LIBRARY})
else (MSVC)
  target_link_libraries(embedding destruct python2.7)
endif(MSVC)
