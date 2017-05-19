#!/usr/bin/env sh

# Build wxPdfDocument
if [ ! "$(ls -A ${HOME}/.wxpdfdoc)" ]; then
  wget -O wxpdfdoc.tar.gz "https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.tar.gz"
  tar -xzf wxpdfdoc.tar.gz
  cd wxpdfdoc-0.9.5
  ./configure --prefix=${HOME}/.wxpdfdoc --enable-unicode --disable-shared --silent
  travis_wait make -s
  make install
  cd ..
else 
  printf 'wxPdfDoc will not be built (%s/.wxpdfdoc not empty).\n' "$HOME"
fi
