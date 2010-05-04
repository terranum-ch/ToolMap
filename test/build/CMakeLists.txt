# CMAKE BUILD FILE FOR TESTING TOOLMAP
# USE AND MYSQL AND WXWIDGETS (CONSOLE)
# PART OF QA FOR TOOLMAP 2
# (c) LUCIEN SCHREIBER CREALP 2009
cmake_minimum_required(VERSION 2.4)

MESSAGE ("DBG : USING UNIT TESTING")


## POLICY FOR WINDOWS ONLY ???
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


# SWITCH FOR BUILDING TEST ALONE
# IF PROGNAME ISN'T DEFINED, WE 
# SHOULD CREATE A LIB WITH TOOLMAP CODE
IF(NOT PROGNAME)
	SET(PROGNAME "ToolMap")
	SET(PROGNAME_DEFINED_INTERNALLY TRUE)
	
	# WXWIDGETS
	# DEFINE ALL INCLUDES AND SO ON
	 SET(WXWINDOWS_USE_GL 0) 
	 INCLUDE ("../../build/cmake/FindwxWidgets.cmake")

	#MYSQL EMBEDDED
	INCLUDE ("../../build/cmake/findMySQLEmbedded.cmake")

	#GEOS - GDAL libs
	 INCLUDE ("../../build/cmake/findGISLib.cmake")

	#CURL
	INCLUDE ("../../build/cmake/FindCURL.cmake")

	## CREATE THE LIB IF NEEDED
	#CREATE LIB WITH ALL MAIN PROJECT CLASS
	#search all source files
	FILE (GLOB_RECURSE LIB_SRC_FILES "../../src/*.cpp")
	SOURCE_GROUP(source FILES ${LIB_SRC_FILES})
	#search all source files
	# GET MAIN SOURCE FILE
	SET(MY_SEARCHED_FILE "../../src/core/toolmap.cpp")
	FILE(GLOB MAIN_SRC_FILE "${MY_SEARCHED_FILE}")
	#MESSAGE("Main file is : ${MAIN_SRC_FILE}")
	LIST(LENGTH LIB_SRC_FILES MYLENGTH)
	MESSAGE("Number of source files is : ${MYLENGTH}")
	LIST(FIND LIB_SRC_FILES "${MAIN_SRC_FILE}" MYFOUND)
	MESSAGE("Main src file is found : ${MYFOUND}")
	#REMOVE MAIN FROM SRC_FILES
	IF(MYFOUND EQUAL -1)
		MESSAGE(FATAL_ERROR "Following file not found : ${MY_SEARCHED_FILE}")
	ENDIF(MYFOUND EQUAL -1)
	LIST(REMOVE_AT LIB_SRC_FILES ${MYFOUND})

	# search all headers files
	FILE (GLOB_RECURSE LIB_HEAD_FILES "../../../src/*.h")
	SOURCE_GROUP(header FILES ${LIB_HEAD_FILES})
	INCLUDE_DIRECTORIES("../../../src")

	ADD_LIBRARY("${PROGNAME}_lib" STATIC ${LIB_SRC_FILES} ${LIB_HEAD_FILES})
ENDIF(NOT PROGNAME)
SET (PROGNAMETEST ${PROGNAME}_test)


#prepare parameters file
SET (UNIT_TESTING_PATH CACHE PATH "Path where are located the projects for unit testing")
IF (NOT UNIT_TESTING_PATH)
	MESSAGE(FATAL_ERROR "Unable to locate the test files such as test projects")
ENDIF (NOT UNIT_TESTING_PATH)
SET (UNIT_TESTING_USE_PROXY CACHE BOOL "Do we use a proxy for connecting to the web ?")
FIND_PATH(PARAM_SOURCE_DIR test_param.h.in
  ${PROJECT_SOURCE_DIR}/../src_test/
  ${PROJECT_SOURCE_DIR}/../test/src_test/
  ${PROJECT_SOURCE_DIR}
)

IF (PARAM_SOURCE_DIR)
	CONFIGURE_FILE(${PARAM_SOURCE_DIR}/test_param.h.in 
	"${PROJECT_BINARY_DIR}/test_param.h")
	INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}£)

ELSE(PARAM_SOURCE_DIR)
	MESSAGE("test_param.h.in not found, unit testing not availlable")
	MESSAGE( ${PROJECT_SOURCE_DIR})
	MESSAGE (FATAL_ERROR "unit testing not availlable")
ENDIF(PARAM_SOURCE_DIR)


#search all source files
FILE (GLOB SRC_FILES ../src_test/*.cpp)
SOURCE_GROUP (source FILES ${SRC_FILES})

# search all headers files
FILE (GLOB HEAD_FILES "../src_test/*.h")
SOURCE_GROUP ("test-files" FILES ${HEAD_FILES})


MESSAGE ("Build type is : " ${CMAKE_BUILD_TYPE})




## CXXTEST LIBRARY STUFF
# Use Python interpreter
FIND_PACKAGE(PythonInterp)
# Fait la liaison avec la bibliothèque CxxTest  
INCLUDE("../../build/cmake/FindCxxTest.cmake")
SET(CXXTEST_USE_PYTHON 1)
INCLUDE_DIRECTORIES(${CXXTEST_INCLUDE_DIR})
ENABLE_TESTING()

CXXTEST_ADD_TEST(${PROGNAMETEST} main.cpp ${HEAD_FILES})
TARGET_LINK_LIBRARIES(${PROGNAMETEST} "${PROGNAME}_lib")

#ONLY USED FOR ADDING HEADERS TO PROJECT
IF(APPLE)
ADD_LIBRARY("dummy_files" SHARED EXCLUDE_FROM_ALL main.cpp ${HEAD_FILES})
ENDIF(APPLE)



# SHOULD WE USE THE MT VERSION FOR DEFAULT 
# COMPATIBILITY WITH < WINXP
IF(PROGNAME_DEFINED_INTERNALLY)

#SOULD WE ADD THE GDIPLUS.dll INTO project
# ONLY FOR Windows, do nothing under other 
# OS. 
# CHECK ARE DONE INTERNALLY YOU 
# CAN CALL THIS WITHOUT FEAR...
SET (PROGNAME ${PROGNAMETEST})
INCLUDE ("../../build/cmake/Use_wingdiplus.cmake")

IF (WIN32)
	INCLUDE ("../../build/cmake/Use_visualstudioMT.cmake")
ENDIF (WIN32)
ENDIF(PROGNAME_DEFINED_INTERNALLY)
