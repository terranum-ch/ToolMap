#! /bin/sh
# Script for creating linux (ubuntu) debug bin
# for ToolMap 2
# (c) Lucien Schreiber CREALP 2009

# 1) Checkout the latest svn trunk
# 2) Update makefile using cmake (must exist)
# 3) run makefile

#PARAMETERS
VARBINDIR="/home/maison/programmation/toolmap2/bin/dbg"
VARTRUNKDIR="/home/maison/programmation/toolmap2/trunk"
#not used actually
VARINSTALLERDIR="/home/maison/programmation/toolmap2/bin"
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
make --silent  --no-print-directory
echo "3)Building project... DONE"
echo $VARLINE

#creating installer
#echo "4) Creating installer..."
#echo "4) Creating installer... DONE"


