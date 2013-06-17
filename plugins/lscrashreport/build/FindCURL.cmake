# - Find curl
# Find the native CURL headers and libraries.
#
#  CURL_INCLUDE_DIRS - where to find curl/curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curl found.


SET (SEARCH_CURL TRUE CACHE BOOL "Sould we search for Curl ?" )
SET (SEARCH_CURL_LIB_PATH CACHE PATH "Path to the CURL libs (empty for unix systems)")

IF (SEARCH_CURL)

	# if windows
	IF (WIN32)
		FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
  				${SEARCH_CURL_LIB_PATH}/include 
  				${SEARCH_CURL_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
   		
		SET(CURL_LIBRARIES ${CURL_LIBRARIES}
		           debug "${SEARCH_CURL_LIB_PATH}/lib/DLL-Debug/libcurld_imp.lib"   optimized "${SEARCH_CURL_LIB_PATH}/lib/DLL-Release/libcurl_imp.lib")

	
	# if unix / mac and search geos
	ELSE (WIN32)
		
		FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h
  				${SEARCH_CURL_LIB_PATH}/include
  				${SEARCH_CURL_LIB_PATH})
  		
  		FIND_LIBRARY(CURL_LIBRARIES
	  		curl
	  		PATH ${SEARCH_CURL_LIB_PATH}/lib	${SEARCH_CURL_LIB_PATH})
  		  		  			
	ENDIF (WIN32)
	
	
	MARK_AS_ADVANCED(
				CURL_INCLUDE_DIRS
				CURL_LIBRARIES
					)
  
  
   IF(CURL_INCLUDE_DIRS)
    INCLUDE_DIRECTORIES("${CURL_INCLUDE_DIRS}")
    MESSAGE ("DBG : CURL include is ${CURL_INCLUDE_DIRS}")
  ENDIF(CURL_INCLUDE_DIRS)
 
  IF(CURL_LIBRARIES)
    LINK_LIBRARIES(${CURL_LIBRARIES})
    MESSAGE ("DBG : CURL lib is ${CURL_LIBRARIES}")
  ENDIF(CURL_LIBRARIES)
ENDIF(SEARCH_CURL)
