# INSTALLER
# use git version for package number
set(VERSION_PATCH ${lsVERSION_SOFT_VERSION})

if (WITH_TOOLBASVIEW)
    add_dependencies(${CMAKE_PROJECT_NAME} ToolBasView)
endif ()

# install ToolMap (application)
install(TARGETS ${CMAKE_PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin)

# install GPL licence file
install(FILES build/cpack/LICENSE.txt DESTINATION .)

if (APPLE)
    message(STATUS "Skipping share directory on OSX")
else()
    install(DIRECTORY ${CMAKE_BINARY_DIR}/share DESTINATION .)
endif()

# search dll to install
if (WIN32)
    install(
            DIRECTORY ${CMAKE_BINARY_DIR}/bin/
            DESTINATION bin
            FILES_MATCHING
            PATTERN "*.dll"
    )
endif ()


if (WIN32)
    # install ToolBasView
    if (WITH_TOOLBASVIEW)
        install(PROGRAMS
                ${EXTERNAL_DIR}/bin/ToolBasView.exe
                DESTINATION bin)
    endif ()

    # install webfiles
    # this part is a partial duplicates of copywebfiles.cmake
    FILE(GLOB WEBFILES "resource/web/*.xml")
    install(FILES ${WEBFILES} DESTINATION share/toolmap)

    # install proj files
    install(DIRECTORY ${PROJ_SHARE_PATH} DESTINATION share)
    install(FILES resources/set-env.bat DESTINATION .)

    # install Microsoft Visual Studio librairies (MSVCP140.DLL, etc.)
    include(InstallRequiredSystemLibraries)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)

elseif (UNIX AND NOT APPLE)

    # install ToolBasView
    if (WITH_TOOLBASVIEW)
        install(PROGRAMS
                ${EXTERNAL_DIR}/bin/ToolBasView
                DESTINATION bin)
    endif ()

elseif (APPLE)

    # install ToolBasView
    if (WITH_TOOLBASVIEW)
        install(DIRECTORY
                ${CMAKE_BINARY_DIR}/ToolBasView-prefix/src/ToolBasView-build/ToolBasView.app
                DESTINATION .
                USE_SOURCE_PERMISSIONS)
    endif ()

endif (WIN32)

# COMMON PROPERTIES
set(CPACK_PACKAGE_VERSION_MAJOR "${CMAKE_PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CMAKE_PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CMAKE_PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ToolMap - open source tool for GIS creation")
set(CPACK_PACKAGE_VENDOR "Terranum")
set(CPACK_STRIP_FILES ON) # tell cpack to strip all debug symbols from all files

# IDENTIFY ARCHITECTURE
set(CPACK_PACKAGE_ARCH "unkown-architecture")
if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
    if (CMAKE_CL_64)
        set(CPACK_PACKAGE_ARCH "win64")
    elseif (MINGW)
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64 bits
            set(CPACK_PACKAGE_ARCH "mingw64")
        elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
            # 32 bits
            set(CPACK_PACKAGE_ARCH "mingw32")
        endif ()
    elseif (WIN32)
        set(CPACK_PACKAGE_ARCH "win32")
    endif ()
    message(STATUS "${CPACK_PACKAGE_ARCH} architecture")
endif (${CMAKE_SYSTEM_NAME} MATCHES Windows)

if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES i686)
        set(CPACK_PACKAGE_ARCH "linux32")
    elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64)
        if (${CMAKE_CXX_FLAGS} MATCHES " -m32 ")
            set(CPACK_PACKAGE_ARCH "linux32")
        else ()
            set(CPACK_PACKAGE_ARCH "linux64")
        endif (${CMAKE_CXX_FLAGS} MATCHES " -m32 ")
    else ()
        set(CPACK_PACKAGE_ARCH "linux")
    endif ()
endif (${CMAKE_SYSTEM_NAME} MATCHES Linux)

if (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    set(CPACK_PACKAGE_ARCH "mac64")
endif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)

set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_ARCH}")

# OS SPECIFIC PROPERTIES
if (APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${CMAKE_PROJECT_NAME}")
    set(CPACK_DMG_FORMAT "UDBZ")
endif (APPLE)

if (WIN32)
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "ToolMap")
    set(CPACK_PACKAGE_EXECUTABLES "ToolMap;ToolMap")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/LICENSE.txt")
    set(CPACK_CREATE_DESKTOP_LINKS "ToolMap")

    set(CPACK_GENERATOR "NSIS")
    set(CPACK_WIX_UPGRADE_GUID "938C138A-625D-4F79-9551-872638D1BA28")
    set(CPACK_NSIS_DISPLAY_NAME "ToolMap")
    set(CPACK_NSIS_CONTACT "Terranum toolmap@terranum.ch")
    set(CPACK_NSIS_HELP_LINK "www.terranum.ch")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    set(CPACK_NSIS_URL_INFO_ABOUT "www.terranum.ch")
    set(CPACK_NSIS_COMPRESSOR "lzma")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
    set(CPACK_NSIS_MODIFY_PATH "ON")
    # Icon in the add/remove control panel. Must be an .exe file
    set(CPACK_NSIS_INSTALLED_ICON_NAME ToolMap.exe)
    if (CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    else ()
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    endif ()

    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_LIST_DIR}/windows/toolmap.png")
    set(CPACK_WIX_UI_DIALOG "${CMAKE_CURRENT_LIST_DIR}/windows/installer_bg.bmp")
    set(CPACK_WIX_UI_BANNER "${CMAKE_CURRENT_LIST_DIR}/windows/installer_top.bmp")
    set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY "ToolMap")
    set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_LIST_DIR}/windows/set-env-vars.xml")
endif (WIN32)


#
# LINUX
#

IF (UNIX AND NOT APPLE)
    if (USE_APPIMAGE)
        find_file(MYSQL_CLIENT_LIB libmysqlclient.so ${MYSQL_MAIN_DIR}/lib)
        message(STATUS "MYSQL_CLIENT_LIB: ${MYSQL_CLIENT_LIB}")
        list(APPEND APPIMAGE_LIBS ${GDAL_LIBRARIES} ${GEOS_LIBRARIES} ${CURL_LIBRARIES} ${MYSQL_CLIENT_LIB})
        list(APPEND APPIMAGE_DAT "${CMAKE_BINARY_DIR}/share" "${CMAKE_BINARY_DIR}/mysql")
        message(STATUS "APPIMAGE_LIBS: ${APPIMAGE_LIBS}")
        APPIMAGE_PACKAGE(${CMAKE_PROJECT_NAME} "ToolMap" "${APPIMAGE_DAT}" "" "${APPIMAGE_LIBS}")
    else (USE_APPIMAGE)
        set(CPACK_GENERATOR "DEB")
        INSTALL(FILES "art/toolmap.png" DESTINATION "/usr/share/pixmaps")
        INSTALL(FILES "build/cpack/linux/toolmap.desktop" DESTINATION "/usr/share/applications")
        set(CPACK_DEBIAN_PACKAGE_NAME "ToolMap")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Terranum <toolmap@terranum.ch>")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
        SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libgeos-c1, libcurl3-gnutls, libwrap0, libproj0, libodbc1, unixODBC")
        SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "In order to handle geological information in an efficient way and for easily transfer it into geological information system, the research center on alpine environment (CREALP) developed in close collaboration with the Swiss Geological Survey (SGS) the software package ToolMap. The development is now taken over by Terranum (www.terranum.ch). ToolMap allows structuring and storing geological data through a robust relational database, vectorising field data and building up multilayer GIS models with high topological integrity. Thus, it permits a much faster production of GIS datasets readily available for a wide variety of applications.")
        SET(CPACK_DEBIAN_PACKAGE_SECTION "Sciences")
        SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/LICENSE.txt")
    endif (USE_APPIMAGE)

ENDIF (UNIX AND NOT APPLE)

include(CPack)
