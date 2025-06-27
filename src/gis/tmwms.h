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

    bool DownloadCapabilities(const wxString & output_xml_file_name, const wxString &lang=wxEmptyString);
    bool GetLayers(wxArrayString& layers_names,
                   wxArrayString& layers_titles,
                   wxArrayString& layers_abstracts, wxArrayString &layers_crs);;

    wxString GetWMSUrl() const {return m_wms_url;}
    wxString GetWMSCapabilitiesURL(const wxString & lang = wxEmptyString) const {
        return m_wms_url + "&REQUEST=GetCapabilities" + (lang.IsEmpty() ? "" : "&lang=" + lang);
    }
};

/// Class for creating a XML file from a WMS server using GDALDriver::CreateCopy capability
/// see here for tutorial https://gdal.org/en/stable/tutorials/raster_api_tut.html
/// GDAL needs to be initialized before using this class, see tmGISData::InitGISDrivers or GDALAllRegister
class tmWMSFileXML {
  private:
    wxString m_wms_url = wxEmptyString;

  public:
    explicit tmWMSFileXML(const wxString& wms_url);

    bool CreateXML(const wxString & layer_name, const wxString& output_xml_file_name, const wxString &projection_epsg);
    wxString GetWMSUrl() const {return m_wms_url;}

    wxString GetWMSLayerURL(const wxString & layer_name, const wxString & projection_epsg) const {
        return m_wms_url + "&REQUEST=GetMap&LAYERS=" + layer_name + "&CRS=" + projection_epsg;
    }
};

#endif  // TMWMS_H
