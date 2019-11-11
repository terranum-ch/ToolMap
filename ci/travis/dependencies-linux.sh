#!/usr/bin/env sh

sudo apt-get -qq update
sudo apt-get install -y git cmake
sudo apt-get install -y libcurl4-openssl-dev
sudo apt-get install -y libgeos-dev
sudo apt-get install -y libaio-dev
sudo apt-get install -y libnuma-dev
sudo apt-get install -y libwebkitgtk-dev
sudo apt-get install -y libwrap0-dev
sudo apt-get install -y cxxtest

# Build libraries
chmod +x ci/travis/build-wxwidgets.sh
ci/travis/build-wxwidgets.sh
chmod +x ci/travis/build-proj.sh
ci/travis/build-proj.sh
chmod +x ci/travis/build-gdal.sh
ci/travis/build-gdal.sh
chmod +x ci/travis/build-wxpdfdoc.sh
ci/travis/build-wxpdfdoc.sh
chmod +x ci/travis/build-mysql.sh
ci/travis/build-mysql.sh