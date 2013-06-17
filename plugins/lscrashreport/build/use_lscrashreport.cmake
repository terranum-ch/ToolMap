# SCRIPT FOR BUILDING lsCrashReport
#OPTIONS - PARAMETERS 
SET(LSCRASHREPORT_NAME "lsCrashReport")
SET(USE_CRASHREPORT 1 CACHE BOOL "Use ${LSCRASHREPORT_NAME} component?")
IF(USE_CRASHREPORT)
	#FINDING OUR PATH
	FIND_PATH(LSCRASHREPORT_PATH "lscrashreport.h" 
	"/plugins/lscrashreport/src/"
	"../plugins/lscrashreport/src/"
	"../../plugins/lscrashreport/src/"
	"../../../plugins/lscrashreport/src/"
	"../../../../plugins/lscrashreport/src/"
	"/lib/lscrashreport/src/"
	"../lib/lscrashreport/src/"
	"../../lib/lscrashreport/src/"
	"../../../lib/lscrashreport/src/"
	"../../../../lib/lscrashreport/src/"
	"/lscrashreport/src/"
	"../lscrashreport/src/"
	"../../lscrashreport/src/"
	"../../../lscrashreport/src/"
	"../../../../lscrashreport/src/"
	 NO_DEFAULT_PATH)
	 IF(NOT LSCRASHREPORT_PATH)
		 MESSAGE(FATAL_ERROR "Unable to find lscrashreport.h")
	 ENDIF(NOT LSCRASHREPORT_PATH)
	
	# CURL for sending crashreport
	INCLUDE("${LSCRASHREPORT_PATH}/../build/FindCURL.cmake")

 	#
 	# CREATE LIBRARY 
 	#
 	#search all source files
 	MESSAGE(${LSCRASHREPORT_PATH})
 	FILE (GLOB LSCRASH_SRC_FILES "${LSCRASHREPORT_PATH}/*.cpp")
 	SOURCE_GROUP(source FILES ${LSCRASH_SRC_FILES})

 	# search all headers files
 	FILE (GLOB LSCRASH_HEAD_FILES "${LSCRASHREPORT_PATH}/*.h")
 	SOURCE_GROUP(header FILES ${LSCRASH_HEAD_FILES})
 	INCLUDE_DIRECTORIES("${LSCRASHREPORT_PATH}")
 	ADD_LIBRARY("${LSCRASHREPORT_NAME}" STATIC ${LSCRASH_SRC_FILES} ${LSCRASH_HEAD_FILES})
	
	#MESSAGE(" src:::::: ${LSCRASH_SRC_FILES}")
ENDIF(USE_CRASHREPORT)

CONFIGURE_FILE("${LSCRASHREPORT_PATH}/lscrashreport_param.h.in" "${PROJECT_BINARY_DIR}/lscrashreport_param.h")
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})





