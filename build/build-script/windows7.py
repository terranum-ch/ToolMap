import os
import shutil

# VARIABLES FOR BUILDING ON WINDOWS (CREALP)
import subprocess

gwxWidgets = "D:\\LIB\\wxWIDGETS-SVN" #wxWIDGETS config
gwxWidgetsVersion = "D:\\LIB\\wxWIDGETS-SVN"
gDirTrunk = "D:\\PRJ\\TOOLMAP2\\trunk"
gDirBin = "D:\\PRJ\\TOOLMAP2\\bin"
gDirInstall = "D:\\PRJ\\TOOLMAP2\\install"
gDirGdal = "D:\\LIB\\LIB_GDAL"
gDirSQL = "D:\\LIB\\LIB_MYSQL"
gDirUnitTest = "D:\\PRJ\\TOOLMAP2\\unit_testing"
gDirIncludeCxx = "D:\\LIB\\cxxtest"
gDirPythonCxx = "D:\\LIB\\cxxtest\\bin\\cxxtestgen"
gDirCurl = "D:\\LIB\\LIB_CURL" 
gDirGeos = "D:\\LIB\\geos-3.3.8"
gDirWXPDF = "D:\\LIB\\wxpdfdoc-0.9.3"
gDirSQLITE = "D:\\LIB\\LIB_SQLITE"


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
    myBuildtype = buildtype
    if (myBuildtype == "Release"):
        myBuildtype = "RelWithDebInfo"

    myVarsFile = os.listdir(directory)
    myTarget = ""
    for file in myVarsFile:
        if (file.find(".sln") > 0):
            myTarget = file
            break
    
    myConfiguration = "/property:Configuration="+ myBuildtype
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
    myCmdInstaller.append("/X!define PRODUCT_VERSION '2.5." + svnnumner + "'")
    myCmdInstaller.append("/XOutFile " + gDirInstall + os.sep + "InstallToolMap_r" + svnnumner + ".exe") 
    myCmdInstaller.append(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "install_script-release.nsi")
    myProcess = subprocess.Popen(myCmdInstaller, 0, None, None, None,  None, None, False, False, gDirInstall)
    myProcess.wait()
    print ("Renaming installer")
    installname = "InstallToolMap_r" + svnnumner + ".exe"
    shutil.move(gDirTrunk + os.sep + "install" + os.sep + "windows" + os.sep + "InstallToolMap.exe", gDirInstall + os.sep + installname)
    # copy pdb file for debugging to directory
    drive, tail = os.path.split(gDirBin)
    myPDBBasePath = os.path.join(drive, "pdb")
    myPDBPath = os.path.join(myPDBBasePath, str(svnnumner))
    # pdb files
    myPDBfiles = {"lscrashreport.pdb", "lsversion.pdb", "toolmap.pdb", "toolmap_lib.pdb", "ToolMap.exe"}
    if (os.path.exists(myPDBPath)==True):
        print("PDB files not copied, directory exists !!!!")
        return installname

    os.mkdir(myPDBPath)
    for file in myPDBfiles:
        shutil.copyfile(os.path.join(gDirBin, "RelWithDebInfo", file), os.path.join(myPDBPath, file))
    return installname


def DoPostBuildCommand():
    """copy DLL to directories"""
    myDll = (os.path.join(gDirGdal,"bin","gdal110.dll"), #,os.path.join(gDirGeos,"bin","Debug", "geos.dll"),
             os.path.join(gDirGeos,"src","geos_c.dll"),
             os.path.join(gDirSQL, "Embedded", "DLL", "release", "libmysqld.dll"),
             os.path.join(gDirCurl, "lib", "DLL-Debug","libcurld.dll"),
             os.path.join(gDirCurl, "lib", "DLL-Release","libcurl.dll"),
             os.path.join(gDirSQLITE, "lib", "sqlite3.dll")
        )

    for dll in myDll:
        shutil.copyfile(dll, os.path.join(gDirBin, "Debug", os.path.basename(dll)))
        if (os.path.exists(os.path.join(gDirBin, "RelWithDebInfo"))):
            shutil.copyfile(dll, os.path.join(gDirBin, "RelWithDebInfo", os.path.basename(dll)))

        print ("copying: ", dll)

#if __name__ == '__main__':
#    gCreateInstaller("1")
