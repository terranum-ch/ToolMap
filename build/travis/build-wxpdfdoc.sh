#!/usr/bin/env sh

# Build wxPdfDocument
if [ ! "$(ls -A ${HOME}/.wxpdfdoc)" ]; then
  wget -q -O wxpdfdoc.tar.gz "https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.tar.gz" > /dev/null
  tar -xzf wxpdfdoc.tar.gz
  cd wxpdfdoc-0.9.5
  if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    ./configure --prefix=${HOME}/.wxpdfdoc --with-wx-prefix=${HOME}/.wxwidgets --enable-unicode --disable-shared --silent
  else
    ./configure --prefix=${HOME}/.wxpdfdoc --with-wx-prefix=/usr/local --enable-unicode --disable-shared --silent
  fi
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
  printf 'wxPdfDoc was built.\n'
else 
  printf 'wxPdfDoc will not be built (%s/.wxpdfdoc not empty).\n' "$HOME"
fi
