#!/usr/bin/env sh

sudo apt-get -qq update
sudo apt-get install -y libproj-dev
sudo apt-get install -y libgeos-dev
sudo apt-get install -y libaio-dev
sudo apt-get install -y libnuma-dev
sudo apt-get install -y libwebkitgtk-dev
sudo apt-get install -y libwrap0-dev
sudo apt-get install -y cxxtest

# wxWidgets
sudo apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc
sudo apt-add-repository 'deb http://repos.codelite.org/wx3.1/ubuntu/ trusty universe'
sudo apt-get update
sudo apt-get install libwxbase3.1-0-unofficial libwxbase3.1-dev libwxgtk3.1-0-unofficial libwxgtk3.1-dev wx3.1-headers wx-common