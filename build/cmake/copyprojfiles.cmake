# COPY PROJ FILES
# PART OF TOOLMAP BUILD PROCESS

SET(PROJ_DESTINATION "share/proj")

IF (WIN32)
    find_path(PROJ_SHARE_PATH
            NAMES epsg ntv1_can.dat esri.extra proj.db
            HINTS ${SEARCH_PROJ_PATH}
            PATH_SUFFIXES share share/proj
            NO_DEFAULT_PATH)
    if (NOT PROJ_SHARE_PATH)
        message(SEND_ERROR "PROJ share not found.")
    endif ()
    message(STATUS "PROJ share: ${PROJ_SHARE_PATH}")
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJ_SHARE_PATH}"
            "${CMAKE_BINARY_DIR}/${PROJ_DESTINATION}")
    message(STATUS "Copying: ${PROJ_SHARE_PATH} -> ${CMAKE_BINARY_DIR}/${PROJ_DESTINATION}")
ENDIF (WIN32)
