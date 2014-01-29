# Author(s):
#  Solal Jacob <sja@digital-forensic.org>

option(DOC_GENERATE "Generate documentation" ON)

if (DOC_GENERATE)
  add_custom_target(doc)
  add_dependencies(destruct doc)
  set(DOC_INSTALL_DIR ${CMAKE_INSTALL_ORIG_PREFIX}/share/doc/${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
#
### DOXYGEN / DOC DEVELOPER CONFIGURATION
#
  option(DOC_GENERATE_DEVELOPER "Build developer guide" ON)

  set(DOC_DEVELOPER_PROJECT_NAME "destruct")
  set(DOC_DEVELOPER_PROJECT_BRIEF "destruct : data structure explosion library")
  set(DOC_DEVELOPER_LOGO_FILE_PATH "none.png")
  set(DOC_DEVELOPER_VERSION "0.1")
  set(DOC_DEVELOPER_OUTPUT_TYPES HTML ) #LATEX
  #set(DOC_DEVELOPER_LATEX_OUTPUT_FILE "refman.tex")
  set(DOC_DEVELOPER_HTML_OUTPUT_FILE "index.html")
  set(DOC_DEVELOPER_SOURCES
  #"${CMAKE_SOURCE_DIR}/*.cpp"
  "${CMAKE_SOURCE_DIR}/"
  "${CMAKE_SOURCE_DIR}/protocol"
  "${CMAKE_SOURCE_DIR}/doc/developer/destruct.coverpage")
  message("${DOC_DEVELOPER_SOURCES}")

  add_subdirectory(developer)
endif(DOC_GENERATE)
