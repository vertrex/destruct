# Author(s):
#  Solal Jacob <sja@digital-forensic.org>

option(DOC_GENERATE "Generate documentation" OFF)

if (DOC_GENERATE)
  add_custom_target(destruct_doc)
  add_dependencies(destruct destruct_doc)
  set(DOC_DESTRUCT_INSTALL_DIR ${CMAKE_INSTALL_ORIG_PREFIX}/share/doc/${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
#
### DOXYGEN / DOC DEVELOPER CONFIGURATION
#
  option(DOC_DESTRUCT_GENERATE_DEVELOPER "Build developer guide" ON)

  #set(DOC_DEVELOPER_PROJECT_NAME "${PROJECT_NAME}")
  #set(DOC_DEVELOPER_PROJECT_BRIEF "${PROJECT_FULL_NAME}")
  set(DOC_DESTRUCT_DEVELOPER_PROJECT_NAME "destruct")
  set(DOC_DESTRUCT_DEVELOPER_PROJECT_BRIEF "destruct : data structure explosion library")
  set(DOC_DESTRUCT_DEVELOPER_VERSION "0.1")
  set(DOC_DESTRUCT_DEVELOPER_OUTPUT_TYPES HTML) #LATEX
  #set(DOC_DEVELOPER_LATEX_OUTPUT_FILE "refman.tex")
  set(DOC_DESTRUCT_DEVELOPER_HTML_OUTPUT_FILE "index.html")
  set(DOC_DESTRUCT_DEVELOPER_SOURCES
  #"${CMAKE_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_BINARY_DIR}/../"
  "${CMAKE_CURRENT_BINARY_DIR}/../protocol"
  "${CMAKE_CURRENT_BINARY_DIR}/developer/destruct.coverpage")
  add_subdirectory(developer)
  option(DOC_DESTRUCT_GENERATE_GUIDE_HTML "Build HTML application guide" ON)
endif(DOC_GENERATE)
