# COPY HTML AND JS FILES
# PART OF TOOLMAP BUILD PROCESS

FILE (GLOB WEBFILES "../resource/web/*.js" "../resource/web/*.html")
MESSAGE ("Webfiles: ${WEBFILES}")
