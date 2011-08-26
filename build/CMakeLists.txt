# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007
cmake_minimum_required(VERSION 2.4)


## POLICY FOR WINDOWS ONLY ???
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET (PROGNAME ToolMap)
PROJECT (${PROGNAME})

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


MESSAGE ("Build type is : " ${CMAKE_BUILD_TYPE})


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

# CREATE A LIBRARY WITH ALL SRC
ADD_LIBRARY("${PROGNAME}_lib" STATIC ${SRC_FILES} ${HEAD_FILES})

IF( APPLE )
	SET (APP_ICON "toolmap2.icns")
	#bundle variables for compilation under Mac OS
	SET(MACOSX_BUNDLE_INFO_STRING "GIS program implementing the SION Method")
	SET(MACOSX_BUNDLE_ICON_FILE ${APP_ICON})
	#MACOSX_BUNDLE_GUI_IDENTIFIER
	#MACOSX_BUNDLE_LONG_VERSION_STRING
	SET(MACOSX_BUNDLE_BUNDLE_NAME "${PROGNAME}")
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "NEUWILEN")
	SET(MACOSX_BUNDLE_BUNDLE_VERSION "2.2")
	SET(MACOSX_BUNDLE_COPYRIGHT "Lucien Schreiber (c) CREALP 2011")
	
	# copy icon
	SET_SOURCE_FILES_PROPERTIES("../art/toolmap2.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	SET(MAIN_SRC_FILE ${MAIN_SRC_FILE} "../art/toolmap2.icns")	
ENDIF (APPLE)


TARGET_LINK_LIBRARIES("${PROGNAME}_lib" ${LSVERSION_NAME}) 
# conditionnal compilation if windows needs ressource
IF (WIN32)

	SOURCE_GROUP (resource FILES ../src/resource.rc)
 	ADD_EXECUTABLE ("${PROGNAME}" WIN32 ${MAIN_SRC_FILE} ../src/gui/resource.rc)
 	TARGET_LINK_LIBRARIES ("${PROGNAME}" ${WXWINDOWS_LIBS} "${PROGNAME}_lib")

ELSE (WIN32)
 	
 	ADD_EXECUTABLE (${PROGNAME} MACOSX_BUNDLE ${MAIN_SRC_FILE} ${HEAD_FILES})
	TARGET_LINK_LIBRARIES ("${PROGNAME}" "${PROGNAME}_lib")

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
SET (USE_UNITTEST CACHE BOOL "Should we Create the Unit Test (XP) for ${PROGNAME}")
IF (USE_UNITTEST)
	ADD_SUBDIRECTORY("../test/build" "${PROJECT_BINARY_DIR}/test")
ENDIF (USE_UNITTEST)

IF (NOT APPLE AND NOT WIN32)
  INSTALL(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION /usr/bin)
  INSTALL(FILES "../art/toolmap32.png" DESTINATION "/usr/share/pixmaps")
  INSTALL(FILES "../install/linux/toolmap.desktop" DESTINATION "/usr/share/applications")
ENDIF(NOT APPLE AND NOT WIN32)


