/***************************************************************************
 tmimportdatawiz.cpp
 
 -------------------
 copyright            : (C) 2018 TERRANUM Pascal Horton
 email                : pascal.horton at terranum dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "tmimportdatawiz.h"
#include "../gis/tmimport.h"
#include "../gis/tmimportgis.h"
#include "../gis/tmimportcsv.h"
#include "../database/database_tm.h"

ImportDataWizard::ImportDataWizard(wxWindow *window, wxWindowID id, DataBaseTM *database) :
        tmWizardImport::tmWizardImport(window, id, _("Import data")),
        m_Import(nullptr),
        m_pDatabase(database)
{
    m_Import = new tmImportGIS();
    this->Connect( wxID_ANY, wxEVT_WIZARD_BEFORE_PAGE_CHANGED, wxWizardEventHandler( ImportDataWizard::OnWizardBeforePageChanged ) );
}

ImportDataWizard::~ImportDataWizard()
{
    wxDELETE(m_Import);
    this->Disconnect( wxID_ANY, wxEVT_WIZARD_BEFORE_PAGE_CHANGED, wxWizardEventHandler( ImportDataWizard::OnWizardBeforePageChanged ) );
}

int ImportDataWizard::GetPageIndex(wxWizardPage* page)
{
    for (int i = 0; i < m_pages.size(); ++i) {
        if (m_pages[i] == page) {
            return i;
        }
    }
    wxFAIL;

    return wxNOT_FOUND;
}

void ImportDataWizard::OnWizardBeforePageChanged(wxWizardEvent &event)
{
    auto *page = dynamic_cast<wxWizardPageSimple *> (event.GetPage());

    switch (GetPageIndex(page)) {
        case (0): { // Leaving selection the file type (SHP or CSV)

            // Change the file picker accordingly to the type of file.
            wxDELETE(m_Import);
            wxASSERT(m_radioBtnSHP->GetValue() != m_radioBtnCSV->GetValue());
            ResetFilePicker(page);
            EnableNextButton(false);

            break;
        }
        case (1): { // Leaving File picker page

            // If going back, enable the Next button again
            if (!event.GetDirection()) {
                EnableNextButton();
                return;
            }

            if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
                // Shapefiles do not need the definition of X/Y
                page->SetNext(m_pages[3]);
            } else {
                page->SetNext(m_pages[2]);
                SetXYColumnsOptions();
            }

            SetTargetsOptions();

            break;
        }
        case (2):{ // Leaving choice of X/Y columns

            wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            GetXYColumnsSelection();

            break;
        }
        case (3):{ // Leaving choice of target

            // If going back, shapefiles do not need the definition of X/Y
            if (!event.GetDirection()) {
                if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
                    page->SetPrev(m_pages[1]);
                } else {
                    page->SetPrev(m_pages[2]);
                }
                return;
            }

            GetTargetSelection();

            // If frame, terminate
            if(m_Import->GetTarget() == TOC_NAME_FRAME) {
                page->SetNext(nullptr);
                return;
            }

            SetLayerOptions();

            break;
        }
        case (4):{ // Leaving choice of layer

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            GetLayerSelection();



            EnableNextButton(false);

            break;
        }
        case (5):{

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            EnableNextButton(false);

            break;
        }
        case (6):{

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            break;
        }
        default:
            wxLogError(_("Page index not recognized (%d)."), GetPageIndex(page));
    }

}

void ImportDataWizard::GetLayerSelection() const
{
    m_Import->SetLayerName(m_choiceLayer->GetStringSelection());
}

void ImportDataWizard::SetLayerOptions() const
{
    m_choiceLayer->Clear();
    m_choiceLayer->Append(m_pDatabase->GetLayerNameByType(m_Import->GetTarget()));
    m_choiceLayer->SetSelection(0);
}

void ImportDataWizard::GetTargetSelection() const
{
    wxArrayInt supportedTargets = m_Import->GetTargetSupported();
    wxASSERT(supportedTargets.GetCount() == m_choiceTarget->GetCount());
    int myTarget = supportedTargets.Item(m_choiceTarget->GetSelection());
    m_Import->SetTarget((TOC_GENERIC_NAME) myTarget);
}

void ImportDataWizard::SetTargetsOptions() const
{
    wxArrayString supportedTargetsName = m_Import->GetTargetSupportedName();
    m_choiceTarget->Clear();
    m_choiceTarget->Append(supportedTargetsName);
    m_choiceTarget->SetSelection(0);
}

void ImportDataWizard::GetXYColumnsSelection() const
{
    wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

    tmImportCSV *importCSV = (tmImportCSV *) m_Import;
    if (m_choiceX->GetCount() > 0 && m_choiceY->GetCount() > 0) {
        importCSV->SetColumn(m_choiceX->GetSelection(), m_choiceY->GetSelection());
    } else {
        importCSV->SetColumn(wxNOT_FOUND, wxNOT_FOUND);
    }
}

void ImportDataWizard::SetXYColumnsOptions() const
{
    wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

    // Fill X/Y lists
    tmImportCSV *importCSV = (tmImportCSV *) m_Import;
    wxArrayString cols = importCSV->ListColumns();
    for (unsigned int i = 0; i < cols.GetCount(); i++) {
        wxString colText = wxString::Format(_("Column %d: %s"), i + 1, cols.Item(i).c_str());
        cols.Item(i) = colText;
    }

    m_choiceX->Clear();
    m_choiceY->Clear();
    m_choiceX->Append(cols);
    m_choiceY->Append(cols);

    int xSelection = 0;
    int ySelection = 0;
    if (importCSV->GetColumnX() != wxNOT_FOUND && importCSV->GetColumnY() != wxNOT_FOUND) {
        xSelection = importCSV->GetColumnX();
        ySelection = importCSV->GetColumnY();
    }
    m_choiceX->SetSelection(xSelection);
    m_choiceY->SetSelection(ySelection);
}

void ImportDataWizard::ResetFilePicker(const wxWizardPageSimple *page)
{
    wxFilePickerCtrl *filePicker = NULL;

    m_infoValueCtrl1->SetLabel(wxEmptyString);
    m_infoValueCtrl2->SetLabel(wxEmptyString);

    if (m_radioBtnSHP->GetValue()) {

        m_Import = new tmImportGIS();
        filePicker = new wxFilePickerCtrl(page->GetNext(), wxID_ANY, wxEmptyString, _("Select a SHP file"),
                                          _("ESRI's shapefile (*.shp)|*.shp"));
        m_infoLabelCtrl1->SetLabel(_("Geometry type:"));
        m_infoLabelCtrl2->SetLabel(_("Number of features:"));

    } else if (m_radioBtnCSV->GetValue()) {

        m_Import = new tmImportCSV();
        filePicker = new wxFilePickerCtrl(page->GetNext(), wxID_ANY, wxEmptyString, _("Select a CSV file"),
                                          _("Semi-colon separeted file (*.csv)|*.csv"));
        m_infoLabelCtrl1->SetLabel(_("Number of columns:"));
        m_infoLabelCtrl2->SetLabel(_("Number of rows:"));

    }

    m_sizerFilePath->Replace(m_filePicker, filePicker);
    page->GetNext()->RemoveChild(m_filePicker);
    m_filePicker->Disconnect(wxEVT_COMMAND_FILEPICKER_CHANGED,
                             wxFileDirPickerEventHandler(ImportDataWizard::OnFileChanged), NULL, this);
    wxDELETE(m_filePicker);
    m_filePicker = filePicker;
    m_filePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED,
                          wxFileDirPickerEventHandler(ImportDataWizard::OnFileChanged), NULL, this);
    page->GetNext()->Layout();
}

void ImportDataWizard::OnWizardFinished(wxWizardEvent &event)
{

}

void ImportDataWizard::EnableNextButton(bool state)
{
    SetControlEnable(wxID_FORWARD, state);
}

void ImportDataWizard::EnableBackButton(bool state)
{
    SetControlEnable(wxID_BACKWARD, state);
}

void ImportDataWizard::SetControlEnable(int id, bool state)
{
    wxWindow *win = wxWindow::FindWindowById(id);
    if (win) win->Enable(state);
}

void ImportDataWizard::OnFileChanged(wxFileDirPickerEvent &event)
{
    wxASSERT(m_Import);

    if (!m_Import->Open(event.GetPath())) {
        return;
    }

    m_infoValueCtrl2->SetLabel(wxString::Format(_T("%ld"), m_Import->GetFeatureCount()));

    if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
        m_infoValueCtrl1->SetLabel(TM_GIS_SPATIAL_TYPES_STRING[m_Import->GetGeometryType()]);
    } else if (m_Import->GetFileType() == tmIMPORT_TYPE_CSV) {
        m_infoValueCtrl1->SetLabel(wxString::Format(_T("%d"), m_Import->GetFieldCount()));
    } else {
        wxFAIL;
    }

    if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
        if (m_Import->GetGeometryType() > LAYER_SPATIAL_POINT) {
            wxLogError(_("Unable to import shapefiles of type : '%s'"),
                       TM_GIS_SPATIAL_TYPES_STRING[m_Import->GetGeometryType()].c_str());
            return;
        }
    }

    EnableNextButton();
}