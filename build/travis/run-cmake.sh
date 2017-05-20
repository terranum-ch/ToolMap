#!/usr/bin/env sh

# CMake
mkdir bin
cd bin
cmake -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_GEOS_PATH=/usr -DMYSQL_MAIN_DIR=/usr -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 ../build
