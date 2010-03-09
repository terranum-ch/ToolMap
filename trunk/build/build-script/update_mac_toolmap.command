#! /bin/sh
# Script for creating mac release bin
# for ToolMap 2
# (c) Lucien Schreiber CREALP 2009

# 1) Checkout the latest svn trunk
# 2) Update makefile using cmake (must exist)
# 3) run makefile
# 4) create dmg 

#PARAMETERS
VARBINDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/bin"
VARTRUNKDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/trunk"
VARINSTALLERDIR="/Users/lucien/DATA/PRJ/TOOLMAP2/bin"
VARLINE="----------------------------------"

# PARAMETERS
PARAMGISPATH=/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBGIS
PARAMWXPATH=/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBWX
PARAMMYSQLPATH=/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBMYSQL
PARAMUNITTEST=1
PARAMUNITESTLIB=/Users/lucien/DATA/PROGRAMATION/_LIB/cxxtest
PARAMCURLLIBSEARCH=1
PARAMUNIT_TESTING_PATH=/Users/lucien/DATA/PRJ/TOOLMAP2/unit_testing

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

cd $VARBINDIR
[ -d 32-dbg ] || mkdir 32-dbg
cd $VARBINDIR/32-dbg

cmake -GXcode $VARTRUNKDIR/build -DCMAKE_OSX_ARCHITECTURES:TEXT="i386" -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=$PARAMGISPATH -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE="$PARAMWXPATH/bin/wx-config" -DMYSQL_MAIN_DIR:PATH=$PARAMMYSQLPATH -DMYSQL_IS_LOGGING:BOOL=0 -DSVN_DURING_BUILD:BOOL=1 -DCMAKE_BUILD_TYPE:TEXT="Debug" -DSEARCH_CURL:BOOL=$PARAMCURLLIBSEARCH -DUSE_UNITTEST:BOOL=$PARAMUNITTEST -DUNIT_TESTING_PATH=$PARAMUNIT_TESTING_PATH -DCXXTEST_DIRECTORY=$PARAMUNITESTLIB

echo "2) Updating cmake file... DONE"
echo $VARLINE

#compiling project
echo "3)Building project..."
cd $VARBINDIR/32-dbg
xcodebuild -configuration Debug
echo "3)Building project... DONE"
echo $VARLINE




#RELEASE VERSION
echo -n "Build release version (y/n)"
read value

if [ "$value" == "n" ]  
then 
exit 0 
fi

cd $VARBINDIR
[ -d 32-rel ] || mkdir 32-rel
cd $VARBINDIR/32-rel



#update cmake project file
echo "2) Updating cmake file..."

cmake -GXcode $VARTRUNKDIR/build -DCMAKE_OSX_ARCHITECTURES:TEXT="i386" -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=$PARAMGISPATH -DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE="$PARAMWXPATH/bin/wx-config" -DMYSQL_MAIN_DIR:PATH=$PARAMMYSQLPATH -DMYSQL_IS_LOGGING:BOOL=1 -DSVN_DURING_BUILD:BOOL=1 -DCMAKE_BUILD_TYPE:TEXT="Release" -DSEARCH_CURL:BOOL=$PARAMCURLLIBSEARCH


echo "2) Updating cmake file... DONE"
echo $VARLINE

#compiling project
echo "3)Building project..."
cd $VARBINDIR/32-rel
xcodebuild -configuration Release
echo "3)Building project... DONE"
echo $VARLINE

echo -n "Build installer (y/n) ?"
read value

if [ "$value" == "n" ]  
then 
exit 0 
fi



#copying file for installer
echo "4a) Copy files for installer"
cd $VARTRUNKDIR/install/mac/background/
mkdir $VARBINDIR/32-rel/Release/.background
cp tm_dmg_background.jpg $VARBINDIR/32-rel//Release/.background
cp Applications $VARBINDIR/32-rel//Release
cp InstallDS_Store $VARBINDIR/32-rel//Release/.DS_Store
echo "4a) Copy files for installer... DONE"

#move buggy files  out from folder
echo "4b) Move static library out of folder"
cd $VARBINDIR/32-rel//Release
mv libToolMap2_lib.a $VARBINDIR/32-rel/libToolMap2_lib.a

#creating installer
echo "4) Creating installer..."
cd $VARINSTALLERDIR
hdiutil create -volname "ToolMap" -srcfolder $VARBINDIR/32-rel/Release ToolMap2_r$VARSVNNUMBER.dmg
echo "4) Creating installer... DONE"

#move buggy files back into folder
echo "5) Move static library back into folder"
cd $VARBINDIR/32-rel
mv libToolMap2_lib.a $VARBINDIR/32-rel/Release/libToolMap2_lib.a
