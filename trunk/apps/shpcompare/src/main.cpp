/***************************************************************************
 $main.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/dir.h>
#include "shpcompare.h"

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, "v", "verbose", "Be more verbose" },
    { wxCMD_LINE_NONE }
};



void PrintArray(const wxArrayString * array, const wxString & msg){
    if (array->GetCount() == 0) {
        return;
    }
    wxPrintf(_T("\n"));
    wxPrintf(msg + _T("\n"));
    wxString myDeco;
    for (int l = 0; l< msg.Len() ; l++) {
        myDeco.Append(_T("-"));
    }
    wxPrintf(myDeco + _T("\n"));
    
    for (unsigned int i = 0; i< array->GetCount(); i++) {
        wxPrintf(array->Item(i) + _T("\n"));
    }
    wxPrintf(_T("\n"));
}



int main(int argc, char **argv)
{
    // debugging string for OSX
    // this is needed for viewing string content with Xcode !!
#ifdef __WXDEBUG__
    wxString myTest = _T("Test debugging");
    myTest.ToUTF8().data();
    myTest.Len();
#endif
    
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    wxInitializer initializer;
    if ( !initializer ){
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
    
    wxString myLogoTxt = _T("*\n* shpCompare \n* Compare two ESRI Shapefiles \n* (c) Copyright 2012 Lucien Schreiber - CREALP . All Rights Reserved. \n*\n");
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.AddParam(_T("[reference file]"), wxCMD_LINE_VAL_STRING);
    parser.AddParam(_T("[files to compare]"),wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_MULTIPLE);
    parser.SetLogo(myLogoTxt);
    
    if (parser.Parse() != 0) {
        return 0;
    }
    
    wxPrintf(myLogoTxt);
    bool beVerbose = parser.Found("verbose");
    const wxString myReferenceFile = parser.GetParam(0);
    
    wxArrayString myInputFilesTxt;
    int myParamCount = parser.GetParamCount();
    for (int i = 1; i< myParamCount; i++) {
        myInputFilesTxt.Add(parser.GetParam(i));
    }
     
    // compare!
    ShpCompare myCompare;
    myCompare.SetReferenceFileName(myReferenceFile);
    for (unsigned int f = 0; f < myInputFilesTxt.GetCount(); f++) {
        myCompare.AddFileNameToCheck(myInputFilesTxt[f]);
    }
    
    if (myCompare.DoCompare() == false) {
        if (beVerbose) {
            PrintArray(myCompare.GetMessagesRef(), _("Debug messages:"));
        }
        return 0;
    }
    
    if (beVerbose) {
        PrintArray(myCompare.GetMessagesRef(), _("Debug messages:"));
    }
    
    if (myCompare.HasErrors() == false) {
        wxPrintf(_("Files are similar, no differences found!\n"));
        return 0;
    }
    
    PrintArray(myCompare.GetErrorsRef(), _("Errors found:"));
    
    
}
