# COPY THE ERRMSG FILE TO THE CORRECT LOCATION
# THIS WILL ONLY WORK IF MYSQL WAS FOUND
# (c) Lucien Schreiber CREALP 2012

SET (MYSQL_ERRSQL_FROM_PATH "")
SET (MYSQL_ERRSQL_TO_PATH "")

IF (MYSQL_INCLUDE_DIR)
	MESSAGE (STATUS "MYSQL Include dir defined : ${MYSQL_INCLUDE_DIR}")
	SET (MYSQL_SHARE_DIR "${MYSQL_INCLUDE_DIR}../share/english")
	FIND_FILE(MYSQL_ERRMSG_FILE
			errmsg.sys
			PATHS
			/opt/MySQL/share/mysql
			${MYSQL_INCLUDE_DIR}/../share/english
			${MYSQL_INCLUDE_DIR}/../share/
			)
	IF(NOT MYSQL_ERRMSG_FILE)
		MESSAGE(WARNING "errmsg.sys file not found!")
	ENDIF()

	IF(APPLE)
		ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}
			POST_BUILD
			COMMAND mkdir -p
			 ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql
			COMMAND cp ${MYSQL_ERRMSG_FILE}
			${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql/errmsg.sys
			)
	ENDIF(APPLE)


    IF(WIN32)
        FILE (TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mysql WIN_MYSQL_DIR)
        FILE (TO_NATIVE_PATH ${MYSQL_SHARE_DIR}/errmsg.sys  WIN_MYSQL_ERRMSG_SOURCE)
        FILE (TO_NATIVE_PATH ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mysql/errmsg.sys  WIN_MYSQL_ERRMSG_DEST)

        ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}
                POST_BUILD
                COMMAND IF exist ${WIN_MYSQL_DIR} (echo ${WIN_MYSQL_DIR} exists) ELSE (mkdir ${WIN_MYSQL_DIR})
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MYSQL_ERRMSG_FILE}	$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/mysql/
                )

        # Copy optimized MySQL  dll under WINDOWS
        FIND_FILE (MYSQL_DLL_NAME
                libmysqld.dll
                HINTS ${MYSQL_MAIN_DIR}/Embedded/DLL/release
                ${MYSQL_MAIN_DIR}/lib
                NO_DEFAULT_PATH)
        IF (NOT MYSQL_DLL_NAME)
            MESSAGE (SEND_ERROR "MySQL dll not found in ${MYSQL_MAIN_DIR}")
        ENDIF()

        ADD_CUSTOM_COMMAND (TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MYSQL_DLL_NAME}"
                "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
    ENDIF(WIN32)

    # COPY errmsg.sys under Linux
    IF(UNIX AND NOT APPLE)
        FILE(COPY ${MYSQL_INCLUDE_DIR}/../share/english/errmsg.sys
                DESTINATION ${PROJECT_BINARY_DIR}/mysql)
    ENDIF(UNIX AND NOT APPLE)

ENDIF(MYSQL_INCLUDE_DIR)



  
  
