# CMAKE BUILD FILE FOR INSTALLING GIS LIB (GDAL / GEOS)
# UNDER MAC : AKA COPYING LIBS INTO BUNDLE AND CHANGING
# PATH TO TAKE LOCAL LIBS
# (c) LUCIEN SCHREIBER CREALP 2008

###################################################
# DON'T FORGET TO DEFINE PROGNAME IN CMAKELISTS.TXT
# AND CHANGE GEOS_VERSION AND GEOS_C_VERSION
# FOR NEW LIB VERSION...
###################################################

#definitions
SET (LIBNAME "Library")
SET (BUNDLEPATH "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROGNAME}.app")
SET (BUNDLELIBPATH "${BUNDLEPATH}/Contents/${LIBNAME}")
SET (GEOS_VERSION "3.1.1")
SET (GEOS_C_VERSION "1.6.0")




# this code will only be called if apple
IF (APPLE)

	#GET_TARGET_PROPERTY (PROG_LOCATION ${PROGNAME} LOCATION)
	
	MESSAGE ("DBG : Library will be copied into : ${BUNDLELIBPATH}")
	
	#create the "library" path
	EXEC_PROGRAM( "mkdir -p  ${BUNDLELIBPATH}")
	
	
	IF (SEARCH_GEOS)
		# copy all GEOS libs to the bundle 
		EXEC_PROGRAM( "cp ${SEARCH_GIS_LIB_PATH}/lib/libgeos-${GEOS_VERSION}.dylib ${BUNDLELIBPATH}")
		EXEC_PROGRAM( "cp ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.${GEOS_C_VERSION}.dylib ${BUNDLELIBPATH}")
		
		# change install_name from GEOS libs into the bundle 
		EXEC_PROGRAM("install_name_tool -id  @executable_path/../${LIBNAME}/libgeos-${GEOS_VERSION}.dylib ${BUNDLELIBPATH}/libgeos-${GEOS_VERSION}.dylib") 
		EXEC_PROGRAM("install_name_tool -id  @executable_path/../${LIBNAME}/libgeos_c.${GEOS_C_VERSION}.dylib ${BUNDLELIBPATH}/libgeos_c.${GEOS_C_VERSION}.dylib") 
		EXEC_PROGRAM("install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos-${GEOS_VERSION}.dylib @executable_path/../${LIBNAME}/libgeos-${GEOS_VERSION}.dylib ${BUNDLELIBPATH}/libgeos_c.${GEOS_C_VERSION}.dylib") 
		   
		# we have to change the programm (but only after build...)
		#GET_TARGET_PROPERTY (PROG_LOCATION ${PROGNAME} LOCATION)
		ADD_CUSTOM_COMMAND( TARGET ${PROGNAME} POST_BUILD
       COMMAND install_name_tool  ARGS -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.1.dylib @executable_path/../${LIBNAME}/libgeos_c.${GEOS_C_VERSION}.dylib ${BUNDLEPATH}/Contents/MacOS/${PROGNAME}
       COMMENT "Updating program GEOS libs")
	
	
	ENDIF(SEARCH_GEOS)
	
	
	IF(SEARCH_GDAL)
	# copy GDAL lib to the bundle 
	EXEC_PROGRAM( "cp ${SEARCH_GIS_LIB_PATH}/lib/libgdal.1.dylib ${BUNDLELIBPATH}")

	# change install_name from GEOS libs into the bundle 
	EXEC_PROGRAM("install_name_tool -id  @executable_path/../${LIBNAME}/libgdal.1.dylib ${BUNDLELIBPATH}/libgdal.1.dylib") 
	EXEC_PROGRAM("install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos-${GEOS_VERSION}.dylib @executable_path/../${LIBNAME}/libgeos-${GEOS_VERSION}.dylib ${BUNDLELIBPATH}/libgdal.1.dylib")
	EXEC_PROGRAM("install_name_tool -change ${SEARCH_GIS_LIB_PATH}/lib/libgeos_c.1.dylib @executable_path/../${LIBNAME}/libgeos_c.${GEOS_C_VERSION}.dylib ${BUNDLELIBPATH}/libgdal.1.dylib") 

	# we have to change the programm for GDAL (but only after build...)
	ADD_CUSTOM_COMMAND( TARGET ${PROGNAME} POST_BUILD
    COMMAND install_name_tool  ARGS -change ${SEARCH_GIS_LIB_PATH}/lib/libgdal.1.dylib @executable_path/../${LIBNAME}/libgdal.1.dylib ${BUNDLEPATH}/Contents/MacOS/${PROGNAME}
    COMMENT "Updating program GDAL libs")
	
	
	
	
	ENDIF (SEARCH_GDAL)

ENDIF (APPLE)