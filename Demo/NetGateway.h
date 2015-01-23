/*
 *NetGateway.h
 *Written by Shijie
 *Modified on 11/3/2013
 *Interface for Srun gateway login/logout
 *(c)2013 Dreamware(R) Inc. All rights reserved.
 */

#pragma once

#include <curl\curl.h>

#define URL_IPV4_LOGIN		"http://202.204.105.195:3333/cgi-bin/do_login"
#define URL_IPV4_LOGOUT		"http://202.204.105.195:3333/cgi-bin/force_logout"
#define URL_IPV6_LOGIN		"http://[2001:da8:214:102:d6be:d9ff:feaa:422a]:3333/cgi-bin/do_login"
#define URL_IPV6_LOGOUT		"http://[2001:da8:214:102:d6be:d9ff:feaa:422a]:3333/cgi-bin/force_logout"
#define URL_IPV4_SERVICE    "http://202.204.105.195:8800/do_services.php"
#define URL_IPV6_SERVICE    "http://[2001:da8:214:102:d6be:d9ff:feaa:422a]:8800/do_services.php"
#define URL_IPV4_SVC        "http://202.204.105.195:8800/services.php"
#define URL_IPV6_SVC        "http://[2001:da8:214:102:d6be:d9ff:feaa:422a]:8800/services.php"
#define URL_IPV4_USERINFO   "http://202.204.105.195:8800/services.php?action=info"
#define URL_IPV6_USERINFO   "http://[2001:da8:214:102:d6be:d9ff:feaa:422a]:8800/services.php?action=info"



class NetGateway
{
public:
	NetGateway();
	virtual ~NetGateway(void);

public:
	void Login(TCHAR *name,TCHAR *pswd,bool loginmode);	/*loginmode = 0,1,2(both,v4,v6)*/
	void Logout(TCHAR *name,TCHAR *pswd,bool loginmode);
	void Service(TCHAR *name,TCHAR *pswd); /*当ipv4,6同时可用时，打开两个自服务页面*/
	void getUserInfo();//由Service调用，登陆后获取用户信息

public:
	wchar_t* doProcess(char* retcode,bool ipv4mode); //处理网关返回结果
	bool     doPostWithCookie(char *url,char *data=NULL);
	bool	 doPost(char* url,char *data=NULL); 
	void	 feedback(wchar_t *v4=L"",wchar_t *v6=L"");//反馈更新用户界面
	void     openURL(wchar_t *url);
	char*    getProperty(char *src,char *regex );
	bool	 makeServiceJS(char *url,char *name,char *psw,char *path);
	char*    parseHTML(char *html);
	bool     ping(char *url,char *token);  
	void     online(); //启动时测试是否已经连网(ipv4 or ipv6?)

public:
	bool    launchonce;
	bool    userinfo;

public:
	char*	username;
	char*	password;

	CURL*	curl;
    char*	uid;

	bool	ipv4Connected;
	bool	ipv6Connected;

	bool	ipv4Enabled;
	bool	ipv6Enabled;

	static char *r;  	
 
protected: 

	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *p) ;
};