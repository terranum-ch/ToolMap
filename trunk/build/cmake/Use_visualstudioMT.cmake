# CMAKE BUILD FILE FOR USING
# MT Library (compatibility with < WinXP
# (c) LUCIEN SCHREIBER CREALP 2007
# partial work from mysql cmakefile 

SET (USE_MT_LIBRARY CACHE BOOL "Should we use the MT option for building, use it if lib were build with the MT option")

IF (USE_MT_LIBRARY)
  	# replace /MDd with /MTd
    STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELEASE          ${CMAKE_C_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_C_FLAGS_RELWITHDEBINFO   ${CMAKE_C_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG            ${CMAKE_C_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG_INIT       ${CMAKE_C_FLAGS_DEBUG_INIT})

    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELEASE        ${CMAKE_CXX_FLAGS_RELEASE})
    STRING(REPLACE "/MD"  "/MT"  CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG          ${CMAKE_CXX_FLAGS_DEBUG})
    STRING(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG_INIT     ${CMAKE_CXX_FLAGS_DEBUG_INIT})

    # Disable automatic manifest generation.
    STRING(REPLACE "/MANIFEST" "/MANIFEST:NO" CMAKE_EXE_LINKER_FLAGS 
    	   ${CMAKE_EXE_LINKER_FLAGS})
    # Explicitly disable it since it is the default for newer versions of VS
    STRING(REGEX MATCH "MANIFEST:NO" tmp_manifest ${CMAKE_EXE_LINKER_FLAGS})
    IF(NOT tmp_manifest)
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    ENDIF(NOT tmp_manifest)

ENDIF (USE_MT_LIBRARY)