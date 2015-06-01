# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Solal Jacob <sja@digital-forensic.org>

if(DOC_DESTRUCT_GENERATE_DEVELOPER)
  add_custom_target(destruct_doc.developer)
  add_dependencies(destruct_doc destruct_doc.developer)

  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    string (REPLACE ";" " " DOC_DESTRUCT_DEVELOPER_SOURCE_FILES "${DOC_DESTRUCT_DEVELOPER_SOURCES}")
    set(DOC_DESTRUCT_DEVELOPER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

    foreach (type ${DOC_DESTRUCT_DEVELOPER_OUTPUT_TYPES})
      set(DOC_DESTRUCT_DEVELOPER_${type}_OUTPUT "YES")
      string(TOLOWER "${type}" type_low)
      set(DOC_DESTRUCT_DEVELOPER_${type}_OUTPUT_FILE_ABS "${CMAKE_CURRENT_BINARY_DIR}/${type_low}/${DOC_DESTRUCT_DEVELOPER_${type}_OUTPUT_FILE}")
      add_custom_target (destruct_doc.developer.${type_low} DEPENDS ${DOC_DESTRUCT_DEVELOPER_${type}_OUTPUT_FILE_ABS})
      add_dependencies (destruct_doc.developer destruct_doc.developer.${type_low})
      list (APPEND DOC_DESTRUCT_DEVELOPER_OUTPUT_FILES_ABS "${DOC_DESTRUCT_DEVELOPER_${type}_OUTPUT_FILE_ABS}")
    endforeach (type)
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    @ONLY)

    add_custom_command (OUTPUT ${DOC_DESTRUCT_DEVELOPER_OUTPUT_FILES_ABS}
                        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
                        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" ${DOC_DESTRUCT_DEVELOPER_SOURCES}
                        VERBATIM)
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "html;")

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html"
            DESTINATION ${DOC_DESTRUCT_INSTALL_DIR}/developer
            COMPONENT destruct_doc
            OPTIONAL)
  else(DOXYGEN_FOUND)
    set(DOC_DESTRUCT_GENERATE_DEVELOPER OFF)
  endif(DOXYGEN_FOUND)
endif(DOC_DESTRUCT_GENERATE_DEVELOPER)
