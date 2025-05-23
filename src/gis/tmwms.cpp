//
// Created by lsc on 23.05.25.
//

#include "tmwms.h"

tmWMSBrowser::tmWMSBrowser(const wxString& wms_url) {
    m_wms_url = wms_url;
}

/// @brief Download and store the XML file containing the WMS capabilities (Layers and other information)
bool tmWMSBrowser::DownloadCapabilities() {
    m_xml_capabilities = wxEmptyString;

    // download the XML file using curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, m_wms_url.mb_str().data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_xml_capabilities);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

/// @brief Callback function to write the downloaded data into a wxString
size_t tmWMSBrowser::WriteToString(void* ptr, size_t size, size_t nmemb, void* userdata){
    auto* wxStr = static_cast<wxString*>(userdata);
    size_t totalSize = size * nmemb;
    wxStr->Append(static_cast<char*>(ptr), totalSize);
    return totalSize;
}