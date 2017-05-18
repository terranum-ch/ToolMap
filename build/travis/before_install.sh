#!/usr/bin/env sh

# Install required libraries
sudo apt-get -qq update
sudo apt-get install -y libwxgtk3.0-dev
sudo apt-get install -y libwxgtk-media3.0-dev
sudo apt-get install -y libwxgtk-webview3.0-dev
sudo apt-get install -y libproj-dev
sudo apt-get install -y libgeos-dev
sudo apt-get install -y libmariadbd-dev
sudo apt-get install -y libaio-dev
sudo apt-get install -y liblz4-dev
sudo apt-get install -y libnuma-dev

# Build GDAL2
if [ ! -d "${HOME}/.gdal" ]; then
  cd(${HOME}/.gdal)
  wget -O gdal.tar.gz "http://download.osgeo.org/gdal/2.1.3/gdal-2.1.3.tar.gz"
  tar -xvzf gdal.tar.gz
  cd gdal-2.1.3
  ./configure --with-geos=/usr --with-static-proj4=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal
  make
  sudo make install
  cd(${TRAVIS_BUILD_DIR})
fi

# Build wxPdfDocument
if [ ! -d "${HOME}/.wxpdfdoc" ]; then
  cd(${HOME}/.wxpdfdoc)
  wget -O wxpdfdoc.tar.gz "https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.tar.gz"
  tar -xvzf wxpdfdoc.tar.gz
  cd wxpdfdoc-0.9.5
  ./configure --enable-unicode --disable-shared
  make
  sudo make install
  cd(${TRAVIS_BUILD_DIR})
fi

# CMake
mkdir bin
cd bin
cmake -DSEARCH_GIS_LIB_PATH=/usr -DMYSQL_MAIN_DIR=/usr -DSEARCH_WXPDFDOCUMENT_PATH=/usr/local -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 ../build
