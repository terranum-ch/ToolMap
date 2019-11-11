#!/bin/bash

# Build PROJ
wget -q -O proj.tar.gz "https://github.com/OSGeo/PROJ/releases/download/6.2.1/proj-6.2.1.tar.gz" > /dev/null
tar -xzf proj.tar.gz
cd proj-6.2.1
./configure --prefix=/usr/local --silent
make -j4 > /dev/null
sudo make install > /dev/null
cd ..
