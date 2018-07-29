#!/bin/bash

REBUILD_WXPDF=false

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  WX_HOME=${HOME}/.wxwidgets
else
  WX_HOME=${HOME}/.wxwidgets
fi

# Build wxPdfDocument
if [ ! "$(ls -A ${HOME}/.wxpdfdoc)" ] || [ "$REBUILD_WXPDF" = true ]; then
  if [ "$REBUILD_WXPDF" = true ]; then
    rm -rf ${HOME}/.wxpdfdoc
  fi
  wget -q -O wxpdfdoc.tar.gz "https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.tar.gz" > /dev/null
  tar -xzf wxpdfdoc.tar.gz
  cd wxpdfdoc-0.9.5
  ./configure --prefix=${HOME}/.wxpdfdoc --with-wx-prefix=${WX_HOME} --enable-unicode --disable-shared --silent
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
  printf 'wxPdfDoc was built.\n'
else 
  printf 'wxPdfDoc will not be built (%s/.wxpdfdoc not empty).\n' "$HOME"
fi
