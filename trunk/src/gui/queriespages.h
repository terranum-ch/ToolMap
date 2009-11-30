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


#endif
