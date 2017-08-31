#!/usr/bin/env sh

#brew install wxmac
brew install proj
brew install geos
brew install cxxtest

# Build libraries
chmod +x build/travis/build-wxwidgets-osx.sh
build/travis/build-wxwidgets-osx.sh
chmod +x build/travis/build-gdal.sh
build/travis/build-gdal.sh
chmod +x build/travis/build-wxpdfdoc.sh
build/travis/build-wxpdfdoc.sh
chmod +x build/travis/build-mysql.sh
build/travis/build-mysql.sh