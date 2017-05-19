#!/usr/bin/env sh

# Build MariaDB
if [ ! "$(ls -A ${HOME}/.mariadb)" ]; then
  wget -q -O mariadb.tar.gz "https://github.com/MariaDB/server/archive/mariadb-10.1.23.tar.gz" > /dev/null
  tar -xzf mariadb.tar.gz
  cd server-mariadb-10.1.23
  cmake . -DCMAKE_INSTALL_PREFIX=${HOME}/.mariadb -DPLUGIN_TOKUDB=NO -DWITH_EMBEDDED_SERVER=1 -DDISABLE_SHARED=ON > /dev/null
  make > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'MariaDB will not be built (%s/.mariadb not empty).\n' "$HOME"
fi
