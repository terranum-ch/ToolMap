#!/usr/bin/env sh

# CMake
mkdir bin
cd bin
cmake -DSEARCH_GIS_LIB_PATH=${HOME}/.gdal -DMYSQL_MAIN_DIR=/usr -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 ../build
