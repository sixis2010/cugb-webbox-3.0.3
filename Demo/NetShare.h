/*
 *NetShare.h
 *Written by Shijie
 *Modified on 11/3/2013
 *Interface for Srun gateway login/logout
 *(c)2013 Dreamware(R) Inc. All rights reserved.
 */


#pragma once

#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include <objbase.h>
#include <netcon.h>
#include <stdio.h>

#include <NetCon.h>
#include <locale.h>

#include <wlanapi.h>
#include <WinNetWk.h>
#include <netlistmgr.h>

#include <ShObjIdl.h>

#pragma comment (lib, "wlanapi.lib")
#pragma comment (lib, "Mpr.lib")

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

class Window;

class NetShare
{
public:
	NetShare(void);
	NetShare(TCHAR *username,TCHAR *password);
	virtual ~NetShare(void);

public:

	HRESULT AddSharingNet ();
	HRESULT StartHostednetwork();

	HRESULT	share(); 

	bool    createWifi();
	bool    closeWifi();
	bool    deleteWifi();
	bool    openWifi();
	bool    checkWifi();
 
	bool	setUser(TCHAR *_username,TCHAR *_userpswd);

private:

	WCHAR *userName;
	WCHAR *userPswd;
	 
	INetSharingManager* pNSM;
	Window *window;
};