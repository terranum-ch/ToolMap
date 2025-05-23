#include "gtest/gtest.h"
#include "test_param.h"
#include "../../src/gis/tmwms.h"
#include <wx/filename.h>


TEST(WMSBrowserTest, Constructor) {
    tmWMSBrowser wmsBrowser("http://example.com/wms");
    EXPECT_EQ(wmsBrowser.GetWMSUrl(), "http://example.com/wms");
}

TEST(WMSBrowserTest, DownloadCapabilities) {
    tmWMSBrowser wmsBrowser("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities&lang=fr");

    wxFileName xml_output(g_TestPathEXPORT + _T("/wms_output.xml"));
    if (xml_output.Exists()) {
        wxRemoveFile(xml_output.GetFullPath());
    }
    wxLogMessage("Xml Output: '%s'", xml_output.GetFullPath());
    EXPECT_TRUE(wmsBrowser.DownloadCapabilities(xml_output.GetFullPath()));
}

TEST(WMSBrowserTest, GetLayers) {
    tmWMSBrowser wmsBrowser("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities&lang=fr");
    EXPECT_TRUE(wmsBrowser.DownloadCapabilities(g_TestPathEXPORT + _T("/wms_output.xml")));

    wxArrayString layers_names, layers_titles, layers_abstracts;
    EXPECT_TRUE(wmsBrowser.GetLayers(layers_names, layers_titles, layers_abstracts));
    EXPECT_FALSE(layers_names.IsEmpty());
    EXPECT_FALSE(layers_titles.IsEmpty());
    EXPECT_FALSE(layers_abstracts.IsEmpty());

    // Print the layer names, titles and abstracts
    for (size_t i = 0; i < layers_names.GetCount(); ++i) {
        wxLogMessage("Layer Name: %s", layers_names[i]);
        wxLogMessage("Layer Title: %s", layers_titles[i]);
        wxLogMessage("Layer Abstract: %s", layers_abstracts[i]);
    }
}