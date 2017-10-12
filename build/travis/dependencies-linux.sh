#!/usr/bin/env sh

sudo apt-get -qq update
sudo apt-get install -y git cmake
sudo apt-get install -y libcurl4-openssl-dev
sudo apt-get install -y libproj-dev
sudo apt-get install -y libgeos-dev
sudo apt-get install -y libaio-dev
sudo apt-get install -y libnuma-dev
sudo apt-get install -y libwebkitgtk-dev
sudo apt-get install -y libwrap0-dev
sudo apt-get install -y cxxtest

# Build libraries
chmod +x build/travis/build-wxwidgets.sh
build/travis/build-wxwidgets.sh
chmod +x build/travis/build-gdal.sh
build/travis/build-gdal.sh
chmod +x build/travis/build-wxpdfdoc.sh
build/travis/build-wxpdfdoc.sh
chmod +x build/travis/build-mysql.sh
build/travis/build-mysql.sh