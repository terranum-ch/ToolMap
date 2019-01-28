#!/bin/bash

REBUILD_GDAL=true

# Build GDAL2
if [ ! "$(ls -A ${HOME}/.gdal)" ] || [ "$REBUILD_GDAL" = true ]; then
  if [ "$REBUILD_GDAL" = true ]; then
    rm -rf ${HOME}/.gdal
  fi
  wget -q -O gdal.tar.gz "http://download.osgeo.org/gdal/2.3.0/gdal-2.3.0.tar.gz" > /dev/null
  tar -xzf gdal.tar.gz
  cd gdal-2.3.0
  ./configure --prefix=${HOME}/.gdal --with-geos=/usr/bin/geos-config --with-curl=/usr/bin/curl-config --with-proj=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal --with-threads=yes --silent
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'GDAL will not be built (%s/.gdal not empty).\n' "$HOME"
fi
