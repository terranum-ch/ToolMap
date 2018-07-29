#!/usr/bin/env sh

brew install proj
brew upgrade geos

wget -q -O cxxtest.tar.gz "https://github.com/CxxTest/cxxtest/archive/4.3.tar.gz"
tar -xzf cxxtest.tar.gz -C ${HOME}

# Build libraries
chmod +x ci/travis/build-wxwidgets-osx.sh
ci/travis/build-wxwidgets-osx.sh
chmod +x ci/travis/build-gdal-osx.sh
ci/travis/build-gdal-osx.sh
chmod +x ci/travis/build-wxpdfdoc.sh
ci/travis/build-wxpdfdoc.sh
chmod +x ci/travis/build-mysql.sh
ci/travis/build-mysql.sh

# Changing permissions of Homebrew libraries
sudo chmod -R 777 /usr/local/Cellar