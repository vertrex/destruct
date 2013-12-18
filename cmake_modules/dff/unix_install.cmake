
####
#
#  
# Cmakefile in charge of creating install targets for Unix systems
#
#
####


# Compress manpage
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dff.1.gz
                   COMMAND ${GZIP_TOOL} -c ${CMAKE_CURRENT_SOURCE_DIR}/doc/man/dff.1 > ${CMAKE_CURRENT_BINARY_DIR}/dff.1.gz
                   COMMENT "Building dff.1.gz")
add_custom_target(manpage ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/dff.1.gz)

# Install man page
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dff.1.gz
        DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/share/man/man1
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME dff.1.gz)

# Install docs and licenses
install(FILES README COPYRIGHT LICENSE LICENSE-THIRDPARTY
        DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/share/doc/${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ressources/dff.png ${CMAKE_CURRENT_SOURCE_DIR}/ressources/dff.xpm
 	DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/share/pixmaps
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ressources/dff.desktop
 	DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/share/applications
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CONSOLE_SCRIPT}
	DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/bin/
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	RENAME dff)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${GRAPHICAL_SCRIPT}
	DESTINATION ${CMAKE_INSTALL_ORIG_PREFIX}/bin/
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	RENAME dff-gui)


#install(FILES ${BFIO_LIBRARY} DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${BFIO_LIBRARY}.1" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${BFIO_LIBRARY}.1.0.0" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")

#install(FILES ${PFF_LIBRARY} DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${PFF_LIBRARY}.1" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${PFF_LIBRARY}.1.0.0" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")

#install(FILES ${EWF_LIBRARY} DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${EWF_LIBRARY}.2" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")
#install(FILES "${EWF_LIBRARY}.2.0.0" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib/dff/")

#if (PYREGFI_PATH)
#  install(FILES "${PYREGFI_PATH}/__init__.py" DESTINATION "${PYTHON_SITE_PACKAGES_PATH}/pyregfi/")
#  install(FILES "${PYREGFI_PATH}/structures.py" DESTINATION "${PYTHON_SITE_PACKAGES_PATH}/pyregfi/")
#  install(FILES "${PYREGFI_PATH}/winsec.py" DESTINATION "${PYTHON_SITE_PACKAGES_PATH}/pyregfi/")
#  install(FILES "${PYREGFI_PATH}/libregfi.so" DESTINATION "${CMAKE_INSTALL_ORIG_PREFIX}/lib")
#endif()


ADD_CUSTOM_COMMAND(
 	OUTPUT ${CMAKE_BINARY_DIR}/ctags
	COMMAND ctags --languages=+C++,+Python --exclude=[*.cxx,*wrap.h] --recurse=yes ${CMAKE_SOURCE_DIR}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

ADD_CUSTOM_TARGET(tags DEPENDS ${CMAKE_BINARY_DIR}/ctags)

