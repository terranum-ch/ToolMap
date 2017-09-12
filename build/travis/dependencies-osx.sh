#!/usr/bin/env sh

brew install proj
brew upgrade geos

wget -q -O cxxtest.tar.gz "https://github.com/CxxTest/cxxtest/archive/4.3.tar.gz"
tar -xzf cxxtest.tar.gz -C ${HOME}

# Build libraries
chmod +x build/travis/build-wxwidgets-osx.sh
build/travis/build-wxwidgets-osx.sh
chmod +x build/travis/build-gdal-osx.sh
build/travis/build-gdal-osx.sh
chmod +x build/travis/build-wxpdfdoc.sh
build/travis/build-wxpdfdoc.sh
chmod +x build/travis/build-mysql.sh
build/travis/build-mysql.sh