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
#include "../database/database_tm.h"

class QueriesWizard;
class QueriesPageLayer;
class QueriesPageName;
class QueriesPageGeneric;
class QueriesPageSelection;
class QueriesPageAttribut1;
class QueriesPageExpert;
class QueriesPageAttribut2;
class QueriesPageSQL;
class QueriesPageObject;
class QueriesPageObjectType;
class QueriesPageGeomLine;
class QueriesPageGeomNodes;

class ListGenReport;


const int QUERIES_MARGIN_SIZE = 20;


class QueriesGeneric {
private:
    wxString m_Name;
	
    wxString m_SQL;
	
};


WX_DECLARE_OBJARRAY(wxRadioButton*, wxArrayRadioButton);


class QueriesPageIntro : public wxWizardPage {
private:
    QueriesWizard * m_Parent;
	wxArrayRadioButton m_RadioBtn; 
	DataBaseTM * m_pDB;
		
	QueriesPageLayer * m_PageLayer;
	QueriesPageName * m_PageName;
	QueriesPageGeneric * m_PageGeneric;
	QueriesPageSelection * m_PageSelection;
	QueriesPageAttribut1 * m_PageSelectionAttribut1;
	QueriesPageExpert * m_PageExpert;
	QueriesPageSQL * m_PageExpertSQL;
	QueriesPageObject * m_PageObject;
	QueriesPageObjectType * m_PageObjectType;
	QueriesPageGeomLine * m_PageGeomLine;
	QueriesPageGeomNodes * m_PageGeomNode;
	
	
	int _GetRadioBoxSelection() const;

	
	// event function
	//void OnPageChanging(wxWizardEvent & event);
	//void OnRadioButtonChange( wxCommandEvent& event );
	//DECLARE_EVENT_TABLE();
	
	
public:
     QueriesPageIntro(QueriesWizard * parent, DataBaseTM * database);
	
    ~QueriesPageIntro();
	
	virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
	
    void CreateControls();
	
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};






class QueriesLayerList : public ListGenReport {
public:
    QueriesLayerList(wxWindow * parent, wxWindowID id, wxSize size = wxDefaultSize);
    virtual ~QueriesLayerList();
	
	void SetSelection(long index, bool selected = true);

};






class QueriesPageLayer : public wxWizardPageSimple {
private:
	QueriesWizard * m_Parent;
	QueriesLayerList * m_ListLayers;
	DataBaseTM * m_pDB;
	
	PrjMemLayersArray m_Layers;

	void _CreateControls();

public:
     QueriesPageLayer(QueriesWizard * parent, DataBaseTM * database, wxWizardPage * prev, wxWizardPage * next);
	
    ~QueriesPageLayer();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
    
};




/***************************************************************************//**
Choose spatial type of Objects
Lucien Schreiber (c) CREALP 2010
21 janvier 2010
*******************************************************************************/
class QueriesPageObjectType : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
    DataBaseTM * m_pDB;
	wxRadioBox* m_LayerType;
	
    void _CreateControls();
	
	
public:
    QueriesPageObjectType(QueriesWizard * parent, DataBaseTM * database,
						  wxWizardPage * prev, wxWizardPage * next);
    ~QueriesPageObjectType();
	
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};




/***************************************************************************//**
Choose object value
Lucien Schreiber (c) CREALP 2010
21 janvier 2010
*******************************************************************************/
class QueriesPageObject : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
    DataBaseTM * m_pDB;
	wxListBox* m_ListType;

	PrjMemObjectsArray m_Objects;
	
    void _CreateControls();
	
	
public:
    QueriesPageObject(QueriesWizard * parent, DataBaseTM * database, wxWizardPage * prev, wxWizardPage * next);
    ~QueriesPageObject();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
};







class QueriesPageSelection : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxChoice * m_SelTypeList;
	DataBaseTM * m_pDB;
	PrjMemObjectsArray m_Types;
	
    void _CreateControls();
	
	
public:
     QueriesPageSelection(QueriesWizard * parent, DataBaseTM * database, wxWizardPage * prev, wxWizardPageSimple * next);
	~QueriesPageSelection();
	virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};



 /***************************************************************************//**
Select advanced attribution type
Lucien Schreiber (c) CREALP 2010
27 janvier 2010
 *******************************************************************************/
class QueriesPageAttribut1 : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
    DataBaseTM * m_pDB;
	wxWizardPageSimple *m_PageName;
	wxRadioBox* m_AdvAttribRadio;

	QueriesPageAttribut2 * m_QueryPageAttribut2;
	
	
    void _CreateControls();
	
	
public:
    QueriesPageAttribut1(QueriesWizard * parent, DataBaseTM * database, wxWizardPage * prev, wxWizardPage * next);
    ~QueriesPageAttribut1();
	
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};





class QueriesPageAttribut2 : public wxWizardPageSimple {
private:
	QueriesWizard * m_Parent;
	DataBaseTM* m_pDB;
	wxListBox* m_AdvAttributs;
	wxFlatButton * m_ReloadButton;
	
    void _CreateControls();
	void _LoadAttributs();
	
	void OnReloadAttributs(wxCommandEvent & event );
	void OnDeleteAttribut( wxKeyEvent& event );
	
	
public:
	QueriesPageAttribut2(QueriesWizard * parent, DataBaseTM * database,
						wxWizardPageSimple * prev, wxWizardPageSimple * next);
	
    ~QueriesPageAttribut2();
	
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




/***************************************************************************//**
Selecting small lines
Lucien Schreiber (c) CREALP 2010
27 janvier 2010
*******************************************************************************/
class QueriesPageGeomLine : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
    wxSpinCtrl* m_SpinLineSize;
	DataBaseTM * m_pDB;
    void _CreateControls();
	
	
public:
    QueriesPageGeomLine(QueriesWizard * parent, DataBaseTM * database,
						wxWizardPage * prev, wxWizardPage * next);
    ~QueriesPageGeomLine();
	
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
	
};


/***************************************************************************//**
Selecting lines with few nodes
Lucien Schreiber (c) CREALP 2010
27 janvier 2010
*******************************************************************************/
class QueriesPageGeomNodes : public wxWizardPageSimple {
private:
    QueriesWizard * m_Parent;
	wxSpinCtrl* m_SpinFewNodes;
    DataBaseTM * m_pDB;
    void _CreateControls();
	
	
public:
    QueriesPageGeomNodes(QueriesWizard * parent, DataBaseTM * database,
						 wxWizardPage * prev, wxWizardPage * next);
	
    ~QueriesPageGeomNodes();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
};
	



class QueriesPageName : public wxWizardPageSimple {
private:
	wxTextCtrl* m_TextName;
	wxCheckBox* m_RunQueryBox;
	QueriesWizard * m_Parent;
	
    void _CreateControls();
	
public:
    QueriesPageName(QueriesWizard * parent, wxWizardPage * prev, wxWizardPage * next);
	
    ~QueriesPageName();
	
    virtual bool TransferDataToWindow();
	
    virtual bool TransferDataFromWindow();
	
};





#endif
