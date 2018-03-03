# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIR - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curl found.

SET(SEARCH_CURL_PATH CACHE PATH "Path to the CURL libs (empty for unix systems)")

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

ELSE (WIN32)
    FIND_PATH(CURL_INCLUDE_DIR curl/curl.h
            ${SEARCH_CURL_PATH}/include
            ${SEARCH_CURL_PATH})

    FIND_LIBRARY(CURL_LIBRARIES curl PATH ${SEARCH_CURL_PATH}/lib ${SEARCH_CURL_PATH})
ENDIF (WIN32)

MARK_AS_ADVANCED(
        CURL_INCLUDE_DIR
        CURL_LIBRARIES
)

IF (CURL_INCLUDE_DIR)
    INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIR}")
    MESSAGE(STATUS "CURL include is ${CURL_INCLUDE_DIR}")
ENDIF (CURL_INCLUDE_DIR)

IF (CURL_LIBRARIES)
    LINK_LIBRARIES(${CURL_LIBRARIES})
    MESSAGE(STATUS "CURL lib is ${CURL_LIBRARIES}")
ENDIF (CURL_LIBRARIES)
