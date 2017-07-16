# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007
cmake_minimum_required(VERSION 2.6)

## POLICY FOR WINDOWS ONLY ???
#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

PROJECT ("ToolMap")

#Use O2 optimisation
IF(APPLE)
  MESSAGE(STATUS "REPLACING O3 WITH O2")
  STRING(REPLACE "O3"  "O2"  CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
  STRING(REPLACE "O3"  "O2"  CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
  MESSAGE(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
  MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

  # add no optimization level to Debug version. Otherwise debugging didn't work correctly
  SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
  SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0") 
  MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
  MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
ENDIF(APPLE)

# Sources and headers 
FILE (GLOB_RECURSE SRC_FILES src/*.cpp)
SET(MY_SEARCHED_FILE "src/core/toolmap.cpp")
FILE(GLOB MAIN_SRC_FILE "${MY_SEARCHED_FILE}")
LIST(LENGTH SRC_FILES MYLENGTH)
LIST(FIND SRC_FILES "${MAIN_SRC_FILE}" MYFOUND)
IF(MYFOUND EQUAL -1)
  MESSAGE(FATAL_ERROR "Following file not found : ${MY_SEARCHED_FILE}")
ENDIF(MYFOUND EQUAL -1)
LIST(REMOVE_AT SRC_FILES ${MYFOUND})
FILE (GLOB_RECURSE HEAD_FILES "src/*.h")
SOURCE_GROUP (header FILES ${HEAD_FILES})

# Includes 
SET(WXWINDOWS_USE_GL 0)
INCLUDE ("build/cmake/FindwxWidgets.cmake")
INCLUDE ("build/cmake/findMySQLEmbedded.cmake")
INCLUDE ("build/cmake/findGISLib.cmake")
INCLUDE ("${PROJECT_SOURCE_DIR}/src/components/wxtreemultictrl/wxtreectrloptions.cmake")
INCLUDE ("build/cmake/FindCURL.cmake")
INCLUDE ("build/cmake/findwxpdfdocument.cmake")
INCLUDE("plugins/lsversion/build/use_lsversion.cmake")
INCLUDE ("plugins/lscrashreport/build/use_lscrashreport.cmake")

# CREATE A LIBRARY WITH ALL SRC
ADD_LIBRARY("${CMAKE_PROJECT_NAME}_lib" STATIC ${SRC_FILES} ${HEAD_FILES})

IF( APPLE )
  # copy icon
  SET_SOURCE_FILES_PROPERTIES("art/toolmap2.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  SET(MAIN_SRC_FILE ${MAIN_SRC_FILE} "art/toolmap2.icns")	
ENDIF (APPLE)

TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSVERSION_NAME}) 
TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSCRASHREPORT_NAME})

# conditionnal compilation if windows needs ressource
IF (WIN32)
  SOURCE_GROUP (resource FILES src/resource.rc)
  ADD_EXECUTABLE ("${CMAKE_PROJECT_NAME}" WIN32 ${MAIN_SRC_FILE} src/gui/resource.rc)
  TARGET_LINK_LIBRARIES ("${CMAKE_PROJECT_NAME}" ${WXWINDOWS_LIBS} "${CMAKE_PROJECT_NAME}_lib")
ELSE (WIN32)
  ADD_EXECUTABLE (${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${MAIN_SRC_FILE} ${HEAD_FILES})
  IF(APPLE)
    TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib")
  ELSE(APPLE)
    #LINUX
    TARGET_LINK_LIBRARIES(${LSVERSION_NAME} fontconfig)
    TARGET_LINK_LIBRARIES(${LSCRASHREPORT_NAME} fontconfig)
    TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" fontconfig)
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}_lib" fontconfig)
  ENDIF(APPLE)
ENDIF (WIN32)

IF (APPLE)
  # Copy plist to ToolMap (should be after add executable!
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/build/custominfo.plist)
ENDIF(APPLE)

# adding multi-core support for Visual Studio Compiler
IF(MSVC)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    MESSAGE(STATUS "Adding multiprocessor support for Visual Studio (/MP Flag)")
ENDIF(MSVC)

INCLUDE ("build/cmake/installMacGISLIB.cmake")
INCLUDE ("build/cmake/installMacLibTiff.cmake")
INCLUDE ("build/cmake/Use_wingdiplus.cmake")
INCLUDE ("build/cmake/Use_visualstudioMT.cmake")
INCLUDE ("build/cmake/useMySQLEmbedded.cmake")
INCLUDE ("build/cmake/GDALCopyLibs.cmake")
INCLUDE ("plugins/lscrashreport/build/CURLCopyLibs.cmake")
INCLUDE ("build/cmake/copywebfiles.cmake")

# UNIT TEST PROGRAM
SET (USE_UNITTEST CACHE BOOL "Should we Create the Unit Test (XP) for ${CMAKE_PROJECT_NAME}")
IF (USE_UNITTEST)
  ADD_SUBDIRECTORY("test/build" "${PROJECT_BINARY_DIR}/test")
ENDIF (USE_UNITTEST)



# TODO: Move this to create_installer.cmake
IF (UNIX AND NOT APPLE)
  INSTALL(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
  INSTALL(FILES "art/toolmap.png" DESTINATION "/usr/share/pixmaps")
  INSTALL(FILES "install/linux/toolmap.desktop" DESTINATION "/usr/share/applications")

  SET(CPACK_GENERATOR "DEB")
  SET(CPACK_PACKAGE_NAME "toolmap")
  #SET(CPACK_PACKAGE_VERSION "2.4.1328") // in python build script!
  SET(CPACK_PACKAGE_VERSION_MAJOR "2")
  SET(CPACK_PACKAGE_VERSION_MINOR "5")
  SET(CPACK_PACKAGE_VERSION_PATCH "${lsVERSION_SOFT_VERSION}")

  #SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucien Schreiber <lucien.schreiber@crealp.vs.ch>")
  SET(CPACK_PACKAGE_VENDOR "CREALP")
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libmysqlclient18 (>= 5.1), mysql-server-core-5.5, libgeos-c1, libcurl3-gnutls, libgdal1-1.7.0, libwrap0")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Let's bring consistency into your maps")
  # SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "In order to handle geological information in an efficient way and for easily transfer it into geological information system, the research center on alpine environment (CREALP) developed in close collaboration with the Swiss Geological Survey (SGS) the software package ToolMap. ToolMap allows structuring and storing geological data through a robust relational database, vectorising field data and building up multilayer GIS models with high topological integrity. Thus, it permits a much faster production of GIS datasets readily available for a wide variety of applications.")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  INCLUDE(CPack)
ENDIF(UNIX AND NOT APPLE)

