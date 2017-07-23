# CMAKE BUILD FILE FOR TESTING TOOLMAP
# USE AND MYSQL AND WXWIDGETS (CONSOLE)
# PART OF QA FOR TOOLMAP 2
# (c) LUCIEN SCHREIBER CREALP 2009
cmake_minimum_required(VERSION 2.4)

include(ExternalProject)
set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)

MESSAGE (STATUS "USING UNIT TESTING")

## POLICY FOR WINDOWS ONLY ???
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


SET (PROGNAMETEST ${CMAKE_PROJECT_NAME}_test)

#prepare parameters file
SET (UNIT_TESTING_PATH CACHE PATH "Path where are located the projects for unit testing")
IF (NOT UNIT_TESTING_PATH)
	ExternalProject_Add(test-files
			URL https://github.com/terranum-ch/ToolMap-test-files/archive/v2.6.tar.gz
			UPDATE_COMMAND ""
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ""
			INSTALL_COMMAND
			${CMAKE_COMMAND} -E copy_directory
			${CMAKE_BINARY_DIR}/test/test-files-prefix/src/test-files
			${CMAKE_BINARY_DIR}/test/test-files
			)
	set(UNIT_TESTING_PATH "${CMAKE_BINARY_DIR}/test/test-files")
ENDIF (NOT UNIT_TESTING_PATH)
message(STATUS "Test files location: ${UNIT_TESTING_PATH}")

SET (UNIT_TESTING_USE_PROXY CACHE BOOL "Do we use a proxy for connecting to the web ?")
FIND_PATH(PARAM_SOURCE_DIR test_param.h.in
  ${PROJECT_SOURCE_DIR}/src_test/
  ${PROJECT_SOURCE_DIR}/test/src_test/
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
FILE (GLOB TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src_test/*.cpp)
FILE (GLOB TEST_HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src_test/*.h)

SET(CXXTEST_USE_PYTHON 1)
find_package(CxxTest)
if(CXXTEST_FOUND)
	MESSAGE(STATUS "Adding tests")
	include_directories(${CXXTEST_INCLUDE_DIR}) 
	enable_testing()
	if (USE_CODECOV)
		MESSAGE(STATUS "Processing code coverage.")
		include(${CMAKE_CURRENT_SOURCE_DIR}/../../build/cmake/CodeCoverage.cmake)
		CXXTEST_ADD_TEST(${PROGNAMETEST} main.cpp ${TEST_HEAD_FILES})
		TARGET_LINK_LIBRARIES(${PROGNAMETEST} "${CMAKE_PROJECT_NAME}_lib")
		if(UNIX AND NOT APPLE)
		    find_package(GTK2)
			if(GTK2_FOUND)
			    include_directories(${GTK2_INCLUDE_DIRS})
                target_link_libraries(${PROGNAMETEST} ${GTK2_LIBRARIES})
			endif()
		endif(UNIX AND NOT APPLE)
		setup_target_for_coverage(${PROJECT_NAME}-coverage ${PROGNAMETEST} coverage)
	else(USE_CODECOV)
		CXXTEST_ADD_TEST(test_advancedbatchattribution test_advancedbatchattribution.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_advancedbatchattribution.h")
		CXXTEST_ADD_TEST(test_backup test_backup.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_backup.h")
		CXXTEST_ADD_TEST(test_database_tm test_database_tm.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_database_tm.h")
		CXXTEST_ADD_TEST(test_database test_database.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_database.h")
		CXXTEST_ADD_TEST(test_databaseresult test_databaseresult.cpp  "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_databaseresult.h")
		CXXTEST_ADD_TEST(test_export test_export.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_export.h")
		CXXTEST_ADD_TEST(test_exportconcat test_exportconcat.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_exportconcat.h")
		CXXTEST_ADD_TEST(test_new_database_tm test_new_database_tm.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_new_database_tm.h")
		CXXTEST_ADD_TEST(test_pdf test_pdf.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_pdf.h")
		CXXTEST_ADD_TEST(test_tmcoordconvert test_tmcoordconvert.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmcoordconvert.h")
		CXXTEST_ADD_TEST(test_tmdataintegrity test_tmdataintegrity.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmdataintegrity.h")
		CXXTEST_ADD_TEST(test_tmdrawereditline test_tmdrawereditline.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmdrawereditline.h")
		CXXTEST_ADD_TEST(test_tmgettingfieldsinfo test_tmgettingfieldsinfo.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmgettingfieldsinfo.h")
		CXXTEST_ADD_TEST(test_tmgisimport test_tmgisimport.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmgisimport.h")
		CXXTEST_ADD_TEST(test_tmmemoryzoom test_tmmemoryzoom.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmmemoryzoom.h")
		CXXTEST_ADD_TEST(test_tmpercent test_tmpercent.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmpercent.h")
		CXXTEST_ADD_TEST(test_tmpointorient test_tmpointorient.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmpointorient.h")
		CXXTEST_ADD_TEST(test_tmpointorientattrib test_tmpointorientattrib.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmpointorientattrib.h")
		CXXTEST_ADD_TEST(test_tmprojectupdate test_tmprojectupdate.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmprojectupdate.h")
		CXXTEST_ADD_TEST(test_tmprojectupdate2 test_tmprojectupdate2.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmprojectupdate2.h")
		CXXTEST_ADD_TEST(test_tmqueries test_tmqueries.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmqueries.h")
		CXXTEST_ADD_TEST(test_tmstatsrecords test_tmstatsrecords.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmstatsrecords.h")
		CXXTEST_ADD_TEST(test_tmwindowposition test_tmwindowposition.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tmwindowposition.h")
		CXXTEST_ADD_TEST(test_tooldanglingnodes test_tooldanglingnodes.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../src_test/test_tooldanglingnodes.h")

		TARGET_LINK_LIBRARIES(test_advancedbatchattribution "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_backup "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_database_tm "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_database "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_databaseresult  "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_export "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_exportconcat "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_new_database_tm "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_pdf "${CMAKE_PROJECT_NAME}_lib")
		if (UNIX)
			TARGET_LINK_LIBRARIES(test_pdf "${WXPDFDOCUMENT_LIBRARY}")
		endif (UNIX)
		TARGET_LINK_LIBRARIES(test_tmcoordconvert "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmdataintegrity "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmdrawereditline "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmgettingfieldsinfo "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmgisimport "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmmemoryzoom "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmpercent "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmpointorient "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmpointorientattrib "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmprojectupdate "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmprojectupdate2 "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmqueries "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmstatsrecords "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tmwindowposition "${CMAKE_PROJECT_NAME}_lib")
		TARGET_LINK_LIBRARIES(test_tooldanglingnodes "${CMAKE_PROJECT_NAME}_lib")
	endif(USE_CODECOV)
endif(CXXTEST_FOUND)


# COPY ERRMSG.SYS TO EXPECTED DIRECTORY!
IF(APPLE)
	FILE(COPY ${MYSQL_ERRMSG_FILE}
			DESTINATION ${PROJECT_BINARY_DIR}/test/${CMAKE_PROJECT_NAME}.app/Contents/mysql)
ELSEIF(WIN32)
	FILE(COPY ${MYSQL_ERRMSG_FILE}
			DESTINATION ${PROJECT_BINARY_DIR}/test/mysql)
	add_custom_command(TARGET test_advancedbatchattribution POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${MYSQL_ERRMSG_FILE}"
		"${PROJECT_BINARY_DIR}/test/${CMAKE_CFG_INTDIR}/mysql/errmsg.sys")
ELSEIF(UNIX)
    FILE(COPY ${MYSQL_ERRMSG_FILE}
            DESTINATION ${PROJECT_BINARY_DIR}/test/mysql)
ENDIF()

# COPY DLL UNDER WINDOWS
IF (WIN32)
	FILE (TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/*.dll" ALL_DLL_PATH_ORIGIN)
	FILE (TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/test/${CMAKE_CFG_INTDIR}" ALL_DLL_PATH_DESTINATION)
	add_custom_command(TARGET test_advancedbatchattribution POST_BUILD
		COMMAND copy  
		${ALL_DLL_PATH_ORIGIN} 
		${ALL_DLL_PATH_DESTINATION})
ENDIF(WIN32)

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
return()




