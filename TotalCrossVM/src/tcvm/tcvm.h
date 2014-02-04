/*********************************************************************************
 *  TotalCross Software Development Kit                                          *
 *  Copyright (C) 2000-2012 SuperWaba Ltda.                                      *
 *  All Rights Reserved                                                          *
 *                                                                               *
 *  This library and virtual machine is distributed in the hope that it will     *
 *  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         *
 *                                                                               *
 *********************************************************************************/



#ifndef TCVM_H
#define TCVM_H

//#define ENABLE_TRACE


#ifdef darwin
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define __gl2_h_
#endif



#ifdef ANDROID
#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "../tcvm/tcapi.h"

#if defined(WINCE) || defined(WIN32)
#if defined WP8
 #define INCL_WINSOCK_API_PROTOTYPES 1
#else
 #define INCL_WINSOCK_API_PROTOTYPES 0
#endif
 #define INCL_WINSOCK_API_TYPEDEFS 1
 #include "winsock2.h"
 #include <windows.h>
 #include <winnt.h>
 #include <Mmsystem.h>
 #if defined (WIN32) && !defined (WINCE) && _MSC_VER < 1500 && !defined __cplusplus
  #include <stdint.h>
 #endif
 #if _WIN32_WCE >= 300
  #include <notify.h>
 #endif
 #if !defined WP8
 typedef HWAVEOUT MediaClipHandle;
 typedef WAVEHDR  MediaClipHeader;
#endif
#endif

#include <stdarg.h>
#include <stdio.h>

#if defined(DISABLE_RAS)
#include "../init/noras_ids/noras.inc"
#endif

#ifdef darwin
#define inline
#endif

#if defined WP8

#include "cppwrapper.h"
#include "ThreadEmulation.h"
#include <winsock2.h>

#define GetSystemInfo(a) GetNativeSystemInfo(a)

#define Beep(t,d) do {} while (0)
#define GetTickCount() GetTickCount64()

#define LoadLibrary(x) LoadPackagedLibrary(x, 0)
#define CreateFile(a, b, c, d, e, f, g) CreateFile2(a, b, c, e, 0)
 typedef unsigned char boolean;
#define FindFirstFile(a, b) FindFirstFileEx(a, FindExInfoStandard, b, FindExSearchNameMatch, NULL, 0)
#define VirtualAlloc(a, b, c, d) malloc(a * b)
#define GetModuleHandle(a) 0
#define GetMessage(a, b, c, d) 0
#define PostMessage(a, b, c, d)
#define TranslateMessage(a)
#define DispatchMessage(b)
#define GetWindowLong(a, b) 0
#define SetWindowLong(a, b, c)
#define PeekMessage(a, b, c, d, e) 0
#define GetDeviceCaps(a, b) 0
#define DeleteDC(a)
#define GetSystemMetrics(a) 0
#define GetClassName(a, b, c)
#define lstrlen(a) _tcslen(a)
#define lstrcmpi(a, b) _tcsicmp(a, b)
#define SetForegroundWindow(a)
#define GetModuleFileName(a, b, c)
#define EnumWindows(a, b)
#define SetWindowPos(a, b, c, d, e, f, g) 0
#define ExtEscape(a, b, c, d, e, f) 0
#define ReleaseDC(a, b)
#define SystemParametersInfo(a, b, c, d)
#define GetClientRect(a, b)
#define SelectPalette(a, b, c)
#define RealizePalette(a)
#define SelectObject(a, b) 0
#define BitBlt(a, b, c, d, e, f, g, h, i)
#define DeleteObject(a)
#define GetLogicalDriveStrings(a, b) 0

#endif

#if !(defined(FORCE_LIBC_ALLOC) || defined(ENABLE_WIN32_POINTER_VERIFICATION))
#include "dlmalloc.h"                                                         
#endif
#include "xtypes.h"
#include "../event/specialkeys.h"
#include "mem.h"
#include "datastructures.h"
#include "jchar.h"
#include "tcclass.h"
#include "tcexception.h"
#include "tcmethod.h"
#include "tcfield.h"
#include "tcthread.h"
#include "tcz.h"
#include "opcodes.h"
#include "utils.h"
#include "debug.h"
#include "objectmemorymanager.h"
#include "../tests/tc_testsuite.h"
#include "../nm/instancefields.h"
#include "../nm/ui/GraphicsPrimitives.h"
#include "../nm/ui/PalmFont.h"
#include "context.h"
#include "nativelib.h"
#include "../event/event.h"
#include "../init/settings.h"
#include "../init/startup.h"
#include "errormsg.h"
#include "../init/globals.h"
#include "../init/demo.h"
#ifdef PALMOS
#include "palm_posix.h"
#endif

#ifdef darwin
#undef inline
#endif

/// Executes a Java method. The method must be retrieved with getMethod.
TC_API TValue executeMethod(Context currentContext, Method method, ...);
typedef TValue (*executeMethodFunc)(Context currentContext, Method method, ...);
/// check if the given range is inside the bounds of the array (start < x <= end)
TC_API bool checkArrayRange(Context currentContext, TCObject obj, int32 start, int32 count);
typedef bool (*checkArrayRangeFunc)(Context currentContext, TCObject obj, int32 start, int32 count);

#endif
