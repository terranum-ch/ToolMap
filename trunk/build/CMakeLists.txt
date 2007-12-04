# CMAKE BUILD FILE FOR ToolMap 2
# (c) LUCIEN SCHREIBER CREALP 2007


PROJECT (ToolMap2)

#search all source files
FILE (GLOB_RECURSE SRC_FILES ../src/*.cpp)
SOURCE_GROUP (source FILES ${SRC_FILES})

# search all headers files
FILE (GLOB_RECURSE HEAD_FILES "../src/*.h")
SOURCE_GROUP (header FILES ${HEAD_FILES})


# WXWIDGETS
# DEFINE ALL INCLUDES AND SO ON
 INCLUDE ("cmake/Use_wxWindows.cmake")

# Getting the subversion number and
# generating a file called svn_version.h

IF (WIN32)

ELSE (WIN32)
ADD_CUSTOM_COMMAND(
	OUTPUT ../src/core/svn_version.h
	COMMAND sh
	ARGS version_svn_unix.sh
	)
ENDIF(WIN32)
 
# conditionnal compilation if windows needs ressource
IF (WIN32)

	SOURCE_GROUP (resource FILES ../src/resource.rc)
 	ADD_EXECUTABLE (ToolMap2 WIN32 ${SRC_FILES} ${HEAD_FILES} ../src/gui/resource.rc)
 	TARGET_LINK_LIBRARIES (ToolMap2 ${WXWINDOWS_LIBS})

ELSE (WIN32)
 
 	ADD_EXECUTABLE (ToolMap2 ${SRC_FILES} ${HEAD_FILES} )
 	#TARGET_LINK_LIBRARIES (ToolMap2)

ENDIF (WIN32)



