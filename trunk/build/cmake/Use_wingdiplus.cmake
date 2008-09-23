# CMAKE BUILD FILE FOR USING
# GDIPLUS.dll in case of windows. 
# gdiplus.dll must be provided if intended to target < WinXP
# (c) LUCIEN SCHREIBER CREALP 2008

#################################
# Progname sould be defined for	#
# this program to work			#
# SET (PROGNAME ToolMap2)		#
#################################



# this code will only be called if Windows (32 or 64)
IF (WIN32)
	SET (USE_GDIPLUS_LIBRARY CACHE BOOL "Should we use the GDIPLUS.dll (better drawing)")

	IF (USE_GDIPLUS_LIBRARY)
		MESSAGE ("Using gdiplus.dll for better drawings")
		TARGET_LINK_LIBRARIES (${PROGNAME} "gdiplus")
	ENDIF (USE_GDIPLUS_LIBRARY)


ENDIF (WIN32)
