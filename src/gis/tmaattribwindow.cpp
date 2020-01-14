/***************************************************************************
 tmaattribwindow.cpp
                    Display the Advanced Attribution Window
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmaattribwindow.h"

// init static member
wxRect tmAAttribWindow::m_WndPos = wxRect(wxDefaultPosition, wxDefaultSize);


/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::tmAAttribWindow(wxWindow *parent, PrjMemLayersArray *layers,
                                 wxArrayString *values,
                                 const tmLayerValueArray &arrayidname,
                                 wxWindowID id,
                                 const wxString &title, const wxPoint &pos,
                                 const wxSize &size, long style) :
        wxDialog(parent, id, title, pos, size, style)
{
    InitMemberValue();
    CreateControls();

    m_LayerNameID = arrayidname;
    m_Layers = layers;
    m_Values = values;
    m_iTotalControls = GetNumberControls();
    CheckValuesAndControls();
    SetStatusNumberControl(m_iTotalControls, m_Layers->GetCount());
}


/***************************************************************************//**
 @brief Init member values with default values
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::InitMemberValue()
{
    m_AAttribTree = NULL;
    m_Layers = NULL;
    m_Values = NULL;
    m_Ctrls.Clear();
    m_iTotalControls = 0;
    m_Status = NULL;
}


/***************************************************************************//**
 @brief Check for correct number of values and controls
 @details Ensure that we have the same number of values that we have controls
 @return true if we have the same number of controls and values
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAAttribWindow::CheckValuesAndControls()
{
    if (m_iTotalControls <= 0)
        return false;

    if ((signed) m_Values->GetCount() != m_iTotalControls) {
        wxLogError(_T("Not same number of values (%ld) that they are controls (%d)"),
                   m_Values->GetCount(), m_iTotalControls);


        // setting same number of values
        m_Values->Clear();
        for (int i = 0; i < m_iTotalControls; i++)
            m_Values->Add(wxEmptyString);

        return false;
    }
    return true;
}


/***************************************************************************//**
 @brief Display a message into the status bar
 @param ictrl Number of loaded control to display into status bar
 @param layers Number of layers loaded
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
void tmAAttribWindow::SetStatusNumberControl(int ictrl, unsigned int layers)
{
    wxASSERT(m_Status);
    m_Status->SetStatusText(wxString::Format(_("%d Control(s) in %d layer(s)"),
                                             ictrl, layers));
}


/***************************************************************************//**
 @brief Create default controls for UI
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
void tmAAttribWindow::CreateControls()
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer *bSizer20;
    bSizer20 = new wxBoxSizer(wxVERTICAL);

    m_AAttribTree = new tmAAttribTree(this, wxID_ANY);
    bSizer20->Add(m_AAttribTree, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *bSizer21;
    bSizer21 = new wxBoxSizer(wxHORIZONTAL);

    m_CancelBtn = new wxButton(this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer21->Add(m_CancelBtn, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5);

    wxButton *m_button19;
    m_button19 = new wxButton(this, wxID_OK, _("&Attribute"), wxDefaultPosition, wxDefaultSize, 0);
    m_button19->SetDefault();
    bSizer21->Add(m_button19, 0, wxBOTTOM | wxLEFT | wxRIGHT, 5);

    bSizer20->Add(bSizer21, 0, wxALIGN_RIGHT, 5);

    m_Status = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP | wxNO_BORDER);
    bSizer20->Add(m_Status, 0, wxALL | wxGROW, 0);


    this->SetSizer(bSizer20);
    //this->Layout();
    //bSizer20->Fit( this );

    SetWindowPosition();
}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 March 2009
 *******************************************************************************/
tmAAttribWindow::~tmAAttribWindow()
{
    GetWindowPosition();
}


/***************************************************************************//**
 @brief Get Window position
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::GetWindowPosition()
{
    m_WndPos.SetPosition(this->GetPosition());
    m_WndPos.SetSize(this->GetSize());
}


/***************************************************************************//**
 @brief Set Window position
 @details Checks that we are inside the visible part of the screen
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void tmAAttribWindow::SetWindowPosition()
{
    if (m_WndPos.IsEmpty()) {
        this->SetSize(400, 300);
        this->CenterOnParent();
    } else {
        // check that we are inside the screen
        wxSize myScreenSize = wxGetDisplaySize();
        wxRect myScreenRect(wxPoint(0, 0), myScreenSize);
        if (myScreenRect.Contains(m_WndPos)) {
            this->SetSize(m_WndPos);
        } else {
            this->Center(wxVERTICAL | wxHORIZONTAL);
        }
    }
}


/***************************************************************************//**
 @brief Get the number of controls
 @details This function iterate for all controls stored into PrjMemLayersArray.
 This function is called once in the ctor. After that you want to get the
 number of ctrl, use the m_iTotalControls member value.
 @return  Total number of controls returned
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
int tmAAttribWindow::GetNumberControls()
{
    wxASSERT (m_Layers);

    int iNumControl = 0;
    for (unsigned int i = 0; i < m_Layers->GetCount(); i++) {
        iNumControl += m_Layers->Item(i)->m_pLayerFieldArray.GetCount();
    }

    if (iNumControl == 0) {
        wxLogError(_T("No control returned, maybe no fields for advanced attribution"));
    }

    return iNumControl;
}


/***************************************************************************//**
 @brief Transfer data to window
 @details This function is called automatically when the tmAAttribWindow is
 displayed
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAAttribWindow::TransferDataToWindow()
{
    wxASSERT (m_Layers);
    wxASSERT (m_Values);
    wxASSERT (m_AAttribTree);
    //wxASSERT (m_iTotalControls > 0);

    tmAAttribCtrl *mypCtrl = NULL;
    tmAAttribCtrlStruct myCtrl;
    int iCtrlPos = 0;

    m_CancelBtn->SetFocus();
    for (unsigned int i = 0; i < m_Layers->GetCount(); i++) {
        wxString myName = m_LayerNameID.Item(i).m_Value;
        myName.Append(_T(" - ("));
        myName.Append(m_Layers->Item(i)->m_LayerName);
        myName.Append(_T(")"));

        m_AAttribTree->AddLayerNode(myName);
        for (unsigned int j = 0; j < m_Layers->Item(i)->m_pLayerFieldArray.GetCount(); j++) {
            ProjectDefMemoryFields *mypField = m_Layers->Item(i)->m_pLayerFieldArray.Item(j);
            wxASSERT(mypField);
            ProjectDefMemoryFields myField;
            myField = *mypField;
            mypCtrl = m_AAttribTree->AddControl(myField);
            myCtrl.m_Ctrl = mypCtrl;
            m_Ctrls.Add(myCtrl);
            SetValue(iCtrlPos, mypCtrl);
            iCtrlPos++;
        }
    }
    return true;
}


/***************************************************************************//**
 @brief Transfert data from the window
 @details Called automatically when the dialog is closed.
 @author Lucien Schreiber (c) CREALP 2009
 @date 18 March 2009
 *******************************************************************************/
bool tmAAttribWindow::TransferDataFromWindow()
{
    wxASSERT (m_Values->GetCount() == (unsigned) m_iTotalControls);
    bool bStart = true;
    tmAAttribCtrl *myCtrl = NULL;
    for (int i = 0; i < m_iTotalControls; i++) {
        myCtrl = m_AAttribTree->IterateAllCtrl(bStart);
        bStart = false;

        if (myCtrl == NULL) {
            wxASSERT_MSG(0, _T("Control null returned : Error"));
            break;
        }

        m_Values->Item(i) = myCtrl->GetControlValue().c_str();
    }
    return true;
}


/***************************************************************************//**
 @brief Set value to the specified control
 @details Transfert value from the wxArrayString to the tmAAttribCtrl with a
 security. wxArrayString is checked for pos. and if the position exists, we
 also checks for wxEmptyString.
 @param pos the position of the control (index)
 @param ctrl Adress of a valid tmAAttribCtrl object (a check is made in debug
 mode)
 @return  true if all checks are correct and a value was set to the control,
 false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAAttribWindow::SetValue(unsigned int pos, tmAAttribCtrl *ctrl)
{
    wxASSERT (m_Values);
    wxASSERT (ctrl);
    if (pos >= m_Values->GetCount()) {
        wxLogDebug(_T("Value for control @ position %d doesn't exist !"), pos);
        return false;
    }

    wxString mySetVal = m_Values->Item(pos);
    if (mySetVal.IsEmpty()) {
        ctrl->SetEmptyValue();
        return false;
    }

    ctrl->SetControlValue(mySetVal);
    return true;
}

