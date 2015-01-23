#include "Network.h"
#include "Utils.h"


char *Network::r = "";

Network::Network(void)
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
}

Network::~Network(void)
{
}

bool Network::ping(char *url,char *token)
{ 
	if(doPost(url))
	{
		return(strstr(r,"baidu.com"));	 		 
	}
	return false;
}

bool Network::doPost( char* url,char *data)
{  
	r="";
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
	if(data) curl_easy_setopt(curl,CURLOPT_POSTFIELDS,data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 

	CURLcode ret = curl_easy_perform(curl); 
  
	return (ret==CURLE_OK);
}

bool Network::doPostWithCookie( char* url,char *data)
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

size_t Network::write_data(void *buffer, size_t size, size_t nmemb, void *p) 
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

bool Network::makeServiceJS(char *url,char *name,char *psw,char *path)
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

char* Network::getProperty(char *src,char *regex )
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

bool Network::goOnline(wchar_t *username,wchar_t *pswd)
{
	char *data = new char[wcslen(username)+wcslen(pswd)+strlen("userName=&userPassword=")+1];
	sprintf(data,"userName=%s&userPassword=%s",Utils::w2c(username),Utils::w2c(pswd));
	data[strlen(data)-1]='\0';
	MessageBox(NULL,Utils::c2w(data),L"",0);
	return doPost("http://www.objcoder.com:8088/footprint/api/Register",data);
}