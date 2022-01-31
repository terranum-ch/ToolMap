# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007
cmake_minimum_required(VERSION 3.0)

## POLICY FOR WINDOWS ONLY ???
#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

PROJECT("ToolMap")
set(VERSION_MAJOR "3")
set(VERSION_MINOR "2")
set(VERSION_PATCH "0") # will be modified later during cmake process

#Export for sourcetrail
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Use O2 optimisation
IF (APPLE)
    MESSAGE(STATUS "REPLACING O3 WITH O2")
    STRING(REPLACE "O3" "O2" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
    STRING(REPLACE "O3" "O2" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    MESSAGE(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
    MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

    # add no optimization level to Debug version. Otherwise debugging didn't work correctly
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
ENDIF (APPLE)

# Enable tests
SET(USE_UNITTEST CACHE BOOL "Should we Create the Unit Test (XP) for ${CMAKE_PROJECT_NAME}")

# Enable code coverage
if (USE_UNITTEST AND CMAKE_COMPILER_IS_GNUCXX)
    option(USE_CODECOV "Sould we do code coverage with lcov ?" OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O0 -fprofile-arcs -ftest-coverage")
else (CMAKE_COMPILER_IS_GNUCXX)
    set(USE_CODECOV OFF)
endif ()

# Enable output of compile commands (for Sourcetrail)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# External projects
include(ExternalProject)
set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${EXTERNAL_DIR}")
include_directories(${EXTERNAL_DIR}/include)
link_directories(${EXTERNAL_DIR}/lib)
link_directories(${EXTERNAL_DIR}/lib64)

# Path to MySQL
set(MYSQL_MAIN_DIR CACHE PATH "Path to MySQL main directory.")

# Option to get ToolBasView
set(WITH_TOOLBASVIEW ON CACHE BOOL "Should we build ToolBasView along with ToolMap?")

# Add ToolBasView as external project
if (WITH_TOOLBASVIEW)
    ExternalProject_Add(ToolBasView
            GIT_REPOSITORY https://github.com/terranum-ch/ToolBasView
            UPDATE_COMMAND ""
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DSEARCH_WXWIDGETS_PATH=${SEARCH_WXWIDGETS_PATH} -DWXWINDOWS_ROOT_DIR=${WXWINDOWS_ROOT_DIR} -DSEARCH_GDAL_PATH=${SEARCH_GDAL_PATH} -DSEARCH_PROJ_PATH=${SEARCH_PROJ_PATH} -DSEARCH_GEOS_PATH=${SEARCH_GEOS_PATH} -DSEARCH_SQLITE_PATH=${SEARCH_SQLITE_PATH} -DSEARCH_CURL_PATH=${SEARCH_CURL_PATH} -DMYSQL_MAIN_DIR=${MYSQL_MAIN_DIR} -DSEARCH_GEOS=1 -DSEARCH_GDAL=1
            BUILD_COMMAND ${CMAKE_COMMAND} --build .
            )
endif ()

# Sources and headers 
FILE(GLOB_RECURSE SRC_FILES src/*.cpp)
SET(MY_SEARCHED_FILE "src/core/toolmap.cpp")
FILE(GLOB MAIN_SRC_FILE "${MY_SEARCHED_FILE}")
LIST(LENGTH SRC_FILES MYLENGTH)
LIST(FIND SRC_FILES "${MAIN_SRC_FILE}" MYFOUND)
IF (MYFOUND EQUAL -1)
    MESSAGE(FATAL_ERROR "Following file not found : ${MY_SEARCHED_FILE}")
ENDIF (MYFOUND EQUAL -1)
LIST(REMOVE_AT SRC_FILES ${MYFOUND})
FILE(GLOB_RECURSE HEAD_FILES "src/*.h")
SOURCE_GROUP(header FILES ${HEAD_FILES})

# Conan deps
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Includes
INCLUDE("build/cmake/findMySQLEmbedded.cmake")
INCLUDE("${PROJECT_SOURCE_DIR}/src/components/wxtreemultictrl/wxtreectrloptions.cmake")
INCLUDE("build/cmake/findwxpdfdocument.cmake")
INCLUDE("plugins/lsversion/build/use_lsversion.cmake")
INCLUDE("plugins/lscrashreport/build/use_lscrashreport.cmake")

if (UNIX AND NOT APPLE)
    SET(USE_APPIMAGE FALSE CACHE BOOL "Should we use AppImage to create an installer on Linux ?")
    if (USE_APPIMAGE)
        INCLUDE("build/cmake/LinuxAppImageBuild.cmake")
    endif ()
endif ()

# CREATE A LIBRARY WITH ALL SRC
ADD_LIBRARY("${CMAKE_PROJECT_NAME}_lib" STATIC ${SRC_FILES} ${HEAD_FILES})

IF (APPLE)
    # copy icon
    SET_SOURCE_FILES_PROPERTIES("art/toolmap2.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    SET(MAIN_SRC_FILE ${MAIN_SRC_FILE} "art/toolmap2.icns")
ENDIF (APPLE)

TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSVERSION_NAME})
TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSCRASHREPORT_NAME})

# conditionnal compilation if windows needs ressource
IF (WIN32)
    SOURCE_GROUP(resource FILES src/resource.rc)
    ADD_EXECUTABLE("${CMAKE_PROJECT_NAME}" WIN32 ${MAIN_SRC_FILE} src/gui/resource.rc)
    TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}" ${WXWINDOWS_LIBS} "${CMAKE_PROJECT_NAME}_lib")
ELSE (WIN32)
    ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${MAIN_SRC_FILE} ${HEAD_FILES})
    IF (APPLE)
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib")
    ELSE (APPLE)
        #LINUX
        TARGET_LINK_LIBRARIES(${LSVERSION_NAME} fontconfig)
        TARGET_LINK_LIBRARIES(${LSCRASHREPORT_NAME} fontconfig)
        TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" fontconfig)
        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib" fontconfig)
    ENDIF (APPLE)
ENDIF (WIN32)

if (UNIX)
    add_definitions(-std=c++11)
endif (UNIX)

IF (APPLE)
    # Copy plist to ToolMap (should be after add executable!
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/build/custominfo.plist)
ENDIF (APPLE)

# adding multi-core support for Visual Studio Compiler
IF (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    MESSAGE(STATUS "Adding multiprocessor support for Visual Studio (/MP Flag)")
ENDIF (MSVC)

INCLUDE("build/cmake/installMacGISLIB.cmake")
INCLUDE("build/cmake/installMacLibTiff.cmake")
INCLUDE("build/cmake/Use_wingdiplus.cmake")
INCLUDE("build/cmake/Use_visualstudioMT.cmake")
INCLUDE("build/cmake/useMySQLEmbedded.cmake")
INCLUDE("build/cmake/GDALCopyLibs.cmake")
INCLUDE("plugins/lscrashreport/build/CURLCopyLibs.cmake")
INCLUDE("build/cmake/copyprojfiles.cmake")
INCLUDE("build/cmake/copywebfiles.cmake")

# UNIT TEST PROGRAM
IF (USE_UNITTEST)
    ADD_SUBDIRECTORY("test/build" "${PROJECT_BINARY_DIR}/test")
ENDIF (USE_UNITTEST)

# create installer using CPACK
include("build/cpack/cpack_installer.cmake")

