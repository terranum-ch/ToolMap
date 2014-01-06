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
    m_LoadingTimer.SetOwner(this);
    m_ZoomingTimer.SetOwner(this);
}



tmWebFrame::~tmWebFrame(){
}



void tmWebFrame::OnEventLoaded (wxWebViewEvent & event){
    m_Status = TMWEBFRAME_STATUS_LOADED;
    wxLogMessage(_("page loaded!"));
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
    wxLogMessage(_("timeout!"));
}


void tmWebFrame::LoadURL (const wxString & url){
    m_Status = TMWEBFRAME_STATUS_NONE;
    GetWebControl()->LoadURL(url);
}


void tmWebFrame::_CreateControls(){
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_WebView = wxWebView::New(this, ID_WEBVIEW_CONTROL);
	m_WebView->SetWindowStyle(wxBORDER_NONE);
	bSizer1->Add( m_WebView, 1, wxEXPAND, 0 );
	
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


bool tmWebFrame::IsLoaded (){
    if (m_WebView->IsBusy() == false) {
        m_LoadingTimer.Stop();
        return true;
    }
    
    // start a timer
    if (m_LoadingTimer.IsRunning() == false) {
        m_LoadingTimer.StartOnce(WEB_MAX_WAIT_LOADING_MS);
        wxLogMessage("Starting timer");
    }
    else {
        wxTheApp->Yield();
    }
    return false;
}


void tmWebFrame::ZoomToExtend (tmRealRect coord){
    if (m_Status != TMWEBFRAME_STATUS_LOADED) {
        wxLogError(_("Page not loaded (%d), zooming isn't possible"), m_Status);
        return;
    }
    
    wxString myCode = wxString::Format(_T("map.zoomToExtent(new OpenLayers.Bounds(%f, %f, %f, %f), false);"),
                                       coord.x_min, coord.y_min,
                                       coord.x_max, coord.y_max);
    GetWebControl()->RunScript(myCode);
}



bool tmWebFrame::HasZoomed(){
    if (m_Status == TMWEBFRAME_STATUS_TIMEOUT) {
        m_Status = TMWEBFRAME_STATUS_LOADED;
        return true;
    }
    
    if (m_ZoomingTimer.IsRunning() == false) {
        m_ZoomingTimer.StartOnce(WEB_WAIT_ZOOMING_MS);
        wxLogMessage("Starting zooming timer");
    }
    else {
        wxTheApp->Yield();
    }
    return false;
}



wxBitmap * tmWebFrame::GetPageAsBitmap (const wxSize new_size){
    if (m_Status != TMWEBFRAME_STATUS_LOADED) {
        wxLogMessage(_("Error loading web raster: %d"), m_Status);
        return NULL;
    }
    
    // copy screen
    // wxClientDC blit is not working on OSX.
/*#ifdef __WXMAC__
    wxRect myWebPosition = m_WebView->GetScreenRect();
    wxScreenDC myScreenDC;
    wxBitmap myFullScreenBmp = myScreenDC.GetAsBitmap();
    
    wxSize myCutSize (myWebPosition.GetWidth(), myWebPosition.GetHeight());
    if (new_size != wxDefaultSize) {
        myCutSize = new_size;
    }
    wxBitmap myWebBmp = myFullScreenBmp.GetSubBitmap(wxRect(myWebPosition.GetX(), myWebPosition.GetY(), myCutSize.GetWidth(), myCutSize.GetHeight()));
    
    wxImage myImg = myWebBmp.ConvertToImage();
	wxFileName myTempImageName (wxStandardPaths::Get().GetAppDocumentsDir(), _T("test_image_toolmap.png"));
	myImg.SaveFile(myTempImageName.GetFullPath());
    return new wxBitmap(myWebBmp);

#else */
    wxSize myWebSize = m_WebView->GetSize();
    if (new_size != wxDefaultSize) {
        wxSize myResultSize = myWebSize - new_size;
        if (abs( myResultSize.x) > 1 && abs(myResultSize.y) > 1) {
            wxLogError(_("Web frame has not the correct size! Expected: %d, %d. Found: %d, %d"), new_size.GetWidth(), new_size.GetHeight(), myWebSize.GetWidth(), myWebSize.GetHeight());
            return NULL;
        }
    }
    
	wxSize myBmpSize = new_size;
	if (new_size == wxDefaultSize){
		myBmpSize = myWebSize;
	}

    wxClientDC myDC (m_WebView);
    wxBitmap myTmpBmp (myBmpSize);
    wxMemoryDC myBmpDC;
    myBmpDC.SelectObject(myTmpBmp);
	int myWebSizeWidth = myWebSize.GetWidth();
#ifdef __WXMSW__
	int myScrollBarWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, m_WebView);
	myWebSizeWidth -= myScrollBarWidth; // remove scrollbar width on windows.
#endif

	myBmpDC.StretchBlit(0, 0, myBmpSize.GetWidth(), myBmpSize.GetHeight(), &myDC, 0, 0, myWebSizeWidth, myWebSize.GetHeight());
    myBmpDC.SelectObject(wxNullBitmap);
    return new wxBitmap(myTmpBmp);
//#endif
    return NULL;
}

