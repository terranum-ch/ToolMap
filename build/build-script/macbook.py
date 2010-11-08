# VARIABLES FOR BUILDING TOOLMAP ON OSX 
gwxWidgets = "/Users/lucien/DATA/PROGRAMATION/_LIB/32/_LIBWX/bin/wx-config" #wxWIDGETS config
gDirTrunk = "/Users/lucien/DATA/PRJ/TOOLMAP2/trunk"
gDirBin = "/Users/lucien/DATA/PRJ/TOOLMAP2/bin"
gDirInstall = "/Users/lucien/DATA/PRJ/TOOLMAP2/install"
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


#CONFIG SPECIFIC
def gBeforeConfig():
    print ("No special configurations")


# BUILD SPECIFIC
def gBuildCommand(buildtype="Debug", directory = ""):
    if (buildtype == "Release"):
        return ['xcodebuild', '-configuration', 'Release']
    return ['xcodebuild', '-configuration', 'Debug']



#TEST SPECIFIC
def runBeforeTest():
    "Run this function before test for settings some parameters. Actually only coping ToolMap.app"
    if(os.path.exists(gDirBin + os.sep + "test/Debug/ToolMap2.app") == False):
        exception = shutil.copytree(gDirBin + os.sep + "Debug/ToolMap2.app", gDirBin + os.sep + "test/ToolMap2.app")

def gCreateInstaller(svnnumner):
    "Create installer for mac"
    print ("Copying files out of Release folder")
    shutil.move(gDirBin + os.sep + "Release" + os.sep + "libToolMap2_lib.a", gDirBin + os.sep + "libToolMap2_lib.a")
    
    print ("Copying background files...")
    myReleasePath = gDirBin + os.sep + "Release"
    os.mkdir(myReleasePath + os.sep + ".background")
    
    shutil.copy(gDirTrunk + os.sep + "install" + os.sep + "mac" + os.sep + "background" + os.sep + "tm_dmg_background.jpg", myReleasePath + os.sep + ".background" + os.sep + "tm_dmg_background.jpg")
    shutil.copy(gDirTrunk + os.sep + "install" + os.sep + "mac" + os.sep + "background" + os.sep + "InstallDS_Store", myReleasePath + os.sep + ".DS_Store")
    os.symlink("/Applications", myReleasePath + os.sep + "Applications")
    
    print("Creating installer")
    mycommand = ['hdiutil','create', '-volname','ToolMap', '-srcfolder', gDirBin + os.sep + "Release", "ToolMap2_r"+ svnnumner +".dmg"]
    try:
        myProcess = subprocess.Popen(mycommand, 0, None, None, None,  None, None, False, False, gDirInstall)
        myProcess.wait()
    except:
        print("creating installer failed")
    print ("Creatign installer done!")
    
    print ("Copying files back to Release folder")
    
    shutil.rmtree(myReleasePath + os.sep + ".background")
    os.unlink(myReleasePath + os.sep + "Applications")
    shutil.move(gDirBin + os.sep + "libToolMap2_lib.a", gDirBin + os.sep +  "Release" + os.sep + "libToolMap2_lib.a")
    print ("Creating installer finished")
    return "ColtopGIS_r"+svnnumner+".dmg"