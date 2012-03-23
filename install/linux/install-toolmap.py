#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       install-toolmap.py
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
import shutil
import subprocess


if __name__ == '__main__':
	print("---------------------------")
	print("     ToolMap   Installer   ")
	print("---------------------------")
	print("This program will install ToolMap on your hard drive")
	print("Where do you want to install ToolMap ?")
	myInstallDir = raw_input("(default is /opt): /opt" + chr(8)*4)
	if not myInstallDir:
		myInstallDir = "/opt"
	myInstalName = os.path.join(myInstallDir,"ToolMap.Package") 
	print("Will install ToolMap in '{0}'".format(myInstalName))
	myContinue = raw_input("Continue ? [Y/N]: ")
	if (myContinue.lower() != "y"):
		exit()
	print("Installing...")
	
	# remove old version if needed!
	if (os.path.exists(myInstalName) == True):
		print("Removing old version of ToolMap")
		try:
			myProcess = subprocess.Popen("sudo rm -rf {0}".format(myInstalName),shell=True)
			myProcess.wait()
		except:
			print ("Unable to remove old ToolMap version")
		print("Old version removed OK!")
	
	# copy new version
	myScriptPathFileName = os.path.split(__file__)
	myScriptPath = myScriptPathFileName[0]
	myLocalPackagePath = os.path.join(myScriptPath,"ToolMap.Package")
	if (os.path.exists(myLocalPackagePath)==False):
		print ("'{}' Not found, installer corrupted !".format(myLocalPackagePath))
		print ("Try downloading again from www.toolmap.ch")
		exit()
	try:
		myProcess = subprocess.Popen("sudo cp -Rp \"{0}\" \"{1}\"".format(myLocalPackagePath, myInstalName),shell=True)
		myProcess.wait()
	except:
		print("Installing failed!")
		exit()
	print ("ToolMap copied OK!")
	
	# allow acces to any user
	try:
		myProcess = subprocess.Popen("sudo chmod -R 777 \"{0}\"".format(myInstalName),shell=True)
		myProcess.wait()
	except:
		print ("Error setting access to all user!")
		
	
	# make and install shortcut
	myDestopContent = ("[Desktop Entry]",
	"Encoding=UTF-8", "Version=2.3", "Name=ToolMap", 
	"Name[fr]=ToolMap", "Comment=Tool for creating GIS data", 
	"Comment[fr]=Outil de création de données SIG", "Exec={0}".format(myInstalName+os.sep+"ToolMap.cde"),
	"Icon={0}".format(myInstalName+os.sep+"toolmap.png"), "Terminal=false", "Type=Application", 
	"Categories=Education;Application")
	with open(os.path.join(myScriptPath,"ToolMap.desktop"), 'w') as myFile:
		for item in myDestopContent:
			print >> myFile, item
	
	try:
		myProcess = subprocess.Popen("sudo mv -f \"{0}\" \"{1}\"".format(os.path.join(myScriptPath,"ToolMap.desktop"), "/usr/share/applications/ToolMap.desktop"), shell=True)
		myProcess.wait()
	except:
		print("Shortcut not copied! Error!")
	print ("Installation completed!")
	print ("ToolMap is now installed, you may now remove downloaded files")

	
