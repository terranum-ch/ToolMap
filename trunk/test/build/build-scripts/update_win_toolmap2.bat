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
cmake %trunkdir%\test\build
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