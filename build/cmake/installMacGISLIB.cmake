# CMAKE BUILD FILE FOR INSTALLING GIS LIB (GDAL / GEOS)
# UNDER MAC : AKA COPYING LIBS INTO BUNDLE AND CHANGING
# PATH TO TAKE LOCAL LIBS
# (c) LUCIEN SCHREIBER TERRANUM 2017
###################################################
# Run make install to create the ToolMap.App with
# all the librairies

# this code will only be called if apple
IF (APPLE)
    # change cmake_install_prefix from /usr/local to the binary directiory
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

    # Use BundleUtilities to copy all dynlib for the application to work.
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.app \"\" \"\")
    " COMPONENT Runtime)
ENDIF (APPLE)
