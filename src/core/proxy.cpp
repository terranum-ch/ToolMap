//
// Created by Pascal on 15/07/2025.
//

#include "proxy.h"

#ifdef __WXMSW__
#include <windows.h>
#include <winhttp.h>
#include <wx/string.h>
#endif

#include <wx/fileconf.h>

wxString GetSystemProxy() {
    wxString proxyInfo = wxEmptyString;

#ifdef __WXMSW__
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieConfig = {};
    if (WinHttpGetIEProxyConfigForCurrentUser(&ieConfig)) {
        WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions = {};
        autoProxyOptions.fAutoLogonIfChallenged = TRUE;

        if (ieConfig.lpszAutoConfigUrl) {
            autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
            autoProxyOptions.lpszAutoConfigUrl = ieConfig.lpszAutoConfigUrl;
        } else if (ieConfig.fAutoDetect) {
            autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
            autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
        }

        WINHTTP_PROXY_INFO winProxyInfo = {};
        HINTERNET hSession = WinHttpOpen(L"ProxyResolver", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                         WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

        if (hSession &&
            WinHttpGetProxyForUrl(hSession, L"https://wms.geo.admin.ch", &autoProxyOptions, &winProxyInfo)) {
            if (winProxyInfo.lpszProxy)
                proxyInfo = wxString(winProxyInfo.lpszProxy);

            // Clean up WINHTTP_PROXY_INFO
            if (winProxyInfo.lpszProxy)
                GlobalFree(winProxyInfo.lpszProxy);
            if (winProxyInfo.lpszProxyBypass)
                GlobalFree(winProxyInfo.lpszProxyBypass);
        } else {
            wxLogInfo("WinHttpGetProxyForUrl failed. No proxy used or PAC returned DIRECT.");
        }

        if (hSession)
            WinHttpCloseHandle(hSession);

        // Clean up WINHTTP_CURRENT_USER_IE_PROXY_CONFIG
        if (ieConfig.lpszAutoConfigUrl)
            GlobalFree(ieConfig.lpszAutoConfigUrl);
        if (ieConfig.lpszProxy)
            GlobalFree(ieConfig.lpszProxy);
        if (ieConfig.lpszProxyBypass)
            GlobalFree(ieConfig.lpszProxyBypass);
    }
#else
    // For other platforms, we can return an empty string or handle it differently
#endif

    return proxyInfo;
}

wxString GetProxy() {
    wxConfigBase *config = wxFileConfig::Get();

    wxString myProxyInfo = wxEmptyString;
    wxString proxyToUse = wxEmptyString;
    bool useSystemProxy = true;

    if (config) {
        myProxyInfo = config->Read("UPDATE/proxy_info", wxEmptyString);
        useSystemProxy = config->ReadBool("UPDATE/use_system_proxy", true);
    }

    if (useSystemProxy) {
        proxyToUse = GetSystemProxy();

        wxArrayString parts = wxSplit(proxyToUse, ';');
        for (const auto& part : parts) {
            if (part.StartsWith("https=")) {
                proxyToUse = part.AfterFirst('=');
            } else if (part.StartsWith("http=")) {
                proxyToUse = part.AfterFirst('=');
            }
        }

    } else {
        proxyToUse = myProxyInfo;
    }

    return proxyToUse;
}