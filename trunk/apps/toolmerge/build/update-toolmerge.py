#!/usr/bin/env python
# encoding: utf-8
"""
update-toolmerge.py

Created by Lucien Schreiber on 2012-08-09.
Copyright (c) 2012 . All rights reserved.
"""

import sys
import os
from subprocess import *

builddir = "/Users/lucien/DATA/PRJ/TOOLMAP2/bin_apps"

if __name__ == '__main__':
	myFileName = os.path.split(__file__)
	try:
		p = Popen("cmake -GXcode " + myFileName[0] + "  -DCMAKE_OSX_ARCHITECTURES:TEXT=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET:TEXT=10.6 -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBWXSVN/bin/wx-config -DMYSQL_MAIN_DIR:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBMYSQL -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBGIS", shell=True, cwd=builddir)
		p.wait()
	except:
		print("Error creating makefile")
