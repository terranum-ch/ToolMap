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
#ifdef __WXMSW__
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
    if (WinHttpGetIEProxyConfigForCurrentUser(&proxyConfig)) {
        if (proxyConfig.lpszProxy) {
            wxString proxy(proxyConfig.lpszProxy);
            GlobalFree(proxyConfig.lpszProxy);
            if (proxyConfig.lpszProxyBypass) GlobalFree(proxyConfig.lpszProxyBypass);
            if (proxyConfig.lpszAutoConfigUrl) GlobalFree(proxyConfig.lpszAutoConfigUrl);
            return proxy;
        }
        if (proxyConfig.lpszProxyBypass) GlobalFree(proxyConfig.lpszProxyBypass);
        if (proxyConfig.lpszAutoConfigUrl) GlobalFree(proxyConfig.lpszAutoConfigUrl);
    }
    return wxEmptyString;
#else
    // For other platforms, we can return an empty string or handle it differently
    return wxEmptyString;
#endif
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