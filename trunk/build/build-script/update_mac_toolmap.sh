#! /bin/sh
# Script for creating mac release bin
# for ToolMap 2
# (c) Lucien Schreiber CREALP 2009

# 1) Checkout the latest svn trunk
# 2) Update makefile using cmake (must exist)
# 3) run makefile
# 4) create dmg 

#PARAMETERS
VARBINDIR="/Users/Lucien/DATA/PRJ/TOOLMAP2/bin/mac_cmake_release"
VARTRUNKDIR="/Users/Lucien/DATA/PRJ/TOOLMAP2/trunk"
VARINSTALLERDIR="/Users/Lucien/DATA/PRJ/TOOLMAP2/bin"
VARLINE="----------------------------------"

#title
echo $VARLINE
echo "   Creating ToolMap binary "
echo "  (c) Lucien Schreiber CREALP"
echo $VARLINE

#updating svn
echo "1) Getting last SVN version..."
cd $VARTRUNKDIR
svn update
VARSVNNUMBER=`svnversion -n`
echo '1) Getting last SVN version... DONE (\c'
echo  $VARSVNNUMBER ')'	
echo $VARLINE

#update cmake project file
echo "2) Updating cmake file..."
cmake $VARBINDIR
echo "2) Updating cmake file... DONE"
echo $VARLINE

#compiling project
echo "3)Building project..."
cd $VARBINDIR
xcodebuild -configuration Release
echo "3)Building project... DONE"
echo $VARLINE

#creating installer
echo "4) Creating installer..."
cd $VARINSTALLERDIR
hdiutil create -srcfolder $VARBINDIR/Release ToolMap2_r$VARSVNNUMBER.dmg
echo "4) Creating installer... DONE"


