# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIRS - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curl found.

SET(SEARCH_CURL TRUE CACHE BOOL "Sould we search for Curl ?")
SET(SEARCH_CURL_PATH CACHE PATH "Path to the CURL libs (empty for unix systems)")

IF (SEARCH_CURL)
    # if windows
    IF (WIN32)
        FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
                PATHS
                ${SEARCH_CURL_PATH}/include
                ${SEARCH_CURL_PATH}
                NO_DEFAULT_PATH)

        FIND_LIBRARY(CURL_LIBRARY_DEBUG
                NAMES libcurld
                PATHS
                ${SEARCH_CURL_PATH}/lib
                ${SEARCH_CURL_PATH}/lib/DLL-Debug/
                DOC "LibCURL debug library"
                NO_DEFAULT_PATH)

        FIND_LIBRARY(CURL_LIBRARY_RELEASE
                NAMES libcurl
                PATHS
                ${SEARCH_CURL_PATH}/lib
                ${SEARCH_CURL_PATH}/lib/DLL-Release/
                DOC "LibCURL library"
                NO_DEFAULT_PATH)

        # SET(CURL_LIBRARIES ${CURL_LIBRARIES}
        #     debug ${CURL_LIBRARY_DEBUG}
        #     optimized ${CURL_LIBRARY_RELEASE})
        SET(CURL_LIBRARIES ${CURL_LIBRARY_RELEASE})

        # if unix / mac and search geos
    ELSE (WIN32)

        # empty curl path
        IF (SEARCH_CURL_PATH)
            FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
                    HINTS ${SEARCH_CURL_PATH}/include
                    ${SEARCH_CURL_PATH} NO_DEFAULT_PATH)
            # search for libs
            FIND_LIBRARY(CURL_LIBRARIES
                    curl
                    HINTS ${SEARCH_CURL_PATH}/lib ${SEARCH_CURL_PATH} NO_DEFAULT_PATH)
        ELSE (SEARCH_CURL_PATH)
            FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
                    ${SEARCH_CURL_PATH}/include
                    ${SEARCH_CURL_PATH})

            FIND_LIBRARY(CURL_LIBRARIES
                    curl
                    PATH ${SEARCH_CURL_PATH}/lib ${SEARCH_CURL_PATH})

        ENDIF (SEARCH_CURL_PATH)

    ENDIF (WIN32)


    MARK_AS_ADVANCED(
            CURL_INCLUDE_DIRS
            CURL_LIBRARIES
    )


    ## IF GEOS PARAMETERS ARE DEFINED, USE THEM
    IF (CURL_INCLUDE_DIRS)
        INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIRS}")
        MESSAGE(STATUS "CURL include is ${CURL_INCLUDE_DIRS}")
    ENDIF (CURL_INCLUDE_DIRS)

    IF (CURL_LIBRARIES)
        LINK_LIBRARIES(${CURL_LIBRARIES})
        MESSAGE(STATUS "CURL lib is ${CURL_LIBRARIES}")
    ENDIF (CURL_LIBRARIES)

ENDIF (SEARCH_CURL)
