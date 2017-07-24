#!/usr/bin/env sh

mkdir bin
cd bin
cmake -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_GIS_LIB_PATH=${HOME}/.gdal -DSEARCH_GEOS_PATH=/usr/local -DMYSQL_MAIN_DIR=${HOME}/.mysql -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 -DUSE_UNITTEST=1 -DCXXTEST_INCLUDE_DIR=/usr/local -DCXXTEST_PYTHON_TESTGEN_EXECUTABLE=/usr/local/bin/cxxtestgen ..
make -j4
