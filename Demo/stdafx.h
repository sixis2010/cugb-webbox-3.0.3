#pragma once

#define SAFE_RELEASE(l) if(l){(l)->release();(l)=NULL;}

#define __GIRL__

#include "targetver.h"
 
#include <Windows.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#import "C:/RegExp.tlb" no_namespace named_guids

#ifdef _DEBUG
#pragma comment(lib, "../lib/XCGUId.lib")
#else
#pragma comment(lib, "../lib/XCGUI.lib")
#endif

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib" )
#pragma comment(lib, "wldap32.lib")

#include "../lib/xcgui.h"

#define WM_USER_NOTIFYICON WM_USER+100