# CMAKE BUILD FILE FOR INSTALLING LIBTIFF 
# UNDER MAC : AKA COPYING LIBS INTO BUNDLE AND CHANGING
# PATH TO TAKE LOCAL LIBS
# (c) LUCIEN SCHREIBER CREALP 2008

###################################################
# DON'T FORGET TO DEFINE PROGNAME IN CMAKELISTS.TXT
####################################################

#definitions
SET (LIBNAME "Library")
SET (BUNDLEPATH "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROGNAME}.app")
SET (BUNDLELIBPATH "${BUNDLEPATH}/Contents/${LIBNAME}")
SET (LIBRARY_LIBTIFF_NAME "libtiff.3.dylib")


# this code will only be called if apple
IF (APPLE)

	SET(INSTALL_LIBTIFF_INTO_BUNDLE CACHE BOOL "Install the libtiff into the bundle (Mac PPC only ?)")
	IF (INSTALL_LIBTIFF_INTO_BUNDLE)


	#FIND THE LIBRARY
	FIND_LIBRARY(LIBTIFF_MAC_LIBRARIES	tiff.3)
		IF (LIBTIFF_MAC_LIBRARIES)
	
			MESSAGE ("DBG : Library will be copied into : ${BUNDLELIBPATH}")
			MESSAGE ("DBG : libtiff found : ${LIBTIFF_MAC_LIBRARIES}")
			#create the "library" path
			EXEC_PROGRAM( "mkdir -p  ${BUNDLELIBPATH}")
			
			# copy LIBTIFF libs to the bundle 
			EXEC_PROGRAM( "cp ${LIBTIFF_MAC_LIBRARIES} ${BUNDLELIBPATH}")
			
			# change install_name from libtiff libs into the bundle 
			EXEC_PROGRAM("install_name_tool -id  @executable_path/../${LIBNAME}/${LIBRARY_LIBTIFF_NAME} ${BUNDLELIBPATH}/${LIBRARY_LIBTIFF_NAME}") 
		
			# we have to change the programm (but only after build...)
			ADD_CUSTOM_COMMAND( TARGET ${PROGNAME} POST_BUILD
       		COMMAND install_name_tool  ARGS -change ${LIBTIFF_MAC_LIBRARIES} @executable_path/../${LIBNAME}/${LIBRARY_LIBTIFF_NAME} ${BUNDLEPATH}/Contents/MacOS/${PROGNAME}
       		COMMENT "Updating program LibTiff libs")
			
			
		ENDIF (LIBTIFF_MAC_LIBRARIES)
	

	ENDIF (INSTALL_LIBTIFF_INTO_BUNDLE)

ENDIF (APPLE)