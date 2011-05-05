# VARIABLES FOR BUILDING ON WINDOWS (CREALP) 
gwxWidgets = "D:\\LIB\\wxWIDGETS-SVN" #wxWIDGETS config
gwxWidgetsVersion = "D:\\LIB\\wxWIDGETS-SVN"
gDirTrunk = "D:\\PRJ\\TOOLMAP2\\trunk"
gDirBin = "D:\\PRJ\\TOOLMAP2\\bin"
gDirInstall = "D:\\PRJ\\TOOLMAP2\\install"
gDirGis = "D:\\LIB\\LIB_GDAL"
gDirSQL = "D:\\LIB\\LIB_MYSQL"
gDirUnitTest = "D:\\PRJ\\TOOLMAP2\\unit_testing"
gDirCxx = "D:\\LIB\\cxxtest"
gDirCurl = "D:\\LIB\\LIB_CURL" 
gDirGeos = "D:\\LIB\\geos-3.2.2"

gCmakeEnv = "Visual Studio 10" #result of cmake --help
gCmakeSpecific = ['-DUSE_MT_LIBRARY:BOOL=1']
gCmakeSpecific.append("-DMYSQL_IS_LOGGING:BOOL=1")
gCmakeSpecific.append("-DUSE_GDIPLUS_LIBRARY:BOOL=1")
gCmakeSpecific.append("-DWXWINDOWS_USE_GL:BOOL=0")
gCmakeSpecific.append("-DSEARCH_GEOS_LIB_PATH:PATH=" + gDirGeos)
gCmakeSpecific.append("-DSEARCH_CURL_LIB_PATH:PATH=" + gDirCurl)
#gCmakeSpecific.append("-DPROXY_PARAM:TEXT=proxy.vs.ch:8080")

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
    myCmdInstaller = ["C:\\Program Files (x86)\\NSIS\\makensis.exe"]
    myCmdInstaller.append("/XSetCompressor /FINAL lzma")
    myCmdInstaller.append("/X!define PRODUCT_VERSION '2.2." + svnnumner + "'")
    myCmdInstaller.append("/XOutFile " + gDirInstall + os.sep + "InstallToolMap_d" + svnnumner + ".exe") 
    myCmdInstaller.append(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "install_script.nsi")
    myProcess = subprocess.Popen(myCmdInstaller, 0, None, None, None,  None, None, False, False, gDirInstall)
    myProcess.wait()
    print ("Renaming installer")
    shutil.move(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "InstallToolMap.exe", gDirInstall + os.sep + "InstallToolMap_d" + svnnumner + ".exe")
