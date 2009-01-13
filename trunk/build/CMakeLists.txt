# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007
cmake_minimum_required(VERSION 2.4)


## POLICY FOR WINDOWS ONLY ???
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET (PROGNAME ToolMap2)
PROJECT (${PROGNAME})

#search all source files
FILE (GLOB_RECURSE SRC_FILES ../src/*.cpp)
SOURCE_GROUP (source FILES ${SRC_FILES})

# search all headers files
FILE (GLOB_RECURSE HEAD_FILES "../src/*.h")
SOURCE_GROUP (header FILES ${HEAD_FILES})


MESSAGE ("Build type is : " ${CMAKE_BUILD_TYPE})

#IF (CMAKE_BUILD_TYPE STREQUAL "Release")
#	SET(WX_CONFIG_ARGS_LIBS "${WX_CONFIG_ARGS_LIBS} --debug=no" )
#	MESSAGE ("Build type is release"  ${WX_CONFIG_ARGS_LIBS})
#ELSE (CMAKE_BUILD_TYPE STREQUAL "Release")
#	SET(WX_CONFIG_ARGS_LIBS "${WX_CONFIG_ARGS_LIBS} --debug=no" )
#	MESSAGE ("Build type is debug")
#ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")


# WXWIDGETS
# DEFINE ALL INCLUDES AND SO ON
 SET(WXWINDOWS_USE_GL 0)
 
 INCLUDE ("cmake/Use_wxWindows.cmake")
 #INCLUDE ("cmake/UsewxWidgets.cmake")
 
 
 #MYSQL EMBEDDED
 INCLUDE ("cmake/findMySQLEmbedded.cmake")


#GEOS - GDAL libs
 INCLUDE ("cmake/findGISLib.cmake")
 

# Getting the subversion number and
# generating a file called svn_version.h
IF (WIN32)
	EXECUTE_PROCESS(
		COMMAND version_svn_win.bat
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../build/
	)
ELSE (WIN32)
	EXECUTE_PROCESS(
		COMMAND sh version_svn_unix.sh
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../build/
	)
ENDIF(WIN32)





IF( APPLE )
	SET (APP_ICON "toolmap2.icns")
	#bundle variables for compilation under Mac OS
	SET(MACOSX_BUNDLE_INFO_STRING "GIS program implementing the SION Method")
	SET(MACOSX_BUNDLE_ICON_FILE ${APP_ICON})
	#MACOSX_BUNDLE_GUI_IDENTIFIER
	#MACOSX_BUNDLE_LONG_VERSION_STRING
	SET(MACOSX_BUNDLE_BUNDLE_NAME "${PROGNAME}")
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "Alpha")
	SET(MACOSX_BUNDLE_BUNDLE_VERSION "2.0.Alpha")
	SET(MACOSX_BUNDLE_COPYRIGHT "Lucien Schreiber (c) CREALP 2008")
	
	# Need to copy the icon file
	EXEC_PROGRAM( "mkdir -p       ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROGNAME}.app/Contents/Resources")
	EXEC_PROGRAM( "cp ${PROJECT_SOURCE_DIR}/../art/${APP_ICON} ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROGNAME}.app/Contents/Resources")
	
ENDIF (APPLE)


 
# conditionnal compilation if windows needs ressource
IF (WIN32)

	SOURCE_GROUP (resource FILES ../src/resource.rc)
 	ADD_EXECUTABLE (ToolMap2 WIN32 ${SRC_FILES} ${HEAD_FILES} ../src/gui/resource.rc)
 	TARGET_LINK_LIBRARIES (ToolMap2 ${WXWINDOWS_LIBS})

ELSE (WIN32)
 	
 	ADD_EXECUTABLE (ToolMap2 MACOSX_BUNDLE ${SRC_FILES} ${HEAD_FILES} )
 	#SET_TARGET_PROPERTIES(ToolMap2 LINK_FLAGS_ RELEASE)
 	#TARGET_LINK_LIBRARIES (ToolMap2)

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


