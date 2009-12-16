/***************************************************************************
								queriespages.h
                    Pages definition for the QueryWizard
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

#ifndef _QUERIESPAGES_H
#define _QUERIESPAGES_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "querieswizard.h"

class QueriesWizard;
class QueriesPageLayer;
class QueriesPageName;
class QueriesPageGeneric;
class QueriesPageSelection;
class QueriesPageExpert;
class QueriesPageAttribut;
class QueriesPageSQL;


class QueriesGeneric {
private:
    wxString m_Name;
	
    wxString m_SQL;
	
};



class QueriesPageIntro : public wxWizardPage {
private:
    QueriesWizard * m_Parent;
	wxRadioBox* m_radiobtn;
	
	QueriesPageLayer * m_PageLayer;
	QueriesPageName * m_PageName;
	QueriesPageGeneric * m_PageGeneric;
	QueriesPageSelection * m_PageSelection;
	QueriesPageExpert * m_PageExpert;
	QueriesPageAttribut * m_PageSelectionAttribut;
	QueriesPageSQL * m_PageExpertSQL;
	
	
	// event function
	void OnPageChanging(wxWizardEvent & event);
	
public:
    QueriesPageIntro(QueriesWizard * parent);
	
    ~QueriesPageIntro();
	
	virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
	
    void CreateControls();
	
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};






class QueriesPageLayer : public wxWizardPageSimple {
private:
	QueriesWizard * m_Parent;
	wxListBox * m_ListLayers;

public:
    QueriesPageLayer(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next);
	
    ~QueriesPageLayer();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
    void CreateControls();
};




class QueriesPageSelection : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxChoice * m_SelTypeList;
    void _CreateControls();
	
	
public:
    QueriesPageSelection(QueriesWizard * parent, wxWizardPage * prev, wxWizardPageSimple * next);
	~QueriesPageSelection();
	virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};



class QueriesPageAttribut : public wxWizardPageSimple {
private:
    void _CreateControls();
	QueriesWizard * m_Parent;
	
	wxCheckBox * m_CheckAdvAttrib;
	wxStaticBoxSizer* m_AdvSizer;
	wxStaticText* m_AdvText;
	wxListBox* m_AdvAttributs;
	
	
public:
    QueriesPageAttribut(QueriesWizard * parent, wxWizardPageSimple * prev, wxWizardPageSimple * next);
	
    ~QueriesPageAttribut();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
};




class QueriesPageGenericData {
private:
    wxArrayInt m_TargetLayer;
	
    wxArrayString m_Description;
	
    wxArrayString m_SQL;
	
    void _Add(int layertarger, const wxString & description, const wxString & sql);
	
	
public:
    QueriesPageGenericData();
	
    ~QueriesPageGenericData();
	
	void GetData(int index, int & layertarget, wxString & description, wxString & sql);
	wxArrayString GetDescription();
};






class QueriesPageGeneric : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxListBox* m_ListGeneric;
	QueriesPageGenericData * m_GenericData;
	
protected:
    void _CreateControls();
	
	
public:
    QueriesPageGeneric(QueriesWizard * parent, wxWizardPage * prev = NULL, wxWizardPage * next = NULL);
    ~QueriesPageGeneric();
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};




class QueriesPageExpert : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxRadioBox * m_LayerType;
	
    void _CreateControls();
	
	
public:
    QueriesPageExpert(QueriesWizard * parent, wxWizardPage * prev, wxWizardPageSimple * next);
    ~QueriesPageExpert();
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};




class QueriesPageSQL : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxTextCtrl * m_SQLText;
    void _CreateControls();
	
	
public:
	QueriesPageSQL(QueriesWizard * parent, wxWizardPageSimple * prev = NULL,
				   wxWizardPageSimple * next = NULL);
	
    ~QueriesPageSQL();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
};




class QueriesPageName : public wxWizardPageSimple {
private:
	wxTextCtrl* m_TextName;
	QueriesWizard * m_Parent;
	
    void _CreateControls();
	
public:
    QueriesPageName(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next);
	
    ~QueriesPageName();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
};





#endif
