#include "gtest/gtest.h"
#include "test_param.h"
#include "../../src/gis/tmwms.h"


TEST(WMSBrowserTest, Constructor) {
    tmWMSBrowser wmsBrowser("http://example.com/wms");
    EXPECT_EQ(wmsBrowser.GetWMSUrl(), "http://example.com/wms");
}

TEST(WMSBrowserTest, DownloadCapabilities) {
    tmWMSBrowser wmsBrowser("https://wms.geo.admin.ch/?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities&lang=fr");
    EXPECT_TRUE(wmsBrowser.DownloadCapabilities());
    EXPECT_FALSE(wmsBrowser.GetXMLCapabilities().IsEmpty());
    //wxLogMessage(wmsBrowser.GetXMLCapabilities());
}