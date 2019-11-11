# Options
if ($env:APPVEYOR) {
  $MSC_VER=1923
  $VS_VER_NB="16"
  $VS_VER_YR="2019"
  $CMAKE_GENERATOR="-Ax64"
  $TMP_DIR="C:\projects\tmp"
  $LIB_DIR="C:\projects\libs"
  $WIX_DIR="C:\projects\wix"
  $CXXTEST_DIR="C:\projects\cxxtest"
  $PATCH_DIR="C:\projects\toolmap\ci\appveyor\patches"
} else {
  $MSC_VER=1923
  $VS_VER_NB="16"
  $VS_VER_YR="2019"
  $CMAKE_GENERATOR="-Ax64"
  $TMP_DIR="$env:UserProfile\Downloads\tmp"
  $LIB_DIR="$env:UserProfile\ToolMap-libs"
  $WIX_DIR="C:\Program Files\WiX"
  $CXXTEST_DIR="$LIB_DIR\cxxtest"
  $PATCH_DIR="D:\Development\ToolMap\ci\appveyor\patches"
}

$WITH_DEBUG_LIBS=$false
$MYSQL_BUILD_TYPE="RelWithDebInfo"

# Force rebuilding some libraries
$REBUILD_WX=$false
$REBUILD_WXPDF=$false
$REBUILD_PROJ=$false
$REBUILD_GEOS=$false
$REBUILD_GDAL=$false
$REBUILD_MYSQL=$false
$REBUILD_CURL=$false

# Timer
$stopwatchlibs = [system.diagnostics.stopwatch]::StartNew()

# Set Visual Studio version
$VS_VER="Visual Studio $VS_VER_NB $VS_VER_YR Win64"
if ($VS_VER_YR -ge "2019") {
  $VS_VER="Visual Studio $VS_VER_NB $VS_VER_YR"
}

# Setup VS environment
# https://stackoverflow.com/questions/2124753/how-can-i-use-powershell-with-the-visual-studio-command-prompt
pushd "C:\Program Files (x86)\Microsoft Visual Studio\$VS_VER_YR\Community\VC\Auxiliary\Build"
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio $VS_VER_YR Command Prompt variables set." -ForegroundColor Yellow

set CL=/MP

# All external dependencies are installed in the defined directory
if(-not (Test-Path -Path $LIB_DIR)) {
  mkdir $LIB_DIR > $null
}
if(-not (Test-Path -Path $TMP_DIR)) {
  mkdir $TMP_DIR > $null
}

# CMake version
cmake --version

# Install cxxtest
Write-Host "`nInstalling cxxtest" -ForegroundColor Yellow
cd $TMP_DIR
$CXXTEST_URL="https://github.com/CxxTest/cxxtest/archive/4.4.zip"
if ($env:APPVEYOR) {
  appveyor DownloadFile $CXXTEST_URL -FileName cxxtest.zip > $null
} else {
  Invoke-WebRequest -Uri $CXXTEST_URL -OutFile cxxtest.zip
}
7z x cxxtest.zip -o"$TMP_DIR" > $null
move "$TMP_DIR\cxxtest-*" "$CXXTEST_DIR"
$env:Path += ";$CXXTEST_DIR"

# List dirs already present from cache
Write-Host "`nContent of the cache:" -ForegroundColor Yellow
Get-ChildItem "$LIB_DIR"
  
# Install wxWidgets
if(-not (Test-Path -Path "$LIB_DIR\wxwidgets\include") -Or $REBUILD_WX) {
  Write-Host "`nBuilding wxWidgets" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxwidgets") {
    Remove-Item "$LIB_DIR\wxwidgets" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxwidgets" > $null
  $WX_URL="https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $WX_URL -FileName wxwidgets.zip > $null
  } else {
    Invoke-WebRequest -Uri $WX_URL -OutFile wxwidgets.zip
  }
  7z x wxwidgets.zip -o"$TMP_DIR\wxwidgets" > $null
  cd "$TMP_DIR\wxwidgets\build\msw"
  nmake -f makefile.vc BUILD=release UNICODE=1 MONOLITHIC=1 > $null
  nmake -f makefile.vc BUILD=debug UNICODE=1 MONOLITHIC=1 > $null
  move "$TMP_DIR\wxwidgets\include" "$LIB_DIR\wxwidgets\include"
  copy "$TMP_DIR\wxwidgets\lib\vc_lib\mswu\wx\setup.h" "$LIB_DIR\wxwidgets\include\wx\setup.h"
  move "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h" "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h_old"
  copy "$TMP_DIR\wxwidgets\lib\vc_lib\mswu\wx\msw\rcdefs.h" "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h"
  move "$TMP_DIR\wxwidgets\lib" "$LIB_DIR\wxwidgets\lib"
} else {
  Write-Host "`nwxWidgets already in cache" -ForegroundColor Yellow
}
$env:WXWIN = "$LIB_DIR\wxwidgets"
# List files
Get-ChildItem "$LIB_DIR/wxwidgets"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install wxPDFDocument
if(-not (Test-Path -Path "$LIB_DIR\wxpdfdoc\include") -Or $REBUILD_WXPDF) {
  Write-Host "`nBuilding wxPDFDocument" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxpdfdoc") {
    Remove-Item "$LIB_DIR\wxpdfdoc" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxpdfdoc" > $null
  $WXPDF_URL="https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $WXPDF_URL -FileName wxpdfdoc.zip > $null
  } else {
    Invoke-WebRequest -Uri $WXPDF_URL -OutFile wxpdfdoc.zip
  }
  7z x wxpdfdoc.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\wxpdfdoc-*" "$TMP_DIR\wxpdfdoc"
  cd "$TMP_DIR\wxpdfdoc\build"
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=0 > $null
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=1 > $null
  move "$TMP_DIR\wxpdfdoc\include" "$LIB_DIR\wxpdfdoc\include"
  move "$TMP_DIR\wxpdfdoc\lib" "$LIB_DIR\wxpdfdoc\lib"
} else {
  Write-Host "`nwxPDFDocument already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/wxpdfdoc"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install curl
if(-not (Test-Path -Path "$LIB_DIR\curl\include") -Or $REBUILD_CURL) {
  Write-Host "`nBuilding curl" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\curl") {
    Remove-Item "$LIB_DIR\curl" -Force -Recurse
  }
  mkdir "$LIB_DIR\curl" > $null
  $CURL_URL="https://github.com/curl/curl/archive/curl-7_64_1.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $CURL_URL -FileName curl.zip > $null
  } else {
    Invoke-WebRequest -Uri $CURL_URL -OutFile curl.zip
  }
  7z x curl.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\curl-*" "$TMP_DIR\curl"
  cd "$TMP_DIR\curl"
  .\buildconf.bat
  cd "$TMP_DIR\curl\winbuild"
  nmake -f Makefile.vc mode=dll DEBUG=NO MACHINE=x64 > $null
  Copy-Item "$TMP_DIR\curl\builds\libcurl-vc-x64-release-dll-ipv6-sspi-winssl\bin" -Destination "$LIB_DIR\curl\bin" -Recurse
  Copy-Item "$TMP_DIR\curl\builds\libcurl-vc-x64-release-dll-ipv6-sspi-winssl\include" -Destination "$LIB_DIR\curl\include" -Recurse
  Copy-Item "$TMP_DIR\curl\builds\libcurl-vc-x64-release-dll-ipv6-sspi-winssl\lib" -Destination "$LIB_DIR\curl\lib" -Recurse
} else {
  Write-Host "`ncurl already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/curl"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install sqlite
if(-not (Test-Path -Path "$LIB_DIR\sqlite\include") -Or $REBUILD_CURL) {
    Write-Host "`nBuilding sqlite" -ForegroundColor Yellow
    cd $TMP_DIR
    if(Test-Path -Path "$LIB_DIR\sqlite") {
        Remove-Item "$LIB_DIR\sqlite" -Force -Recurse
    }
    mkdir "$LIB_DIR\sqlite" > $null
    mkdir "$LIB_DIR\sqlite\bin" > $null
    mkdir "$LIB_DIR\sqlite\lib" > $null
    mkdir "$LIB_DIR\sqlite\include" > $null
    $SQLITE_SRC_URL="https://www.sqlite.org/2019/sqlite-amalgamation-3270200.zip"
    $SQLITE_DLL_URL="https://www.sqlite.org/2019/sqlite-dll-win64-x64-3270200.zip"
    $SQLITE_TOOLS_URL="https://www.sqlite.org/2019/sqlite-tools-win32-x86-3270200.zip"
    if ($env:APPVEYOR) {
        appveyor DownloadFile $SQLITE_SRC_URL -FileName sqlite_src.zip > $null
        appveyor DownloadFile $SQLITE_DLL_URL -FileName sqlite_dll.zip > $null
        appveyor DownloadFile $SQLITE_TOOLS_URL -FileName sqlite_tools.zip > $null
    } else {
        Invoke-WebRequest -Uri $SQLITE_SRC_URL -OutFile sqlite_src.zip
        Invoke-WebRequest -Uri $SQLITE_DLL_URL -OutFile sqlite_dll.zip
        Invoke-WebRequest -Uri $SQLITE_TOOLS_URL -OutFile sqlite_tools.zip
    }
    7z x sqlite_src.zip -o"$TMP_DIR" > $null
    7z x sqlite_dll.zip -o"$TMP_DIR" > $null
    7z x sqlite_tools.zip -o"$TMP_DIR" > $null
    move "$TMP_DIR\sqlite-tools*" "$TMP_DIR\sqlitetools"
    move "$TMP_DIR\sqlite-*" "$TMP_DIR\sqlite"
    lib /def:sqlite3.def
    copy "$TMP_DIR\sqlite3.dll" "$LIB_DIR\sqlite\bin\sqlite3.dll"
    copy "$TMP_DIR\sqlite3.lib" "$LIB_DIR\sqlite\lib\sqlite3.lib"
    copy "$TMP_DIR\sqlitetools\sqlite3.exe" "$LIB_DIR\sqlite\bin\sqlite3.exe"
    copy "$TMP_DIR\sqlite\sqlite3.h" "$LIB_DIR\sqlite\include\sqlite3.h"
    copy "$TMP_DIR\sqlite\sqlite3ext.h" "$LIB_DIR\sqlite\include\sqlite3ext.h"
} else {
    Write-Host "`nsqlite already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/sqlite"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install Proj
if(-not (Test-Path -Path "$LIB_DIR\proj\include") -Or $REBUILD_PROJ) {
  Write-Host "`nBuilding Proj" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\proj") {
    Remove-Item "$LIB_DIR\proj" -Force -Recurse
  }
  mkdir "$LIB_DIR\proj" > $null
  $PROJ_URL="https://github.com/OSGeo/PROJ/releases/download/6.2.1/proj-6.2.1.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $PROJ_URL -FileName proj.zip > $null
  } else {
    Invoke-WebRequest -Uri $PROJ_URL -OutFile proj.zip
  }
  7z x proj.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\proj-*" "$TMP_DIR\proj"
  cd "$TMP_DIR\proj"
  mkdir build
  cd build
  cmake -G"$VS_VER" $CMAKE_GENERATOR -DCMAKE_PREFIX_PATH="$LIB_DIR\sqlite" -DPROJ_TESTS=OFF -DBUILD_PROJINFO=OFF -DBUILD_CCT=OFF -DBUILD_CS2CS=OFF -DBUILD_GEOD=OFF -DBUILD_GIE=OFF -DBUILD_PROJ=OFF -DBUILD_PROJINFO=OFF -DBUILD_LIBPROJ_SHARED=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$LIB_DIR\proj" .. > $null
  cmake --build . --config Release > $null
  cmake --build . --config Release --target INSTALL > $null
} else {
  Write-Host "`nProj already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/proj"
Get-ChildItem "$LIB_DIR/proj/bin"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install Geos
if(-not (Test-Path -Path "$LIB_DIR\geos\include") -Or $REBUILD_GEOS) {
  Write-Host "`nBuilding Geos" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\geos") {
    Remove-Item "$LIB_DIR\geos" -Force -Recurse
  }
  mkdir "$LIB_DIR\geos" > $null
  $GEOS_URL="http://download.osgeo.org/geos/geos-3.6.1.tar.bz2"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $GEOS_URL -FileName geos.tar.bz2 > $null
  } else {
    Invoke-WebRequest -Uri $GEOS_URL -OutFile geos.tar.bz2
  }
  7z x geos.tar.bz2 -o"$TMP_DIR" > $null
  7z x geos.tar -o"$TMP_DIR" > $null
  move "$TMP_DIR\geos-*" "$TMP_DIR\geos"
  rm "$TMP_DIR\geos\nmake.opt"
  copy "$PATCH_DIR\geos-3.6.1-nmake.opt" "$TMP_DIR\geos\nmake.opt"
  cd "$TMP_DIR\geos"
  nmake -f makefile.vc WIN64=YES > $null
  if ($WITH_DEBUG_LIBS) {
    nmake -f makefile.vc WIN64=YES BUILD_DEBUG=YES > $null
  }
  move "$TMP_DIR\geos\src" "$LIB_DIR\geos\src"
  move "$TMP_DIR\geos\include" "$LIB_DIR\geos\include"
  move "$TMP_DIR\geos\capi" "$LIB_DIR\geos\capi"
} else {
  Write-Host "`nGeos already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/geos"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install Gdal
if(-not (Test-Path -Path "$LIB_DIR\gdal\include") -Or $REBUILD_GDAL) {
  Write-Host "`nBuilding Gdal" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\gdal") {
    Remove-Item "$LIB_DIR\gdal" -Force -Recurse
  }
  mkdir "$LIB_DIR\gdal" > $null
  $GDAL_URL="https://github.com/OSGeo/gdal/releases/download/v3.0.2/gdal302.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $GDAL_URL -FileName gdal.zip > $null
  } else {
    Invoke-WebRequest -Uri $GDAL_URL -OutFile gdal.zip
  }
  7z x gdal.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\gdal-*" "$TMP_DIR\gdal"
  cd "$TMP_DIR\gdal"
  $LIB_DIR_REV=$LIB_DIR -replace '\\','/'
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" PROJ_INCLUDE="-I$LIB_DIR_REV/proj/include" PROJ_LIBRARY="$LIB_DIR_REV/proj/lib/proj_6_2.lib" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" PROJ_INCLUDE="-I$LIB_DIR_REV/proj/include" PROJ_LIBRARY="$LIB_DIR_REV/proj/lib/proj_6_2.lib" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB install > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" PROJ_INCLUDE="-I$LIB_DIR_REV/proj/include" PROJ_LIBRARY="$LIB_DIR_REV/proj/lib/proj_6_2.lib" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB devinstall > $null
} else {
  Write-Host "`nGdal already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/gdal"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install Mysql
if(-not (Test-Path -Path "$LIB_DIR\mysql\include") -Or $REBUILD_MYSQL) {
  Write-Host "`nBuilding Mysql" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\mysql") {
    Remove-Item "$LIB_DIR\mysql" -Force -Recurse
  }
  mkdir "$LIB_DIR\mysql" > $null
  $MYSQL_URL="https://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.36.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $MYSQL_URL -FileName mysql.zip > $null
  } else {
    Invoke-WebRequest -Uri $MYSQL_URL -OutFile mysql.zip
  }
  7z x mysql.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\mysql-*" "$TMP_DIR\mysql"
  cd "$TMP_DIR\mysql"
  rm "$TMP_DIR\mysql\sql\sql_table.cc"
  copy "$PATCH_DIR\mysql-5.6.36-sql_table.cc" "$TMP_DIR\mysql\sql\sql_table.cc"
  cmake . -G"$VS_VER" $CMAKE_GENERATOR -DCMAKE_BUILD_TYPE=$MYSQL_BUILD_TYPE -DCMAKE_INSTALL_PREFIX="$LIB_DIR\mysql" -DWITH_UNIT_TESTS:BOOL=OFF -DFEATURE_SET:STRING=small > $null
  if ($env:APPVEYOR) {
    cmake --build . --config $MYSQL_BUILD_TYPE --target sql > $null
    cmake --build . --config $MYSQL_BUILD_TYPE --target libmysqld > $null
    mkdir "$LIB_DIR\mysql\lib" > $null
    copy "$TMP_DIR\mysql\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.lib" "$LIB_DIR\mysql\lib\libmysqld.lib"
    copy "$TMP_DIR\mysql\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.dll" "$LIB_DIR\mysql\lib\libmysqld.dll"
    Copy-Item "$TMP_DIR\mysql\include" -Destination "$LIB_DIR\mysql\include" -Recurse
    Copy-Item "$TMP_DIR\mysql\sql\share" -Destination "$LIB_DIR\mysql\share" -Recurse
  } else {
    cmake --build . --config $MYSQL_BUILD_TYPE
    cmake --build . --config $MYSQL_BUILD_TYPE --target INSTALL
  }
} else {
  Write-Host "`nMysql already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/mysql"
