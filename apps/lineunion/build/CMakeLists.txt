# CMAKE BUILD FILE FOR TOOLMERGE 
# (c) LUCIEN SCHREIBER CREALP 2012
cmake_minimum_required(VERSION 2.6)

PROJECT ("LineUnion")

#search all source files
FILE (GLOB_RECURSE SRC_FILES "../src/*.cpp")
FILE (GLOB_RECURSE HEAD_FILES "../src/*.h")
SET (SRC_FILES ${SRC_FILES}
  "../../../src/database/database.cpp"
  "../../../src/database/databaseresult.cpp")
SET (HEAD_FILES ${HEAD_FILES} 
  "../../../src/database/database.h"
  "../../../src/database/databaseresult.h")
MESSAGE(${SRC_FILES})
SOURCE_GROUP (source FILES ${SRC_FILES})
SOURCE_GROUP (header FILES ${HEAD_FILES})

# WXWIDGETS
# DEFINE ALL INCLUDES AND SO ON
INCLUDE ("../../../build/cmake/FindwxWidgets.cmake")

#MYSQL EMBEDDED
INCLUDE ("../../../build/cmake/findMySQLEmbedded.cmake")

#GEOS - GDAL libs
INCLUDE ("../../../build/cmake/findGISLib.cmake")

INCLUDE_DIRECTORIES("../../../src/database/")

# LSVERSION COMMPONENT
#INCLUDE("../plugins/lsversion/build/use_lsversion.cmake")

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${HEAD_FILES} ${SRC_FILES})

# SHOULD WE USE THE MT VERSION FOR DEFAULT 
# COMPATIBILITY WITH < WINXP
IF (WIN32)
  INCLUDE ("cmake/Use_visualstudioMT.cmake")
ENDIF (WIN32)

#Copy errmsg.sys to correct path
#INCLUDE ("cmake/useMySQLEmbedded.cmake")





