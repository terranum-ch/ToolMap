/***************************************************************************
 tmimportcsv.cpp
 
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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


#include "tmimportcsv.h"
#include "../database/database_tm.h"
#include "tmgisdatavector.h"
#include "../core/tmpercent.h"


bool tmImportCSV::_ResetReading()
{
    wxASSERT(m_FileStream);
    if (m_FileStream->IsSeekable() == false) {
        wxLogError(_("Unable to reset reading"));
        return false;
    }
    m_FileStream->SeekI(0);
    return true;
}


bool tmImportCSV::_GetNextData(double &x, double &y)
{
    wxASSERT(m_TextStream);

    if (m_Xcolumn == wxNOT_FOUND || m_Ycolumn == wxNOT_FOUND) {
        return false;
    }

    wxString myLine = m_TextStream->ReadLine();
    if (myLine.IsEmpty()) {
        return false;
    }

    wxArrayString myTokenArray;
    wxStringTokenizer myTokenizer(myLine, _T(";"));
    while (myTokenizer.HasMoreTokens()) {
        myTokenArray.Add(myTokenizer.GetNextToken());
    }


    if (m_Xcolumn >= (signed) myTokenArray.GetCount() && m_Ycolumn >= (signed) myTokenArray.GetCount()) {
        return false;
    }

    double myX = 0.0;
    double myY = 0.0;

    if (myTokenArray.Item(m_Xcolumn).ToCDouble(&myX) == false) {
        wxLogMessage(myTokenArray.Item(m_Xcolumn));
        return false;
    }

    if (myTokenArray.Item(m_Ycolumn).ToCDouble(&myY) == false) {
        wxLogMessage(myTokenArray.Item(m_Ycolumn));
        return false;
    }

    x = myX;
    y = myY;
    return true;
}


tmImportCSV::tmImportCSV()
{
    m_Xcolumn = wxNOT_FOUND;
    m_Ycolumn = wxNOT_FOUND;
    m_TextStream = NULL;
    m_FileStream = NULL;
    m_FileType = tmIMPORT_TYPE_CSV;
}


tmImportCSV::~tmImportCSV()
{
    wxDELETE(m_FileStream);
    wxDELETE(m_TextStream);
}


bool tmImportCSV::Open(const wxFileName &filename)
{
    wxDELETE(m_FileStream);
    wxDELETE(m_TextStream);

    if (!tmImport::Open(filename)) {
        return false;
    }

    m_FileStream = new wxFileInputStream(filename.GetFullPath());
    if (m_FileStream == NULL) {
        wxLogError(_("Unable to open '%s'"), filename.GetFullName().c_str());
        return false;
    }

    m_TextStream = new wxTextInputStream(*m_FileStream);
    wxASSERT(m_TextStream);

    m_GeometryType = LAYER_SPATIAL_POINT; // CSV are always points.

    // counting features
    long iCount = 0;
    while (1) {
        if (m_TextStream->ReadLine().IsEmpty()) {
            break;
        }
        iCount++;
    }
    m_FeatureCount = iCount;
    _ResetReading();
    // counting fields
    if (iCount > 0) {
        wxStringTokenizer myTokenizer(m_TextStream->ReadLine(), _T(";"));
        m_FieldsCount = myTokenizer.CountTokens();
        _ResetReading();
    }
    return true;
}


void tmImportCSV::ListColumns(wxArrayString &columns)
{
    _ResetReading();
    wxASSERT(m_TextStream);
    wxString myLine1 = m_TextStream->ReadLine();

    wxStringTokenizer tokenizer(myLine1, _T(";"));
	while (tokenizer.HasMoreTokens())
	{
		wxString token = tokenizer.GetNextToken();
        columns.Add(token);
	}

    _ResetReading();
}


void tmImportCSV::GuessXYcolumns(wxArrayString &columns)
{
    if (m_Xcolumn == wxNOT_FOUND && m_Ycolumn == wxNOT_FOUND) {
        for (int i = 0; i < columns.GetCount(); i++) {
            wxString val = columns.Item(i);
            if (val.IsSameAs("x", false) || val.IsSameAs("lon", false)) {
                m_Xcolumn = i;
            }
            if (val.IsSameAs("y", false) || val.IsSameAs("lat", false)) {
                m_Ycolumn = i;
            }
        }
    }

}


bool tmImportCSV::GetFieldNames(wxArrayString &Fields)
{
    ListColumns(Fields);

    return true;
}


void tmImportCSV::SetColumn(int x, int y)
{
    m_Xcolumn = x;
    m_Ycolumn = y;
}


bool tmImportCSV::IsOk()
{
    if (tmImport::IsOk() == false) {
        return false;
    }

    if (m_Xcolumn == wxNOT_FOUND || m_Ycolumn == wxNOT_FOUND) {
        return false;
    }

    if (m_TextStream == NULL) {
        return false;
    }
    return true;
}


bool tmImportCSV::_ImportToPointLayer(DataBaseTM *database, wxProgressDialog *progress)
{
    tmGISDataVectorMYSQL *myGeomDB = new tmGISDataVectorMYSQL();
    tmGISDataVectorMYSQL::SetDataBaseHandle(database);

    long iCount = 0;
    wxStopWatch sv;
    tmPercent tpercent(GetFeatureCount());
    while (true) {
        double x = 0;
        double y = 0;
        if (!_GetNextData(x, y)) {
            break;
        }
        OGRPoint *myOGRPt = (OGRPoint *) OGRGeometryFactory::createGeometry(wkbPoint);
        myOGRPt->setCoordinateDimension(2);
        myOGRPt->setX(x);
        myOGRPt->setY(y);

        if (myGeomDB->AddGeometry(myOGRPt, -1, GetTarget()) == wxNOT_FOUND) {
            OGRGeometryFactory::destroyGeometry(myOGRPt);
            wxLogError(_("Error importing geometry into the project"));
            break;
        }
        iCount++;
        OGRGeometryFactory::destroyGeometry(myOGRPt);

        bool bStop = false;
        tpercent.SetValue(iCount);
        if (tpercent.IsNewStep() && progress != NULL) {
            if (!progress->Update(tpercent.GetPercent(), wxEmptyString)) {
                bStop = true;
            }
        }

        if (bStop) {
            wxLogMessage(_("Adding gis data into project stopped by user."));
            break;
        }
    }

    sv.Pause();
    wxLogMessage(_("%ld feature added in %ld [ms]"), iCount, sv.Time());
    wxDELETE(myGeomDB);
    return true;
}


bool tmImportCSV::_ImportToLineLayer(DataBaseTM *database, wxProgressDialog *progress)
{
    tmGISDataVectorMYSQL *myGeomDB = new tmGISDataVectorMYSQL();
    tmGISDataVectorMYSQL::SetDataBaseHandle(database);

    long iCount = 0;
    wxStopWatch sv;
    tmPercent tpercent(GetFeatureCount());
    OGRLineString *myOGRLine = (OGRLineString *) OGRGeometryFactory::createGeometry(wkbLineString);

    while (true) {
        double x = 0;
        double y = 0;
        if (!_GetNextData(x, y)) {
            break;
        }
        myOGRLine->addPoint(x, y);
        iCount++;

        bool bStop = false;
        tpercent.SetValue(iCount);
        if (tpercent.IsNewStep() && progress != NULL) {
            if (progress->Update(tpercent.GetPercent(), wxEmptyString) == false) {
                bStop = true;
            }
        }

        if (bStop) {
            wxLogMessage(_("Adding gis data into project stopped by user."));
            break;
        }
    }

    if (myGeomDB->AddGeometry(myOGRLine, -1, GetTarget()) == wxNOT_FOUND) {
        OGRGeometryFactory::destroyGeometry(myOGRLine);
        wxLogError(_("Error importing geometry into the project"));
        wxDELETE(myGeomDB);
        return false;
    }
    OGRGeometryFactory::destroyGeometry(myOGRLine);
    wxDELETE(myGeomDB);
    sv.Pause();
    wxLogMessage(_("%ld feature added in %ld [ms]"), iCount, sv.Time());
    return true;
}


bool tmImportCSV::Import(DataBaseTM *database, wxProgressDialog *progress)
{
    wxASSERT(database);
    if (IsOk() == false) {
        wxLogError(_("Importation failed"));
        return false;
    }

    // skip first line
    wxASSERT(m_TextStream);
    m_TextStream->ReadLine();

    bool bReturn = false;
    switch (GetTarget()) {
        case TOC_NAME_LINES:
            bReturn = _ImportToLineLayer(database, progress);
            break;

        case TOC_NAME_LABELS:
        case TOC_NAME_POINTS:
            bReturn = _ImportToPointLayer(database, progress);
            break;

        default:
            wxFAIL;
            break;
    }

    return bReturn;
}


bool tmImportCSV::GetExistingAttributeValues(const wxString &attName, wxArrayString &values)
{
    _ResetReading();
    wxASSERT(m_TextStream);
    wxString firstLine = m_TextStream->ReadLine();

    wxStringTokenizer myTokenizer(firstLine, _T(";"));
    int iField = -1;
    int counter = 0;
    while (myTokenizer.HasMoreTokens()) {
        wxString token = myTokenizer.GetNextToken();
        if (attName.IsSameAs(token)) {
            iField = counter;
            break;
        }
		counter++;
    }

    if (iField < 0 ) {
        wxLogError(_("Could not find the attribute '%s' in the file"), attName);
        return false;
    }

    while (true) {
        wxString newLine = m_TextStream->ReadLine();
        if (newLine.IsEmpty()) {
            break;
        }

        wxArrayString myTokenArray;
        wxStringTokenizer myTokenizer(newLine, _T(";"));
        while (myTokenizer.HasMoreTokens()) {
            myTokenArray.Add(myTokenizer.GetNextToken());
        }

        if (iField >= (signed) myTokenArray.GetCount()) {
            continue;
        }

        wxString fieldVal = myTokenArray.Item(iField);

        if (!fieldVal.IsEmpty()) {
            bool isNew = true;
            for (int i = 0; i < values.GetCount(); ++i) {
                if (fieldVal.IsSameAs(values.Item(i))) {
                    isNew = false;
                    break;
                }
            }

            if (isNew) {
                values.Add(fieldVal);
            }
        }
    }

    _ResetReading();

    return true;
}


wxArrayInt tmImportCSV::GetTargetSupported()
{
    wxArrayInt mySupportedTargets;
    mySupportedTargets.Add((int) TOC_NAME_POINTS);
    mySupportedTargets.Add((int) TOC_NAME_LABELS);
    return mySupportedTargets;
}

