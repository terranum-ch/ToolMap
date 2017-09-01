#!/usr/bin/env sh

brew install proj
brew install geos

wget -q -O cxxtest.tar.gz "https://github.com/CxxTest/cxxtest/archive/4.3.tar.gz"
tar -xzf cxxtest.tar.gz

# Build libraries
chmod +x build/travis/build-wxwidgets-osx.sh
build/travis/build-wxwidgets-osx.sh
chmod +x build/travis/build-gdal.sh
build/travis/build-gdal.sh
chmod +x build/travis/build-wxpdfdoc.sh
build/travis/build-wxpdfdoc.sh
chmod +x build/travis/build-mysql.sh
build/travis/build-mysql.sh