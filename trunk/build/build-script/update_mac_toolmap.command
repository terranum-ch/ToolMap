#! /bin/sh
# Script for creating mac release bin
# for ToolMap 2
# (c) Lucien Schreiber CREALP 2009

# 1) Checkout the latest svn trunk
# 2) Update makefile using cmake (must exist)
# 3) run makefile
# 4) create dmg 

#PARAMETERS
VARBINDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/bin/32_rel"
VARBINDIR_DBG="/Users/lucien/DATA/PRJ/TOOLMAP2/bin/32_dbg"
VARTRUNKDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/trunk"
VARINSTALLERDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/bin"
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


# DEBUG VERSION
#update cmake project file
echo "2) Updating cmake file...(DEBUG)"
cmake $VARBINDIR_DBG
echo "2) Updating cmake file... DONE"
echo $VARLINE

#compiling project
echo "3)Building project..."
cd $VARBINDIR_DBG
xcodebuild -configuration Debug
echo "3)Building project... DONE"
echo $VARLINE


echo -n "Build release version and installer (y/n) ?"
read value

if [ "$value" == "n" ]  
then 
exit 0 
fi







#RELEASE VERSION
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

#copying file for installer
echo "4a) Copy files for installer"
cd $VARTRUNKDIR/install/mac/background/
mkdir $VARBINDIR/Release/.background
cp tm_dmg_background.jpg $VARBINDIR/Release/.background
cp Applications $VARBINDIR/Release
cp InstallDS_Store $VARBINDIR/Release/.DS_Store
echo "4a) Copy files for installer... DONE"

#move buggy files  out from folder
echo "4b) Move static library out of folder"
cd $VARBINDIR/Release
mv libToolMap2_lib.a $VARBINDIR/libToolMap2_lib.a

#creating installer
echo "4) Creating installer..."
cd $VARINSTALLERDIR
hdiutil create -volname "ToolMap" -srcfolder $VARBINDIR/Release ToolMap2_r$VARSVNNUMBER.dmg
echo "4) Creating installer... DONE"

#move buggy files back into folder
echo "5) Move static library back into folder"
cd $VARBINDIR
mv libToolMap2_lib.a $VARBINDIR/Release/libToolMap2_lib.a
