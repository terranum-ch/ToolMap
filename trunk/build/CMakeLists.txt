# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007
cmake_minimum_required(VERSION 2.4)

## POLICY FOR WINDOWS ONLY ???
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

PROJECT ("ToolMap")

#Use O2 optimisation
IF(APPLE)
	MESSAGE("REPLACING O3 WITH O2")
	STRING(REPLACE "O3"  "O2"  CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
	STRING(REPLACE "O3"  "O2"  CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
	MESSAGE("CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
	MESSAGE("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
ENDIF(APPLE)


#search all source files
FILE (GLOB_RECURSE SRC_FILES ../src/*.cpp)

# GET MAIN SOURCE FILE
SET(MY_SEARCHED_FILE "../src/core/toolmap.cpp")
FILE(GLOB MAIN_SRC_FILE "${MY_SEARCHED_FILE}")

LIST(LENGTH SRC_FILES MYLENGTH)
MESSAGE("Number of source files is : ${MYLENGTH}")
LIST(FIND SRC_FILES "${MAIN_SRC_FILE}" MYFOUND)
MESSAGE("Main src file is found : ${MYFOUND}")

#REMOVE MAIN FROM SRC_FILES
IF(MYFOUND EQUAL -1)
	MESSAGE(FATAL_ERROR "Following file not found : ${MY_SEARCHED_FILE}")
ENDIF(MYFOUND EQUAL -1)
LIST(REMOVE_AT SRC_FILES ${MYFOUND})

# search all headers files
FILE (GLOB_RECURSE HEAD_FILES "../src/*.h")
SOURCE_GROUP (header FILES ${HEAD_FILES})


# WXWIDGETS
# DEFINE ALL INCLUDES AND SO ON
SET(WXWINDOWS_USE_GL 0)
INCLUDE ("cmake/FindwxWidgets.cmake")
 
#MYSQL EMBEDDED
INCLUDE ("cmake/findMySQLEmbedded.cmake")

#GEOS - GDAL libs
INCLUDE ("cmake/findGISLib.cmake")
 
#wxTreeMultiCtrl
INCLUDE ("${PROJECT_SOURCE_DIR}/../src/components/wxtreemultictrl/wxtreectrloptions.cmake")

#CURL
INCLUDE ("cmake/FindCURL.cmake")

#wxPDFDocument
INCLUDE ("cmake/findwxpdfdocument.cmake")

# LSVERSION COMMPONENT
INCLUDE("../plugins/lsversion/build/use_lsversion.cmake")

# LSCRASHREPORT COMPONENT
INCLUDE ("../plugins/lscrashreport/build/use_lscrashreport.cmake")


# CREATE A LIBRARY WITH ALL SRC
ADD_LIBRARY("${CMAKE_PROJECT_NAME}_lib" STATIC ${SRC_FILES} ${HEAD_FILES})


IF( APPLE )
	SET (APP_ICON "toolmap2.icns")
	#bundle variables for compilation under Mac OS
	SET(MACOSX_BUNDLE_INFO_STRING "GIS program implementing the SION Method")
	SET(MACOSX_BUNDLE_ICON_FILE ${APP_ICON})
	#MACOSX_BUNDLE_GUI_IDENTIFIER
	#MACOSX_BUNDLE_LONG_VERSION_STRING
	SET(MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}")
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "BOWIL")
	SET(MACOSX_BUNDLE_BUNDLE_VERSION "2.4")
	SET(MACOSX_BUNDLE_COPYRIGHT "Lucien Schreiber (c) CREALP 2012")
	
	# copy icon
	SET_SOURCE_FILES_PROPERTIES("../art/toolmap2.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	SET(MAIN_SRC_FILE ${MAIN_SRC_FILE} "../art/toolmap2.icns")	
ENDIF (APPLE)

TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSVERSION_NAME}) 
TARGET_LINK_LIBRARIES("${CMAKE_PROJECT_NAME}_lib" ${LSCRASHREPORT_NAME})

# conditionnal compilation if windows needs ressource
IF (WIN32)
	SOURCE_GROUP (resource FILES ../src/resource.rc)
 	ADD_EXECUTABLE ("${CMAKE_PROJECT_NAME}" WIN32 ${MAIN_SRC_FILE} ../src/gui/resource.rc)
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


# copy libs to bundle if we are under mac...
# check is done internally.
INCLUDE ("cmake/installMacGISLIB.cmake")


#SOULD WE ADD THE LIBTIFF INTO BUNDLE
# ONLY FOR CERTAIN MAC WITH PPC 
# CHECK ARE DONE INTERNALLY YOU 
# CAN CALL THIS WITHOUT FEAR...
INCLUDE ("cmake/installMacLibTiff.cmake")

#SOULD WE ADD THE GDIPLUS.dll INTO project
# ONLY FOR Windows, do nothing under other 
# OS. 
# CHECK ARE DONE INTERNALLY YOU 
# CAN CALL THIS WITHOUT FEAR...
INCLUDE ("cmake/Use_wingdiplus.cmake")

# SHOULD WE USE THE MT VERSION FOR DEFAULT 
# COMPATIBILITY WITH < WINXP
IF (WIN32)
	INCLUDE ("cmake/Use_visualstudioMT.cmake")
ENDIF (WIN32)

#Copy errmsg.sys to correct path
INCLUDE ("cmake/useMySQLEmbedded.cmake")

# UNIT TEST PROGRAM
SET (USE_UNITTEST CACHE BOOL "Should we Create the Unit Test (XP) for ${CMAKE_PROJECT_NAME}")
IF (USE_UNITTEST)
	ADD_SUBDIRECTORY("../test/build" "${PROJECT_BINARY_DIR}/test")
ENDIF (USE_UNITTEST)

IF (NOT APPLE AND NOT WIN32)
  INSTALL(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
  INSTALL(FILES "../art/toolmap32.png" DESTINATION "/usr/share/pixmaps")
  INSTALL(FILES "../install/linux/toolmap.desktop" DESTINATION "/usr/share/applications")
  
  SET(CPACK_GENERATOR "DEB")
  SET(CPACK_PACKAGE_NAME "toolmap")
  #SET(CPACK_PACKAGE_VERSION "2.4.1328") // in python build script!
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucien Schreiber")
  SET(CPACK_PACKAGE_VENDOR "CREALP")
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libmysqlclient18 (>= 5.1), mysql-server-core-5.5, libgeos-c1, libcurl3-gnutls, libgdal1-1.7.0, libwrap0")
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "In order to handle geological information in an efficient way and for easily transfer it into geological information system, the research center on alpine environment (CREALP) developed in close collaboration with the Swiss Geological Survey (SGS) the software package ToolMap. ToolMap allows structuring and storing geological data through a robust relational database, vectorising field data and building up multilayer GIS models with high topological integrity. Thus, it permits a much faster production of GIS datasets readily available for a wide variety of applications.")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  
  INCLUDE(CPack)
ENDIF(NOT APPLE AND NOT WIN32)


