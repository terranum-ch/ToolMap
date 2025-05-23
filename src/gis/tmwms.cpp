//
// Created by lsc on 23.05.25.
//

#include "tmwms.h"

tmWMSBrowser::tmWMSBrowser(const wxString& wms_url) {
    m_wms_url = wms_url;
}

/// @brief Download and store the XML file containing the WMS capabilities (Layers and other information)
bool tmWMSBrowser::DownloadCapabilities(const wxString& output_xml_file_name) {
    m_wms_xml_file = wxFileName(output_xml_file_name);

    CURL* curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    wxFile file(m_wms_xml_file.GetFullPath(), wxFile::write);
    if (!file.IsOpened()) {
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, m_wms_url.mb_str().data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    file.Close();

    return res == CURLE_OK;
}

/// @brief Get the layers names, titles and abstracts from the XML capabilities
bool tmWMSBrowser::GetLayers(wxArrayString& layers_names, wxArrayString& layers_titles,
                             wxArrayString& layers_abstracts) {
    if (m_wms_xml_file.GetFullPath().IsEmpty()) {
        return false;
    }

    layers_names.Clear();
    layers_titles.Clear();
    layers_abstracts.Clear();

    // Parse the XML capabilities to extract layer names, titles and abstracts
    wxXmlDocument doc;
    if (!doc.Load(m_wms_xml_file.GetFullPath())) {
        return false;
    }

    wxXmlNode* root = doc.GetRoot();
    if (!root) {
        return false;
    }

    // structure is <Capabilities><Layer><Layer>
    wxXmlNode* capabilityNode = root->GetChildren();
    while (capabilityNode) {
        if (capabilityNode->GetName() == "Capability") {
            wxXmlNode* layerMasterNode = capabilityNode->GetChildren();
            while (layerMasterNode) {
                if (layerMasterNode->GetName() == "Layer") {
                    wxXmlNode* layerNode = layerMasterNode->GetChildren();
                    while (layerNode) {
                        if (layerNode->GetName() == "Layer") {
                            wxString name, title, abstract;

                            wxXmlNode* childNode = layerNode->GetChildren();
                            while (childNode) {
                                if (childNode->GetName() == "Name") {
                                    name = childNode->GetNodeContent();
                                } else if (childNode->GetName() == "Title") {
                                    title = childNode->GetNodeContent();
                                } else if (childNode->GetName() == "Abstract") {
                                    abstract = childNode->GetNodeContent();
                                }
                                childNode = childNode->GetNext();
                            }

                            //wxLogMessage("Nom : %s, Titre : %s, Résumé : %s", name, title, abstract);
                            layers_names.Add(name);
                            layers_titles.Add(title);
                            layers_abstracts.Add(abstract);
                        }
                        layerNode = layerNode->GetNext();
                    }
                }
                layerMasterNode = layerMasterNode->GetNext();
            }
        }
        capabilityNode = capabilityNode->GetNext();
    }
    return true;
}

/// @brief Callback function to write the downloaded data into a wxFile
size_t tmWMSBrowser::WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata) {
    auto* file = static_cast<wxFile*>(userdata);
    size_t totalSize = size * nmemb;
    file->Write(ptr, totalSize);
    return totalSize;
}