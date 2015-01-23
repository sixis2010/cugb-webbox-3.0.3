#include "NetUpdate.h"
#include <string>
#include <fstream>
#include "Window.h"
#include "Utils.h"

int NetUpdate::cur_version= 1;

NetUpdate::NetUpdate(void)
{ 
	window = &Window::GetSharedInstance();

	std::string path = Utils::w2c(Utils::getPath());

	path.append("\\profile\\version");

	//MessageBox(NULL,Utils::c2w(path.c_str()),L"14",0);

	std::ifstream is(path);
	std::string v;
	is>>v;
	 
	cur_version = _wtoi(Utils::c2w(v.c_str()));
	Utils::setRegistry(L"version",Utils::c2w(v.c_str()));

	update();
}


NetUpdate::~NetUpdate(void)
{
}
 
struct FtpFile 
{ 
        char *filename; 
        FILE *stream; 
}; 

size_t NetUpdate::write_data(void *buffer, size_t size, size_t nmemb, void *stream) 
{ 
    struct FtpFile *out=(struct FtpFile *)stream; 
    if (out && !out->stream) 
    { 
        out->stream=fopen(out->filename, "wb"); 
        if (!out->stream) 
        { 
            return -1; 
        } 
    }  

    return fwrite(buffer, size, nmemb, out->stream); 
}  
 
int NetUpdate::progress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow) 
{  
    return 0;
}  

bool NetUpdate::download(char *url,char *path)
{ 
	CURL *curl; 
	CURLcode res; 
	//double m_gauge1;
	struct FtpFile ftpfile= 
	{  
		path,NULL 
	}; 
	curl_global_init(CURL_GLOBAL_DEFAULT); 
  
	curl = curl_easy_init(); 
  
	if (curl) 
	{                 
		curl_easy_setopt(curl, CURLOPT_URL,url);                 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);                 
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);                
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress);                 
		//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, m_gauge1); 
		 
		res = curl_easy_perform(curl); 		 
		curl_easy_cleanup(curl); 
  
		if (CURLE_OK == res) 
		{
			
		}
	} 

	if (ftpfile.stream) 
	{               
		fclose(ftpfile.stream); 
	}
                 
	curl_global_cleanup(); 

	return true;
}


struct threadInfo {
   UINT nMilliSecond;	 
} Info;

HANDLE hThread;
DWORD ThreadID;

UINT NetUpdate::ThreadFunc(LPVOID lpParam)
{
    threadInfo* pInfo=(threadInfo*)lpParam;
     
	CURL *curl; 
	CURLcode res; 

	std::string dir =  Utils::w2c(Utils::getPath());
	dir.append("\\cache\\update.exe");
 
	struct FtpFile ftpfile= 
	{   
		(char *)dir.c_str(),NULL 
	}; 

	curl_global_init(CURL_GLOBAL_DEFAULT); 
  
	curl = curl_easy_init(); 
  
	if (curl) 
	{                 
		curl_easy_setopt(curl, CURLOPT_URL,"http://www.objcoder.com:8088/WebboxInstaller.exe");                 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);                 
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);                
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress);                 
		//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, m_gauge1); 
		 
		res = curl_easy_perform(curl); 		 
		curl_easy_cleanup(curl); 
 
		if (CURLE_OK == res) 
		{
			Utils::exec(Utils::c2w(dir.c_str()));
		}
		else 
		{
			return -1;
		}
	} 

	if (ftpfile.stream) 
	{               
		fclose(ftpfile.stream); 
	}
        
	std::string src =  Utils::w2c(Utils::getPath());
	src.append("\\cache\\version");
		
	std::string tar = Utils::w2c(Utils::getPath());
	tar.append("\\profile\\version");

	std::string cmd = "move "+src+" "+tar;
	Utils::exec(Utils::c2w(cmd.c_str()));
	curl_global_cleanup(); 

	return true;
 
}

bool NetUpdate::update()
{ 
	 
	hThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)&NetUpdate::checkVersion,0,0,0);
	 
    
	return true;
}

bool NetUpdate::checkVersion(LPVOID lpParam)
{  
	while(1)
	{
		std::string path =  Utils::w2c(Utils::getPath());
		path.append("\\cache\\version");
		if(download("http://www.objcoder.com:8088/version",(char *)path.c_str()))
		{ 
			std::ifstream is(path.c_str());
			std::string line;
		 
			is>>line;	 
			is.close();
	    
			if(line!="")
			{			 
				int vers =  _wtoi(Utils::c2w(line.c_str())); 
 
				if( vers > cur_version)
				{  
					ThreadFunc(NULL);
				}			 
			}		 
		}
	 
		Sleep(15000);
	}

	return true;
}