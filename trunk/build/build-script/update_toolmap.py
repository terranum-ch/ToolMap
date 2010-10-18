# BUILD SCRIPT FOR CREATING TOOLMAP BINARY ON EVERY PLATEFORM
# Python 3.0 IS REQUIRED
# (c) Lucien Schreiber 2010
import shutil
import os.path


# script info
print ("\n----------------------------------------------------------")
print ("BUILD SCRIPT FOR CREATING TOOLMAP BINARY ON EVERY PLATEFORM")
print ("----------------------------------------------------------\n")

#
# FUNCTIONS
#
def askUserWithCheck (msg, allowedval=['Y', 'y', 'N', 'n'], errormsg = "Unsupported value"):
    "Ask user for entering a value with validy check, looping until correct value is entered"
    while(1):
        myValue = input(msg)
        if (myValue.isdigit()):
            myValue = int(myValue)
        if (myValue in allowedval):
            return myValue
        print (errormsg)

   
def runTests (msg, directory):
    "Run ctest and display error log if an error occur"
    print ("Running " + msg + " TEST in " + directory)
    myRetcode = 0
    try:
        myProcess = subprocess.Popen(['ctest'], 0, None, None, None,  None, None, False, False, directory)
        myRetcode = myProcess.wait()
    except:
        print ("Running "+ msg +" TEST  FAILED")
        return False
    if (myRetcode != 0):
        print ("Running "+ msg +" TEST  FAILED")
        print ("----------------------------------------------------------\n")
        myErrorFile = directory + os.sep + "Testing" + os.sep + "Temporary" + os.sep + "LastTest.log"
        try:
            readErrorFile = open(myErrorFile, 'r')
        except:
            print("Error reading log : " + myErrorFile)
            return False
        icount = 1
        for line in readErrorFile:
            print (str(icount) + " " + line, end='')
            icount = icount + 1
        print (readErrorFile.readlines())
        return False
    
    print ("Running "+ msg +" TEST DONE")
    #print ("retcode: " + str(myRetcode))
    print ("----------------------------------------------------------\n")
    return True



#
# load variables possible variable files
#
myScriptPathName = os.path.split(__file__)
myVarsFile = os.listdir(myScriptPathName[0])

# copy the list before iterating... otherwise side effects.
myIterateList = myVarsFile[:]
for myFile in myIterateList:
    if (myFile.find(".py") <= 0 or myFile == myScriptPathName[1]):
        myVarsFile.remove (myFile)
icount = 1
for myFile in myVarsFile:
    print ("(",icount, ") -- ", myFile[:-3])
    icount = icount + 1

if (len(myVarsFile) == 0):
    print("no variable found, exiting")
    exit()
    
myvarAllowedValues = list(range(1,len(myVarsFile) + 2))
varChoose = askUserWithCheck("Select variable file number to use for build: ", myvarAllowedValues)
mySelectedVar = myVarsFile[varChoose-1]


try:
    exec(open(myScriptPathName[0] + os.sep + mySelectedVar).read())
except:
    print ("Using variables from ", mySelectedVar, "isn't possible!")
    print (myScriptPathName[0] + os.sep + mySelectedVar)
    exit(); 


#
# generic questions
#
doClean = askUserWithCheck("Clean directory before building ? (Y / N): ").upper()
doRelease = askUserWithCheck("Build release version ? (Y / N): ").upper()
doRunTests = askUserWithCheck("Run tests after build ? (Y/N): ").upper()
doInstall = askUserWithCheck("Create installer ? (not valid for Linux) (Y / N): ").upper()
doUploadInstaller = input("Upload Installer to FTP ? (Set password for Yes and leave empty for No) ")
print ("----------------------------------------------------------\n")

#
# display variables
#
print ("Using variables from :", mySelectedVar)
print ("wxWidgets :", gwxWidgets)
print ("Bin       :", gDirBin)
print ("Trunk     :", gDirTrunk)
print ("GIS       :", gDirGis)
print ("Unit test :", gDirUnitTest)
print ("CXX       :", gDirCxx)
print ("----------------------------------------------------------\n")


doGo = askUserWithCheck ("Build project with thoses variables ? (Y / N): ")
if (not doGo):
    exit()

#
# get latest version
#
import subprocess
print ("Updating with latest version from Subversion")
try:
    myProcess = subprocess.Popen(['svn', 'update', gDirTrunk])
    myProcess.wait()
except:
    print ("Error updating SVN!")
    exit()
print ("Updating SVN DONE!")
print ("----------------------------------------------------------\n")


#
# Clean directory
#
if (doClean == 'Y' and gDirBin != ''):
    print ("Cleaning directory")
    try:
        shutil.rmtree(gDirBin)
        os.mkdir(gDirBin)
        print ("Cleaning directory DONE")
        print ("----------------------------------------------------------\n")
    except:
        print("Unable to remove directory :", gDirBin)



#
# Configure using CMAKE
#
mycmakeCommandLine = ['cmake']
mycmakeCommandLine.append("-G" + gCmakeEnv)
mycmakeCommandLine.append(gDirTrunk + os.sep + "build")
mycmakeCommandLine = mycmakeCommandLine + gCmakeSpecific
mycmakeCommandLine.append("-DSEARCH_GIS_LIB_PATH:PATH=" + gDirGis)
mycmakeCommandLine.append("-DMYSQL_MAIN_DIR:PATH=" + gDirSQL)
if (gDirCurl):
    mycmakeCommandLine.append("-DSEARCH_CURL_LIB_PATH:PATH=" + gDirCurl)
if (gDirGeos):
    mycmakeCommandLine.append("-DSEARCH_GEOS_LIB_PATH:PATH=" + gDirGeos)
mycmakeCommandLine.append("-DUSE_UNITTEST:BOOL=1")
mycmakeCommandLine.append("-DUNIT_TESTING_PATH:PATH=" + gDirUnitTest)
mycmakeCommandLine.append("-DCXXTEST_DIRECTORY:PATH=" + gDirCxx)

print (" ".join(mycmakeCommandLine))
try:
    myProcess = subprocess.Popen(mycmakeCommandLine, 0, None, None, None,  None, None, False, False, gDirBin)
    myProcess.wait()
except:
    print("Configuring project FAILED!")
    exit()
 
print ("Configuration DONE")
print ("----------------------------------------------------------\n")


#
#building
#

print ("Building DEBUG ")
try:
    myProcess = subprocess.Popen(gBuildDebug, 0, None, None, None,  None, None, False, False, gDirBin)
    myProcess.wait()
except:
    print ("Building DEBUG version FAILED")
    exit()
print ("Building DEBUG DONE")
print ("----------------------------------------------------------\n")



if (doRelease == 'Y' and len(gBuildRelease) > 0):
    print ("Building RELEASE ")
    try:
        myProcess = subprocess.Popen(gBuildRelease, 0, None, None, None,  None, None, False, False, gDirBin)
        myProcess.wait()
    except:
        print ("Building RELEASE version FAILED")
        exit()
    print ("Building RELEASE DONE")
    print ("----------------------------------------------------------\n")



#
# configuring and building all tests
#
if (doRunTests == 'Y'):
    runBeforeTest()
    if (not runTests("toolmap", gDirBin + os.sep + "test")):
        exit()

   

        

        
    
    









