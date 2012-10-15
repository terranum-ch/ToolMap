# CMAKE BUILD FILE FOR INSTALLING GIS LIB (GDAL / GEOS)
# UNDER MAC : AKA COPYING LIBS INTO BUNDLE AND CHANGING
# PATH TO TAKE LOCAL LIBS
# (c) LUCIEN SCHREIBER CREALP 2008
###################################################
# DON'T FORGET TO 
# CHANGE GEOS_VERSION AND GEOS_C_VERSION
# FOR NEW LIB VERSION...
###################################################

#definitions
SET (LIBNAME "Library")
SET (BUNDLEPATH "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app")
SET (BUNDLELIBPATH "${BUNDLEPATH}/Contents/${LIBNAME}")
SET (GEOS_VERSION "3.3.5")
SET (GEOS_C_VERSION "1")
SET (CURL_VERSION "4")
#SET (PROJ_VERSION "0.6.6")

# this code will only be called if apple
IF (NOT APPLE)
	return()
ENDIF(NOT APPLE)

IF(${SEARCH_GIS_LIB_PATH} EQUAL "" )
	MESSAGE(FATAL_ERROR "SEARCH_GIS_LIB_PATH Undefined! Copying libs impossible")
ENDIF(${SEARCH_GIS_LIB_PATH} EQUAL "" )

MESSAGE("GIS LIB PATH: ${SEARCH_GIS_LIB_PATH}")
### COPY GEOS ###
IF(SEARCH_GEOS)
	ADD_CUSTOM_COMMAND( TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
		 COMMAND mkdir -p 	${BUNDLELIBPATH}
		COMMAND cp ${SEARCH_GIS_LIB_PATH}/lib/libgeos.dylib ${BUNDLELIBPATH}/libgeos.dylib
		COMMAND cp ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.dylib ${BUNDLELIBPATH}/libgeos_c.dylib
		COMMAND install_name_tool -id  @executable_path/../${LIBNAME}/libgeos.dylib ${BUNDLELIBPATH}/libgeos.dylib
		COMMAND install_name_tool -id  @executable_path/../${LIBNAME}/libgeos_c.dylib ${BUNDLELIBPATH}/libgeos_c.dylib
		COMMAND install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos-${GEOS_VERSION}.dylib @executable_path/../${LIBNAME}/libgeos.dylib ${BUNDLELIBPATH}/libgeos_c.dylib       
	
		COMMAND install_name_tool  -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.1.dylib @executable_path/../${LIBNAME}/libgeos_c.dylib ${BUNDLEPATH}/Contents/MacOS/${CMAKE_PROJECT_NAME}
	      COMMENT "Copying and updating program GEOS libs"
	)
ENDIF(SEARCH_GEOS)

### COPY GDAL & PROJ ###
IF(SEARCH_GDAL)
	ADD_CUSTOM_COMMAND( TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
		COMMAND mkdir -p 	${BUNDLELIBPATH}
		COMMAND cp ${SEARCH_GIS_LIB_PATH}/lib/libgdal.dylib ${BUNDLELIBPATH}
		COMMAND cp ${SEARCH_GIS_LIB_PATH}/lib/libproj.dylib ${BUNDLELIBPATH}
		COMMAND install_name_tool -id  @executable_path/../${LIBNAME}/libgdal.dylib ${BUNDLELIBPATH}/libgdal.dylib
		COMMAND install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos-${GEOS_VERSION}.dylib @executable_path/../${LIBNAME}/libgeos.dylib ${BUNDLELIBPATH}/libgdal.dylib
		COMMAND install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.1.dylib @executable_path/../${LIBNAME}/libgeos_c.dylib ${BUNDLELIBPATH}/libgdal.dylib
		COMMAND install_name_tool -id  @executable_path/../${LIBNAME}/libproj.dylib ${BUNDLELIBPATH}/libproj.dylib
		COMMAND install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libproj.0.dylib @executable_path/../${LIBNAME}/libproj.dylib ${BUNDLELIBPATH}/libgdal.dylib
		
		COMMAND install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgdal.1.dylib @executable_path/../${LIBNAME}/libgdal.dylib ${BUNDLEPATH}/Contents/MacOS/${CMAKE_PROJECT_NAME}
	
	    COMMENT "Copying and updating GDAL libs"
	)
ENDIF(SEARCH_GDAL)



IF(SEARCH_CURL_LIB_PATH)
	ADD_CUSTOM_COMMAND( TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
		COMMAND mkdir -p 	${BUNDLELIBPATH}
		COMMAND cp ${SEARCH_CURL_LIB_PATH}/lib/libcurl.dylib ${BUNDLELIBPATH}
		COMMAND install_name_tool -id  @executable_path/../${LIBNAME}/libcurl.dylib ${BUNDLELIBPATH}/libcurl.dylib
		COMMAND install_name_tool -change ${SEARCH_CURL_LIB_PATH}/lib/libcurl.${CURL_VERSION}.dylib @executable_path/../${LIBNAME}/libcurl.dylib ${BUNDLELIBPATH}/libgdal.dylib		
		COMMAND install_name_tool -change ${SEARCH_CURL_LIB_PATH}/lib/libcurl.${CURL_VERSION}.dylib @executable_path/../${LIBNAME}/libcurl.dylib ${BUNDLEPATH}/Contents/MacOS/${CMAKE_PROJECT_NAME}
	COMMENT "Copying and updating CURL libs"
	)
ENDIF(SEARCH_CURL_LIB_PATH)

