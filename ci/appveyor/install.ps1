# Options
if ($env:APPVEYOR) {
  $MSC_VER=1925
  $VS_VER_NB="17"
  $VS_VER_YR="2022"
  $CMAKE_GENERATOR="-Ax64"
  $TMP_DIR="C:\projects\tmp"
  $LIB_DIR="C:\projects\libs"
  $WIX_DIR="C:\projects\wix"
  $CXXTEST_DIR="C:\projects\cxxtest"
} else {
  $MSC_VER=1925
  $VS_VER_NB="17"
  $VS_VER_YR="2022"
  $CMAKE_GENERATOR="-Ax64"
  $TMP_DIR="$env:UserProfile\Downloads\tmp"
  $LIB_DIR="$env:UserProfile\ToolMap-libs"
  $WIX_DIR="C:\Program Files\WiX"
  $CXXTEST_DIR="$LIB_DIR\cxxtest"
}

$TARGET_CPU="x64"

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
$PROGRAM_FILES="Program Files (x86)"
if ($VS_VER_YR -ge "2019") {
  $VS_VER="Visual Studio $VS_VER_NB $VS_VER_YR"
}
if ($VS_VER_YR -ge "2022") {
  $PROGRAM_FILES="Program Files"
}

# Setup VS environment
# https://stackoverflow.com/questions/2124753/how-can-i-use-powershell-with-the-visual-studio-command-prompt
pushd "C:\$PROGRAM_FILES\Microsoft Visual Studio\$VS_VER_YR\Community\VC\Auxiliary\Build"
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

# Install wxPDFDocument
if(-not (Test-Path -Path "$LIB_DIR\wxpdfdoc\include") -Or $REBUILD_WXPDF) {
  Write-Host "`nBuilding wxPDFDocument" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxpdfdoc") {
    Remove-Item "$LIB_DIR\wxpdfdoc" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxpdfdoc" > $null
  $WXPDF_URL="https://github.com/utelle/wxpdfdoc/archive/refs/tags/v1.0.2.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $WXPDF_URL -FileName wxpdfdoc.zip > $null
  } else {
    Invoke-WebRequest -Uri $WXPDF_URL -OutFile wxpdfdoc.zip
  }
  7z x wxpdfdoc.zip -o"$TMP_DIR" > $null
  cd "$TMP_DIR\wxpdfdoc-*\build"
  $Env:WXWIN = "$LIB_DIR/wxwidgets"
  MSBuild.exe wxpdfdoc_vc${VS_VER_NB}.sln /p:Configuration=Release /p:Platform="Win64" -target:wxpdfdoc






  move "$TMP_DIR\wxpdfdoc\include" "$LIB_DIR\wxpdfdoc\include"
  move "$TMP_DIR\wxpdfdoc\lib" "$LIB_DIR\wxpdfdoc\lib"
} else {
  Write-Host "`nwxPDFDocument already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/wxpdfdoc"

if ($stopwatchlibs.Elapsed.TotalMinutes -gt 40) { return }

# Install Mysql
if(-not (Test-Path -Path "$LIB_DIR\mysql\include") -Or $REBUILD_MYSQL) {
  Write-Host "`nBuilding Mysql" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\mysql") {
    Remove-Item "$LIB_DIR\mysql" -Force -Recurse
  }
  mkdir "$LIB_DIR\mysql" > $null
  $MYSQL_URL="https://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.51.zip"
  if ($env:APPVEYOR) {
    appveyor DownloadFile $MYSQL_URL -FileName mysql.zip > $null
  } else {
    Invoke-WebRequest -Uri $MYSQL_URL -OutFile mysql.zip
  }
  7z x mysql.zip -o"$TMP_DIR" > $null
  cd "$TMP_DIR\mysql-*"
  cmake . -G"$VS_VER" $CMAKE_GENERATOR -DCMAKE_BUILD_TYPE=$MYSQL_BUILD_TYPE -DCMAKE_INSTALL_PREFIX="$LIB_DIR\mysql" -DWITH_UNIT_TESTS:BOOL=OFF -DFEATURE_SET:STRING=small > $null
  if ($env:APPVEYOR) {
    cmake --build . --config $MYSQL_BUILD_TYPE --target sql > $null
    cmake --build . --config $MYSQL_BUILD_TYPE --target libmysqld > $null
    mkdir "$LIB_DIR\mysql\lib" > $null
    copy ".\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.lib" "$LIB_DIR\mysql\lib\libmysqld.lib"
    copy ".\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.dll" "$LIB_DIR\mysql\lib\libmysqld.dll"
    Copy-Item ".\include" -Destination "$LIB_DIR\mysql\include" -Recurse
    Copy-Item ".\sql\share" -Destination "$LIB_DIR\mysql\share" -Recurse
  } else {
    cmake --build . --config $MYSQL_BUILD_TYPE
    cmake --build . --config $MYSQL_BUILD_TYPE --target INSTALL
  }
} else {
  Write-Host "`nMysql already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/mysql"
