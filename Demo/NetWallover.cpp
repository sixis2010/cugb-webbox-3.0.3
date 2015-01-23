#include "StdAfx.h"
#include "NetWallover.h"
#include "Utils.h"

NetWallover::NetWallover(void)
{
	delProxy();
	isWallover=false;
}


NetWallover::~NetWallover(void)
{
	delProxy();
}

bool NetWallover::delProxy(wchar_t *conn_name)
{ 
	if(!Utils::exec(L"taskkill /im goagent.exe")||!Utils::exec(L"taskkill /im python27.exe"))
	{
		return false;
	}

    INTERNET_PER_CONN_OPTION_LIST list;
    BOOL    bReturn;
    DWORD   dwBufSize = sizeof(list);
  
    list.dwSize = sizeof(list);
    // NULL == LAN, otherwise connectoid name.
    list.pszConnection = conn_name;
 
    list.dwOptionCount = 1;
    list.pOptions = new INTERNET_PER_CONN_OPTION[list.dwOptionCount];
 
    if(NULL == list.pOptions)
    {        
        return FALSE;
    }
 
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT  ;
 
    bReturn = InternetSetOption(NULL,
        INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
 
    delete [] list.pOptions;
    InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL, 0);

	isWallover = false;
    return bReturn;
}

bool NetWallover::setProxy(wchar_t *new_ip,wchar_t *new_port,wchar_t *conn_name)
{
	wchar_t *path = Utils::makePath(L"\\proxy\\goagent.exe");
	wchar_t *cmdline = new wchar_t[wcslen(path)+wcslen(L"CreateObject(\"wscript.shell\").run \"\",0")];
	wsprintf(cmdline,L"CreateObject(\"wscript.shell\").run \"%s\",0",path);
	//MessageBox(NULL,cmdline,L"",0);
	Utils::saveFile(Utils::w2c(Utils::makePath(L"\\proxy\\run.vbs")),Utils::w2c(cmdline));

	if(!Utils::exec(L"proxy\\run.vbs"))
	{
		return false;
	}

	INTERNET_PER_CONN_OPTION_LIST list;
    BOOL    bReturn;
    DWORD   dwBufSize = sizeof(list);
 
    list.dwSize = sizeof(list);
 
    list.pszConnection = conn_name;
 
    list.dwOptionCount = 3;
    list.pOptions = new INTERNET_PER_CONN_OPTION[3];
 
    if(NULL == list.pOptions)
    {        
        return FALSE;
    }
	
    list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
    list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT |
        PROXY_TYPE_PROXY;
    
    list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	int len = wcslen(new_ip)+wcslen(new_port)+2;
	wchar_t *server = new wchar_t[len];
	wsprintf(server,L"%s:%s",new_ip,new_port);

    list.pOptions[1].Value.pszValue = server;
 
    list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
    list.pOptions[2].Value.pszValue = TEXT("local");
 
    bReturn = InternetSetOption(NULL,
        INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
 
    delete [] list.pOptions;

	isWallover = true;

	if(bReturn) navigate();

    return bReturn;
}

bool NetWallover::navigate()
{	
	Utils::exec(L"explorer https://www.youtube.com");
	Utils::exec(L"explorer https://www.facebook.com");
	return true;
}