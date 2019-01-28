#!/bin/bash

REBUILD_GDAL=true

# Build GDAL2
if [ ! "$(ls -A ${HOME}/.gdal)" ] || [ "$REBUILD_GDAL" = true ]; then
  if [ "$REBUILD_GDAL" = true ]; then
    rm -rf ${HOME}/.gdal
  fi
  wget -q -O gdal.tar.gz "http://download.osgeo.org/gdal/2.4.0/gdal-2.4.0.tar.gz" > /dev/null
  tar -xzf gdal.tar.gz
  cd gdal-2.4.0

  curl-config --version
  /usr/bin/curl-config --version
  /usr/bin/curl/curl-config --version
  /usr/local/curl-config --version
  /usr/local/curl/curl-config --version
  ${HOMEBREW_PREFIX}/opt/curl/bin/curl-config --version

  ./configure --prefix=${HOME}/.gdal --with-geos=/usr/bin/geos-config --with-curl=/usr --with-proj=/usr --with-sqlite3=yes --with-python=no --with-pg=no --with-grass=no --with-jasper=no --with-jpeg=internal --with-png=internal --with-threads=no --silent
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'GDAL will not be built (%s/.gdal not empty).\n' "$HOME"
fi
