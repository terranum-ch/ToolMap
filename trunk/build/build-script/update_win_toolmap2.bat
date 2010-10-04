@ECHO OFF 
REM script for updating toolmap 2 under windows
REM works only for windows
REM 1) Update the svn repositories
REM 2) Run cmake to update the Visual studio solution
REM 3) Run (manually) the ToolMap compilation
REM 4) Make installer 
REM 5) Rename installer
REM 6) Upload installer to ftp (open firewall for curl)


@SET TRUNKDIR=D:\LS\PRJ\TOOLMAP2\trunk
@SET BINDIR=D:\LS\PRJ\TOOLMAP2\bin\prod
@SET VSDIR=C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE
@SET INSTPROG="C:\Program Files\NSIS\makensis.exe"
@SET FTPPROG="C:\Program Files\curl-7.19.6\curl.exe"

@SET MYSQLPATH=D:/LS/PROGRAMATION/LIB/mysql-5.1.51-VS2008
@SET CURLPATH=D:/LS/PROGRAMATION/LIB/LIB_CURL
@SET GEOSPATH=D:/LS/PROGRAMATION/LIB/geos-3.2.2
@SET GDALPATH=D:/LS/PROGRAMATION/LIB/LIB_GDAL
@SET PARAMWXWIN=D:/LS/PROGRAMATION/LIB/wxMSW-2.8.11



ECHO ----------------------------------------
ECHO         UPDATE TOOLMAP 2
ECHO     (c) Lucien Schreiber CREALP
ECHO ----------------------------------------
ECHO 1) Updating repositories ...

D:
cd %trunkdir%
svn update

REM GETTING SUBVERSION NUMBER
FOR /F "usebackq" %%s IN (`svnversion .`) DO @SET REV=%%s
REM ECHO  #define SVN_VERSION _T(^"%REV%^")

ECHO 1) Updating repositories ... DONE (version is : %REV%)

ECHO 2) Making Visual studio solution...
cd %bindir%

SET WXWIN=%PARAMWXWIN%
echo %WXWIN%

cmake %trunkdir%\build -G"Visual Studio 9 2008" -DMYSQL_IS_LOGGING:BOOL=1 -DMYSQL_MAIN_DIR:PATH=%MYSQLPATH% -DSEARCH_CURL:BOOL=1 -DSEARCH_CURL_LIB_PATH:PATH=%CURLPATH% -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=%GDALPATH%  -DSEARCH_GEOS_LIB_PATH:PATH=%GEOSPATH% -DSVN_DURING_BUILD:BOOL=1 -DSVN_DURING_CMAKE:BOOL=1 -DUSE_GDIPLUS_LIBRARY:BOOL=1 -DUSE_MT_LIBRARY:BOOL=1
ECHO 2) Making Visual studio solution... DONE

ECHO -----------------------------------------------
ECHO 3) BUILDING TOOLMAP2 (MAY TAKE SOME TIMES)-----
ECHO -----------------------------------------------

cd %BINDIR%
"%vsdir%\VCExpress.exe" ToolMap2.sln /Out solution.log /Build
echo %ERRORLEVEL%
IF ERRORLEVEL 1 goto QuitErrorBuild
ECHO 3) BUILDING TOOLMAP2 DONE


IF "%1"=="NOINSTALLER" goto WaitForEnter

ECHO 4) MAKING INSTALLER (NSIS)...
d:
cd %trunkdir%\install\windows\lugano
%instprog% /X"SetCompressor /FINAL lzma" /X"!define PRODUCT_VERSION '2.0.%REV%'" install_builder_script.nsi 
ECHO 4) MAKING INSTALLER (NSIS)... DONE
 
ECHO 5) RENAMING INSTALLER...
RENAME InstallToolMap.exe InstallToolMap_d%REV%.exe 
ECHO 5) RENAMING INSTALLER... DONE

:PublishingBinToWWW
ECHO 6) PUBLISHING TO WWW...
%FTPPROG% -# -L  --user wwwcrealp:Chatmoig --upload-file InstallToolMap_d%REV%.exe --proxy proxy.vs.ch:8080  ftp://www2.crealp.ch/htdocs/down/toolmap/
ECHO 6) PUBLISHING TO WWW... DONE

:PublishingLogToWWW
ECHO 6) PUBLISHING LOG TO WWW...
%FTPPROG% -# -L  --user wwwcrealp:Chatmoig --upload-file %BINDIR%\solution.log --proxy proxy.vs.ch:8080  ftp://www2.crealp.ch/htdocs/down/toolmap/
ECHO 6) PUBLISHING LOG TO WWW... (DONE)


:WaitForEnter
  set wait=
  set /p wait=Please press ENTER when ToolMap 2 is build successfully: 
  IF NOT DEFINED wait goto :eof 
  echo Please press only the ENTER key and nothing else
  goto :WaitForEnter

REM :UserPressedENTER

:QuitErrorBuildScript
	goto PublishingLogToWWW