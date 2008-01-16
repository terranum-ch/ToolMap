# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007


PROJECT (ToolMap2)

#search all source files
FILE (GLOB_RECURSE SRC_FILES ../src/*.cpp)
SOURCE_GROUP (source FILES ${SRC_FILES})

# search all headers files
FILE (GLOB_RECURSE HEAD_FILES "../src/*.h")
SOURCE_GROUP (header FILES ${HEAD_FILES})

SET(TRUNK_PATH ${PROJECT_SOURCE_DIR}/.. CACHE PATH "Path to the trunk" )


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

# Getting the subversion number and
# generating a file called svn_version.h
IF (WIN32)
	EXECUTE_PROCESS(
		COMMAND version_svn_win.bat
		WORKING_DIRECTORY ${TRUNK_PATH}/build/
	)
ELSE (WIN32)
	EXECUTE_PROCESS(
		COMMAND sh version_svn_unix.sh
		WORKING_DIRECTORY ${TRUNK_PATH}/build/
	)
ENDIF(WIN32)




#bundle variables for compilation under Mac OS
SET(MACOSX_BUNDLE_INFO_STRING "GIS program implementing the SION Method")
SET(MACOSX_BUNDLE_ICON_FILE ${TRUNK_PATH}/art/toolmap2.icns)
#MACOSX_BUNDLE_GUI_IDENTIFIER
#MACOSX_BUNDLE_LONG_VERSION_STRING
SET(MACOSX_BUNDLE_BUNDLE_NAME "ToolMap2")
SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "Alpha")
SET(MACOSX_BUNDLE_BUNDLE_VERSION "2.0.Alpha")
SET(MACOSX_BUNDLE_COPYRIGHT "Lucien Schreiber (c) CREALP 2007")



 
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



