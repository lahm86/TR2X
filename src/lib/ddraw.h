#pragma once

#include <windows.h>

#define DirectDrawCreate                                                       \
    ((HRESULT(__stdcall *)(                                                    \
        GUID * driver_guid, LPDIRECTDRAW * ddraw, LPUNKNOWN outer))0x00458CF4)

#define DirectDrawEnumerate                                                    \
    ((HRESULT(__stdcall *)(                                                    \
        LPDDENUMCALLBACKA lpCallback, LPVOID lpContext))0x00458CFA)
