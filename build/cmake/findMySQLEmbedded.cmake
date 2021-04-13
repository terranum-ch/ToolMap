# - Find mysqlclient
# Find the native MySQL includes and embedded library
# Modified by Lucien Schreiber from the http://www.cmake.org/Wiki
#
#  MYSQL_INCLUDE_DIR - where to find mysql.h, etc.
#  MYSQL_LIBRARIES   - List of libraries when using MySQL.
#  MYSQL_FOUND       - True if MySQL found.

IF (MYSQL_INCLUDE_DIR)
    # Already in cache, be silent
    SET(MYSQL_FIND_QUIETLY TRUE)
ENDIF (MYSQL_INCLUDE_DIR)

IF (NOT MYSQL_MAIN_DIR)
    FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
            /usr/local/include/mysql
            /usr/include/mysql)
ENDIF (NOT MYSQL_MAIN_DIR)


# SET (MYSQL_MAIN_DIR CACHE PATH "Path to the main MySQL directory")    
IF (WIN32)
    # search include directory based on main path
    FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
            ${MYSQL_MAIN_DIR}
            ${MYSQL_MAIN_DIR}/include)

    # find library
    FIND_LIBRARY(MYSQL_LIBRARY_EMBEDDED_DEBUG
            NAMES libmysqld
            PATHS
            "${MYSQL_MAIN_DIR}/Embedded/DLL/debug"
            "${MYSQL_MAIN_DIR}/lib/debug"
            DOC "MySQL embedded debug library")

    FIND_LIBRARY(MYSQL_LIBRARY_EMBEDDED
            NAMES libmysqld
            PATHS
            "${MYSQL_MAIN_DIR}/Embedded/DLL/release"
            "${MYSQL_MAIN_DIR}/lib"
            DOC "MySQL embedded library")

    # SET(MYSQL_CONFIG_LIBS ${MYSQL_CONFIG_LIBS}
    #    debug ${MYSQL_LIBRARY_EMBEDDED_DEBUG}  
    #    optimized ${MYSQL_LIBRARY_EMBEDDED})
    SET(MYSQL_CONFIG_LIBS ${MYSQL_LIBRARY_EMBEDDED} CACHE FILEPATH "MySQL Libraries for linking")
    # path to libs
    # IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #    SET(MYSQL_CONFIG_LIBS "${MYSQL_MAIN_DIR}/Embedded/DLL/debug/libmysqld.lib" )
    #  ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")        
    #  IF(CMAKE_BUILD_TYPE STREQUAL "Release")
    #    SET(MYSQL_CONFIG_LIBS "${MYSQL_MAIN_DIR}/Embedded/DLL/release/libmysqld.lib" )
    # ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

ELSE (WIN32)
    SET(MYSQL_INCLUDE_DIR "${MYSQL_MAIN_DIR}/include/")
    SET(MYSQL_CONFIG_EXECUTABLE "${MYSQL_MAIN_DIR}/bin/mysql_config")
    SET(MYSQL_CONFIG_ARGS_LIBS "--libmysqld-libs")
    SET(MYSQL_CONFIG_ARGS_CXFLAGS "--cflags")

    EXEC_PROGRAM(${MYSQL_CONFIG_EXECUTABLE}
            ARGS ${MYSQL_CONFIG_ARGS_LIBS}
            OUTPUT_VARIABLE MYSQL_CONFIG_LIBS)
    MESSAGE(STATUS "${MYSQL_CONFIG_LIBS}")

    EXEC_PROGRAM(${MYSQL_CONFIG_EXECUTABLE}
            ARGS ${MYSQL_CONFIG_ARGS_CXFLAGS}
            OUTPUT_VARIABLE MYSQL_CONFIG_CXFLAGS)
    MESSAGE(STATUS "${MYSQL_CONFIG_CXFLAGS}")
ENDIF (WIN32)

FIND_PATH(DATABASE_PROJ_SOURCE_DIR database-config.h.in
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}src/database/
        ${PROJECT_SOURCE_DIR}/src/database/
        ${PROJECT_SOURCE_DIR}/../src/database/
        ${PROJECT_SOURCE_DIR}/../../src/database/
        ${PROJECT_SOURCE_DIR}../../../src/database/
        ${PROJECT_SOURCE_DIR}../../../../src/database/
        )

MARK_AS_ADVANCED(
        MYSQL_LIBRARY
        MYSQL_INCLUDE_DIR
)


## IF PARAMETERS ARE DEFINED, USE THEM
IF (MYSQL_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ENDIF (MYSQL_INCLUDE_DIR)

###IF(WXWINDOWS_LINK_DIRECTORIES)
####  LINK_DIRECTORIES(${WXWINDOWS_LINK_DIRECTORIES})
####ENDIF(WXWINDOWS_LINK_DIRECTORIES)
IF (MYSQL_CONFIG_LIBS)
    LINK_LIBRARIES(${MYSQL_CONFIG_LIBS})
ENDIF (MYSQL_CONFIG_LIBS)

IF (MYSQL_CONFIG_CXFLAGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MYSQL_CONFIG_CXFLAGS}")
ENDIF (MYSQL_CONFIG_CXFLAGS)
  


  
  
