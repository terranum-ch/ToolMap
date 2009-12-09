/***************************************************************************
								test_param.h
					Manage parameters for the tests.
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
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

#ifndef _TEST_PARAM_H_
#define _TEST_PARAM_H_


#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif



static const wxString g_TestPath = _T("/Users/lucien/DATA/PRJ/TOOLMAP2/unit_testing");
static const wxString g_TestPathSHP = g_TestPath + _T("/shp/");
static const wxString g_TestPathMISC = g_TestPath + _T("/misc/");
static const wxString g_TestPathPRJ = g_TestPath +  _T("/prj/");


static const wxString g_TestFileMisc = _T("gpl.txt");
static const wxString g_TestFileSHP_L = _T("arcgis_line_test.shp");
static const wxString g_TestFileSHP_LCadre = _T("DIABLERETS_FRONTIERE.shp");
static const wxString g_TestFileSHP_PLG = _T("cantonsuisse.shp");

// projects
static const wxString g_TestPrj_CombioulaCorrect = _T("combioula_correct");
static const wxString g_TestPrj_CombioulaExercice = _T("combioula_exercice");
static const wxString g_TestPrj_MyTest = _T("mytest1");
static const wxString g_TestPrj_NewQueries = _T("new_queries");
static const wxString g_TestPrj_Dangling = _T("testdangling");
static const wxString g_TestPrj_Edit = _T("testedit");
static const wxString g_TestPrj_Edit13 = _T("testedit_13");
static const wxString g_TestPrj_Export = _T("testexport");
static const wxString g_TestPrj_Fields = _T("testfields");
static const wxString g_TestPrj_Import = _T("testimport");
static const wxString g_TestPrj_LuganoTM = _T("luganoTM");











#endif
