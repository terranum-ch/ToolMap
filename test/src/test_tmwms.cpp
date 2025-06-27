#include "gtest/gtest.h"
#include "test_param.h"
#include "../../src/gis/tmwms.h"
#include "../../src/gis/tmgisdataraster.h"
#include "../../src/gis/tmgisdata.h"
#include <wx/filename.h>


TEST(WMSBrowserTest, Constructor) {
    tmWMSBrowser wmsBrowser("http://example.com/wms");
    EXPECT_EQ(wmsBrowser.GetWMSUrl(), "http://example.com/wms");
}

TEST(WMSBrowserTest, TestTempFileName) {
    wxFileName xml_output(wxFileName::CreateTempFileName("wms_capabilities_"));
    EXPECT_FALSE(xml_output.GetFullPath().IsEmpty());
    wxLogDebug("Xml Output: '%s'", xml_output.GetFullPath());
}

TEST(WMSBrowserTest, DownloadCapabilities) {
    tmWMSBrowser wmsBrowser("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");

    wxFileName xml_output(g_TestPathEXPORT, _T("wms_output.xml"));
    if (xml_output.Exists()) {
        wxRemoveFile(xml_output.GetFullPath());
    }
    wxLogMessage("Xml Output: '%s'", xml_output.GetFullPath());
    EXPECT_TRUE(wmsBrowser.DownloadCapabilities(xml_output.GetFullPath()));
}

TEST(WMSBrowserTest, GetLayers) {
    tmWMSBrowser wmsBrowser("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");
    wxFileName my_outputfile(g_TestPathEXPORT, "wms_output.xml");
    EXPECT_TRUE(wmsBrowser.DownloadCapabilities(my_outputfile.GetFullPath(), "fr"));

    wxArrayString layers_names, layers_titles, layers_abstracts, layers_crs;
    EXPECT_TRUE(wmsBrowser.GetLayers(layers_names, layers_titles, layers_abstracts,layers_crs));
    EXPECT_FALSE(layers_names.IsEmpty());
    EXPECT_FALSE(layers_titles.IsEmpty());
    EXPECT_FALSE(layers_abstracts.IsEmpty());
    EXPECT_FALSE(layers_crs.IsEmpty());

    // Print the layer names, titles and abstracts
    // for (size_t i = 0; i < layers_names.GetCount(); ++i) {
    //     wxLogMessage("Layer Name: %s", layers_names[i]);
    //     wxLogMessage("Layer Title: %s", layers_titles[i]);
    //     wxLogMessage("Layer Abstract: %s", layers_abstracts[i]);
    // }
}

// Initing GIS drivers is necessary for the tmWMSFileXML class to work properly
// as it relies on GDAL to create the XML file.
class TestWMSXml : public ::testing::Test {
protected:
    virtual void SetUp() {
      tmGISData::InitGISDrivers(true, true);
    }
    virtual void TearDown() {
    }
};

TEST_F(TestWMSXml, CreateXML) {
    tmWMSFileXML wmsFileXML("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");
    EXPECT_EQ(wmsFileXML.GetWMSUrl(), "https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");
    wxFileName xml_output(g_TestPathEXPORT + _T("/wms_layer_output.xml"));
    wxLogMessage("Xml Output: '%s'", xml_output.GetFullPath());
    if (xml_output.Exists()) {
        wxRemoveFile(xml_output.GetFullPath());
    }
    EXPECT_TRUE(wmsFileXML.CreateXML("ch.swisstopo.geologie-geocover", xml_output.GetFullPath(), "EPSG:3857"));
}

TEST_F(TestWMSXml, OpenLayer) {
    tmWMSFileXML wmsFileXML("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");
    EXPECT_EQ(wmsFileXML.GetWMSUrl(), "https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0");
    wxFileName xml_output(g_TestPathEXPORT + _T("/wms_layer_output.xml"));
    wxLogMessage("Xml Output: '%s'", xml_output.GetFullPath());
    if (xml_output.Exists()) {
        wxRemoveFile(xml_output.GetFullPath());
    }
    EXPECT_TRUE(wmsFileXML.CreateXML("ch.swisstopo.geologie-geocover", xml_output.GetFullPath(), "EPSG:3857"));

    tmLayerProperties* item = new tmLayerProperties();
    item->InitFromPathAndName(xml_output.GetPath(), xml_output.GetFullName(),
                              tmGISData::GetAllSupportedGISFormatsExtensions());

    // try to open the file for getting the spatial type
    tmGISData* myLayer = tmGISData::LoadLayer(item);
    if (myLayer == nullptr) {
        wxLogError(_("Not able to open the layer : %s"), item->GetNameDisplay().c_str());
        wxDELETE(item);
    }
    EXPECT_NE(myLayer, nullptr);

    // try to open the layer with gdal
    tmGISDataRaster* rasterLayer = dynamic_cast<tmGISDataRaster*>(myLayer);
    EXPECT_NE(rasterLayer, nullptr);
    int bandCount = rasterLayer->GetBandCount();
    EXPECT_EQ(bandCount, 3);

    wxString metadata_html = rasterLayer->GetMetaDataAsHtml();
    wxLogMessage("Metadata HTML: %s", metadata_html);
    EXPECT_FALSE(metadata_html.IsEmpty());

    // trying to get image data
    // owned by image, do not destroy manually.
    unsigned char* imgbuf;
    unsigned int imglen;
    unsigned char* maskbuf;
    unsigned int masklen;

    tmCoordConvert coordConvert(PROJ_SWISS_CH1903);
    rasterLayer->SetCoordConvert(&coordConvert);

    tmRealRect my_reel_coord(496732.99, 116284.07, 506000.56, 123906.90);
    EXPECT_TRUE(rasterLayer->SetSpatialFilter(my_reel_coord, item->GetType()));

    if (rasterLayer->GetImageData(&imgbuf, &imglen, &maskbuf, &masklen, wxSize(100, 100)) != CE_None) {
        fprintf(stderr, "%s line %d : Error loading image data \n ", __FUNCTION__, __LINE__);
        // need cleanup
        // in all case, clean data
        if (imgbuf) {
            CPLFree(imgbuf);
            imgbuf = nullptr;
        }

        if (maskbuf) {
            CPLFree(maskbuf);
            maskbuf = nullptr;
        }
        FAIL();
    }
    SUCCEED();
}

