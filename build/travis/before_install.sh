#!/usr/bin/env sh

# Install required libraries
sudo apt-get -qq update
sudo apt-get install -y libproj-dev
sudo apt-get install -y libgeos-dev
sudo apt-get install -y libmariadbd-dev
sudo apt-get install -y libaio-dev
sudo apt-get install -y liblz4-dev
sudo apt-get install -y libnuma-dev

# Build GDAL2
if [ ! "$(ls -A ${HOME}/.gdal)" ]; then
  wget -O gdal.tar.gz "http://download.osgeo.org/gdal/2.1.3/gdal-2.1.3.tar.gz"
  tar -xvzf gdal.tar.gz
  cd gdal-2.1.3
  ./configure --prefix=${HOME}/.gdal --with-geos=/usr --with-static-proj4=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal
  make
  make install
  cd ..
else 
  printf 'GDAL will not be built (%s/.gdal not empty).\n' "$HOME"
fi

# Build wxWidgets
if [ ! "$(ls -A ${HOME}/.wxwidgets)" ]; then
  wget -O wxwidgets.tar.bz2 "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2"
  tar -xvjf wxwidgets.tar.bz2
  cd wxWidgets-3.1.0
  ./configure --prefix=${HOME}/.wxwidgets --enable-unicode --disable-shared --enable-mediactrl=no
  make
  make install
  cd ..
else 
  printf 'wxWidgets will not be built (%s/.wxwidgets not empty).\n' "$HOME"
fi

# Build wxPdfDocument
if [ ! "$(ls -A ${HOME}/.wxpdfdoc)" ]; then
  wget -O wxpdfdoc.tar.gz "https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.tar.gz"
  tar -xvzf wxpdfdoc.tar.gz
  cd wxpdfdoc-0.9.5
  ./configure --prefix=${HOME}/.wxpdfdoc --enable-unicode --disable-shared
  make
  make install
  cd ..
else 
  printf 'wxPdfDoc will not be built (%s/.wxpdfdoc not empty).\n' "$HOME"
fi

# CMake
mkdir bin
cd bin
cmake -DSEARCH_GIS_LIB_PATH=${HOME}/.gdal -DMYSQL_MAIN_DIR=/usr -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 ../build
