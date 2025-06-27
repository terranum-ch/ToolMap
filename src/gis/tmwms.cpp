//
// Created by lsc on 23.05.25.
//

#include "tmwms.h"
#include <gdal_priv.h>

tmWMSBrowser::tmWMSBrowser(const wxString& wms_url) {
    m_wms_url = wms_url;
}

/// @brief Download and store the XML file containing the WMS capabilities (Layers and other information)
bool tmWMSBrowser::DownloadCapabilities(const wxString& output_xml_file_name, const wxString &lang) {
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

    wxLogDebug("WMS Capabilities URL: %s", GetWMSCapabilitiesURL(lang));
    
    // we skip the verification of the server's certificate on Windows (otherwise not working)
    #ifdef __WINDOWS__
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    #endif
    curl_easy_setopt(curl, CURLOPT_URL, GetWMSCapabilitiesURL(lang).mb_str().data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    file.Close();

    return true;
}

/// @brief Get the layers names, titles and abstracts from the XML capabilities
bool tmWMSBrowser::GetLayers(wxArrayString& layers_names, wxArrayString& layers_titles,
                             wxArrayString& layers_abstracts, wxArrayString &layers_crs) {
    if (m_wms_xml_file.GetFullPath().IsEmpty()) {
        wxLogError("file path is empty!");
        return false;
    }

    layers_names.Clear();
    layers_titles.Clear();
    layers_abstracts.Clear();
    layers_crs.Clear();

    // Parse the XML capabilities to extract layer names, titles and abstracts
    wxXmlDocument doc;
    if (!doc.Load(m_wms_xml_file.GetFullPath())) {
        wxLogError("Unable to load document in xml file!");
        return false;
    }

    wxXmlNode* root = doc.GetRoot();
    if (!root) {
        wxLogError("unable to get root in xml document!");
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
                            // get CRS for the first layer (Projection)
                            if (layers_crs.GetCount() == 0) {
                                wxXmlNode* crsNode = layerNode->GetChildren();
                                while (crsNode) {
                                    if (crsNode->GetName() == "CRS") {
                                        layers_crs.Add(crsNode->GetNodeContent());
                                    }
                                    crsNode = crsNode->GetNext();
                                }
                            }
                            // get Name, Title and Abstract for the layer
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


tmWMSFileXML::tmWMSFileXML(const wxString &wms_url) {
    m_wms_url = wms_url;
}

bool tmWMSFileXML::CreateXML(const wxString &layer_name, const wxString &output_xml_file_name, const wxString &projection_epsg) {
    // Get The WMS URL for the layer
    wxString wms_layer_url = GetWMSLayerURL(layer_name, projection_epsg);
    wxLogDebug("WMS Layer URL: %s", wms_layer_url);

    wxFileName output_file(output_xml_file_name);
    // Use GDAL to create the XML file from the WMS layer
    GDALDataset *poSrcDS = (GDALDataset *) GDALOpen( wms_layer_url.mb_str(wxConvUTF8), GA_ReadOnly );
    GDALDriver *poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName("WMS");
    if( poDriver == NULL ) {
        wxLogError("Failed to open WMS driver");
        return false;
    }
    GDALDataset *poDstDS;
    poDstDS = poDriver->CreateCopy( output_xml_file_name.mb_str(wxConvUTF8), poSrcDS, FALSE,
                                    NULL, NULL, NULL );
    /* Once we're done, close properly the dataset */
    if( poDstDS != NULL )
        GDALClose( (GDALDatasetH) poDstDS );
    GDALClose( (GDALDatasetH) poSrcDS );
    return true;
}