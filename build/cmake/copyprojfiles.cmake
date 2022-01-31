# COPY PROJ FILES
# PART OF TOOLMAP BUILD PROCESS

SET(PROJ_DESTINATION "share/proj")

IF (WIN32)
    find_path(PROJ_SHARE_PATH
            NAMES epsg ntv1_can.dat esri.extra proj.db
            HINTS ${CONAN_RES_DIRS_PROJ}
            PATH_SUFFIXES res share share/proj
            NO_DEFAULT_PATH)
    if (NOT PROJ_SHARE_PATH)
        message(SEND_ERROR "PROJ share not found.")
    endif ()
    message(STATUS "PROJ share: ${PROJ_SHARE_PATH}")
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJ_SHARE_PATH}"
            "${CMAKE_BINARY_DIR}/${PROJ_DESTINATION}")
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/resource/set-env.bat"
            "${CMAKE_BINARY_DIR}")
    message(STATUS "Copying: ${PROJ_SHARE_PATH} -> ${CMAKE_BINARY_DIR}/${PROJ_DESTINATION}")
ENDIF (WIN32)
