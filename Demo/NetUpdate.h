#pragma once

#include "stdafx.h"
#include "Network.h"
#include <curl\curl.h>

class Window;

const int FILE_EXIST = 200;

class NetUpdate
{
public:
	NetUpdate();
	virtual ~NetUpdate();

public:
	static bool download(char *server,char *path);
	static bool update();
	static bool checkVersion(LPVOID lpParam); //true: update available
	static int  cur_version;

	static UINT NetUpdate::ThreadFunc(LPVOID lpParam);
 
	Window *window;

	static int progress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream) ;
};

