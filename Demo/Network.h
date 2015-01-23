#pragma once

#include "stdafx.h"
#include <curl\curl.h>

class Network
{
public:
	Network(void);
	virtual ~Network(void);

public:
	bool     doPostWithCookie(char *url,char *data=NULL);
	bool	 doPost(char* url,char *data=NULL); 
	char*    getProperty(char *src,char *regex );
	bool	 makeServiceJS(char *url,char *name,char *psw,char *path);
	bool     ping(char *url,char *token);  
	bool     goOnline(wchar_t *username,wchar_t *pswd);
protected:
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *p) ;

public:
	static char *r; 
	CURL*	curl;
};

