/*
 * PROJECT:         Aspia (Io Helper DLL)
 * FILE:            io_dll/io_dll.c
 * LICENSE:         LGPL (GNU Lesser General Public License)
 * PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
 */

#include <windows.h>
#include <stddef.h>
#include <strsafe.h>
#include "aspia_dll.h"
#include "aspia.h"


HINSTANCE hInst;
ASPIA_DLL_PARAMS DllParams = {0};

BOOL IsCanceled = FALSE;

PIO_ADD_FOOTER        IoAddFooter       = NULL;
PIO_ADD_VALUE_NAME    IoAddValueName    = NULL;
PIO_SET_ITEM_TEXT     IoSetItemText     = NULL;
PIO_ADD_ICON          IoAddIcon         = NULL;
PIO_ADD_HEADER        IoAddHeader       = NULL;
PIO_ADD_ITEM          IoAddItem         = NULL;
PIO_GET_TARGET        IoGetTarget       = NULL;
PIO_ADD_HEADER_STRING IoAddHeaderString = NULL;


BOOL
AspiaDllInitialize(ASPIA_DLL_PARAMS *Params)
{
    if (!Params) return FALSE;

    DllParams.DebugMode = Params->DebugMode;

    DllParams.ELogShowError   = Params->ELogShowError;
    DllParams.ELogShowInfo    = Params->ELogShowInfo;
    DllParams.ELogShowWarning = Params->ELogShowWarning;

    DllParams.IEShowFile = Params->IEShowFile;
    DllParams.IEShowFtp  = Params->IEShowFtp;
    DllParams.IEShowHttp = Params->IEShowHttp;

    DllParams.hIconsInst   = Params->hIconsInst;
    DllParams.hLangInst    = Params->hLangInst;
    DllParams.hListImgList = Params->hListImgList;
    DllParams.hMainWnd     = Params->hMainWnd;
    DllParams.hListView    = Params->hListView;

    DllParams.SxSmIcon      = Params->SxSmIcon;
    DllParams.SySmIcon      = Params->SySmIcon;
    DllParams.SysColorDepth = Params->SysColorDepth;

    StringCbCopy(DllParams.szCurrentPath,
                 sizeof(DllParams.szCurrentPath),
                 Params->szCurrentPath);

    IoAddFooter       = Params->IoAddFooter;
    IoAddValueName    = Params->IoAddValueName;
    IoSetItemText     = Params->IoSetItemText;
    IoAddIcon         = Params->IoAddIcon;
    IoAddHeader       = Params->IoAddHeader;
    IoAddItem         = Params->IoAddItem;
    IoGetTarget       = Params->IoGetTarget;
    IoAddHeaderString = Params->IoAddHeaderString;

    return TRUE;
}

VOID
CancelInfoGetting(VOID)
{
    IsCanceled = TRUE;
}

BOOL APIENTRY
DllMain(HMODULE hinstDLL,
        DWORD  dwReason,
        LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
            hInst = hinstDLL;
            break;
    }

    return TRUE;
}
