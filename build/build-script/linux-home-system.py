import platform
# VARIABLES FOR BUILDING TOOLMAP ON Linux (HOME) 

gwxWidgets = "/home/lucien/programmation/LIB/_LIBWX/bin/wx-config" #wxWIDGETS config
gwxWidgetsVersion = "/home/lucien/programmation/LIB/wxWidgets-SVN"
gDirTrunk = "/home/lucien/programmation/ToolMap/trunk"
gDirBin = "/home/lucien/programmation/ToolMap/bin_system"
gDirInstall = "/home/lucien/programmation/ToolMap/install"
gDirGdal = ""
gDirSQL = ""
gDirUnitTest = ""
gDirCurl = "" 
gDirGeos = ""
gDirWXPDF = "/home/lucien/programmation/LIB/_LIBPDF"
gDirIncludeCxx = "/home/lucien/programmation/LIB/cxxtest"
gDirPythonCxx = "/home/lucien/programmation/LIB/cxxtest/bin/cxxtestgen"
gProcessor= platform.machine()
myLinuxProcessor="i386"
if(gProcessor=="x86_64"):
    myLinuxProcessor = "amd64"


# CMAKE SPECIFIC
gCmakeEnv = "CodeBlocks - Unix Makefiles" #result of cmake --help
gCmakeSpecific = []
gCmakeSpecific.append("-DCMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE:FILE=" + gwxWidgets)
# for Linux Release build
gCmakeSpecific.append("-DCMAKE_BUILD_TYPE:STRING=Release")

gCmakeSpecific.append("-DCPACK_DEBIAN_PACKAGE_ARCHITECTURE:STRING={}".format(myLinuxProcessor))
#gCmakeSpecific.append("-DCPACK_PACKAGE_VERSION_PATCH:STRING={}".format(1000))



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
    print ("Plateform is: " + gProcessor)
    print("Creating installer for: " + svnnumner)
    myDebName = "toolmap_2.4.{}_{}.deb".format(svnnumner, myLinuxProcessor)
    try:
        myProcess = subprocess.Popen("cpack", shell=True, cwd=gDirBin)
        myProcess.wait()
    except:
        print("creating installer failed")
        return ""
        
    shutil.move(os.path.join(gDirBin, "toolmap-2.4.0-Linux.deb"), os.path.join(gDirInstall, myDebName))
    print ("Creatign installer done! " + myDebName)
    return myDebName
