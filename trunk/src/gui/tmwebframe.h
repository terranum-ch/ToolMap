/***************************************************************************
 tmwebframe
 -------------------
 copyright            : (C) 2013 Lucien Schreiber 
 email                : lucien.schreiber at gmail dot com
 ***************************************************************************/
 
#ifndef tmwebframe_H_
#define tmwebframe_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/webview.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include "../gis/tmgisscale.h"

const int ID_WEBVIEW_CONTROL = wxWindow::NewControlId();
const wxString WEBVIEW_WINDOW_NAME = _T("WEBVIEW_FRAME");
const int WEB_MAX_WAIT_LOADING_MS = 5000;
const int WEB_WAIT_ZOOMING_MS = 200;

enum TMWEBFRAME_STATUS {
    TMWEBFRAME_STATUS_NONE = 0,
    TMWEBFRAME_STATUS_LOADED,
    TMWEBFRAME_STATUS_ERROR,
    TMWEBFRAME_STATUS_TIMEOUT
};


class tmWebFrame : public wxFrame
{
private:
    wxWebView * m_WebView;
    TMWEBFRAME_STATUS m_Status;
    wxString m_PageName;
    wxTimer m_LoadingTimer;
    wxTimer m_ZoomingTimer;
    
    void _CreateControls();
    void OnEventLoaded (wxWebViewEvent & event);
    void OnEventError (wxWebViewEvent & event);
    void OnClose (wxCloseEvent & event);
    void OnTimmerEnd (wxTimerEvent & event);
    DECLARE_EVENT_TABLE();
	
public:
    tmWebFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxSize& size = wxSize( 500,300 ), const wxString& title = _("Web Frame"), const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
    ~tmWebFrame();
    wxWebView * GetWebControl () {return m_WebView;}
    TMWEBFRAME_STATUS GetStatus () {return m_Status;}
    void LoadURL (const wxString & url);
    bool IsLoaded ();
    void LoadPage (const wxString & pagename, tmRealRect coord = tmRealRect());
    void ZoomToExtend (tmRealRect coord);
    bool HasZoomed ();
    wxBitmap * GetPageAsBitmap (const wxSize new_size= wxDefaultSize);
};




#endif
