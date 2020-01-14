#!/usr/bin/env python
# encoding: utf-8
"""
update-shpimporter.py

Created by Lucien Schreiber on 2012-12-21.
Copyright (c) 2012 . All rights reserved.
"""

import os
import sys
from subprocess import *

#  compatibililty Python 2.7 and 3.0
try:
    from Tkinter import *
except:
    from tkinter import *


def GetCmakeDirName():
    pathname = os.path.dirname(sys.argv[0])
    return os.path.abspath(pathname)


def updateSVN():
    myPath = os.path.normpath(os.path.join(GetCmakeDirName(), ".."))
    print("Updating subversion in: {}".format(myPath))
    try:
        p = Popen("svn update", shell=True, cwd=myPath)
        p.wait()
    except:
        print("Error updating subversion in: {}".format(myPath))

    # print version number
    try:
        p = Popen("svnversion", shell=True, cwd=myPath, stdout=PIPE, stderr=PIPE)
        information = p.communicate()
        myVersion = str(information[0])
        myVersionAlone = myVersion[myVersion.find("'") + 1:-3]
        print("Updated version is: {}".format(myVersionAlone))
    except:
        print("Error getting version for {}".format(GetCmakeDirName()))


def buildMacBook():
    builddir = "/Users/lucien/DATA/PRJ/TOOLMAP2/bin_apps/shpimporter"
    try:
        p = Popen(
            "cmake -GXcode " + GetCmakeDirName() + "  -DCMAKE_OSX_ARCHITECTURES:TEXT=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET:TEXT=10.6 -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBWXSVN/bin/wx-config -DMYSQL_MAIN_DIR:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBMYSQL -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=/Users/lucien/DATA/PROGRAMATION/_LIB/64/_LIBGIS",
            shell=True, cwd=builddir)
        p.wait()
    except:
        print("Error creating makefile")


if __name__ == '__main__':
    root = Tk()
    myContainer1 = Frame(root)
    myContainer1.pack()

    label = Label(myContainer1, text="SHPIMPORTER")
    label.pack()

    button3 = Button(myContainer1, text=("Update subversion"), command=updateSVN)
    button3.pack()

    button2 = Button(myContainer1, text="Configure MacBook", command=buildMacBook)
    button2.pack()

    root.mainloop()
