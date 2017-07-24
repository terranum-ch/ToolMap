#!/bin/bash

REBUILD_MYSQL=false

# Build MySQL
if [ ! "$(ls -A ${HOME}/.mysql)" ] || [ "$REBUILD_MYSQL" = true ]; then
  if [ "$REBUILD_MYSQL" = true ]; then
    rm -rf ${HOME}/.mysql
  fi
  sudo apt-get install -y libncurses5-dev bison
  wget -q -O mysql.tar.gz "https://github.com/mysql/mysql-server/archive/mysql-5.6.36.tar.gz"
  tar -xzf mysql.tar.gz
  cd mysql-server-mysql-5.6.36
  pwd
  patch sql/sql_table.cc < ../build/patches/sql_table.cc.patch
  cmake . -DCMAKE_INSTALL_PREFIX=${HOME}/.mysql -DWITH_UNIT_TESTS:BOOL=OFF -DFEATURE_SET:STRING=small
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'MySQL will not be built (%s/.mysql not empty).\n' "$HOME"
fi
