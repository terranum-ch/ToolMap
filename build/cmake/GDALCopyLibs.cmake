IF (WIN32)
    FIND_FILE(GDAL_DLL_NAME
            gdal18.dll NAMES gdal19.dll gdal110.dll gdal200.dll gdal201.dll gdal202.dll
            HINTS ${SEARCH_GDAL_PATH}/bin
            ${SEARCH_GDAL_PATH}
            NO_DEFAULT_PATH)
    IF (NOT GDAL_DLL_NAME)
        MESSAGE(SEND_ERROR "gdal.dll not found in $¢{SEARCH_GDAL_PATH}")
    ENDIF ()

    FIND_FILE(GEOS_DLL_NAME
            geos.dll
            HINTS ${SEARCH_GEOS_PATH}/src
            ${SEARCH_GEOS_PATH}/bin
            ${SEARCH_GEOS_PATH}
            NO_DEFAULT_PATH)
    IF (NOT GEOS_DLL_NAME)
        MESSAGE(SEND_ERROR "geos.dll not found in ${SEARCH_GEOS_PATH}")
    ENDIF ()

    FIND_FILE(GEOS_C_DLL_NAME
            geos_c.dll
            HINTS ${SEARCH_GEOS_PATH}/bin
            ${SEARCH_GEOS_PATH}
            ${SEARCH_GEOS_PATH}/src
            NO_DEFAULT_PATH)
    IF (NOT GEOS_C_DLL_NAME)
        MESSAGE(SEND_ERROR "geos_c.dll not found in ${SEARCH_GEOS_PATH}")
    ENDIF ()

    FIND_FILE(PROJ_DLL_NAME
            proj.dll
            HINTS ${SEARCH_PROJ_PATH}/bin
            ${SEARCH_PROJ_PATH}
            NO_DEFAULT_PATH)
    IF (NOT PROJ_DLL_NAME)
        MESSAGE(SEND_ERROR "proj.dll not found in ${SEARCH_PROJ_PATH}")
    ENDIF ()

    FIND_FILE(CURL_DLL_NAME
            libcurl.dll
            HINTS ${SEARCH_CURL_PATH}/bin
            ${SEARCH_CURL_PATH}
            NO_DEFAULT_PATH)
    IF (NOT CURL_DLL_NAME)
        MESSAGE(WARNING "libcurl.dll not found in ${SEARCH_CURL_PATH}")
    ENDIF ()

    FIND_FILE(SQLITE_DLL_NAME
            sqlite3.dll
            HINTS ${SEARCH_SQLITE_PATH}/bin
            ${SEARCH_SQLITE_PATH}
            NO_DEFAULT_PATH)
    IF (NOT SQLITE_DLL_NAME)
        MESSAGE(WARNING "sqlite3.dll not found in ${SEARCH_SQLITE_PATH}")
    ENDIF ()

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GDAL_DLL_NAME}"
            "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
    MESSAGE(STATUS "GDAL DLL: ${GDAL_DLL_NAME}")

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GEOS_DLL_NAME}"
            "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
    MESSAGE(STATUS "GEOS DLL: ${GEOS_DLL_NAME}")

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GEOS_C_DLL_NAME}"
            "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
    MESSAGE(STATUS "GEOS C DLL: ${GEOS_C_DLL_NAME}")

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJ_DLL_NAME}"
            "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
    MESSAGE(STATUS "PROJ DLL: ${PROJ_DLL_NAME}")

    if (SQLITE_DLL_NAME)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SQLITE_DLL_NAME}"
                "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
        MESSAGE(STATUS "SQLITE DLL: ${SQLITE_DLL_NAME}")
    endif (SQLITE_DLL_NAME)

    if (CURL_DLL_NAME)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CURL_DLL_NAME}"
                "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
        MESSAGE(STATUS "CURL DLL: ${CURL_DLL_NAME}")
    endif (CURL_DLL_NAME)


    FILE(TO_NATIVE_PATH "${SEARCH_GDAL_PATH}/bin/*.dll" ALL_DLL_PATH_ORIGIN)
    FILE(TO_NATIVE_PATH "${SEARCH_GEOS_PATH}/bin/*.dll" ALL_DLL_PATH_ORIGIN)
    FILE(TO_NATIVE_PATH "${SEARCH_PROJ_PATH}/bin/*.dll" ALL_DLL_PATH_ORIGIN)
    FILE(TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}" ALL_DLL_PATH_DESTINATION)

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND copy
            ${ALL_DLL_PATH_ORIGIN}
            ${ALL_DLL_PATH_DESTINATION})
ENDIF (WIN32)
