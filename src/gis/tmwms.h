//
// Created by lsc on 23.05.25.
//

#ifndef TMWMS_H
#define TMWMS_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include  <curl/curl.h>

class tmWMSBrowser {
  private:
    wxString m_wms_url = wxEmptyString;
    wxString m_xml_capabilities = wxEmptyString;

    static size_t WriteToString(void* ptr, size_t size, size_t nmemb, void* userdata);

  public:
    explicit tmWMSBrowser(const wxString& wms_url);

    bool DownloadCapabilities();

    wxString GetWMSUrl() const {return m_wms_url;}
    wxString GetXMLCapabilities() const {return m_xml_capabilities;}
};

#endif  // TMWMS_H
