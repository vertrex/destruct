# Check 64 bit
if( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )
  set( HAVE_64_BITS 0 )
else( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )
  if (WIN32)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DSWIGWORDSIZE32)
  else()
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DSWIGWORDSIZE64)
  endif()
  set( HAVE_64_BITS 1 )
endif( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )

#### Create Global variables used by install targets and cpack

if(PROJECT_EDITION)
  string(TOLOWER ${PROJECT_EDITION} PROJECT_EDITION_LOWER)
  set(CONSOLE_SCRIPT "dff-${PROJECT_EDITION_LOWER}.py")
  set(GRAPHICAL_SCRIPT "dff-${PROJECT_EDITION_LOWER}-gui.py")
else()
  set(CONSOLE_SCRIPT "dff.py")
  set(GRAPHICAL_SCRIPT "dff-gui.py")
endif()


#### Basic Cmake definitions
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")


## Swig We need at least version 2.0.7 of SWIG
find_package(SWIG REQUIRED)
if (${SWIG_VERSION_MAJOR} LESS 2 OR (${SWIG_VERSION_MAJOR} EQUAL 2 AND ${SWIG_VERSION_PATCH} LESS 7))
  message(FATAL_ERROR "Need SWIG version >= 2.0.7 (current version is ${SWIG_VERSION})")
else()
  message(STATUS "Found compatible SWIG version (${SWIG_VERSION})")
endif()
include(${SWIG_USE_FILE})


set(CMAKE_INCLUDE_PATH "${INCLUDEDIR}")
set(CMAKE_LIBRARY_PATH "${LIBDIR}")

# Optional dependencies required version
set(BFIO_REQUIRED_VERSION "20120425")
set(PFF_REQUIRED_VERSION "20120802")


# Set installation mode, include all items (*.py, ...) Default is development mode
option(DEVELOP "Start installation mode ?" OFF)
IF(DEVELOP)
  message("          /==========================\\")
  message("          | Running development mode |")
  message("          \\==========================/")
ENDIF(DEVELOP)

IF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  SET(DEDICATED_BUILD_DIR 1)
  message(STATUS "Building project in dedicated build directory : ${CMAKE_BINARY_DIR}")
ENDIF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_UNSUPPORTED "Build unsupported modules ?" OFF)

option(ENABLE_DEBUG "Compile using -g flag ? Useful for debugging" OFF)
add_definitions(-D__STDC_LIMIT_MACROS)
if(UNIX)
  if(ENABLE_DEBUG)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall)
    message(STATUS "Will use -g for debugging -- yes")
  else(ENABLE_DEBUG)
#change flag here
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -O2)
    message(STATUS "Will use -g for debugging -- no")
  endif(ENABLE_DEBUG)
endif(UNIX)
# $> cmake -DENABLE_DEBUG:BOOLEAN=OFF

option(WITH_IOSTAT "enable statistics on input / output comsumption ?" OFF)
if (WITH_IOSTAT)
  add_definitions(-DWITH_IOSTAT=1)
  message(STATUS "input / output stats enabled")
else (WITH_IOSTAT)
  message(STATUS "input / output stats disabled")
endif (WITH_IOSTAT)

option(WITH_TTT_DEBUG "Compile with two three tree debugging" OFF)
if (WITH_TTT_DEBUG)
  add_definitions(-DTWO_THREE_TREE_DEBUG=1)
  message(STATUS "Compile WITH TwoThreeTree debug information")
else (WITH_TTT_DEBUG)
  message(STATUS "Compile WITHOUT TwoThreeTree debug information")
endif (WITH_TTT_DEBUG)

IF (WIN32)
  option(WINALL "Package with windows Dependencies ?" OFF)
  IF(WINALL)
    message(STATUS "Packaging windows version with dependencies")
  ELSE(WINALL)
    message(STATUS "Packaging windows version without dependencies")
  ENDIF(WINALL)
ENDIF(WIN32)

if (ICU_DEP_PATH)
  if (WIN32)
    if (HAVE_64_BITS)
      set(ICU_PATH_SUFFIX "64")
    else()
      set(ICU_PATH_SUFFIX "")
    endif()
    set(ICU_INCLUDE_PATH "${ICU_DEP_PATH}/include")
    set(ICU_LIBRARIES_PATH "${ICU_DEP_PATH}/lib${ICU_PATH_SUFFIX}")
    set(ICU_DYNLIB_PATH "${ICU_DEP_PATH}/bin${ICU_PATH_SUFFIX}")
  endif (WIN32)
endif (ICU_DEP_PATH)

find_package(ICU REQUIRED)
find_package(AFF)
find_package(BFIO)
find_package(EWF)
find_package(PFF)
find_package(TRE)
find_package(FFmpeg)

if (UNIX)
  find_package(UDEV)
  if (UDEV_FOUND)
    SET(HAVE_UDEV TRUE)
    message(STATUS "udev include and libraries: FOUND")
  endif (UDEV_FOUND)
endif(UNIX)


if (FFMPEG_FOUND)
  message(STATUS "FFmpeg includes and libraries found video module: ENABLED")
else ()
  message(STATUS "FFmpeg includes and libraries not found video module: DISABLED")
endif (FFMPEG_FOUND)


IF (TRE_FOUND)
   add_definitions(-DHAVE_TRE)
   include_directories(${TRE_INCLUDE_DIR})
   message(STATUS "TRE installed version: ${TRE_VERSION}
   approximative matching support : ${TRE_HAVE_APPROX}
   wide character support         : ${TRE_HAVE_WCHAR}
   multibyte character support    : ${TRE_HAVE_MULTIBYTE}")
ENDIF (TRE_FOUND)

if(PFF_FOUND)
   if("${PFF_VERSION}" VERSION_EQUAL "${PFF_REQUIRED_VERSION}" OR "${PFF_VERSION}" VERSION_GREATER "${PFF_REQUIRED_VERSION}")
     message(STATUS "PFF installed version: ${PFF_VERSION}
   >= ${PFF_REQUIRED_VERSION} -- yes")
   else("${PFF_VERSION}" VERSION_EQUAL "${PFF_REQUIRED_VERSION}" OR "${PFF_VERSION}" VERSION_GREATER "${PFF_REQUIRED_VERSION}")
     message(STATUS "PFF installed version: ${PFF_VERSION}
   >= ${PFF_REQUIRED_VERSION} -- no")
     unset(PFF_FOUND)
     unset(PFF_VERSION)
   endif("${PFF_VERSION}" VERSION_EQUAL "${PFF_REQUIRED_VERSION}" OR "${PFF_VERSION}" VERSION_GREATER "${PFF_REQUIRED_VERSION}")
endif(PFF_FOUND)

if(BFIO_FOUND)
   if("${BFIO_VERSION}" VERSION_EQUAL "${BFIO_REQUIRED_VERSION}" OR "${BFIO_VERSION}" VERSION_GREATER "${BFIO_REQUIRED_VERSION}")
     message(STATUS "BFIO installed version: ${BFIO_VERSION}
   >= ${BFIO_REQUIRED_VERSION} -- yes")
   else("${BFIO_VERSION}" VERSION_EQUAL "${BFIO_REQUIRED_VERSION}" OR "${BFIO_VERSION}" VERSION_GREATER "${BFIO_REQUIRED_VERSION}")
     message(STATUS "BFIO installed version: ${BFIO_VERSION}
   >= ${BFIO_REQUIRED_VERSION} -- no")
     unset(BFIO_FOUND)
     unset(BFIO_VERSION)
   endif("${BFIO_VERSION}" VERSION_EQUAL "${BFIO_REQUIRED_VERSION}" OR "${BFIO_VERSION}" VERSION_GREATER "${BFIO_REQUIRED_VERSION}")
endif(BFIO_FOUND)

IF (EWF_FOUND)
   message(STATUS "EWF installed version: ${EWF_VERSION}")
   message(STATUS "library: ${EWF_LIBRARY}")
ENDIF (EWF_FOUND)

if(AFF_FOUND)
  message(STATUS "AFF installed version: ${AFF_VERSION}")
endif(AFF_FOUND)

# Project-wide swig options
#SET(CMAKE_SWIG_FLAGS "-py3")

option(DISABLE_SWIG_THREADING "Wrap cpp code to python without -threads" OFF)
if(DISABLE_SWIG_THREADING)
  message(STATUS "Will we use swig -threads -- no")
else()
  #message(STATUS "Will we use swig -threads -- yes")
  #set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -O -threads)
  set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -threads)
endif(DISABLE_SWIG_THREADING)
# $> cmake -DDISABLE_SWIG_THREADING:BOOLEAN=ON

find_library(HAVE_FUSE NAMES fuse)
if(NOT HAVE_FUSE)
  message(STATUS "(Optional) fuse library not found; file system module 'fuse' will not be built")
endif(NOT HAVE_FUSE)


IF(WIN32)
  SET(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DWIN32 -DSWIGWIN)
  add_definitions("/W3 /D_CRT_SECURE_NO_WARNINGS /wd4290 /nologo")
ENDIF(WIN32)


## Python check
FIND_PACKAGE(PythonInterp REQUIRED)

if (WIN32)
  find_package(PythonLibs)
  SET(PYTHON_BIN_PATH ${PYTHON_EXECUTABLE})
# FIXME for windows validate presence of Python.h in PYTHON_INCLUDE_PATH
endif()

FIND_PACKAGE(PythonLibrary REQUIRED)

# Get Python site packages for installation target
execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

INCLUDE(PythonMacros)

IF (NOT MSVC)
  ADD_DEFINITIONS(-fPIC)
ENDIF ()


if(UNIX)
# Search for gzip program, to compress manpage for Unix
  find_program(GZIP_TOOL
               NAMES gzip
               PATHS /bin
               /usr/bin
               /usr/local/bin)
  if(NOT GZIP_TOOL)
    message(FATAL_ERROR "Unable to find 'gzip' program")
  endif(NOT GZIP_TOOL)
endif(UNIX)

## Python-magic check for Unix only
if(UNIX)
  execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import magic; print magic.__file__" OUTPUT_VARIABLE PYTHON_MAGIC_PATH ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT PYTHON_MAGIC_PATH)
    message(STATUS "Python magic not found. Not needed at build step but mandatory to start DFF.")
  else(NOT PYTHON_MAGIC_PATH)
    message(STATUS "Python magic found: ${PYTHON_MAGIC_PATH}")
  endif(NOT PYTHON_MAGIC_PATH)
endif(UNIX)

## Python-QT bindings check
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import PyQt4; print PyQt4.__path__[0]" OUTPUT_VARIABLE PYTHON_QT4_PATH ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT PYTHON_QT4_PATH)
  message(STATUS "Python QT4 bindings not found. Not needed at build step but mandatory to start DFF.")
else(NOT PYTHON_QT4_PATH)
  message(STATUS "Python QT4 libraries found: ${PYTHON_QT4_PATH} (version: ${PYQT4_VERSION_STR})")
endif(NOT PYTHON_QT4_PATH)

## PyQt linguist transalation updater check, to create or update translation
## files
find_program(PYTHON_QT4_LANGUAGE NAMES pylupdate4 PATHS	${CMAKE_SYSTEM_PROGRAM_PATH} ${PYTHON_QT4_PATH}/bin ${PYTHON_QT4_PATH})
if(PYTHON_QT4_LANGUAGE)
  message(STATUS "Python Qt4 linguist translation files updater found: ${PYTHON_QT4_LANGUAGE}")
else(PYTHON_QT4_LANGUAGE)
  message(STATUS "Python Qt4 linguist translation files updater not found, unable to check for new tranlatable strings.")
endif(PYTHON_QT4_LANGUAGE)

## QT .ts to .qm compiler, used by translator objects
find_program(QT_LANGUAGE_COMPILER NAMES lrelease lrelease-qt4 PATHS ${CMAKE_SYSTEM_PROGRAM_PATH} ${PYTHON_QT4_PATH}/bin ${PYTHON_QT4_PATH})
if(QT_LANGUAGE_COMPILER)
  message(STATUS "QT translation compiler found: ${QT_LANGUAGE_COMPILER}")
else(QT_LANGUAGE_COMPILER)
  message(ERROR "QT translation compiler not found.")
endif(QT_LANGUAGE_COMPILER)

## PyQt UI compiler check, to generate widgets
find_program(PYTHON_QT4_UIC NAMES pyuic4 pyuic4.bat PATHS ${CMAKE_SYSTEM_PROGRAM_PATH} ${PYTHON_QT4_PATH}/bin ${PYTHON_QT4_PATH})
if(PYTHON_QT4_UIC)
  message(STATUS "Python Qt4 user interface compiler found: ${PYTHON_QT4_UIC}")
else(PYTHON_QT4_UIC)
  message(SEND_ERROR "Python Qt4 user interface compiler not found.")
endif(PYTHON_QT4_UIC)

## PyQt resource compiler check, to generate icons
find_program(PYTHON_QT4_RCC NAMES pyrcc4 PATHS ${CMAKE_SYSTEM_PROGRAM_PATH} ${PYTHON_QT4_PATH}/bin ${PYTHON_QT4_PATH})
if(PYTHON_QT4_RCC)
  message(STATUS "Python Qt4 resource compiler found: ${PYTHON_QT4_RCC}")
else(PYTHON_QT4_RCC)
  message(SEND_ERROR "Python Qt4 resource compiler not found.")
endif(PYTHON_QT4_RCC)
