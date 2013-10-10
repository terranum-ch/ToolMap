# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIR - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curl found.

SET (SEARCH_CURL_LIB_PATH CACHE PATH "Path to the CURL libs (empty for unix systems)")

# if windows
IF (WIN32)
  FIND_PATH(CURL_INCLUDE_DIR curl/curl.h ${SEARCH_CURL_LIB_PATH}/include ${SEARCH_CURL_LIB_PATH})

  SET(CURL_LIBRARIES ${CURL_LIBRARIES}
    debug "${CURL_INCLUDE_DIR}/../lib/DLL-Debug/libcurld_imp.lib"   
    optimized "${CURL_INCLUDE_DIR}/../lib/DLL-Release/libcurl_imp.lib")

ELSE (WIN32)
  FIND_PATH(CURL_INCLUDE_DIR curl/curl.h
    ${SEARCH_CURL_LIB_PATH}/include
    ${SEARCH_CURL_LIB_PATH})

  FIND_LIBRARY(CURL_LIBRARIES curl PATH ${SEARCH_CURL_LIB_PATH}/lib	${SEARCH_CURL_LIB_PATH})
ENDIF (WIN32)

MARK_AS_ADVANCED(
  CURL_INCLUDE_DIR
  CURL_LIBRARIES
  )

IF(CURL_INCLUDE_DIR)
  INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIR}")
  MESSAGE (STATUS "CURL include is ${CURL_INCLUDE_DIR}")
ENDIF(CURL_INCLUDE_DIR)

IF(CURL_LIBRARIES)
  LINK_LIBRARIES(${CURL_LIBRARIES})
  MESSAGE (STATUS "CURL lib is ${CURL_LIBRARIES}")
ENDIF(CURL_LIBRARIES)
