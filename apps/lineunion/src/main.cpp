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
#include <wx/filename.h>

#include "database.h"
#include "databaseresult.h"


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


bool _TestUnion(DataBase * database, long layerindex, long limit, OGRGeometry * frame){
    wxASSERT(database);
    wxString myQuery = wxEmptyString;
    
    if(layerindex != wxNOT_FOUND){
        myQuery = wxString::Format(_T("SELECT l.OBJECT_ID, AsWKB(l.OBJECT_GEOMETRY) FROM generic_lines l LEFT JOIN (generic_aat la, dmn_layer_object o) ")
                                   _T(" ON (la.OBJECT_GEOM_ID = l.OBJECT_ID AND o.OBJECT_ID = la.OBJECT_VAL_ID)")
                                   _T(" WHERE o.THEMATIC_LAYERS_LAYER_INDEX = %ld ORDER BY l.OBJECT_ID LIMIT %ld"), layerindex, limit);
        
    }
    else{
        myQuery = wxString::Format(_T("SELECT OBJECT_ID, OBJECT_GEOMETRY FROM generic_lines ORDER BY OBJECT_ID LIMIT %ld "), limit);
    }
    
    if (database->DataBaseQuery(myQuery)==false) {
        wxLogError(_("Query failed! %s"), myQuery);
        return false;
    }
    DataBaseResult myResult;
	database->DataBaseGetResults(&myResult);
	wxASSERT(myResult.HasResults()==true);

    if (myResult.GetRowCount() < limit) {
        return false;
    }
    
    // process the results
    long myOid = 0;
    OGRMultiLineString * myNodedLines = (OGRMultiLineString*) OGRGeometryFactory::createGeometry(wkbMultiLineString);
    for (long i = 0; i < myResult.GetRowCount(); i++) {
		myResult.NextRow();
        
        OGRGeometry * myGeom = NULL;
        myResult.GetValue(1, &myGeom);
        if (myGeom == NULL) {
            wxLogError(_("No geometry returned on loop :d"),i);
            continue;
        }
        
        myResult.GetValue(0, myOid);
        
        if (myGeom->IsEmpty() == true) {
            OGRGeometryFactory::destroyGeometry(myGeom);
            continue;
        }
        
        myNodedLines->addGeometry(myGeom);
        OGRGeometryFactory::destroyGeometry(myGeom);
    }
    wxLogMessage(_("Lastest OID added to collection: %ld"), myOid);
    
    // trying to union the frame
    OGRGeometry * myLines = myNodedLines->Union(frame);
	wxASSERT(myLines);
	OGRGeometryFactory::destroyGeometry(myNodedLines);
    
	int iTotalLines = ((OGRMultiLineString *) myLines)->getNumGeometries();
	wxLogMessage(_("%d lines for creating polygons"), iTotalLines);
    return true;
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

    long myLimit = 1;
    while (_TestUnion(&myDB, myLayerIndex, myLimit, myFrame)==true) {
        myLimit = myLimit + 100;
    }

    OGRGeometryFactory::destroyGeometry(myFrame);
    return 0;
}
