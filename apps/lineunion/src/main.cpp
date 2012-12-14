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

#include "database_tm.h"

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, "v", "verbose", "Be more verbose" },
    { wxCMD_LINE_NONE }
};



void PrintArray(const wxArrayString & array, const wxString & msg){
    wxPrintf(msg + _T("\n"));
    for (unsigned int i = 0; i< array.GetCount(); i++) {
        wxPrintf(array[i] + _T("\n"));
    }
}


OGRGeometry * _GetGeometryFromDatabaseResults(DataBase * database, long & oid){
    MYSQL_ROW row;
	tmArrayULong row_length;
	
	// security check
	if(database->DataBaseHasResults()==false){
        oid = wxNOT_FOUND;
		return NULL;
	}
	
	if (database->DataBaseGetNextRowResult(row, row_length)==false){
		database->DataBaseClearResults();
        oid = wxNOT_FOUND;
		return NULL;
	}
	
    OGRGeometry * geometry = NULL;
    int geometry_col = 1;
    int oid_col = 0;
	// Geometry columns will have the first 4 bytes contain the SRID.
	OGRGeometryFactory::createFromWkb(((unsigned char *)row[geometry_col]) + 4,NULL,&geometry, row_length.Item(geometry_col) - 4 );
	
    if (!row){
		oid = wxNOT_FOUND;
    }
	wxString (row[oid_col], wxConvUTF8).ToLong( &oid);
    return geometry;
}



OGRGeometry * _GetFrame(DataBase * database){
    wxASSERT(database);
    // get frame
    if (database->DataBaseQuery(_T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM generic_frame"))==false){
        wxLogError(_("Error getting geometry for frame"));
        return NULL;
    }
    long myOid = wxNOT_FOUND;
    OGRGeometry * myGeom = _GetGeometryFromDatabaseResults(database, myOid);
    database->DataBaseClearResults();
    return myGeom;
}


int main(int argc, char **argv)
{
    // debugging string for OSX
    // this is needed for viewing string content with Xcode !!
    wxString myTest = _T("Test debugging");
    myTest.ToUTF8().data();
    myTest.Len();
    
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
    
    wxString myLogoTxt = _T("*\n* LineUnion \n* Incremental Union testing\n* Used to highlight lines problem\n* (c) Copyright 2012 Lucien Schreiber - CREALP . All Rights Reserved. \n*\n");
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.AddParam(_T("[toolmap project path]"), wxCMD_LINE_VAL_STRING);
    parser.AddOption(_T("l"), _("layerindex"), _("Numerical layer index, default is all lines"), wxCMD_LINE_VAL_NUMBER, wxNOT_FOUND);
    parser.SetLogo(myLogoTxt);
    
    if (parser.Parse() != 0) {
        return 0;
    }
    
    bool beVerbose = parser.Found("verbose");
    // cmd line is correct !!
    wxPrintf(myLogoTxt);
    wxString myTMProjectTxt = parser.GetParam(0);
    wxFileName myTMProjectName (myTMProjectTxt);

    if (beVerbose) {
        wxPrintf(_("Project: '%s' in '%s'\n"), myTMProjectName.GetName(), myTMProjectName.GetPath());
    }
    
    long myLayerIndex = wxNOT_FOUND;
    parser.Found(_T("l"), &myLayerIndex);
    if (myLayerIndex != wxNOT_FOUND) {
        wxLogMessage(_("Processing lines from layer: %ld"), myLayerIndex);
    }
    else{
        wxLogMessage(_("Processing all lines!"));
    }
    
    
    
    // processing here
    // open database
    DataBase myDB (_T("./"));
    if(myDB.DataBaseOpen(myTMProjectName.GetPath(), myTMProjectName.GetName()) == false){
        wxLogError(_("This isn't a ToolMap project database"));
        return 0;
    }
    
    // get frame
    OGRGeometry * myFrame = _GetFrame(&myDB);
    if (myFrame == NULL) {
        wxLogError(_("Error getting frame!"));
        return 0;
    }

    
    OGRGeometryFactory::destroyGeometry(myFrame);
    return 0;
}
