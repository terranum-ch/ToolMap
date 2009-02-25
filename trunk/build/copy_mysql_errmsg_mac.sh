#! /bin/sh
# Script for adding errmsg.sys file
# into ToolMap bundle (mac only)
# for ToolMap 2
# (c) Lucien Schreiber CREALP 2009
# pass adress of toolmap.app as parameter. ($1)

# 1. Create directory structure
# 2. Copy file

VARMYSQL="/Users/Lucien/DATA/PROGRAMATION/LIB_MAC/LIB_MYSQL"

if [ $1 ]; then
	mkdir $1/Contents/share
	mkdir $1/Contents/share/english
	cp $VARMYSQL/share/mysql/english/errmsg.sys $1/Contents/share/english/errmsg.sys
else
	echo "Error copying errmsg.sys"
fi