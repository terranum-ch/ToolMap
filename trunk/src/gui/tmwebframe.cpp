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
EVT_TIMER(wxID_ANY, tmWebFrame::OnTimmerEnd)
EVT_CLOSE(tmWebFrame::OnClose)
END_EVENT_TABLE()


tmWebFrame::tmWebFrame( wxWindow* parent, wxWindowID id, const wxSize& size, const wxString& title, const wxPoint& pos, long style ) : wxFrame( parent, id, title, pos, wxDefaultSize, style, WEBVIEW_WINDOW_NAME ){
    this->SetClientSize(size);
    _CreateControls();
    m_Status = TMWEBFRAME_STATUS_NONE;
    m_PageName = wxEmptyString;
    m_ZoomAfterLoading = tmRealRect();
}



tmWebFrame::~tmWebFrame(){
}



void tmWebFrame::OnEventLoaded (wxWebViewEvent & event){
    m_Status = TMWEBFRAME_STATUS_LOADED;
    wxLogMessage(_("page loaded!"));
    
    if (m_ZoomAfterLoading != tmRealRect()) {
        ZoomToExtend(m_ZoomAfterLoading);
        m_ZoomAfterLoading = tmRealRect();
    }
}



void tmWebFrame::OnEventError (wxWebViewEvent & event){
    m_Status = TMWEBFRAME_STATUS_ERROR;
    wxLogError(_("WebError: %d "), event.GetInt());
}



void tmWebFrame::OnClose (wxCloseEvent & event){
    Hide();
    event.Veto();
}


void tmWebFrame::OnTimmerEnd (wxTimerEvent & event){
    m_Status = TMWEBFRAME_STATUS_TIMEOUT;
}


void tmWebFrame::LoadURL (const wxString & url){
    m_Status = TMWEBFRAME_STATUS_NONE;
    m_ZoomAfterLoading = tmRealRect();
    
    GetWebControl()->LoadURL(url);
}


void tmWebFrame::_CreateControls(){
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_WebView = wxWebView::New(this, ID_WEBVIEW_CONTROL);
	bSizer1->Add( m_WebView, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}


void tmWebFrame::LoadPage (const wxString & pagename, tmRealRect coord){
    if (m_PageName == pagename) {
        return;
    }
    m_PageName = pagename;
    m_Status = TMWEBFRAME_STATUS_NONE;
    m_ZoomAfterLoading = tmRealRect();
    
	wxString myWebPagePathText = _T("/../../share/toolmap");
#ifdef __WXMSW__
	myWebPagePathText.Replace(_T("/"), _T("\\"));
#endif
	wxFileName myWebPath (wxStandardPaths::Get().GetExecutablePath() +  myWebPagePathText, pagename);
     myWebPath.Normalize();
    
    if (myWebPath.Exists() == false) {
        wxLogError(_("Page: %s didn't exists!"), myWebPath.GetFullPath());
        return;
    }
    GetWebControl()->LoadURL(myWebPath.GetFullPath());
    return;
    
    // unused...
    // load page into wxstring and add zoom function
    wxTextFile myTxtFile ( myWebPath.GetFullPath() );
    myTxtFile.Open();
    wxString myWebPageTxt = myTxtFile.GetFirstLine();
    tmRealRect myEmptyRealRect = tmRealRect();
    while (!myTxtFile.Eof()) {
        wxString myLine = myTxtFile.GetNextLine() + _T("\n");
        myLine.Replace(_T("%"), _T("%%"));
        if (myLine.Matches(_T("*// TODO: Add zoom code here*")) && coord != myEmptyRealRect) {
            myLine = wxString::Format(_T("map.zoomToExtent(new OpenLayers.Bounds(%f, %f, %f, %f), true);"),coord.x_min, coord.y_min, coord.x_max, coord.y_max);
        }
        myWebPageTxt.Append(myLine);
    }
    
    wxLogDebug(myWebPageTxt);
	wxLogDebug(myWebPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
    GetWebControl()->SetPage(myWebPageTxt, myWebPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
}



void tmWebFrame::ZoomToExtend (tmRealRect coord){
    // delay zooming if page not loaded!
    if (m_Status != TMWEBFRAME_STATUS_LOADED) {
        m_ZoomAfterLoading = coord;
        return;
    }
    
     wxString myCode = wxString::Format(_T("map.zoomToExtent(new OpenLayers.Bounds(%f, %f, %f, %f), true);"),coord.x_min, coord.y_min, coord.x_max, coord.y_max);
    GetWebControl()->RunScript(myCode);
}


wxBitmap * tmWebFrame::GetPageAsBitmap (const wxSize new_size){
    if (m_Status != TMWEBFRAME_STATUS_LOADED) {
        // page not loaded... wait
        wxTimer myTimmer (this);
        myTimmer.StartOnce(WEB_MAX_WAIT_MS);
        while (m_Status == TMWEBFRAME_STATUS_NONE) {
            wxTheApp->Yield();
        }
    }
    
    if (m_Status != TMWEBFRAME_STATUS_LOADED) {
        wxLogMessage(_("Error loading web raster: %d"), m_Status);
        return NULL;
    }
    
    // copy frame as bitmap
    wxSize myClientSize = GetClientSize();
    if (myClientSize.GetWidth() == 0 || myClientSize.GetHeight() == 0) {
        wxLogError(_("Web frame size is 0, 0. Unable to get a web raster"));
        return NULL;
    }
    
    wxBitmap myTmpBmp;
    if (new_size == wxDefaultSize) {
        myTmpBmp.Create(myClientSize);
    }
    else {
        myTmpBmp.Create(new_size);
    }
    
    wxClientDC myDC (m_WebView);    
    wxMemoryDC myBmpDC;
    myBmpDC.SelectObject(myTmpBmp);
    if (new_size == wxDefaultSize){
        myBmpDC.Blit(0, 0, myClientSize.GetWidth(), myClientSize.GetHeight(), &myDC, 0, 0);
    }
    // new size asked! Stretch bitmap
    else {
        myBmpDC.StretchBlit(0, 0, new_size.GetWidth(), new_size.GetHeight(), &myDC, 0, 0, myClientSize.GetWidth(), myClientSize.GetHeight());
    }
    myBmpDC.SelectObject(wxNullBitmap);
    
    wxImage myImg = myTmpBmp.ConvertToImage();
	wxFileName myTempImageName (wxStandardPaths::Get().GetAppDocumentsDir(), _T("test_image_toolmap.png"));
	myImg.SaveFile(myTempImageName.GetFullPath());
    
    return new wxBitmap( myTmpBmp ); 
}

