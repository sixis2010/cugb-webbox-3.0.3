#include "stdafx.h"
#include <shlwapi.h>
#include <tchar.h>
#define VER_SUITE_EMBEDDEDNT 0x00000040
#define SM_TABLETPC             86
#define SM_MEDIACENTER          87
#define SM_STARTER              88
#define SM_SERVERR2             89
     
#pragma once

namespace Utils
{
	TCHAR* GetWindowsVersion();
	bool IsWindows64();
	bool IsXP();
}