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

SET (SEARCH_GDAL CACHE BOOL "Sould we search for GDAL ?" )
SET (SEARCH_GEOS CACHE BOOL "Sould we search for GEOS ?" )
SET (SEARCH_GIS_LIB_PATH CACHE PATH "Path to the GIS libs")

# we search for geos lib
IF (SEARCH_GEOS)
	
	# if windows and search geos
	IF (WIN32)
		SET (SEARCH_GEOS_LIB_PATH CACHE PATH "Path to the GEOS libs if not with GDAL")
		
		FIND_PATH(GEOS_INCLUDE_DIR geos_c.h
  				${SEARCH_GEOS_LIB_PATH}/capi
  				${SEARCH_GEOS_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
  		FIND_LIBRARY(GEOS_LIBRARIES
	  		geos_c_i
	  		PATH ${SEARCH_GEOS_LIB_PATH}/source	${SEARCH_GEOS_LIB_PATH} NO_DEFAULT_PATH)
	
	# if unix / mac and search geos
	ELSE (WIN32)
		
		FIND_PATH(GEOS_INCLUDE_DIR geos_c.h
  				${SEARCH_GIS_LIB_PATH}/include
  				${SEARCH_GIS_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
  		FIND_LIBRARY(GEOS_LIBRARIES
	  		geos_c
	  		PATH ${SEARCH_GIS_LIB_PATH}/lib	${SEARCH_GIS_LIB_PATH} NO_DEFAULT_PATH)
  		  		  			
	ENDIF (WIN32)
	
	
	MARK_AS_ADVANCED(
				GEOS_INCLUDE_DIR
				GEOS_LIBRARIES
					)
  
  
  ## IF GEOS PARAMETERS ARE DEFINED, USE THEM
   IF(GEOS_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${GEOS_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${SEARCH_GEOS_LIB_PATH}/source/headers)
    MESSAGE ("DBG : GEOS include is ${GEOS_INCLUDE_DIR}")
  ENDIF(GEOS_INCLUDE_DIR)
 
 
  IF(GEOS_LIBRARIES)
    LINK_LIBRARIES(${GEOS_LIBRARIES})
    MESSAGE ("DBG : GEOS lis is ${GEOS_LIBRARIES}")
  ENDIF(GEOS_LIBRARIES)
	
	#debug message
	#MESSAGE ("DBG : GEOS lis is ${GEOS_LIBRARIES}")
	#MESSAGE ("DBG : GEOS include is ${GEOS_INCLUDE_DIR}")

ENDIF (SEARCH_GEOS)
#end search for geos










#search for GDAL lib
IF (SEARCH_GDAL)
		# if windows and search geos
	IF (WIN32)
		
		FIND_PATH(GDAL_INCLUDE_DIR gdal.h
  				${SEARCH_GIS_LIB_PATH}/include
  				${SEARCH_GIS_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
  		FIND_LIBRARY(GDAL_LIBRARIES
	  		gdal_i
	  		PATH ${SEARCH_GIS_LIB_PATH}/lib
  			${SEARCH_GIS_LIB_PATH}
  			NO_DEFAULT_PATH)
	
	# if unix / mac and search geos
	ELSE (WIN32)
		
		FIND_PATH(GDAL_INCLUDE_DIR gdal.h
  				${SEARCH_GIS_LIB_PATH}/include
  				${SEARCH_GIS_LIB_PATH}
  				NO_DEFAULT_PATH)
  		
  		FIND_LIBRARY(GDAL_LIBRARIES
	  		gdal
	  		PATH ${SEARCH_GIS_LIB_PATH}/lib
  			${SEARCH_GIS_LIB_PATH}
  			NO_DEFAULT_PATH)
  		  		  			
	ENDIF (WIN32)
	
	
	MARK_AS_ADVANCED(
				GDAL_INCLUDE_DIR
				GDAL_LIBRARIES
					)
  
  
  ## IF GDAL PARAMETERS ARE DEFINED, USE THEM
   IF(GDAL_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})
  ENDIF(GDAL_INCLUDE_DIR)
 
 
  IF(GDAL_LIBRARIES)
    LINK_LIBRARIES(${GDAL_LIBRARIES})
  ENDIF(GDAL_LIBRARIES)
	
	#debug message
	MESSAGE ("DBG : GDAL lis is ${GDAL_LIBRARIES}")
	MESSAGE ("DBG : GDAL include is ${GDAL_INCLUDE_DIR}")


ENDIF (SEARCH_GDAL)
#end search for GDAL lib

