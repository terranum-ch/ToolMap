#!/bin/sh
#script for getting the last svn version number 
#works only for unix like (mac, linux)
	cd .. 
	echo -n '#define SVN_VERSION _T("' > src/core/svn_version.h
    svnversion -n                  >> src/core/svn_version.h
    echo '")'   >> src/core/svn_version.h
    echo '' >> src/core/svn_version.h