#!/usr/bin/env sh

# Build GDAL2
if [ ! "$(ls -A ${HOME}/.gdal)" ]; then
  wget -O gdal.tar.gz "http://download.osgeo.org/gdal/2.1.3/gdal-2.1.3.tar.gz"
  tar -xzf gdal.tar.gz
  cd gdal-2.1.3
  ./configure --prefix=${HOME}/.gdal --with-geos=/usr --with-static-proj4=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal --silent --enable-silent-rules
  travis_wait make -s
  make install
  cd ..
else 
  printf 'GDAL will not be built (%s/.gdal not empty).\n' "$HOME"
fi
