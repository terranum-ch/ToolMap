# - Find GIS LIB is used to
# Find the  GEOS AND / OR THE GDAL LIB
# Created by Lucien Schreiber 
#
#  GDAL_INCLUDE_DIR - where to find GDAL headers, etc.
#  GDAL_LIBRARIES   - List of libraries when using GDAL.
#  GDAL_FOUND       - True if GDAL found.
#  GEOS_INCLUDE_DIR - where to find GEOS headers, etc.
#  GEOS_LIBRARIES   - List of libraries when using GEOS
#  GEOS_FOUND       - True if GEOS found.


#define what we search

SET(SEARCH_GDAL CACHE BOOL "Sould we search for GDAL ?")
SET(SEARCH_GEOS CACHE BOOL "Sould we search for GEOS ?")
SET(SEARCH_GIS_LIB_PATH CACHE PATH "Path to a common folder containing GDAL, GEOS, PROJ and SQLITE")
SET(SEARCH_GDAL_PATH CACHE PATH "Path to GDAL base")
SET(SEARCH_GEOS_PATH CACHE PATH "Path to GEOS base")
SET(SEARCH_PROJ_PATH CACHE PATH "Path to PROJ base")
SET(SEARCH_SQLITE_PATH CACHE PATH "Path to SQLITE base")

# if specific path aren't defined, use generic GIS Path
if (NOT SEARCH_GDAL_PATH AND SEARCH_GIS_LIB_PATH)
    SET(SEARCH_GDAL_PATH ${SEARCH_GIS_LIB_PATH})
endif (NOT SEARCH_GDAL_PATH AND SEARCH_GIS_LIB_PATH)

if (NOT SEARCH_GEOS_PATH AND SEARCH_GIS_LIB_PATH)
    SET(SEARCH_GEOS_PATH ${SEARCH_GIS_LIB_PATH})
endif (NOT SEARCH_GEOS_PATH AND SEARCH_GIS_LIB_PATH)

if (NOT SEARCH_PROJ_PATH AND SEARCH_GIS_LIB_PATH)
    SET(SEARCH_PROJ_PATH ${SEARCH_GIS_LIB_PATH})
endif (NOT SEARCH_PROJ_PATH AND SEARCH_GIS_LIB_PATH)

# we search for geos lib
IF (SEARCH_GEOS)

    # if windows and search geos
    # using C API of GEOS:
    IF (WIN32)
        FIND_PATH(GEOS_C_INCLUDE_DIR geos_c.h
                HINTS
                ${SEARCH_GEOS_PATH}/capi
                NO_DEFAULT_PATH)


        FIND_PATH(GEOS_INCLUDE_DIR geos.h
                HINTS
                ${SEARCH_GEOS_PATH}/source/headers
                ${SEARCH_GEOS_PATH}/include
                NO_DEFAULT_PATH)

        FIND_LIBRARY(GEOS_LIBRARIES
                geos_c_i NAMES geos_c
                HINTS ${SEARCH_GEOS_PATH}/source
                ${SEARCH_GEOS_PATH}/src
                ${SEARCH_GEOS_PATH}
                ${SEARCH_GEOS_PATH}/lib/Release
                NO_DEFAULT_PATH)

        # if unix / mac and search geos
    ELSE (WIN32)

        IF (SEARCH_GEOS_PATH)
            FIND_PATH(GEOS_INCLUDE_DIR geos_c.h
                    HINTS ${SEARCH_GEOS_PATH}/include
                    ${SEARCH_GEOS_PATH}
                    NO_DEFAULT_PATH)

            FIND_LIBRARY(GEOS_LIBRARIES
                    geos_c
                    HINTS ${SEARCH_GEOS_PATH}/lib ${SEARCH_GEOS_PATH} NO_DEFAULT_PATH)
        ELSE (SEARCH_GEOS_PATH)
            MESSAGE(STATUS "Searching GEOS into standard paths")
            FIND_PATH(GEOS_INCLUDE_DIR geos_c.h
                    HINTS ${SEARCH_GEOS_PATH}/include
                    ${SEARCH_GEOS_PATH})

            FIND_LIBRARY(GEOS_LIBRARIES
                    geos_c
                    HINTS ${SEARCH_GEOS_PATH}/lib ${SEARCH_GEOS_PATH})
        ENDIF (SEARCH_GEOS_PATH)


    ENDIF (WIN32)

    MARK_AS_ADVANCED(
            GEOS_INCLUDE_DIR
            GEOS_LIBRARIES
    )

    ## IF GEOS PARAMETERS ARE DEFINED, USE THEM
    IF (GEOS_INCLUDE_DIR)
        INCLUDE_DIRECTORIES(${GEOS_INCLUDE_DIR})
        MESSAGE(STATUS "GEOS include is ${GEOS_INCLUDE_DIR}")
    ENDIF (GEOS_INCLUDE_DIR)

    IF (GEOS_C_INCLUDE_DIR)
        MESSAGE(STATUS "GEOS C INCLUDE dir: ${GEOS_C_INCLUDE_DIR}")
        INCLUDE_DIRECTORIES(${GEOS_C_INCLUDE_DIR})
    ENDIF (GEOS_C_INCLUDE_DIR)


    IF (GEOS_LIBRARIES)
        LINK_LIBRARIES(${GEOS_LIBRARIES})
        MESSAGE(STATUS "GEOS lib is ${GEOS_LIBRARIES}")
    ENDIF (GEOS_LIBRARIES)

ENDIF (SEARCH_GEOS)
#end search for geos


#search for GDAL lib
IF (SEARCH_GDAL)
    # if windows
    IF (WIN32)

        FIND_PATH(GDAL_INCLUDE_DIR gdal.h
                HINTS ${SEARCH_GDAL_PATH}/include
                ${SEARCH_GDAL_PATH}
                NO_DEFAULT_PATH)

        FIND_LIBRARY(GDAL_LIBRARIES
                gdal_i
                HINTS ${SEARCH_GDAL_PATH}/lib
                ${SEARCH_GDAL_PATH}
                NO_DEFAULT_PATH)

        # if mac, use the dynamic library
    ELSEIF (APPLE)
        IF (SEARCH_GDAL_PATH)
            FIND_PATH(GDAL_INCLUDE_DIR gdal.h
                    PATHS ${SEARCH_GDAL_PATH}/include
                    ${SEARCH_GDAL_PATH}
                    PATH_SUFFIXES gdal
                    NO_DEFAULT_PATH)

            FIND_LIBRARY(GDAL_LIBRARIES
                    gdal NAMES gdal1 gdal1.6.0 gdal1.7.0 gdal1.8.0 gdal1.9.0
                    PATHS ${SEARCH_GDAL_PATH}/lib
                    ${SEARCH_GDAL_PATH} NO_DEFAULT_PATH)

        ELSE (SEARCH_GDAL_PATH)
            MESSAGE(STATUS "Searching GDAL on standard PATHS")
            FIND_PATH(GDAL_INCLUDE_DIR gdal.h
                    HINTS ${SEARCH_GDAL_PATH}/include
                    ${SEARCH_GDAL_PATH}
                    PATH_SUFFIXES gdal)

            FIND_LIBRARY(GDAL_LIBRARIES
                    gdal NAMES gdal1 gdal1.6.0 gdal1.7.0 gdal1.8.0 gdal1.9.0)
        ENDIF (SEARCH_GDAL_PATH)

        # if linux, use the static library
    ELSE ()
        IF (SEARCH_GDAL_PATH)
            FIND_PATH(GDAL_INCLUDE_DIR gdal.h
                    PATHS ${SEARCH_GDAL_PATH}/include
                    ${SEARCH_GDAL_PATH}
                    PATH_SUFFIXES gdal
                    NO_DEFAULT_PATH)

            FIND_LIBRARY(GDAL_LIBRARIES
                    NAMES libgdal.a gdal1 gdal1.6.0 gdal1.7.0 gdal1.8.0 gdal1.9.0
                    PATHS ${SEARCH_GDAL_PATH}/lib
                    ${SEARCH_GDAL_PATH} NO_DEFAULT_PATH)

            FIND_PROGRAM(GDAL_CONFIG gdal-config
                    ${SEARCH_GDAL_PATH}/bin/
                    NO_DEFAULT_PATH)

        ELSE (SEARCH_GDAL_PATH)
            MESSAGE(STATUS "Searching GDAL on standard PATHS")
            FIND_PATH(GDAL_INCLUDE_DIR gdal.h
                    HINTS ${SEARCH_GDAL_PATH}/include
                    ${SEARCH_GDAL_PATH}
                    PATH_SUFFIXES gdal)

            FIND_LIBRARY(GDAL_LIBRARIES
                    NAMES libgdal.a gdal1 gdal1.6.0 gdal1.7.0 gdal1.8.0 gdal1.9.0)

            FIND_PROGRAM(GDAL_CONFIG gdal-config)

        ENDIF (SEARCH_GDAL_PATH)

        IF (GDAL_CONFIG)
            EXEC_PROGRAM(${GDAL_CONFIG} ARGS --dep-libs OUTPUT_VARIABLE GDAL_DEP_LIBS)
            list(APPEND GDAL_LIBRARIES ${GDAL_DEP_LIBS})
        ENDIF (GDAL_CONFIG)

    ENDIF (WIN32)


    MARK_AS_ADVANCED(
            GDAL_INCLUDE_DIR
            GDAL_LIBRARIES
    )


    ## IF GDAL PARAMETERS ARE DEFINED, USE THEM
    IF (GDAL_INCLUDE_DIR)
        INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})
    ENDIF (GDAL_INCLUDE_DIR)

    IF (GDAL_LIBRARIES)
        LINK_LIBRARIES(${GDAL_LIBRARIES})
    ENDIF (GDAL_LIBRARIES)

    #debug message
    MESSAGE(STATUS "GDAL lib is ${GDAL_LIBRARIES}")
    MESSAGE(STATUS "GDAL include is ${GDAL_INCLUDE_DIR}")

ENDIF (SEARCH_GDAL)
#end search for GDAL lib


# If Linux, use the static PROJ library
IF (UNIX)
    IF (SEARCH_PROJ_PATH)
        FIND_PATH(PROJ_INCLUDE_DIR proj.h proj_api.h
                PATHS ${SEARCH_PROJ_PATH}
                PATH_SUFFIXES include
                NO_DEFAULT_PATH)

        FIND_LIBRARY(PROJ_LIBRARIES
                NAMES libproj.a libproj.so libproj proj
                PATHS ${SEARCH_PROJ_PATH}
                PATH_SUFFIXES lib lib64
                NO_DEFAULT_PATH)

    ELSE (SEARCH_PROJ_PATH)
        MESSAGE(STATUS "Searching PROJ on standard PATHS")
        FIND_PATH(PROJ_INCLUDE_DIR proj.h proj_api.h
                PATH_SUFFIXES include)

        FIND_LIBRARY(PROJ_LIBRARIES
                NAMES libproj.a libproj.so libproj proj
                PATH_SUFFIXES lib lib64)

    ENDIF (SEARCH_PROJ_PATH)

    ## IF PROJ PARAMETERS ARE DEFINED, USE THEM
    IF (PROJ_INCLUDE_DIR)
        INCLUDE_DIRECTORIES(${PROJ_INCLUDE_DIR})
    ENDIF (PROJ_INCLUDE_DIR)

    IF (PROJ_LIBRARIES)
        LINK_LIBRARIES(${PROJ_LIBRARIES})
    ENDIF (PROJ_LIBRARIES)

    #debug message
    MESSAGE(STATUS "PROJ lib is ${PROJ_LIBRARIES}")
    MESSAGE(STATUS "PROJ include is ${PROJ_INCLUDE_DIR}")

ENDIF (UNIX)

#end search for PROJ lib
