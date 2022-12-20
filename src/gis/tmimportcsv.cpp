/***************************************************************************
 tmimportcsv.cpp
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
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

#include "../core/tmpercent.h"
#include "../database/database_tm.h"
#include "tmgisdatavector.h"

bool tmImportCSV::_ResetReading() {
    wxASSERT(m_FileStream);
    if (!m_FileStream->IsSeekable()) {
        wxLogError(_("Unable to reset reading"));
        return false;
    }
    m_FileStream->SeekI(0);
    return true;
}

bool tmImportCSV::_GetNextData(wxArrayString& tokenArray) {
    tokenArray.Clear();

    wxASSERT(m_TextStream);

    wxString myLine = m_TextStream->ReadLine();
    if (myLine.IsEmpty()) {
        return false;
    }

    wxStringTokenizer myTokenizer(myLine, _T(";"), wxTOKEN_RET_EMPTY_ALL);
    while (myTokenizer.HasMoreTokens()) {
        tokenArray.Add(myTokenizer.GetNextToken());
    }

    return true;
}

bool tmImportCSV::_GetCoordinates(const wxArrayString& tokenArray, double& x, double& y) {
    if (m_Xcolumn == wxNOT_FOUND || m_Ycolumn == wxNOT_FOUND) {
        return false;
    }

    if (m_Xcolumn >= (signed)tokenArray.GetCount() && m_Ycolumn >= (signed)tokenArray.GetCount()) {
        return false;
    }

    double myX = 0.0;
    double myY = 0.0;

    if (!tokenArray.Item(m_Xcolumn).ToCDouble(&myX)) {
        wxLogMessage(tokenArray.Item(m_Xcolumn));
        return false;
    }

    if (!tokenArray.Item(m_Ycolumn).ToCDouble(&myY)) {
        wxLogMessage(tokenArray.Item(m_Ycolumn));
        return false;
    }

    x = myX;
    y = myY;

    return true;
}

tmImportCSV::tmImportCSV() {
    m_Xcolumn = wxNOT_FOUND;
    m_Ycolumn = wxNOT_FOUND;
    m_TextStream = nullptr;
    m_FileStream = nullptr;
    m_FileType = tmIMPORT_TYPE_CSV;
}

tmImportCSV::~tmImportCSV() {
    wxDELETE(m_FileStream);
    wxDELETE(m_TextStream);
}

bool tmImportCSV::Open(const wxFileName& filename) {
    wxDELETE(m_FileStream);
    wxDELETE(m_TextStream);

    if (!tmImport::Open(filename)) {
        return false;
    }

    m_FileStream = new wxFileInputStream(filename.GetFullPath());
    if (m_FileStream == nullptr) {
        wxLogError(_("Unable to open '%s'"), filename.GetFullName().c_str());
        return false;
    }

    m_TextStream = new wxTextInputStream(*m_FileStream);
    wxASSERT(m_TextStream);

    m_GeometryType = LAYER_SPATIAL_POINT;  // CSV are always points.

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

void tmImportCSV::ListFields() {
    m_Fields.Clear();
    _ResetReading();

    wxASSERT(m_TextStream);
    wxString myLine1 = m_TextStream->ReadLine();

    wxStringTokenizer tokenizer(myLine1, _T(";"));
    while (tokenizer.HasMoreTokens()) {
        wxString token = tokenizer.GetNextToken();
        m_Fields.Add(token);
    }

    _ResetReading();
}

wxArrayString tmImportCSV::GetFieldsList() {
    if (m_Fields.IsEmpty()) {
        ListFields();
    }

    return m_Fields;
}

void tmImportCSV::GuessXYfields() {
    if (m_Xcolumn == wxNOT_FOUND && m_Ycolumn == wxNOT_FOUND) {
        for (int i = 0; i < m_Fields.GetCount(); i++) {
            wxString val = m_Fields.Item(i);
            if (val.IsSameAs("x", false) || val.IsSameAs("lon", false)) {
                m_Xcolumn = i;
            }
            if (val.IsSameAs("y", false) || val.IsSameAs("lat", false)) {
                m_Ycolumn = i;
            }
        }
    }
}

bool tmImportCSV::GetFieldNames(wxArrayString& Fields) {
    Fields = GetFieldsList();

    return true;
}

void tmImportCSV::SetXYColumn(int x, int y) {
    m_Xcolumn = x;
    m_Ycolumn = y;
}

bool tmImportCSV::IsOk() {
    if (!tmImport::IsOk()) {
        return false;
    }

    if (m_Xcolumn == wxNOT_FOUND || m_Ycolumn == wxNOT_FOUND) {
        return false;
    }

    if (m_TextStream == nullptr) {
        return false;
    }
    return true;
}

bool tmImportCSV::_ImportToPointLayer(DataBaseTM* database, PrjDefMemManage* prj, wxProgressDialog* progress) {
    wxASSERT(database);
    tmGISDataVectorMYSQL* myGeomDB = new tmGISDataVectorMYSQL();
    tmGISDataVectorMYSQL::SetDataBaseHandle(database);

    long iCount = 0;
    wxStopWatch sv;
    tmPercent tpercent(GetFeatureCount());
    while (true) {
        wxArrayString tokenArray;
        if (!_GetNextData(tokenArray)) {
            break;
        }

        if (ShouldSkipObjectKind(tokenArray)) {
            continue;
        }

        double x = 0;
        double y = 0;
        if (!_GetCoordinates(tokenArray, x, y)) {
            break;
        }
        OGRPoint* myOGRPt = (OGRPoint*)OGRGeometryFactory::createGeometry(wkbPoint);
        myOGRPt->setCoordinateDimension(2);
        myOGRPt->setX(x);
        myOGRPt->setY(y);

        long oid = myGeomDB->AddGeometry(myOGRPt, -1, GetTarget());
        if (oid == wxNOT_FOUND) {
            OGRGeometryFactory::destroyGeometry(myOGRPt);
            wxLogError(_("Error importing geometry into the project"));
            break;
        }
        iCount++;
        OGRGeometryFactory::destroyGeometry(myOGRPt);

        if (prj != nullptr) {
            wxArrayLong oids;
            oids.Add(oid);

            if (!SetObjectKind(database, prj, tokenArray, oids)) {
                break;
            }

            if (!SetAttributes(database, prj, tokenArray, oids)) {
                break;
            }
        }

        bool bStop = false;
        tpercent.SetValue(iCount);
        if (tpercent.IsNewStep() && progress != nullptr) {
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

bool tmImportCSV::Import(DataBaseTM* database, PrjDefMemManage* prj, wxProgressDialog* progress) {
    wxASSERT(database);
    if (!IsOk()) {
        wxLogError(_("Importation failed"));
        return false;
    }

    wxASSERT(m_TextStream);
    ListFields();

    // skip first line
    m_TextStream->ReadLine();

    bool bReturn = false;
    switch (GetTarget()) {
        case TOC_NAME_LABELS:
        case TOC_NAME_POINTS:
            bReturn = _ImportToPointLayer(database, prj, progress);
            break;

        default:
            wxFAIL;
            break;
    }

    return bReturn;
}

bool tmImportCSV::GetExistingAttributeValues(const wxString& attName, wxArrayString& values) {
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

    if (iField < 0) {
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

        if (iField >= (signed)myTokenArray.GetCount()) {
            continue;
        }

        wxString fieldVal = myTokenArray.Item(iField);

        if (!fieldVal.IsEmpty()) {
            bool isNew = true;
            for (int i = 0; i < values.GetCount(); ++i) {
                if (fieldVal.IsSameAs(values.Item(i), false)) {
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

wxArrayInt tmImportCSV::GetTargetSupported() {
    wxArrayInt mySupportedTargets;
    mySupportedTargets.Add((int)TOC_NAME_POINTS);
    mySupportedTargets.Add((int)TOC_NAME_LABELS);
    return mySupportedTargets;
}
