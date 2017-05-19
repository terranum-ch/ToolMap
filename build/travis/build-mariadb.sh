#!/usr/bin/env sh

# Build MariaDB
if [ ! "$(ls -A ${HOME}/.mariadb)" ]; then
  wget -O mariadb.tar.gz "https://github.com/MariaDB/server/archive/mariadb-10.1.23.tar.gz"
  tar -xzf mariadb.tar.gz
  cd server-mariadb-10.1.23
  cmake . -DCMAKE_INSTALL_PREFIX=${HOME}/.mariadb -DPLUGIN_TOKUDB=NO -DWITH_EMBEDDED_SERVER=1 -DDISABLE_SHARED=ON
  make -s
  make install
  cd ..
else 
  printf 'MariaDB will not be built (%s/.mariadb not empty).\n' "$HOME"
fi
