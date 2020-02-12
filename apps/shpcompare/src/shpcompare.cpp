/***************************************************************************
 shpcompare.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "shpcompare.h"

ShpCompare::ShpCompare() {
  m_Errors.Clear();
  m_Messages.Clear();
  m_FilesNamesToCheck.Clear();
  OGRRegisterAll();
}

ShpCompare::~ShpCompare() {}

void ShpCompare::SetReferenceFileName(const wxString &filename) {
  m_ReferenceFileName = wxFileName(filename);
}

const wxArrayString *ShpCompare::GetFilesNamesToCheckRef() const {
  return &m_FilesNamesToCheck;
}

void ShpCompare::AddFileNameToCheck(const wxString &filename) {
  m_FilesNamesToCheck.Add(filename);
}

bool ShpCompare::DoCompare(int resultslimit) {
  OGRDataSource *myRefDS = OGRSFDriverRegistrar::Open((const char *)GetReferenceFileName().mb_str(wxConvUTF8), FALSE);
  if (myRefDS == nullptr) {
    m_Messages.Add(wxString::Format(_("Opening '%s' raster failed!"), GetReferenceFileName()));
    return false;
  }
  OGRLayer *myLayer = myRefDS->GetLayer(0);
  for (unsigned int i = 0; i < GetFilesNamesToCheckRef()->GetCount(); i++) {
    if (!_DoCompareOneFile(myLayer, GetFilesNamesToCheckRef()->Item(i))) {
      m_Errors.Add(
          wxString::Format(_("%s differs from %s"), GetReferenceFileName(), GetFilesNamesToCheckRef()->Item(i)));
    }
  }
  OGRDataSource::DestroyDataSource(myRefDS);
  return true;
}

bool ShpCompare::_DoCompareOneFile(OGRLayer *referencelayer, const wxString &filename) {
  wxASSERT(referencelayer);
  OGRDataSource *myTestDS = OGRSFDriverRegistrar::Open((const char *)filename.mb_str(wxConvUTF8), FALSE);
  if (myTestDS == nullptr) {
    m_Messages.Add(wxString::Format(_("Opening '%s' raster failed!"), filename));
    return false;
  }
  OGRLayer *myLayer = myTestDS->GetLayer(0);
  wxASSERT(myLayer);

  myLayer->ResetReading();
  referencelayer->ResetReading();

  // compare fields
  OGRFeatureDefn *myRefDefn = referencelayer->GetLayerDefn();
  OGRFeatureDefn *myTestDefn = myLayer->GetLayerDefn();

  if (myRefDefn->GetGeomType() != myTestDefn->GetGeomType()) {
    OGRDataSource::DestroyDataSource(myTestDS);
    return false;
  }

  if (myRefDefn->GetFieldCount() != myTestDefn->GetFieldCount()) {
    OGRDataSource::DestroyDataSource(myTestDS);
    return false;
  }

  for (unsigned int i = 0; i < myRefDefn->GetFieldCount(); i++) {
    OGRFieldDefn *myRefField = myRefDefn->GetFieldDefn(i);
    OGRFieldDefn *myTestField = myTestDefn->GetFieldDefn(i);

    if (!_DoCompareFields(myRefField, myTestField)) {
      OGRDataSource::DestroyDataSource(myTestDS);
      m_Errors.Add(_("Fields definition differs!"));
      return false;
    }
  }

  // compare features
  long myRefNbFeatures = referencelayer->GetFeatureCount();
  if (myRefNbFeatures != myLayer->GetFeatureCount()) {
    m_Errors.Add(_("Number of features differs"));
    OGRDataSource::DestroyDataSource(myTestDS);
    return false;
  }

  bool featuresCorrect = true;
  long myErrorNumber = 0;
  m_Messages.Add(wxString::Format(_("%ld features to compare"), myRefNbFeatures));
  for (long i = 0; i < myRefNbFeatures; i++) {
    OGRFeature *myRefFeature = referencelayer->GetNextFeature();
    OGRFeature *myTestFeature = myLayer->GetNextFeature();
    wxASSERT(myRefFeature);
    wxASSERT(myTestFeature);

    // compare fields value
    for (int f = 0; f < myRefFeature->GetFieldCount(); f++) {
      wxString myRefField(myRefFeature->GetFieldAsString(f));
      wxString myTestField(myTestFeature->GetFieldAsString(f));
      if (myRefField != myTestField) {
        wxString myFieldName(myRefFeature->GetFieldDefnRef(f)->GetNameRef());
        m_Errors.Add(wxString::Format(_("Field values differs in '%s' (OID=%ld) : %s / %s"), myFieldName,
                                      myRefFeature->GetFID(), myRefField, myTestField));
        myErrorNumber++;
        featuresCorrect = false;
      }
    }

    // compare geometries
    OGRGeometry *myRefGeom = myRefFeature->GetGeometryRef();
    OGRGeometry *myTestGeom = myTestFeature->GetGeometryRef();
    if (!myRefGeom->Equals(myTestGeom)) {
      m_Errors.Add(wxString::Format(_("Geometry differs for FID %ld"), myRefFeature->GetFID()));
      myErrorNumber++;
      featuresCorrect = false;
    }

    OGRFeature::DestroyFeature(myRefFeature);
    OGRFeature::DestroyFeature(myTestFeature);
  }
  OGRDataSource::DestroyDataSource(myTestDS);

  wxString myLayerName(referencelayer->GetName());
  if (!featuresCorrect) {
    m_Errors.Add(wxString::Format(_("%ld features difference found in %s (either geometry or fields values"),
                                  myErrorNumber, myLayerName));
  } else {
    m_Messages.Add(wxString::Format(_("Layer '%s' checked and correct!"), myLayerName));
  }
  return featuresCorrect;
}

bool ShpCompare::_DoCompareFields(OGRFieldDefn *reffield, OGRFieldDefn *testfield) {
  if (reffield == nullptr || testfield == nullptr) {
    m_Errors.Add(_("Field is Null !"));
    return false;
  }

  if (reffield->GetType() != testfield->GetType()) {
    return false;
  }

  wxString myRefdName(reffield->GetNameRef());
  wxString myTestName(testfield->GetNameRef());
  if (myRefdName != myTestName) {
    return false;
  }

  if (reffield->GetPrecision() != testfield->GetPrecision()) {
    return false;
  }

  if (reffield->GetWidth() != testfield->GetWidth()) {
    return false;
  }

  m_Messages.Add(wxString::Format(_("Fields definitions for : '%s' are similar"), myRefdName));
  return true;
}

bool ShpCompare::HasErrors() {
  if (m_Errors.GetCount() > 0) {
    return true;
  }
  return false;
}
