set (pydestruct_examples_files
embedding.cpp
pyembedding.py
)

add_executable(embedding ${pydestruct_examples_files})

include_directories("../../")
target_link_libraries(embedding destruct python2.7)
