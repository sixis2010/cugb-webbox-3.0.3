#pragma once

#include "stdafx.h"
#include <wininet.h>

#pragma comment(lib,"wininet.lib")

class NetWallover
{
public:
	NetWallover(void);
	~NetWallover(void);

	bool setProxy(wchar_t *ip,wchar_t *port,wchar_t *conn_name=NULL);
	bool delProxy(wchar_t *conn_name=NULL);

	bool isWallover;

private:
	bool navigate();

private:
	wchar_t *ip;
	wchar_t *port;
};