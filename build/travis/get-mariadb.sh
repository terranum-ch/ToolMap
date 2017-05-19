#!/usr/bin/env sh

# Get MariaDB
# https://downloads.mariadb.org/mariadb/repositories/#mirror=heanet-ltd&distro=Ubuntu&distro_release=precise--ubuntu_precise&version=10.1
sudo apt-get install python-software-properties
sudo apt-key adv --recv-keys --keyserver hkp://keyserver.ubuntu.com:80 0xcbcb082a1bb943db
sudo add-apt-repository 'deb [arch=amd64,i386] http://ftp.heanet.ie/mirrors/mariadb/repo/10.1/ubuntu precise main'
sudo apt-get update
sudo apt-get install libmariadbd-dev libmysqlclient-dev
