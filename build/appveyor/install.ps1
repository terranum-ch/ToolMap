# Options
$TMP_DIR="C:\projects\tmp"
$LIB_DIR="C:\projects\libs"
$PATCH_DIR="C:\projects\toolmap\build\appveyor\patches"
$MSC_VER=1910
$ON_APPVEYOR=$true

# Force rebuilding some libraries
$REBUILD_WX=$false
$REBUILD_WXPDF=$false
$REBUILD_PROJ=$false
$REBUILD_GEOS=$false
$REBUILD_GDAL=$false
$REBUILD_MYSQL=$false
$REBUILD_CURL=$false

# Setup VS environment
# https://stackoverflow.com/questions/2124753/how-can-i-use-powershell-with-the-visual-studio-command-prompt
pushd 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build'    
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow

# All external dependencies are installed in the defined directory
if(!(Test-Path -Path $LIB_DIR)) {
  mkdir $LIB_DIR > $null
}
if(!(Test-Path -Path $TMP_DIR)) {
  mkdir $TMP_DIR > $null
}

# Install a recent CMake
if(!(Test-Path -Path "$LIB_DIR\cmake")) {
  Write-Host "`nInstalling CMake" -ForegroundColor Yellow
  cd $TMP_DIR
  $CMAKE_URL="https://cmake.org/files/v3.8/cmake-3.8.0-win64-x64.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $CMAKE_URL -FileName cmake.zip > $null
  } else {
    Invoke-WebRequest -Uri $CMAKE_URL -OutFile cmake.zip
  }
  7z x cmake.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\cmake-*" "$LIB_DIR\cmake"
}
$env:Path += ";$LIB_DIR\cmake\bin"
cmake --version
  
# Install cxxtest
if(!(Test-Path -Path "$LIB_DIR\cxxtest")) {
  Write-Host "`nInstalling cxxtest" -ForegroundColor Yellow
  cd $TMP_DIR
  $CXXTEST_URL="https://github.com/CxxTest/cxxtest/archive/4.3.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $CXXTEST_URL -FileName cxxtest.zip > $null
  } else {
    Invoke-WebRequest -Uri $CXXTEST_URL -OutFile cxxtest.zip
  }
  7z x cxxtest.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\cxxtest-*" "$LIB_DIR\cxxtest"
}
$env:Path += ";$LIB_DIR\cxxtest"
  
# Install wxWidgets
if(!(Test-Path -Path "$LIB_DIR\wxwidgets") -Or $REBUILD_WX) {
  Write-Host "`nBuilding wxWidgets" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxwidgets") {
    Remove-Item "$LIB_DIR\wxwidgets" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxwidgets" > $null
  $WX_URL="https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.zip"
  if ($ON_APPVEYOR) {
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
}
$env:WXWIN = "$LIB_DIR\wxwidgets"

# Install wxPDFDocument
if(!(Test-Path -Path "$LIB_DIR\wxpdfdoc") -Or $REBUILD_WXPDF) {
  Write-Host "`nBuilding wxPDFDocument" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxpdfdoc") {
    Remove-Item "$LIB_DIR\wxpdfdoc" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxpdfdoc" > $null
  $WXPDF_URL="https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $WXPDF_URL -FileName wxpdfdoc.zip > $null
  } else {
    Invoke-WebRequest -Uri $WXPDF_URL -OutFile wxpdfdoc.zip
  }
  7z x wxpdfdoc.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\wxpdfdoc-*" "$TMP_DIR\wxpdfdoc"
  cd "$TMP_DIR\wxpdfdoc\build"
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=1 > $null
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=0 > $null
  move "$TMP_DIR\wxpdfdoc\include" "$LIB_DIR\wxpdfdoc\include"
  move "$TMP_DIR\wxpdfdoc\lib" "$LIB_DIR\wxpdfdoc\lib"
}

# Install Proj
if(!(Test-Path -Path "$LIB_DIR\proj") -Or $REBUILD_PROJ) {
  Write-Host "`nBuilding Proj" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\proj") {
    Remove-Item "$LIB_DIR\proj" -Force -Recurse
  }
  mkdir "$LIB_DIR\proj" > $null
  $PROJ_URL="https://github.com/OSGeo/proj.4/archive/4.9.3.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $PROJ_URL -FileName proj.zip > $null
  } else {
    Invoke-WebRequest -Uri $PROJ_URL -OutFile proj.zip
  }
  7z x proj.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\proj.4-*" "$TMP_DIR\proj"
  cd "$TMP_DIR\proj"
  nmake -f makefile.vc INSTDIR="$LIB_DIR\proj" > $null
  nmake -f makefile.vc INSTDIR="$LIB_DIR\proj" install-all > $null
}

# Install Geos
if(!(Test-Path -Path "$LIB_DIR\geos") -Or $REBUILD_GEOS) {
  Write-Host "`nBuilding Geos" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\geos") {
    Remove-Item "$LIB_DIR\geos" -Force -Recurse
  }
  mkdir "$LIB_DIR\geos" > $null
  $GEOS_URL="http://download.osgeo.org/geos/geos-3.6.1.tar.bz2"
  if ($ON_APPVEYOR) {
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
  nmake -f makefile.vc WIN64=YES BUILD_DEBUG=YES > $null
  move "$TMP_DIR\geos\src" "$LIB_DIR\geos\src"
  move "$TMP_DIR\geos\include" "$LIB_DIR\geos\include"
  move "$TMP_DIR\geos\capi" "$LIB_DIR\geos\capi"
}

# Install Gdal
if(!(Test-Path -Path "$LIB_DIR\gdal") -Or $REBUILD_GDAL) {
  Write-Host "`nBuilding Gdal" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\gdal") {
    Remove-Item "$LIB_DIR\gdal" -Force -Recurse
  }
  mkdir "$LIB_DIR\gdal" > $null
  $GDAL_URL="http://download.osgeo.org/gdal/2.2.1/gdal221.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $GDAL_URL -FileName gdal.zip > $null
  } else {
    Invoke-WebRequest -Uri $GDAL_URL -OutFile gdal.zip
  }
  7z x gdal.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\gdal-*" "$TMP_DIR\gdal"
  cd "$TMP_DIR\gdal"
  $LIB_DIR_REV=$LIB_DIR -replace '\\','/'
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" install > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" devinstall > $null
}

# Install Mysql
if(!(Test-Path -Path "$LIB_DIR\mysql") -Or $REBUILD_MYSQL) {
  Write-Host "`nBuilding Mysql" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\mysql") {
    Remove-Item "$LIB_DIR\mysql" -Force -Recurse
  }
  mkdir "$LIB_DIR\mysql" > $null
  $MYSQL_URL="https://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.36.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $MYSQL_URL -FileName mysql.zip > $null
  } else {
    Invoke-WebRequest -Uri $MYSQL_URL -OutFile mysql.zip
  }
  7z x mysql.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\mysql-*" "$TMP_DIR\mysql"
  cd "$TMP_DIR\mysql"
  rm "$TMP_DIR\mysql\sql\sql_table.cc"
  copy "$PATCH_DIR\mysql-5.6.36-sql_table.cc" "$TMP_DIR\mysql\sql\sql_table.cc"
  mkdir bld > $null
  cd bld
  cmake .. -G"Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX="$LIB_DIR\mysql" -DWITH_UNIT_TESTS:BOOL=OFF -DFEATURE_SET:STRING=small > $null
  cmake --build . --config relwithdebinfo > $null
  cmake --build . --config relwithdebinfo --target INSTALL > $null
}

# Install curl
if(!(Test-Path -Path "$LIB_DIR\curl") -Or $REBUILD_CURL) {
  Write-Host "`nBuilding curl" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\curl") {
    Remove-Item "$LIB_DIR\curl" -Force -Recurse
  }
  mkdir "$LIB_DIR\curl" > $null
  $CURL_URL="https://github.com/curl/curl/archive/curl-7_54_1.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $CURL_URL -FileName curl.zip > $null
  } else {
    Invoke-WebRequest -Uri $CURL_URL -OutFile curl.zip
  }
  7z x curl.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\curl-*" "$TMP_DIR\curl"
  cd "$TMP_DIR\curl\winbuild"
  nmake -f Makefile.vc mode=dll VC=14 DEBUG=NO MACHINE=x64 > $null
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\bin" "$LIB_DIR\curl\bin"
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\include" "$LIB_DIR\curl\include"
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\lib" "$LIB_DIR\curl\lib"
}