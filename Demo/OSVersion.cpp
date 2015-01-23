#include "OSVersion.h"
//get windows version string
TCHAR* Utils::GetWindowsVersion()
{
	TCHAR Version[100];
	TCHAR* ret = L"";

	OSVERSIONINFOEX OS;
	OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL VerEx = GetVersionEx((OSVERSIONINFO*)&OS);
	if (!VerEx)
	{
	   OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	   if (!GetVersionEx((OSVERSIONINFO*)&OS))
		return L"";
	}

	
	switch (OS.dwPlatformId)
	{

	case VER_PLATFORM_WIN32s: 
	   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
	   break;
  
	case VER_PLATFORM_WIN32_WINDOWS:
	   switch (OS.dwMinorVersion)
	   {
	   case 0:
		if ((OS.szCSDVersion[1] == 'B') || (OS.szCSDVersion[1] == 'C'))
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 95 OSR2"));
		else
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 95"));
		break;
    
	   case 10:
		if (OS.szCSDVersion[1] == 'A')
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 98 Second Edition"));
		else
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 98"));
		break;
     
	   case 90:
		_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows Millenium Edition"));
		break; 
	   }
	   break; 
                         
	case VER_PLATFORM_WIN32_NT:
	   if (OS.dwMajorVersion <= 4)  
	   {
		if (VerEx)
		{
		 if (OS.wProductType == VER_NT_SERVER)
		 {
		  if (OS.wSuiteMask & VER_SUITE_ENTERPRISE)
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows NT %d.%d Server Enterprise Edition"), OS.dwMajorVersion, OS.dwMinorVersion);
		  else
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows NT %d.%d Server"), OS.dwMajorVersion, OS.dwMinorVersion);
		 }
		 else if (OS.wProductType == VER_NT_WORKSTATION)
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows NT Workstation %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
		 else
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows NT %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
		}
		else
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows NT %d.%d %s"), OS.dwMajorVersion, OS.dwMinorVersion, OS.szCSDVersion);
	   }
	   else if (OS.dwMajorVersion == 5)
	   {
		switch (OS.dwMinorVersion)
		{
		case 0:
		 if (VerEx)
		 {
		  if (OS.wProductType == VER_NT_WORKSTATION)
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 Professional"));
		  else if (OS.wProductType == VER_NT_SERVER)
		  {
		   if (OS.wSuiteMask & VER_SUITE_ENTERPRISE)
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 Advanced Server"));
		   else if (OS.wSuiteMask & VER_SUITE_DATACENTER)
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 Datacenter Server"));
		   else
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 Server"));
		  }
		  else
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 Home Edition"));
		 }
		 else
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2000 %s"), OS.szCSDVersion);
		 break;
     
		case 1:
		 if (VerEx)
		 {
		  if (OS.wProductType == VER_NT_WORKSTATION)
		  {
		   if (OS.wSuiteMask & VER_SUITE_PERSONAL)
		   {
			if (::GetSystemMetrics(SM_MEDIACENTER) != 0)
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Media Center Edition"));
			else if (::GetSystemMetrics(SM_STARTER) != 0)
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Starter Edition"));
			else if (::GetSystemMetrics(SM_TABLETPC) != 0)
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Tablet PC Edition"));
			else
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Home Edition %s"), OS.szCSDVersion);
		   }
		   else
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Professional %s"), OS.szCSDVersion);
		  }
		  else
		  {
		   if (OS.wSuiteMask & VER_SUITE_EMBEDDEDNT)
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Embedded Edition %s"), OS.szCSDVersion);
		   else
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP %s"), OS.szCSDVersion);
		  }
		 }
		 else
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP %s"), OS.szCSDVersion);
		 break;
     
		case 2:
		 if (VerEx)
		 {
		  if (OS.wProductType == VER_NT_SERVER)
		  {
		   if (::GetSystemMetrics(SM_SERVERR2) != 0)
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows Server 2003 R2"));
		   else
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows Server 2003"));
      
		   if (OS.wSuiteMask & VER_SUITE_BLADE)
		   {
			if (IsWindows64())
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s x64 Web Edition"), Version);
			else
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s Web Edition"), Version);
		   }
		   else if (OS.wSuiteMask & VER_SUITE_ENTERPRISE)
		   {
			if (IsWindows64())
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s x64 Enterprise Edition"), Version);
			else
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s Enterprise Edition"), Version);
		   }
		   else if (OS.wSuiteMask & VER_SUITE_DATACENTER)
		   {
			if (IsWindows64())
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s x64 Datacenter Edition"), Version);
			else
			 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("%s Datacenter Edition"), Version);
		   }
		  }
		  else
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows XP Professional x64 Edition"));
		 }
		 else
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 2003 family"));
		 break;
      
		default:
		 _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
		 break;
		}
	   }
	   else if (OS.dwMajorVersion == 6)
	   {
		if (VerEx)
		{
		 if (OS.wProductType == VER_NT_WORKSTATION)
		 {
		  if (IsWindows64())
		  {		 
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 7 64-bit"));
		  }
		  else
			_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows 7 32-bit"));
		 }
		 else if (OS.wProductType == VER_NT_SERVER)
		  _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows Server 2008"));
		  }
		  else
		   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows Longhorn %s"), OS.szCSDVersion);
	   }
	   else
		_sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
	   break;
   
	default:
	   _sntprintf(Version, sizeof(Version)/sizeof(Version[0]) - 1, _T("Windows %d.%d"), OS.dwMajorVersion, OS.dwMinorVersion);
	   break;
	}
 
	wchar_t *r = new wchar_t[wcslen(Version)];
	wcscpy(r,Version);

	return  r;
	 
}
 
bool Utils::IsWindows64()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)::GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
	BOOL bIsWow64 = FALSE;
	if (fnIsWow64Process)
	   if (!fnIsWow64Process(::GetCurrentProcess(), &bIsWow64))
		bIsWow64 = FALSE;
	return bIsWow64;
}

bool Utils::IsXP()
{
	wchar_t *ver = GetWindowsVersion();
	if(wcsstr(ver,L"Windows XP") != NULL) return true;
	return false;
}