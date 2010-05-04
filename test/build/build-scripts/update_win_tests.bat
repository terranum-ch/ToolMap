@ECHO OFF 
REM script for updating toolmap 2 test under windows
REM works only for windows
REM 1) Update the svn repositories
REM 1b) Remove main.cpp
REM 2) Run cmake to update the Visual studio solution
REM 3) Run (manually) the ToolMap compilation
REM 4) Launch the tests


@SET TRUNKDIR=D:\LS\PRJ\TOOLMAP2\trunk\
@SET BINDIR=D:\LS\PRJ\TOOLMAP2\bin\test
@SET VSDIR=C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE

@SET MYSQLPATH=D:/LS/PROGRAMATION/LIB/mysql-5.1.46-VS2008
@SET CURLPATH=D:/LS/PROGRAMATION/LIB/LIB_CURL
@SET GEOSPATH=D:/LS/PROGRAMATION/LIB/geos-3.2.2
@SET GDALPATH=D:/LS/PROGRAMATION/LIB/LIB_GDAL
@SET PARAMWXWIN=D:/LS/PROGRAMATION/LIB/wxMSW-2.8.11
@SET UNITTESTPATH=D:/LS/PRJ/TOOLMAP2/unit_testing
@SET CXXTESTPATH=D:/LS/PROGRAMATION/LIB/cxxtest


ECHO ----------------------------------------
ECHO         UPDATE TOOLMAP2 TESTS
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


ECHO 1b) REMOVING MAIN.CPP, will be generated again
del %bindir%\main.cpp


ECHO 2) Making Visual studio solution...
cd %bindir%

SET WXWIN=%PARAMWXWIN%
echo %WXWIN%

cmake %trunkdir%\test\build -G"Visual Studio 9 2008" -DMYSQL_IS_LOGGING:BOOL=0 -DMYSQL_MAIN_DIR:PATH=%MYSQLPATH% -DSEARCH_CURL:BOOL=1 -DSEARCH_CURL_LIB_PATH:PATH=%CURLPATH% -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=%GDALPATH%  -DSEARCH_GEOS_LIB_PATH:PATH=%GEOSPATH% -DSVN_DURING_BUILD:BOOL=1 -DSVN_DURING_CMAKE:BOOL=1 -DUSE_GDIPLUS_LIBRARY:BOOL=1 -DUSE_MT_LIBRARY:BOOL=1 -DUSE_UNITTEST:BOOL=1 -DUNIT_TESTING_PATH:PATH=%UNITTESTPATH% -DCXXTEST_DIRECTORY:PATH=%CXXTESTPATH%
ECHO 2) Making Visual studio solution... DONE

ECHO -----------------------------------------------
ECHO 3) BUILDING TOOLMAP2 (MAY TAKE SOME TIMES)-----
ECHO -----------------------------------------------

cd %BINDIR%
"%vsdir%\VCExpress.exe" Project.sln /Out solution.log /Build
echo %ERRORLEVEL%
IF ERRORLEVEL 1 goto QuitErrorBuildScript
ECHO 3) BUILDING TOOLMAP2 DONE

cd %bindir%
ctest.exe
"C:\Program Files\Notepad++\notepad++.exe" %bindir%\Testing\Temporary\LastTest.log
REM notepad.exe %bindir%\Testing\Temporary\LastTest.log
goto :WaitForEnter



:WaitForEnter
  set wait=
  set /p wait=Please press ENTER to continue 
  IF NOT DEFINED wait goto :eof 
  echo Please press only the ENTER key and nothing else
  goto :WaitForEnter

REM :UserPressedENTER

:QuitErrorBuildScript
	ECHO Error detected see solution.log
	goto :WaitForEnter