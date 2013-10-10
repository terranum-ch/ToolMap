IF(WIN32)
  # curl library debug
  FIND_FILE (CURL_DLL_NAME_DEBUG
    libcurld.dll
    HINTS "${CURL_INCLUDE_DIR}/../lib/DLL-Debug"
    NO_DEFAULT_PATH)
  IF (NOT CURL_DLL_NAME_DEBUG)
    MESSAGE (SEND_ERROR "libcurld.dll (debug) not found in ${CURL_INCLUDE_DIR}")
  ENDIF()

  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CURL_DLL_NAME_DEBUG}
    "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

  # curl library release
  FIND_FILE (CURL_DLL_NAME
    libcurl.dll
    HINTS "${CURL_INCLUDE_DIR}/../lib/DLL-Release"
    NO_DEFAULT_PATH)
  IF (NOT CURL_DLL_NAME)
    MESSAGE (SEND_ERROR "libcurld.dll (debug) not found in ${CURL_INCLUDE_DIR}")
  ENDIF()

  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CURL_DLL_NAME}
    "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
ENDIF(WIN32)
