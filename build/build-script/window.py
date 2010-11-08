# VARIABLES FOR BUILDING ON WINDOWS (CREALP) 

gwxWidgets = "D:\\LS\\PROGRAMATION\\LIB\\wxMSW-2.8.11" #wxWIDGETS config
gDirTrunk = "D:\\LS\\PRJ\\TOOLMAP2\\trunk"
gDirBin = "D:\\LS\\PRJ\\TOOLMAP2\\bin\\prod"
gDirInstall = "D:\\LS\\PRJ\\TOOLMAP2\\install"
gDirGis = "D:\\LS\\PROGRAMATION\\LIB\\LIB_GDAL"
gDirSQL = "D:\\LS\\PROGRAMATION\\LIB\\mysql-5.1.51-VS2008"
gDirUnitTest = "D:\\LS\\PRJ\\TOOLMAP2\\unit_testing"
gDirCxx = "D:\\LS\\PROGRAMATION\\LIB\\cxxtest"
gDirCurl = "D:\\LS\\PROGRAMATION\\LIB\\LIB_CURL" 
gDirGeos = "D:\\LS\\PROGRAMATION\\LIB\\geos-3.2.2"

gCmakeEnv = "Visual Studio 9 2008" #result of cmake --help
gCmakeSpecific = ['-DUSE_MT_LIBRARY:BOOL=1']
gCmakeSpecific.append("-DMYSQL_IS_LOGGING:BOOL=1")
gCmakeSpecific.append("-DUSE_GDIPLUS_LIBRARY:BOOL=1")
gCmakeSpecific.append("-DWXWINDOWS_USE_GL:BOOL=0")
gCmakeSpecific.append("-DSEARCH_GEOS_LIB_PATH:PATH=" + gDirGeos)
gCmakeSpecific.append("-DSEARCH_CURL_LIB_PATH:PATH=" + gDirCurl)
gCmakeSpecific.append("-DPROXY_PARAM:TEXT=proxy.vs.ch:8080")

# PLATEFORM SPECIFIC FUNCTION
# CONFIG SPECIFIC
def gBeforeConfig():
    "Initing plateform specific variables"
    print (os.environ['WXWIN'])
    try:
        os.environ['WXWIN']=gwxWidgets
        print (os.environ['WXWIN'])
    except:
        print("Error running pre-configure commands :", gwxWidgets)
        exit()

# BUILD SPECIFIC
def gBuildCommand(buildtype="Debug", directory = ""):
    myVarsFile = os.listdir(directory)
    myTarget = ""
    for file in myVarsFile:
        if (file.find(".sln") > 0):
            myTarget = file
            break
    
    myConfiguration = "/property:Configuration="+ buildtype
    return ['msbuild', myTarget, myConfiguration]

def runBeforeTest():
    "Nothing special for Windows"
    print ("Nothing to be run before tests")

    
# INSTALL SPECIFIC
def gCreateInstaller(svnnumner):
    "Create installer for Windows"
    print ("Creating Windows installer")
    myCmdInstaller = ["C:\\Program Files\\NSIS\\makensis.exe"]
    myCmdInstaller.append("/XSetCompressor /FINAL lzma")
    myCmdInstaller.append("/X!define PRODUCT_VERSION '2.1." + svnnumner + "'")
    myCmdInstaller.append("/XOutFile " + gDirInstall + os.sep + "InstallToolMap_d" + svnnumner + ".exe") 
    myCmdInstaller.append(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "lugano" + os.sep + "install_builder_script.nsi")
    try:
        myProcess = subprocess.Popen(myCmdInstaller, 0, None, None, None,  None, None, False, False, gDirInstall)
        myProcess.wait()
    except:
        print ("Error creating installer with command :", myCmdInstaller)
        exit()
    print ("Renaming installer")
    shutil.move(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "lugano" + os.sep + "InstallToolMap.exe", gDirInstall + os.sep + "InstallToolMap_d" + svnnumner + ".exe")
