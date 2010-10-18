# VARIABLES FOR BUILDING TOOLMAP ON OSX 
import os.path
import shutil


gwxWidgets = "/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBWX/bin/wx-config" #wxWIDGETS config
gDirTrunk = "/Users/lucien/DATA/PRJ/TOOLMAP2/trunk"
gDirBin = "/Users/lucien/DATA/PRJ/TOOLMAP2/bin"
gDirGis = "/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBGIS"
gDirSQL = "/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBMYSQL"
gDirUnitTest = "/Users/lucien/DATA/PRJ/TOOLMAP2/unit_testing"
gDirCxx = "/Users/lucien/DATA/PROGRAMATION/_LIB/cxxtest"
gDirCurl = "" 
gDirGeos = ""

# CMAKE SPECIFIC
gCmakeEnv = "Xcode" #result of cmake --help
gCmakeSpecific = ['-DCMAKE_OSX_ARCHITECTURES:TEXT=i386']
gCmakeSpecific.append("-DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=" + gwxWidgets)

# BUILD SPECIFIC
gBuildDebug = ['xcodebuild', '-configuration', 'Debug'] 
gBuildRelease = ['xcodebuild', '-configuration', 'Release']

#TEST SPECIFIC
def runBeforeTest():
    "Run this function before test for settings some parameters. Actually only coping ToolMap.app"
    if(os.path.exists(gDirBin + os.sep + "test/Debug/ToolMap2.app") == False):
        exception = shutil.copytree(gDirBin + os.sep + "Debug/ToolMap2.app", gDirBin + os.sep + "test/ToolMap2.app")
