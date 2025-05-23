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
#include <wx/xml/xml.h>
#include <wx/file.h>
#include <wx/filename.h>

class tmWMSBrowser {
  private:
    wxString m_wms_url = wxEmptyString;
    wxFileName m_wms_xml_file;

    static size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata);

  public:
    explicit tmWMSBrowser(const wxString& wms_url);

    bool DownloadCapabilities(const wxString & output_xml_file_name);
    bool GetLayers(wxArrayString& layers_names,
                   wxArrayString& layers_titles,
                   wxArrayString& layers_abstracts);;

    wxString GetWMSUrl() const {return m_wms_url;}
};

#endif  // TMWMS_H
