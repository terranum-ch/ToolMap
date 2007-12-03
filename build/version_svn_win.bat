@ECHO OFF 
REM script for getting the last svn version number 
REM works only for windows

FOR /F "usebackq" %%s IN (`svnversion .`) DO @SET REV=%%s
 
ECHO  #define SVN_VERSION _T(^"%REV%^") >../src/core/svn_version.h
