#!/usr/bin/env sh

#brew install wxmac
brew install proj
brew install geos
brew install cxxtest

# wxWidgets
chmod +x build/travis/build-wxwidgets.sh
build/travis/build-wxwidgets.sh