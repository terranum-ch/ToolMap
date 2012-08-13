#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/app.h>
#include <wx/cmdline.h>

#include "tmmergeprojects.h"

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, "m", "merge", "Merge Slave into Master" },
    { wxCMD_LINE_SWITCH, "v", "verbose", "Be more verbose" },
    { wxCMD_LINE_NONE }
};

int main(int argc, char **argv)
{
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
    
    wxString myLogoTxt = _T("*\n* ToolMerge \n* Checking and Merging ToolMap projects \n* (c) Copyright 2012 Lucien Schreiber - CREALP . All Rights Reserved. \n*\n");
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.AddParam(_T("[master project path]"), wxCMD_LINE_VAL_STRING);
    parser.AddParam(_T("[slave project path]"), wxCMD_LINE_VAL_STRING);
    parser.SetLogo(myLogoTxt);
    
    if (parser.Parse() != 0) {
        return 0;
    }
    
    bool beVerbose = parser.Found("verbose");
    
    // cmd line is correct !!
    wxPrintf(myLogoTxt);
    wxString myMasterFileName = parser.GetParam(0);
    wxString mySlaveFileName = parser.GetParam(1);
    
    if (beVerbose) {
        wxPrintf(_("Master: '%s'\nSlave:  '%s'\n"), myMasterFileName, mySlaveFileName);
    }
    
    tmMergeProjects myCheckMerger(myMasterFileName, mySlaveFileName);
    myCheckMerger.SetVerbose(beVerbose);
    // checking here
    if (beVerbose) {
        wxPrintf(_("CHECKING...\n"));
    }
    wxStopWatch sw;
    if(myCheckMerger.CheckSimilar()==true){
        wxPrintf(_("OK projects are similar\n"));
    }
    else {
        wxPrintf(_("Checking FAILED! see bellow\n"));
        wxArrayString myErrors = myCheckMerger.GetErrors();
        for (unsigned int i = 0; i< myErrors.GetCount(); i++) {
            wxPrintf(myErrors.Item(i) + _T("\n"));
        }
    }
    
    if (beVerbose) {
        wxPrintf(_("Checking projects in %ld [ms]\n"), sw.Time());
    }
    
    
    
    if (parser.Found("merge")) {
        // merging here
        if (beVerbose) {
            wxPrintf(_("MERGING...\n"));
        }
        
        if(myCheckMerger.MergeIntoMaster()==true){
            wxPrintf(_("OK Project Merged into '%s' in %ld [ms]"), myMasterFileName, sw.Time());
        }
        else{
            wxPrintf(_("Merge FAILED! see bellow\n"));
            wxArrayString myErrors = myCheckMerger.GetErrors();
            for (unsigned int i = 0; i< myErrors.GetCount(); i++) {
                wxPrintf(myErrors.Item(i) + _T("\n"));
            }
        }
    }
    
    return 0;
}
