#!/bin/bash

REBUILD_PROJ=false

# Build PROJ
if [ ! "$(ls -A ${HOME}/.proj)" ] || [ "$REBUILD_PROJ" = true ]; then
  if [ "$REBUILD_PROJ" = true ]; then
    rm -rf ${HOME}/.proj
  fi
  wget -q -O proj.tar.gz "https://github.com/OSGeo/PROJ/releases/download/6.2.1/proj-6.2.1.tar.gz" > /dev/null
  tar -xzf proj.tar.gz
  cd proj-6.2.1
  ./configure --prefix=${HOME}/.proj --disable-shared --silent
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'Proj will not be built (%s/.proj not empty).\n' "$HOME"
fi
