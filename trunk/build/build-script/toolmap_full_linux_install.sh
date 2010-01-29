#! /bin/sh
# Script for creating linux (ubuntu) 
# ToolMap 2
# (c) Lucien Schreiber CREALP 2010

#set up the following parameters
#PARAMETERS
INSTALLPATH="/home/lucien/Documents/ToolMap"
LIBPATH="/home/lucien/Documents/LIB"



#END PARAMETERS
#don't touch bellow.
VARLINE="----------------------------------"
TRUNKPATH="$INSTALLPATH/trunk"
BINPATH="$INSTALLPATH/bin"
LIBGIS="$LIBPATH/_LIBGIS"
LIBMYSQL="$LIBPATH/_LIBMYSQL"




#installing libraries
#curl
sudo apt-get install libcurl4-gnutls-dev




#download
echo $VARLINE
echo "   Creating ToolMap binary "
echo "  (c) Lucien Schreiber CREALP"
echo $VARLINE


#updating svn
echo "1) Getting last SVN version..."
mkdir $INSTALLPATH


cd $INSTALLPATH
svn co https://213.221.129.20/svncrealp/toolmap2/trunk $TRUNKPATH
VARSVNNUMBER=`svnversion -n`
echo '1) Getting last SVN version... DONE (\c'
echo  $VARSVNNUMBER ')'	
echo $VARLINE



# configure 
echo "2) Configure..."
mkdir $BINPATH
cd $BINPATH
cmake -G"CodeBlocks - Unix Makefiles" "$TRUNKPATH/build"  -DCMAKE_BUILD_TYPE:TEXT="Release" -DSEARCH_GDAL:BOOL=1 -DSEARCH_GEOS:BOOL=1 -DSEARCH_GIS_LIB_PATH:PATH=$LIBGIS -DMYSQL_IS_LOGGING:BOOL=1 -DMYSQL_MAIN_DIR:PATH=$LIBMYSQL -DSEARCH_CURL:BOOL=1 


make


