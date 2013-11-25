/***************************************************************************
 tmwebframe
 -------------------
 copyright            : (C) 2013 Lucien Schreiber 
 email                : lucien.schreiber at gmail dot com
 ***************************************************************************/
 
#include "tmwebframe.h"

BEGIN_EVENT_TABLE(tmWebFrame, wxFrame)
EVT_WEBVIEW_LOADED(ID_WEBVIEW_CONTROL, tmWebFrame::OnEventLoaded)
EVT_WEBVIEW_ERROR(ID_WEBVIEW_CONTROL, tmWebFrame::OnEventError)
END_EVENT_TABLE()


tmWebFrame::tmWebFrame( wxWindow* parent, wxWindowID id, const wxSize& size, const wxString& title, const wxPoint& pos, long style ) : wxFrame( parent, id, title, pos, size, style, WEBVIEW_WINDOW_NAME ){
    _CreateControls();
    m_Status = TMWEBFRAME_STATUS_NONE;
}



tmWebFrame::~tmWebFrame(){
}



void tmWebFrame::OnEventLoaded (wxWebViewEvent & event){
    m_Status = TMWEBFRAME_STATUS_LOADED;
    wxLogMessage(_("page loaded: %s!"), event.GetTarget());
}



void tmWebFrame::OnEventError (wxWebViewEvent & event){
    m_Status = TMWEBFRAME_STATUS_ERROR;
}


void tmWebFrame::LoadURL (const wxString & url){
    m_Status = TMWEBFRAME_STATUS_NONE;
    GetWebControl()->LoadURL(url);
}


void tmWebFrame::_CreateControls(){
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_WebView = wxWebView::New(this, ID_WEBVIEW_CONTROL );
	bSizer1->Add( m_WebView, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}


void tmWebFrame::LoadGoogle (){
    m_Status = TMWEBFRAME_STATUS_NONE;
    //GetWebControl()->SetPage("<html><body>My Super page...</body></html>", _T(""));
    GetWebControl()->LoadURL(_T("file://localhost/Users/lucien/DATA/PRJ/TOOLMAP2/trunk/resource/web/google_satellite_full.html"));
}


