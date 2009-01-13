@ECHO OFF 
REM script for copying files to directory after compilation 
REM works only for windows
REM find & remplace destdir with your destination directory
REM find & remplace sourcedir with your source directory

@SET DESTDIR=D:\LS\PROGRAMATION\LIB\mysql-5.1.30-VS7
@SET SOURCEDIR=D:\LS\PROGRAMATION\LIB\mysql-5.1.30

REM FOR /F "usebackq" %%s IN (`svnversion .`) DO @SET REV=%%s
 
REM ECHO  #define SVN_VERSION _T(^"%REV%^") >../include/svn_version.h

ECHO ----------------------------------------
ECHO COPY MYSQL FROM SOURCE TO BIN
ECHO (c) Lucien Schreiber CREALP
ECHO ----------------------------------------
ECHO Copying MYSQL form : 	%SOURCEDIR%
ECHO To 			%DESTDIR%

REM add options for checking if dirs are ok

ECHO Creating destination structure 
REM cd %destdir%
if not exist %destdir%\bin mkdir %destdir%\bin
if not exist %destdir%\data mkdir %destdir%\data
if not exist %destdir%\share mkdir %destdir%\share
if not exist %destdir%\scripts mkdir %destdir%\scripts
ECHO Creating destination structure DONE


ECHO Creating destination structure for programmers
if not exist %destdir%\include mkdir %destdir%\include
if not exist %destdir%\Embedded mkdir %destdir%\Embedded
if not exist %destdir%\Embedded\DLL mkdir %destdir%\Embedded\DLL
if not exist %destdir%\Embedded\DLL\debug mkdir %destdir%\Embedded\DLL\debug
if not exist %destdir%\Embedded\DLL\release mkdir %destdir%\Embedded\DLL\release
if not exist %destdir%\Embedded\static mkdir %destdir%\Embedded\static
if not exist %destdir%\Embedded\static\debug mkdir %destdir%\Embedded\static\debug
if not exist %destdir%\Embedded\static\release mkdir %destdir%\Embedded\static\release
if not exist %destdir%\lib mkdir %destdir%\lib
if not exist %destdir%\lib\debug mkdir %destdir%\lib\debug
if not exist %destdir%\lib\opt mkdir %destdir%\lib\opt
ECHO Creating destination structure for programmer DONE



REM COPYING DATA

ECHO Copy library (basic)
cd %sourcedir%
copy client\release\*.exe %destdir%\bin
copy client\debug\mysqld.exe %destdir%\bin\mysqld-debug.exe
xcopy scripts\*.* %destdir%\scripts /E
cd %sourcedir%\sql
xcopy share\*.* %destdir%\share /E
ECHO Copy library (basic) DONE



ECHO Copy library (advanced)
cd %sourcedir%
copy libmysql\debug\mysqlclient.lib %destdir%\lib\debug
copy libmysql\debug\libmysql.* %destdir%\lib\debug
copy zlib\debug\zlib.* %destdir%\lib\debug
copy libmysql\release\mysqlclient.lib %destdir%\lib\opt
copy libmysql\release\libmysql.* %destdir%\lib\opt
copy zlib\release\zlib.* %destdir%\lib\opt
xcopy include\*.h %destdir%\include /E
copy libmysql\libmysql.def %destdir%\include
ECHO Copy library (advanced) DONE


ECHO Copy library (embedded)
cd %sourcedir%
copy libmysqld\debug\mysqlserver.* %destdir%\Embedded\static\debug
copy libmysqld\debug\libmysqld.* %destdir%\Embedded\DLL\debug
REM copy zlib\debug\zlib.* %destdir%\lib\debug
copy libmysqld\release\mysqlserver.* %destdir%\Embedded\static\release
copy libmysqld\release\libmysqld.* %destdir%\Embedded\DLL\release
REM copy zlib\release\zlib.* %destdir%\lib\opt
REM copy include\*.h %destdir%\include
copy libmysqld\libmysqld.def %destdir%\include
ECHO Copy library (embedded) DONE