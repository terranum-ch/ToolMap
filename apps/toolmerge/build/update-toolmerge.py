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
from Tkinter import *



def GetCmakeFileName():
	myFileName = os.path.split(__file__)
	myCmakefilename = myFileName[0]
	return myCmakefilename

def buildMacBook():
	builddir = "/Users/lucien/DATA/PRJ/TOOLMAP2/bin_apps/toolmerge"
	try:
		p = Popen("cmake -GXcode " + GetCmakeFileName() + "  -DCMAKE_OSX_ARCHITECTURES:TEXT=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET:TEXT=10.6 -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBWXSVN/bin/wx-config -DMYSQL_MAIN_DIR:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBMYSQL -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBGIS", shell=True, cwd=builddir)
		p.wait()
	except:
		print("Error creating makefile")


def buildMacPro():
	builddir = "/Users/lucien/Documents/PRJ/TOOLMAP2/bin_apps"
	try:
		p = Popen("cmake -GXcode " + GetCmakeFileName() + "  -DCMAKE_OSX_ARCHITECTURES:TEXT=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET:TEXT=10.6 -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=/Users/lucien/Documents/PROGRAMMATION/64/_LIBWXSVN/bin/wx-config -DMYSQL_MAIN_DIR:PATH=/Users/lucien/Documents/PROGRAMMATION/64/_LIBMYSQL -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=/Users/lucien/Documents/PROGRAMMATION/64/_LIBGIS", shell=True, cwd=builddir)
		p.wait()
	except:
		print("Error creating makefile")

if __name__ == '__main__':
	root = Tk()
	myContainer1 = Frame(root)
	myContainer1.pack()

	label = Label(myContainer1, text="ToolMerge")
	label.pack()

	button2 = Button(myContainer1, text="Configure MacBook", command=buildMacBook)
	button2.pack()

	button1 = Button(myContainer1, text="Configure Mac Pro (UNIL)", command=buildMacPro)
	button1.pack()

	root.mainloop()
