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


SET (PROGNAMETEST ${CMAKE_PROJECT_NAME}_test)

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
	INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ELSE(PARAM_SOURCE_DIR)
	MESSAGE("test_param.h.in not found, unit testing not availlable")
	MESSAGE( ${PROJECT_SOURCE_DIR})
	MESSAGE (FATAL_ERROR "unit testing not availlable")
ENDIF(PARAM_SOURCE_DIR)


#search all source files
FILE (GLOB TEST_SRC_FILES ../src_test/*.cpp)
FILE (GLOB TEST_HEAD_FILES ../src_test/*.h)


## CXXTEST LIBRARY STUFF
# Use Python interpreter
FIND_PACKAGE(PythonInterp)
# Fait la liaison avec la bibliothèque CxxTest  
INCLUDE("../../build/cmake/FindCxxTest.cmake")
SET(CXXTEST_USE_PYTHON 1)
INCLUDE_DIRECTORIES(${CXXTEST_INCLUDE_DIR})
ENABLE_TESTING()

CXXTEST_ADD_TEST(${PROGNAMETEST} main.cpp ${TEST_HEAD_FILES})
SOURCE_GROUP ("test-source" FILES ${TEST_SRC_FILES} ${TEST_HEAD_FILES})

TARGET_LINK_LIBRARIES(${PROGNAMETEST} "${CMAKE_PROJECT_NAME}_lib")

#ONLY USED FOR ADDING HEADERS TO PROJECT
IF(APPLE)
	ADD_LIBRARY("dummy_files" MODULE EXCLUDE_FROM_ALL main.cpp ${TEST_HEAD_FILES} ${TEST_SRC_FILES})
ENDIF(APPLE)


# COPY ERRMSG.SYS TO EXPECTED DIRECTORY!
IF(APPLE)
	SET (TEST_BINARY_DIR ${PROJECT_BINARY_DIR}/test/)
	#MESSAGE("Test directory:  ${TEST_BINARY_DIR}")
	#MESSAGE("programme test: ${PROJECT_BINARY_DIR} - ${PROGNAMETEST}")
	ADD_CUSTOM_COMMAND(TARGET ${PROGNAMETEST} POST_BUILD
		COMMAND mkdir -p
		${TEST_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql
		COMMAND mkdir -p
		${TEST_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql
		
		COMMAND cp ${MYSQL_INCLUDE_DIR}../share/english/errmsg.sys 
		${TEST_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql/errmsg.sys
		COMMAND cp ${MYSQL_INCLUDE_DIR}../share/english/errmsg.sys 
		${TEST_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql/errmsg.sys
		COMMENT "Copying errmsg.sys for Unit tests"
		)
ENDIF(APPLE)



# SHOULD WE USE THE MT VERSION FOR DEFAULT 
# COMPATIBILITY WITH < WINXP
#IF(PROGNAME_DEFINED_INTERNALLY)

#SOULD WE ADD THE GDIPLUS.dll INTO project
# ONLY FOR Windows, do nothing under other 
# OS. 
# CHECK ARE DONE INTERNALLY YOU 
# CAN CALL THIS WITHOUT FEAR...
SET (MYPROGNAME ${CMAKE_PROJECT_NAME})
SET (PROGNAME ${PROGNAMETEST})
INCLUDE ("../../build/cmake/Use_wingdiplus.cmake")

IF (WIN32)
	INCLUDE ("../../build/cmake/Use_visualstudioMT.cmake")
ENDIF (WIN32)
SET (PROGNAME ${MYPROGNAME})
#ENDIF(PROGNAME_DEFINED_INTERNALLY)

