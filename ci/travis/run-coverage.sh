#!/usr/bin/env bash

mkdir bin
cd bin
cmake -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_GEOS_PATH=/usr -DMYSQL_MAIN_DIR=${HOME}/.mysql -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 -DUSE_UNITTEST=1 -DUSE_CODECOV=1 -DCMAKE_BUILD_TYPE=Debug -DCXXTEST_INCLUDE_DIR=/usr -DCXXTEST_PYTHON_TESTGEN_EXECUTABLE=/usr/bin/cxxtestgen ..
make -j$(nproc) ToolMap-coverage > /dev/null
lcov --directory .. --capture --output-file coverage.info &> /dev/null
lcov --remove coverage.info '/usr/*' 'test/*' 'bin/*' '*/lib/*' '*/include/*' --output-file coverage.info --quiet > /dev/null
bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports" 
