# - Findunittestlib is used to
# Find the  UNIT TESTING LIB
# Created by Lucien Schreiber 
#
#  UTEST_INCLUDE_DIR - where to find GDAL headers, etc.
#  UTEST_LIBRARIES   - List of libraries when using GDAL.
#  UTEST_FOUND       - True if GDAL found.


SET (UTEST_LIB_PATH CACHE PATH "Path to the GIS libs")

	
	# if windows and search unittest
	IF (WIN32)
		FIND_PATH(UTEST_INCLUDE_DIR TestSuite.h
  				${UTEST_LIB_PATH}/include/cppunit
  				${UTEST_LIB_PATH}
  				NO_DEFAULT_PATH)
  		 		  		
  		FIND_LIBRARY(UTEST_LIBRARIES
	  		cppunit
	  		PATH ${UTEST_LIB_PATH}/lib	${UTEST_LIB_PATH} NO_DEFAULT_PATH)
	  			
	
	# if unix / mac and search UTEST
	ELSE (WIN32)
		
		FIND_PATH(UTEST_INCLUDE_DIR TestSuite.h
  				${UTEST_LIB_PATH}/include/cppunit/
  				${UTEST_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
  		FIND_LIBRARY(UTEST_LIBRARIES
	  		cppunit
	  		PATH ${UTEST_LIB_PATH}/lib	${UTEST_LIB_PATH} NO_DEFAULT_PATH)
  		  		  			
	ENDIF (WIN32)
	
	
	MARK_AS_ADVANCED(
				UTEST_INCLUDE_DIR
				UTEST_LIBRARIES
					)
  
  
  ## IF GEOS PARAMETERS ARE DEFINED, USE THEM
   IF(UTEST_INCLUDE_DIR)
    INCLUDE_DIRECTORIES("${UTEST_INCLUDE_DIR}/..")
    MESSAGE ("DBG : Unit testing include is ${UTEST_INCLUDE_DIR}/..")
  ENDIF(UTEST_INCLUDE_DIR)
 
 
  IF(UTEST_LIBRARIES)
    LINK_LIBRARIES(${UTEST_LIBRARIES})
    MESSAGE ("DBG : Unit testing lib is ${UTEST_LIBRARIES}")
    SET (UTEST_FOUND TRUE)
  ENDIF(UTEST_LIBRARIES)
	
	#debug message
	#MESSAGE ("DBG : GEOS lis is ${GEOS_LIBRARIES}")
	#MESSAGE ("DBG : GEOS include is ${GEOS_INCLUDE_DIR}")




