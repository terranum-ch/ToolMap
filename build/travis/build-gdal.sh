#!/usr/bin/env sh

# Build GDAL2
if [ ! "$(ls -A ${HOME}/.gdal)" ]; then
  wget -q -O gdal.tar.gz "http://download.osgeo.org/gdal/2.1.3/gdal-2.1.3.tar.gz" > /dev/null
  tar -xzf gdal.tar.gz
  cd gdal-2.1.3
  ./configure --prefix=${HOME}/.gdal --with-geos=/usr/bin/geos-config --with-static-proj4=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal --silent
  make -j2 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'GDAL will not be built (%s/.gdal not empty).\n' "$HOME"
fi
