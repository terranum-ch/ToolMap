# VARIABLES FOR BUILDING TOOLMAP ON Linux (HOME) 

gwxWidgets = "/home/lucien/programmation/LIB/_LIBWX/bin/wx-config" #wxWIDGETS config
gwxWidgetsVersion = "/home/lucien/programmation/LIB/wxWidgets-SVN"
gDirTrunk = "/home/lucien/programmation/ToolMap/trunk"
gDirBin = "/home/lucien/programmation/ToolMap/bin"
gDirInstall = ""
gDirGdal = "/home/lucien/programmation/LIB/_LIBGIS"
gDirSQL = "/home/lucien/programmation/LIB/_LIBMYSQL"
gDirUnitTest = ""
gDirCxx = "/home/lucien/programmation/LIB/cxxtest"
gDirCurl = "" 
gDirGeos = ""
gDirWXPDF = ""


# CMAKE SPECIFIC
gCmakeEnv = "CodeBlocks - Unix Makefiles" #result of cmake --help
gCmakeSpecific = []
gCmakeSpecific.append("-DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=" + gwxWidgets)
# for Linux Release build
gCmakeSpecific.append("-DCMAKE_BUILD_TYPE:STRING=Release")

# PLATEFORM SPECIFIC FUNCTION
# CONFIG SPECIFIC
def gBeforeConfig():
    "Initing plateform specific variables"
    print ("Nothing to init")

# BUILD SPECIFIC
def gBuildCommand(buildtype="Debug", directory = ""):
    myCommand = ['make']
    myCommand.append("--jobs=" + str(NumberOfProc))    
    return myCommand

