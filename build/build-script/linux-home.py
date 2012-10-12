# VARIABLES FOR BUILDING TOOLMAP ON Linux (HOME) 

gwxWidgets = "/home/lucien/programmation/LIB/_LIBWX/bin/wx-config" #wxWIDGETS config
gwxWidgetsVersion = "/home/lucien/programmation/LIB/wxWidgets-SVN"
gDirTrunk = "/home/lucien/programmation/ToolMap/trunk"
gDirBin = "/home/lucien/programmation/ToolMap/bin"
gDirInstall = "/home/lucien/programmation/ToolMap/install"
gDirGdal = "/home/lucien/programmation/LIB/_LIBGIS"
gDirSQL = "/home/lucien/programmation/LIB/_LIBMYSQL"
gDirUnitTest = ""
gDirCurl = "" 
gDirGeos = ""
gDirWXPDF = "/home/lucien/programmation/LIB/_LIBPDF"
gDirIncludeCxx = "/home/lucien/programmation/LIB/cxxtest"
gDirPythonCxx = "/home/lucien/programmation/LIB/cxxtest/bin/cxxtestgen"

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


def gCreateInstaller(svnnumner):
    "Create installer for Linux"
    print("Creating installer for: " + svnnumner)
    myDebName = "toolmap_2.4.{0}-0debian1_amd64.deb".format(svnnumner)
    try:
        myProcess = subprocess.Popen("cpack", shell=True, cwd=gDirBin)
        myProcess.wait()
    except:
        print("creating installer failed")
        return ""
        
    shutil.move(os.path.join(gDirBin, "toolmap-0.1.1-Linux.deb"), os.path.join(gDirInstall, myDebName))
    print ("Creatign installer done!" + myDebName)
    return myDebName
