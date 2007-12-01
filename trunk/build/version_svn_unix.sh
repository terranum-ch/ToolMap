#!/bin/sh
#script for getting the last svn version number 
#works only for unix like (mac, linux)
#    echo  'const char* svn_version(void) { const char* SVN_Version ="\c' > ../src/core/svn_version.cpp
	echo '#define SVN_VERSION _T("\c' > ../src/core/svn_version.h
    svnversion -n                  >> ../src/core/svn_version.h
    echo '")'   >> ../src/core/svn_version.h
    echo '' >> ../src/core/svn_version.h