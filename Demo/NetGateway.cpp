/*
 *NetGateway.cpp
 *Written by Shijie
 *Modified on 11/3/2013
 *Interface for Srun gateway login/logout
 *(c)2013 Dreamware(R) Inc. All rights reserved.
 */

#include "stdafx.h"
#include "NetGateway.h"
#include "Utils.h"
#include "Window.h" 
#include <regex>
#include <fstream>

Window *window;

char *NetGateway::r="";

NetGateway::NetGateway()
{
	CoInitialize(NULL);

	curl_global_init(CURL_GLOBAL_ALL);
 
	CURLcode ret;
	 
	ret = curl_global_init(CURL_GLOBAL_WIN32);

	if(CURLE_OK != ret)
	{
		curl_global_cleanup();
		return ;
	}

	curl=curl_easy_init();

	if(NULL == curl)
	{
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return ;
	} 
	  
	ipv4Connected = false;
	ipv6Connected = false;

	uid=NULL;
 
	/*Get single instance of window*/
	window = &Window::GetSharedInstance();

	online();   
	
	launchonce=true;
	userinfo=false;
}

NetGateway::~NetGateway(void)
{
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	CoUninitialize();
}
 
bool NetGateway::ping(char *url,char *token)
{ 
	if(doPost(url))
	{
		return(strstr(r,"baidu.com"));	 		 
	}
	return false;
}

void NetGateway::online()
{	
	ipv4Connected = ping("http://www.baidu.com","baidu.com");
	ipv6Connected = ping("http://ipv6.baidu.com","baidu.com");
	feedback();
}
 
wchar_t* NetGateway::doProcess(char* retcode,bool ipv4mode)
{	 
	wchar_t *msg; 

	/*This function could be rewritten with mappings*/
	if( strstr( retcode, "user_tab_error"))	
	{
		 msg = L"认证程序未启动";		 
	}  
	else if( strstr( retcode, "username_error"))
	{
		msg = L"用户名错误";
	}
	else if( strstr( retcode, "non_auth_error"))	
	{	
		msg =L"您无须认证，可直接上网"; 		
	}
	else if( strstr( retcode, "password_error"))	
	{	
		msg = L"密码错误"; 		
	}
	else if( strstr( retcode, "status_error"))	
	{	
		msg = L"用户已欠费，请尽快充值"; 		 
	}
	else if( strstr( retcode, "available_error"))
	{	
		msg = L"用户已禁用"; 		
	}
	else if( strstr( retcode, "ip_exist_error"))	
	{	

		msg = L"您的IP尚未下线，请等待2分钟再试"; 		
	}
	else if( strstr( retcode, "usernum_error"))	
	{	
		msg = L"用户数已达上限"; 	
	}
	else if( strstr( retcode, "online_num_error"))
	{	
		msg = L"该帐号的登录人数已超过限额,如果怀疑帐号被盗用，请联系管理员"; 		
		if(ipv4mode)
		{
			ipv4Connected=ping("http://www.baidu.com","baidu.com");		 	
		}
		else 
		{
			ipv6Connected = ping("http://ipv6.baidu.com","baidu.com");
		}
	}
    else if( strstr( retcode, "mode_error"))		
	{	
		msg = L"系统已禁止WEB方式登录，请使用客户端"; 		
	}
    else if( strstr( retcode, "time_policy_error"))	
	{
		msg = L"当前时段不允许连接"; 		
	}
    else if( strstr( retcode, "flux_error"))		
	{	
		msg = L"您的流量已超支"; 		
	}
    else if( strstr( retcode, "minutes_error"))	
	{	
		msg = L"您的时长已超支";		
	}
    else if( strstr( retcode, "ip_error"))		
	{	
		msg = L"您的IP地址不合法"; 		
	}
    else if( strstr( retcode, "mac_error"))		
	{	
		msg = L"您的MAC地址不合法";	
	}
    else if( strstr( retcode, "sync_error"))		
	{	
		msg = L"您的资料已修改，正在等待同步，请2分钟后再试";		
	}
    else if( strstr( retcode, "logout_ok") ||
		     strstr( retcode, "logout_error")||
			 strstr( retcode, "连接已断开"))		
	{	
		if(ipv4mode) ipv4Connected = false;
		else ipv6Connected = false;

		 if( strstr( retcode, "logout_error"))	
		 {
			 msg = L"您不在线上";
		 } 
		 else if( strstr( retcode, "连接已断开"))
		 {
			msg = L"注销成功";	
		 }
		 else  if( strstr( retcode, "logout_ok"))
		 {
			msg = L"注销成功，请等1分钟后登录";	
		 } 
	} 
	else /*login success*/
	{
		if(ipv4mode) ipv4Connected = true;
		else ipv6Connected = true;

		msg = L"登陆成功";
		uid = retcode;
	}
  
	return  msg;  
}
 
void NetGateway::Login(TCHAR *name,TCHAR *pswd,bool loginmode)
{  
	username= Utils::w2c(name); 
	password= Utils::w2c(pswd);
 
	int len=strlen(username)+strlen(password)+strlen("?username=&password=&drop=0&type=3&n=99"); 
	
	char *data = new char[len+1]; 
	sprintf(data,"username=%s&password=%s&drop=0&type=3&n=99",username,password);
	 
    data[len]='\0';
	
	wchar_t *r4=L"";
	wchar_t *r6=L"";

	ipv4Connected = ipv6Connected =false;

	if( (loginmode == 0 || loginmode == 1) && doPost(URL_IPV4_LOGIN,data))
	{ 
		r4 = doProcess(r,true);		 	
	}   

	if( (loginmode == 0 || loginmode == 2) && doPost(URL_IPV6_LOGIN,data))
	{ 
		r6 = doProcess(r,false);		 		 
	}
 
	feedback(r4,r6); 
}
 
void NetGateway::Logout(TCHAR *name,TCHAR *pswd,bool loginmode)
{	 
	username=Utils::w2c(name);
	password=Utils::w2c(pswd);
 
	int len=strlen(username)+strlen(password)+strlen("?username=&password=&drop=0&type=3&n=99"); 
 
	char *data = new char[len+1];
 
	sprintf(data,"username=%s&password=%s&drop=0&type=3&n=99",username,password);
	 
    data[len]='\0';
	
	wchar_t *r4=L"";
	wchar_t *r6=L"";
	 
	if((loginmode == 0 || loginmode == 1) && doPost(URL_IPV4_LOGOUT,data))
	{
		r4 = doProcess(r,true);		
	}
 
	if((loginmode == 0 || loginmode == 2) && doPost(URL_IPV6_LOGOUT,data))
	{
		r6 = doProcess(r,false);
	}
   
	feedback(r4,r6);
}
 
bool NetGateway::doPost( char* url,char *data)
{  
	r="";
	curl_easy_setopt(curl,CURLOPT_URL,url);
	if(data) curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 

	CURLcode ret = curl_easy_perform(curl); 
  
	return (ret==CURLE_OK);
}

bool NetGateway::doPostWithCookie( char* url,char *data)
{ 
	r=""; 
	curl_slist *headlist = NULL;
	char buf[] = "Expect:";
	headlist = curl_slist_append(headlist,buf);
	headlist = curl_slist_append(headlist,"application/x-www-form-urlencoded");

	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headlist);
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR,"cache/cookie"); 
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 
	CURLcode ret = curl_easy_perform(curl); 
 
	if(ret != CURLE_OK) return false;
	 
	r="";
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"cache/cookie");
	curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"cache/cookie_login"); 
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);  
	ret = curl_easy_perform(curl);
 
	return (ret==CURLE_OK);
}

size_t NetGateway::write_data(void *buffer, size_t size, size_t nmemb, void *p) 
{   
	int len= strlen((char *)buffer);
 
	for(int i=len-1;i>0;--i)
	{ 
		if((((char *)buffer)[i] == '0' || ((char *)buffer)[i]=='O'))
		{
			strncpy((char *)buffer,(char *)buffer, len-1);
			break;
		}

		if(((char *)buffer)[i]=='\n') continue;
		else break;
	}
	 
	if(strlen(r)>0 || strcmp(r,"")==0) 
	{
		int len=strlen(r)+strlen((char *)buffer);
		char *t=new char[len];
		sprintf(t,"%s%s",r,(char *)buffer);
		r=t;
	}
	else 
	{
		r=(char *)buffer;
	}

	return nmemb;
}

void NetGateway::feedback(wchar_t *ipv4String,wchar_t *ipv6String)
{   
	int len=wcslen(ipv4String)+wcslen(ipv6String)+wcslen(L"IPV4:")+wcslen(L"IPV6:")+1;

	wchar_t *result = new wchar_t[len]; 
	wsprintf(result,L"%s%s\n%s%s",L"IPV4：",ipv4String,L"IPV6：",ipv6String);
	result[len] = L'\0';
 
	if(!(wcscmp(ipv4String,L"")==0 && wcscmp(ipv6String,L"") == 0 ) && !(ipv4Connected && ipv6Connected))
	{ 
		window->Message(result);
	}

	if(ipv4Connected && ipv6Connected) 
	{
		XBtn_SetImageLeave(window->hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b6.png"),1));

		XBtn_SetText(window->hBtLogin,L"IPV4 && IPV6 已经连接");
	}
	else if(ipv4Connected) 
	{
		XBtn_SetImageLeave(window->hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b2.png"),1));
		XBtn_SetText(window->hBtLogin,L"IPV4 已经连接");
	}
	else if(ipv6Connected) 
	{
		XBtn_SetImageLeave(window->hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b1.png"),1));
		XBtn_SetText(window->hBtLogin,L"IPV6 已经连接");
	}
	else 
	{		 
		XBtn_SetText(window->hBtLogin,L"点击登陆");
	} 

	XWnd_RedrawWnd(window->hWindow,1);
 
}

 
void NetGateway::openURL(wchar_t *url)
{
	wchar_t *cmdline = new wchar_t[2048];
	wsprintf(cmdline,L"cmd /c explorer.exe %s",url);
  
	while (true)
		if ((int)ShellExecute(NULL,NULL,L"cmd.exe",cmdline,NULL,SW_HIDE) > 32) break; 
}

bool NetGateway::makeServiceJS(char *url,char *name,char *psw,char *path)
{  
	char *scriptFormat = "ie = new ActiveXObject(\"InternetExplorer.Application\");\n"		 
	"ie.visible = true;\n"
	"ie.navigate(\"%s\");\n"
	"while(ie.busy){WScript.sleep(100);}\n"
	"var document = ie.document;\n"
	"var window = document.parentWindow;\n"
	"var form = document.forms[0];\n"
	"if(form)\n"
	"{\n"
	"	form.user_login_name.value = \"%s\";\n"
	"	form.user_password.value   = \"%s\";\n" 
	"	form.target = \"_self\";\n"
	"	form.submit();\n"
	"}";

	int len = strlen(scriptFormat)+strlen(url)+strlen(name)+strlen(psw)+1;
	char *result = new char[len];
	sprintf(result,scriptFormat,url,name,psw);
	 
	return Utils::saveFile(path,result);
}

void NetGateway::Service(wchar_t *name,wchar_t *pswd)
{
	username=Utils::w2c(name);
	password=Utils::w2c(pswd);
 
	char *path4 = Utils::w2c(Utils::makePath(L"\\script\\ipv4Service.js"));
	char *path6 = Utils::w2c(Utils::makePath(L"\\script\\ipv6Service.js"));
   	
	if(makeServiceJS(URL_IPV4_SVC,username,password,path4))
	{
		openURL(Utils::c2w(path4));
	}	 
	 
	if(makeServiceJS(URL_IPV6_SVC,username,password,path6))
	{
		openURL(Utils::c2w(path6));
	} 
}
 
char* NetGateway::getProperty(char *src,char *regex )
{
	IRegExpPtr regExpPtr(__uuidof(RegExp));
	regExpPtr->PutGlobal(VARIANT_FALSE);
	regExpPtr->PutPattern(regex);	 
	IMatchCollectionPtr matches = regExpPtr->Execute(src);	 
 
	if( matches->GetCount()==0) 
		return "failed";

	IMatchPtr match = matches->GetItem(0L);		 
	_bstr_t str= match->GetValue();
	 
	char *t = str;
   
	int n=strlen(regex);
	int l=0;
	int r=n-1;

	for(;l<n&&regex[l]!='(';++l); 
	for(;r>0&&regex[r]!=')';--r);
 
	char *c1 ,*c2;

	if(r>l)
	{ 
		c1 = new char[l+1];
		c2 = new char[n-r];

		strncpy(c1,regex,l); c1[l]='\0';
		strncpy(c2,regex+r+1,n-r-1); c2[n-r-1]='\0';
   
		t=Utils::str_replace(t,c1,"");
		t=Utils::str_replace(t,c2,"");		
	}

	return t;
}

char *NetGateway::parseHTML(char *html)
{   
	char *p_username = getProperty(html,"<inputname=\"user_real_name\"size=\"16\"maxlength=\"10\"type=\"text\"value=\"(.*)\"readonly>&nbsp");
	if(strcmp(p_username,"failed")==0)
	{
		p_username = getProperty(html,"<inputname=\"user_real_name\"size=\"16\"maxlength=\"10\"type=\"text\"value=\"(.*)\">&nbsp;&nbsp;");
	}
	
	char *p_major    = getProperty(html,"<textareaname=\"user_description\"cols=\"50\"rows=\"5\">(.*)</textarea>");
	if(strcmp(p_major,"failed")==0)
	{
		p_major= getProperty(html,"<textareaname=\"user_description\"cols=\"50\"rows=\"5\"readonly>(.*)</textarea>");
	}

	char *p_ip       = getProperty(html,"绑定的IP地址</td><tdclass=\"maintd\"colspan=\"3\"height=\"26\">(.*)&nbsp;&nbsp;&nbsp;&nbsp;</td></tr><tr><tdclass=\"maintd\"align=\"right\"bgcolor=\"#f0f0f0\">绑定的MAC地址");
	char *p_bandwidth= getProperty(html,"带宽</td><tdclass=\"maintd\"colspan=\"3\"><p>(.*)</p><p>IPV6上行");
	char *p_consume  = getProperty(html,"使用情况</td><tdclass=\"maintd\"colspan=\"3\"height=\"26\"><p>(.*)</p><p>已用时长");
	char *p_used     = getProperty(html,"已用流量(.*)；</p><p>可用流量");
	char *p_left     = getProperty(html,"</p><p>可用流量：(.*)</p><p>购买的流量");

	char *p_flow     = new char[strlen(p_used)+strlen(p_left)+7]; 
	sprintf(p_flow,"%s\n可用流量：%s",p_used,p_left);
	
	char *userinfo   = new char[strlen(p_username)+strlen(p_major)+strlen(p_ip)+strlen(p_bandwidth)+strlen(p_consume)+strlen(p_flow)+strlen("中国地质大学（北京）")+20];

	sprintf(userinfo,"中国地质大学（北京）\n%s  %s\nIP地址：%s\n%s\n%s\n%s\n",p_username,p_major,p_ip,p_bandwidth,p_consume,p_flow);

	userinfo[strlen(userinfo)-1]='\0';

	userinfo = Utils::str_replace(userinfo,"；"," ");
	userinfo = Utils::str_replace(userinfo,"：",": ");
	userinfo = Utils::str_replace(userinfo,"(0为不限制)","");
	userinfo = Utils::str_replace(userinfo,"已消费","\n已消费"); 
	userinfo = Utils::str_replace(userinfo,"(IPV4)","已用流量(IPV4)");
	userinfo = Utils::str_replace(userinfo,"(IPV6)","\n已用流量(IPV6)");
	userinfo = Utils::str_replace(userinfo,"IPV4上行","上行");

	window->UpdateUserInfo(Utils::c2w(userinfo));

	return "";
}

void NetGateway::getUserInfo()
{	
	char *data = new char[2048];	 
	sprintf(data,"action=login&user_login_name=%s&user_password=%s",username,password);
   
	if(doPostWithCookie(URL_IPV4_SERVICE,data))
	{     
		if(doPost(URL_IPV4_USERINFO))
		{
			Utils::fileFilter(r);
#ifdef _DEBUG
			//Utils::saveFile("C:\\ipv4Service.html", r);	
#endif
		    parseHTML(r);
		}
	}
	 
	if(doPostWithCookie(URL_IPV6_SERVICE,data))
	{
		if(doPost(URL_IPV6_USERINFO))
		{
			if(!ipv4Connected)
			{
				Utils::fileFilter(r);
#ifdef _DEBUG
				//Utils::saveFile("C:\\ipv6Service.html", r);
#endif
				parseHTML(r);
			}
		}
	}	

	userinfo = true;
}