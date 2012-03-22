#!/ur/bin/env python
# -*- coding: utf-8 -*-
#
#       create-package-cde.py
#       
#       Copyright 2012 Lucien Schreiber <lucien.schreiber@crealp.vs.ch>
#       
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.
#       
#       

import os
import subprocess
import shutil

def LocateCDE ():
	""" Function search CDE bin"""
	CDEPath = input("CDE path: ")
	#CDEPath = "/home/lucien/programmation/CDE"
	if (CDEPath == ""):
		print("CDE Path is empty")
		exit()
	return CDEPath



def CleanDirectory (directory):
	""" Clean Dir if exists"""
	try:
		shutil.rmtree(directory)
	except:
		print("Unable to remove directory :",directory)
	


def patchOptionFile(filepathname, patchfilename):
	"""patch CDE option file seemless integration"""
	print ("Patching file:", filepathname)
	shutil.copyfile(patchfilename, filepathname)
	print ("Ok file patched!")
	pass


if __name__ == '__main__':
	myScriptPathFileName = os.path.split(__file__)
	myScriptName = myScriptPathFileName[0]
	
	myCDEPath = LocateCDE()
	myPackagePath=myCDEPath+os.sep+"ToolMap.Package"
	
	myVersiontxt = input("Version number (format 2.3.1199): ")
	#myVersiontxt = "2.1.1199"
	myPlateform = input("Plateform: (amd64 or i386): ")
	#myPlateform="amd64"
	
	#myInstallDirectory = input("Install directory (where to put result file): ")
	myTrunkDirectory = input("ToolMap Trunk directory: ")

	# RUN CDE to create ToolMap package only if not exists
	if (os.path.exists(myPackagePath)==True):
		print("Package exists : ", myPackagePath)
		exit()
	try:
		p = subprocess.Popen("./cde -o ToolMap.Package ToolMap", shell=True, cwd=myCDEPath)
		p.wait()
	except:
		print("Creating ToolMap package failed!")
		exit()
	print("Creating ToolMap package success!")
	
	# tuning cde.option
	patchOptionFile(myPackagePath+os.sep+"cde.options", myScriptName+os.sep+"cde.options.template")

	# removing directories
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Bureau")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Documents")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Public")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Téléchargements")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Desktop")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Downloads")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Music")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Pictures")
	CleanDirectory(myPackagePath+os.sep+"cde-root/home/lucien/Videos")
	
	# adding icon into package
	shutil.copyfile(myTrunkDirectory+os.sep+"art"+os.sep+"toolmap.png",myPackagePath+os.sep+"toolmap.png") 
	
	# adding default preferences into package
	shutil.copyfile(myTrunkDirectory+os.sep+"install"+os.sep+"linux"+os.sep+"ToolMap.pref",myPackagePath+os.sep+"cde-root/home/lucien/.ToolMap") 

	# copy install and readme file locally for archive
	shutil.copyfile(myTrunkDirectory+os.sep+"install"+os.sep+"linux"+os.sep+"readme.txt", myCDEPath+os.sep+"readme.txt")
	shutil.copyfile(myTrunkDirectory+os.sep+"install"+os.sep+"linux"+os.sep+"install-toolmap.py", myCDEPath+os.sep+"install-toolmap.py")

	# compressing package
	try:
		p=subprocess.Popen("tar -Jcvf {0} {1} {2} {3}".format("toolmap-package"+"_"+myVersiontxt+"_"+myPlateform+".tar.lzma","ToolMap.Package", "readme.txt", "install-toolmap.py"),shell=True,cwd=myCDEPath)
		p.wait()
	except:
		print("Failed to compress :", myPackagePath)
		exit() 
	
	myInstallDirectory =os.path.dirname(myTrunkDirectory)+os.sep+"install"
	# moving package to install directory
	shutil.move(myCDEPath+os.sep+"toolmap-package"+"_"+myVersiontxt+"_"+myPlateform+".tar.lzma",myInstallDirectory+os.sep+"toolmap-package"+"_"+myVersiontxt+"_"+myPlateform+".tar.lzma")
	
	
