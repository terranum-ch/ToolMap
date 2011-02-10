/***************************************************************************
								tmabout_dlg.cpp
					about dialog (inspired by firefox about)
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber
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

// comment doxygen

#include "tmabout_dlg.h"


// image
#include "../img/misc_bmp.h"
#include "../core/svn_version.h"	// for latest svn version


BEGIN_EVENT_TABLE( tmAboutDLG, wxDialog )
	EVT_BUTTON (ID_ABOUT_BTN,tmAboutDLG::OnButton)
END_EVENT_TABLE()



tmAboutDLG::tmAboutDLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	CreateControls(parent);
}

tmAboutDLG::~tmAboutDLG()
{
}


wxString tmAboutDLG::GetVersionText()
{
	wxString myVersion = _("version 2.1.");
	myVersion.Append(SVN_VERSION);
	myVersion.Append(_T(" - "));
	myVersion.Append(g_CodeName);
	return myVersion;

}


wxString tmAboutDLG::GetAuthorsText()
{
	wxString myContributors = _("<CENTER><B>Programming</B><BR>Lucien Schreiber<BR><BR>");
	myContributors.Append( _("<CENTER><B>Design and Functions</B><BR>Pascal Ornstein<BR>Mario Sartori<BR>Lucien Schreiber<BR><BR>"));
	myContributors.Append( _("<CENTER><B>Main icon</B><BR>Chantal Bloch"));

	myContributors.Append(_T("</CENTER>"));
	return myContributors;
}



wxString tmAboutDLG::GetButtonText()
{
	if (m_PanelLicence->IsShown()==true)
		return _("Show contributors");
	else
		return _("Show license");
}


void tmAboutDLG::CreateControls(wxWindow * parent)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );

	wxPanel* myPanelImg;
	myPanelImg = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	myPanelImg->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );

	wxStaticBitmap * m_bitmap1 = new wxStaticBitmap( myPanelImg, wxID_ANY, *_img_icon_toolmap_about, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer37->Add( m_bitmap1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_TextVersion = new wxStaticText( myPanelImg, wxID_ANY, GetVersionText(), wxDefaultPosition, wxDefaultSize, 0 );
	m_TextVersion->Wrap( -1 );
	m_TextVersion->SetFont( wxFont( 18, 70, 90, 92, false, wxEmptyString ) );

	bSizer37->Add( m_TextVersion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer37->Add( 0, 5, 1, wxEXPAND, 5 );

	wxStaticText* m_staticText23;
	m_staticText23 = new wxStaticText( myPanelImg, wxID_ANY, _("complex geological GIS made simple"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	bSizer37->Add( m_staticText23, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer37->Add( 0, 5, 1, wxEXPAND, 5 );

	myPanelImg->SetSizer( bSizer37 );
	myPanelImg->Layout();
	bSizer37->Fit( myPanelImg );
	bSizer36->Add( myPanelImg, 1, wxEXPAND, 5 );

	m_PanelLicence = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_PanelLicence->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	wxStaticText* m_staticText22;
	m_staticText22 = new wxStaticText( m_PanelLicence, wxID_ANY, _(" Copyright \u00A9 2011  CREALP \nCopyright \u00A9 2011 SWISSTOPO\n\nThis program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText22->Wrap( -1 );
	wxFont myOrgFont = m_staticText22->GetFont();
	int mySize = myOrgFont.GetPointSize();
#ifdef __WINDOWS__
	mySize++;
#endif
	m_staticText22->SetFont( wxFont( mySize-1, 70, 90, 90, false, wxEmptyString ) );

	bSizer40->Add( m_staticText22, 0, wxALL, 5 );

	m_PanelLicence->SetSizer( bSizer40 );
	m_PanelLicence->Layout();
	bSizer40->Fit( m_PanelLicence );
	bSizer36->Add( m_PanelLicence, 0, wxEXPAND, 5 );

	m_PanelAuthor = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_PanelAuthor->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	m_PanelAuthor->Hide();

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );

	wxHtmlWindow* m_HtmlAuthors;
	m_HtmlAuthors = new wxHtmlWindow( m_PanelAuthor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_NO_SELECTION|wxHW_SCROLLBAR_AUTO );
	m_HtmlAuthors->SetPage(GetAuthorsText());
	bSizer41->Add( m_HtmlAuthors, 1, wxALL|wxEXPAND, 5 );

	m_PanelAuthor->SetSizer( bSizer41 );
	m_PanelAuthor->Layout();
	bSizer41->Fit( m_PanelAuthor );
	bSizer36->Add( m_PanelAuthor, 1, wxEXPAND, 5 );

	wxPanel* m_PanelButton;
	m_PanelButton = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxHORIZONTAL );

	wxHyperlinkCtrl* m_hyperlink2;
	m_hyperlink2 = new wxHyperlinkCtrl( m_PanelButton, wxID_ANY, _("www.toolmap.ch"), wxT("http://www.toolmap.ch"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer39->Add( m_hyperlink2, 0, wxALL, 5 );


	bSizer39->Add( 0, 0, 1, wxEXPAND, 5 );

	m_Button = new wxButton( m_PanelButton, ID_ABOUT_BTN ,GetButtonText(), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_Button, 0, wxALL, 5 );

	bSizer38->Add( bSizer39, 1, wxEXPAND, 5 );

	m_PanelButton->SetSizer( bSizer38 );
	m_PanelButton->Layout();
	bSizer38->Fit( m_PanelButton );
	bSizer36->Add( m_PanelButton, 0, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer36 );
	this->Layout();
	bSizer36->Fit( this );
	CenterOnParent(wxBOTH);
}



void tmAboutDLG::OnButton(wxCommandEvent & event)
{
	bool myStatus = m_PanelLicence->IsShown();

	m_PanelLicence->Show(!myStatus);
	m_PanelAuthor->Show(myStatus);
	m_Button->SetLabel(GetButtonText());
    this->Layout();
	event.Skip();
}
