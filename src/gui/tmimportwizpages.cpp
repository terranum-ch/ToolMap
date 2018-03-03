/***************************************************************************
 tmimportwizpages.cpp

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


#include "tmimportwizpages.h"
#include "tmimportwiz.h"
#include <wx/spinctrl.h>
#include "../gis/tmimportgis.h"
#include "../gis/tmimportcsv.h"


//
////////////// INTRODUCTION WIZARD PAGE ////////////////
//
void ImportWizIntro::_CreateControls()
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer1;
    bSizer1 = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sbSizer1;
    sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("File type:")), wxVERTICAL);

    m_radioBtn1 = new wxRadioButton(this, wxID_ANY, _("ESRI's shapefiles"), wxDefaultPosition, wxDefaultSize,
                                    wxRB_GROUP);
    m_radioBtn1->SetValue(true);
    sbSizer1->Add(m_radioBtn1, 0, wxALL, 5);

    m_radioBtn2 = new wxRadioButton(this, wxID_ANY, _("CSV files (semi-colon separated)"), wxDefaultPosition,
                                    wxDefaultSize, 0);
    sbSizer1->Add(m_radioBtn2, 0, wxALL, 5);

    bSizer1->Add(sbSizer1, 0, wxALL | wxEXPAND, 5);

    //this->SetSizer( bSizer1 );
    //this->Layout();
    //bSizer1->Fit( this );
    SetSizerAndFit(bSizer1);
    this->Centre(wxBOTH);

}

ImportWizIntro::ImportWizIntro(ImportWizard *parent, wxWizardPage *next) :
        wxWizardPageSimple(parent, NULL, next)
{
    m_Parent = parent;
    _CreateControls();
    m_Parent->SetImport(new tmImportGIS());
}

ImportWizIntro::~ImportWizIntro()
{
}


bool ImportWizIntro::TransferDataToWindow()
{
    if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_SHP) {
        m_radioBtn1->SetValue(true);
    } else {
        m_radioBtn2->SetValue(true);
    }
    return true;
}


bool ImportWizIntro::TransferDataFromWindow()
{
    tmImport *myImport = m_Parent->GetImport();
    wxDELETE(myImport);
    wxASSERT(m_radioBtn1->GetValue() != m_radioBtn2->GetValue());

    if (m_radioBtn1->GetValue() == true) {
        m_Parent->SetImport(new tmImportGIS());
    }

    if (m_radioBtn2->GetValue() == true) {
        m_Parent->SetImport(new tmImportCSV());
    }
    return true;
}


//
////////////// INFO WIZARD PAGE ////////////////
//
void ImportWizInfo::_CreateControls()
{
    wxBoxSizer *bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    wxStaticText *m_staticText2;
    m_staticText2 = new wxStaticText(this, wxID_ANY, _("Select a file:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText2->Wrap(-1);
    bSizer3->Add(m_staticText2, 0, wxALL, 5);

    m_FileCtrl = new wxFilePickerCtrl(this, wxID_OPEN, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition,
                                      wxDefaultSize, wxFLP_DEFAULT_STYLE);
    bSizer3->Add(m_FileCtrl, 0, wxEXPAND | wxALL, 5);

    wxStaticBoxSizer *sbSizer2;
    sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("File informations")), wxVERTICAL);

    wxFlexGridSizer *fgSizer1;
    fgSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
    fgSizer1->SetFlexibleDirection(wxBOTH);
    fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    m_InfoLabelCtrl1 = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_InfoLabelCtrl1->Wrap(-1);
    fgSizer1->Add(m_InfoLabelCtrl1, 0, wxALL, 5);

    m_InfoValueCtrl1 = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_InfoValueCtrl1->Wrap(-1);
    fgSizer1->Add(m_InfoValueCtrl1, 0, wxALL, 5);

    m_InfoLabelCtrl2 = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_InfoLabelCtrl2->Wrap(-1);
    fgSizer1->Add(m_InfoLabelCtrl2, 0, wxALL, 5);

    m_InfoValueCtrl2 = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_InfoValueCtrl2->Wrap(-1);
    fgSizer1->Add(m_InfoValueCtrl2, 0, wxALL, 5);

    sbSizer2->Add(fgSizer1, 1, wxEXPAND, 5);

    bSizer3->Add(sbSizer2, 0, wxEXPAND | wxALL, 5);

    SetSizer(bSizer3);
    bSizer3->Fit(this);
}

void ImportWizInfo::OnOpenFile(wxFileDirPickerEvent &event)
{
    tmImport *myImport = m_Parent->GetImport();
    wxASSERT(myImport);

    if (myImport->Open(event.GetPath()) == false) {
        return;
    }

    m_InfoValueCtrl2->SetLabel(wxString::Format(_T("%ld"), myImport->GetFeatureCount()));

    if (myImport->GetFileType() == tmIMPORT_TYPE_SHP) {
        m_InfoValueCtrl1->SetLabel(TM_GIS_SPATIAL_TYPES_STRING[myImport->GetGeometryType()]);
    } else if (myImport->GetFileType() == tmIMPORT_TYPE_CSV) {
        m_InfoValueCtrl1->SetLabel(wxString::Format(_T("%d"), myImport->GetFieldCount()));
    } else {
        wxFAIL;
    }

    if (myImport->GetFileType() == tmIMPORT_TYPE_SHP) {
        if (myImport->GetGeometryType() > LAYER_SPATIAL_POINT) {
            wxLogError(_("Unable to import shapefiles of type : '%s'"),
                       TM_GIS_SPATIAL_TYPES_STRING[myImport->GetGeometryType()].c_str());
            return;
        }
    }

    m_Parent->SetEnableControl(wxID_FORWARD, true);
}


ImportWizInfo::ImportWizInfo(ImportWizard *parent) : wxWizardPage(parent)
{
    m_Parent = parent;
    m_Prev = NULL;
    m_Next = NULL;
    m_PageTarget = new ImportWizTarget(parent, NULL, NULL);
    m_PageCSVOptions = new ImportWizCSVOptions(parent, this, m_PageTarget);
    m_PageTarget->SetPrev(m_PageCSVOptions);
    _CreateControls();

    this->Connect(wxID_OPEN, wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(ImportWizInfo::OnOpenFile));
}

ImportWizInfo::~ImportWizInfo()
{
    wxDELETE(m_PageTarget);
    wxDELETE(m_PageCSVOptions);
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_FILEPICKER_CHANGED,
                     wxFileDirPickerEventHandler(ImportWizInfo::OnOpenFile));

}

wxWizardPage *ImportWizInfo::GetPrev() const
{
    return m_Prev;
}

wxWizardPage *ImportWizInfo::GetNext() const
{
    if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_SHP) {
        m_PageTarget->SetPrev(m_PageCSVOptions->GetPrev());
        return m_PageTarget;
    }
    m_PageTarget->SetPrev(m_PageCSVOptions);
    return m_PageCSVOptions;
}


bool ImportWizInfo::TransferDataToWindow()
{
    wxFilePickerCtrl *myPicker = NULL;
    m_InfoValueCtrl1->SetLabel(wxEmptyString);
    m_InfoValueCtrl2->SetLabel(wxEmptyString);

    m_Parent->SetEnableControl(wxID_FORWARD, false);

    if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_SHP) {
        myPicker = new wxFilePickerCtrl(this, wxID_OPEN, wxEmptyString, _("Select a SHP file"),
                                        _("ESRI's shapefile (*.shp)|*.shp"));
        m_InfoLabelCtrl1->SetLabel(_("Geometry type:"));
        m_InfoLabelCtrl2->SetLabel(_("Number of features:"));
    } else if (m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_CSV) {
        myPicker = new wxFilePickerCtrl(this, wxID_OPEN, wxEmptyString, _("Select a CSV file"),
                                        wxT("Semi-colon separeted file (*.csv)|*.csv"));
        m_InfoLabelCtrl1->SetLabel(_("Number of columns:"));
        m_InfoLabelCtrl2->SetLabel(_("Number of rows:"));
    } else {
        wxFAIL;
    }
    GetSizer()->Replace(m_FileCtrl, myPicker);
    this->RemoveChild(m_FileCtrl);
    wxDELETE(m_FileCtrl);
    m_FileCtrl = myPicker;
    Layout();
    return true;
}


bool ImportWizInfo::TransferDataFromWindow()
{
    m_Parent->SetEnableControl(wxID_FORWARD, true);
    return true;
}


//
////////////// CSV OPTIONS WIZARD PAGE ////////////////
//
void ImportWizCSVOptions::_CreateControls()
{
    wxBoxSizer *bSizer5;
    bSizer5 = new wxBoxSizer(wxVERTICAL);

    wxStaticText *m_staticText7;
    m_staticText7 = new wxStaticText(this, wxID_ANY, _("Select columns for import"), wxDefaultPosition, wxDefaultSize,
                                     0);
    m_staticText7->Wrap(-1);
    bSizer5->Add(m_staticText7, 0, wxALL, 5);

    wxFlexGridSizer *fgSizer2;
    fgSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
    fgSizer2->AddGrowableCol(1);
    fgSizer2->SetFlexibleDirection(wxBOTH);
    fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxStaticText *m_staticText8;
    m_staticText8 = new wxStaticText(this, wxID_ANY, _("X column:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText8->Wrap(-1);
    fgSizer2->Add(m_staticText8, 0, wxALL, 5);

    m_XColCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_XColCtrl->SetSelection(0);
    fgSizer2->Add(m_XColCtrl, 0, wxALL | wxEXPAND, 5);

    wxStaticText *m_staticText10;
    m_staticText10 = new wxStaticText(this, wxID_ANY, _("Y column:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText10->Wrap(-1);
    fgSizer2->Add(m_staticText10, 0, wxALL, 5);

    m_YColCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_YColCtrl->SetSelection(0);
    fgSizer2->Add(m_YColCtrl, 0, wxALL | wxEXPAND, 5);

    bSizer5->Add(fgSizer2, 0, wxEXPAND, 5);

    this->SetSizer(bSizer5);
    bSizer5->Fit(this);
}


ImportWizCSVOptions::ImportWizCSVOptions(ImportWizard *parent, wxWizardPage *prev, wxWizardPage *next) :
        wxWizardPageSimple(parent, prev, next)
{
    m_Parent = parent;
    _CreateControls();
}


ImportWizCSVOptions::~ImportWizCSVOptions()
{
}


bool ImportWizCSVOptions::TransferDataToWindow()
{
    wxASSERT(m_Parent->GetImport()->GetFileType() == tmIMPORT_TYPE_CSV);
    tmImportCSV *myImport = (tmImportCSV *) m_Parent->GetImport();
    wxArrayString myCols = myImport->ListColumns();
    for (unsigned int i = 0; i < myCols.GetCount(); i++) {
        wxString myText = wxString::Format(_("Column: %d, %s"), i + 1,
                                           myCols.Item(i).c_str());
        myCols.Item(i) = myText;
    }

    m_XColCtrl->Clear();
    m_YColCtrl->Clear();
    m_XColCtrl->Append(myCols);
    m_YColCtrl->Append(myCols);

    int myXsel = 0;
    int myYsel = 0;
    if (myImport->GetColumnX() != wxNOT_FOUND && myImport->GetColumnY() != wxNOT_FOUND) {
        myXsel = myImport->GetColumnX();
        myYsel = myImport->GetColumnY();
    }
    m_XColCtrl->SetSelection(myXsel);
    m_YColCtrl->SetSelection(myYsel);
    return true;
}


bool ImportWizCSVOptions::TransferDataFromWindow()
{
    tmImportCSV *myImport = (tmImportCSV *) m_Parent->GetImport();
    if (m_XColCtrl->GetCount() > 0 && m_YColCtrl->GetCount() > 0) {
        myImport->SetColumn(m_XColCtrl->GetSelection(), m_YColCtrl->GetSelection());
    } else {
        myImport->SetColumn(wxNOT_FOUND, wxNOT_FOUND);
    }
    return true;
}


//
////////////// TARGET WIZARD PAGE ////////////////
//
void ImportWizTarget::_CreateControls()
{
    wxBoxSizer *bSizer6;
    bSizer6 = new wxBoxSizer(wxVERTICAL);

    wxStaticText *m_staticText12;
    m_staticText12 = new wxStaticText(this, wxID_ANY, _("Select import target:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText12->Wrap(-1);
    bSizer6->Add(m_staticText12, 0, wxALL, 5);

    wxBoxSizer *bSizer7;
    bSizer7 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *m_staticText13;
    m_staticText13 = new wxStaticText(this, wxID_ANY, _("Import into:"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText13->Wrap(-1);
    bSizer7->Add(m_staticText13, 0, wxALL, 5);

    wxArrayString m_TargetCtrlChoices;
    m_TargetCtrl = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_TargetCtrlChoices, 0);
    m_TargetCtrl->SetSelection(0);
    bSizer7->Add(m_TargetCtrl, 1, wxALL, 5);

    bSizer6->Add(bSizer7, 0, wxEXPAND, 5);

    this->SetSizer(bSizer6);
    bSizer6->Fit(this);
}


ImportWizTarget::ImportWizTarget(ImportWizard *parent, wxWizardPage *prev, wxWizardPage *next) :
        wxWizardPageSimple(parent, prev, next)
{
    m_Parent = parent;
    _CreateControls();
}


ImportWizTarget::~ImportWizTarget()
{
}


bool ImportWizTarget::TransferDataToWindow()
{
    tmImport *myImport = m_Parent->GetImport();
    wxASSERT(myImport);
    wxArrayString mySupportedTargetsName = myImport->GetTargetSupportedName();
    m_TargetCtrl->Clear();
    m_TargetCtrl->Append(mySupportedTargetsName);
    m_TargetCtrl->SetSelection(0);
    return true;
}


bool ImportWizTarget::TransferDataFromWindow()
{
    tmImport *myImport = m_Parent->GetImport();
    wxASSERT(myImport);
    wxArrayInt mySupportedTargets = myImport->GetTargetSupported();
    wxASSERT(mySupportedTargets.GetCount() == m_TargetCtrl->GetCount());
    int myTarget = mySupportedTargets.Item(m_TargetCtrl->GetSelection());
    myImport->SetTarget((TOC_GENERIC_NAME) myTarget);
    return true;
}

