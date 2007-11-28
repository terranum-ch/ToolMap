#!/bin/sh

# script for making bundle application on mac osX
# part of the ToolMap 2 project
# (c) Lucien Schreiber CREALP 2007
PNAME="ToolMap2.app"
LNAME="ToolMap2"

echo "Creating bundle for mac os X"

echo "Removing existing bundle"
	rm -d -R  $PNAME
	
echo "Creating new bundle"
	mkdir -v $PNAME
	mkdir -v $PNAME"/Contents"
	mkdir -v $PNAME"/Contents/MacOS"
	mkdir -v $PNAME"/Contents/Resources"

echo "Copying info.plist into bundle"
	cp -v $LNAME"-Info.plist" $PNAME"/Contents/Info.plist"
	
echo "Copying program to bundle"
	cp -v $LNAME $PNAME"/Contents/MacOS/"
	