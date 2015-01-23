#include "StdAfx.h"
#include "Utils.h"

wchar_t* Utils::charTowchar(const char *chr,wchar_t *wchar,int sz)
{
	MultiByteToWideChar( CP_ACP, 0, chr, strlen(chr)+1, wchar, sz/sizeof(wchar[0]) );  
	return wchar;
}

char* Utils::wcharTochar(const wchar_t *wchr,char *chr,int len)
{
	WideCharToMultiByte( CP_ACP, 0, wchr, -1,  chr, len, NULL, NULL ); 
	return chr;
}

wchar_t *Utils::c2w(const char	*chr)
{
	int len=strlen(chr);
	wchar_t *temp=new wchar_t[len+1];

	charTowchar(chr,temp,sizeof(temp)*(len+1));

	temp[len]=L'\0';

	return temp;
}

char *Utils::w2c(const wchar_t *wchr)
{
	int len=wcslen(wchr);
	char *temp=new char[len+1];

	wcharTochar(wchr,temp,sizeof(temp)*(len+1));

	temp[len]='\0';

	return temp;
}
 
wchar_t *Utils::getPath()
{
	wchar_t *app_path = new wchar_t[MAX_PATH];
	GetModuleFileName(NULL,(LPWSTR)app_path,MAX_PATH);
	//char *dir = Utils::w2c(app_path);
	int index= wcslen(app_path);
	for(;index>=0;--index)	 
		if(app_path[index]==L'\\')		 
			break;
 
	if(index>=0) 
		app_path[index]='\0';

	return app_path;
}

wchar_t *Utils::makePath(wchar_t *dir)
{
	wchar_t *path = new wchar_t[MAX_PATH];
	wsprintf(path,L"%s%s",getPath(),dir);
	return path;
}

bool Utils::saveFile(char *filename,char *text)
{
	FILE *file = fopen(filename,"w");

	if(file)
	{		 
		fprintf(file,"%s",text);
		fclose(file);

		return true;
	}

	return false;
}

void Utils::fileFilter(char *text)
{
	int len=strlen(text);
	 
	int cnt=0;

	for(int i = 0;i<len;++i)
	{
		if(text[i]==' ' || text[i] == '\n' || text[i]=='\r' || text[i] == '\t') continue;
		text[cnt++]=text[i];
	}

	if(cnt<len)
		text[cnt]='\0';
 
}

 
char *Utils::str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins= orig;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }
 
    tmp = result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;  
    }
    strcpy(tmp, orig);
    return result;
}

wchar_t *Utils::makeSentence(wchar_t *text,int sz)
{
	int cnt=0;
	int n=wcslen(text);
	int len=n+n/sz;
	wchar_t *temp=new wchar_t[len];	 
	int index=0;
 
	while(cnt<n)
	{
		int i=0;
		bool isLine=true;
		for(;i<sz;++i)
		{
			if(n==cnt) break;	
			temp[index++]=text[cnt];
			if(text[cnt++]==L'\n')
			{
				isLine=false;
				break;
			}					 			
		}
		if(isLine) temp[index++]=L'\n';			 
	}
	temp[len-1]=L'\0';

	return temp;
}

bool  Utils::exec(wchar_t *cmdline)
{
	int len = wcslen(cmdline)+wcslen(L"cmd /c ")+1;
	wchar_t *cmd = new wchar_t[len];
	wsprintf(cmd,L"cmd /c %s",cmdline);
	
	while (true)
		if ((int)ShellExecute(NULL,NULL,L"cmd.exe",cmd, NULL,SW_HIDE) > 32) break;
	return true;
}

wchar_t *Utils::getRegistry(wchar_t *key)
{
	HKEY hKey;
	TCHAR *sz = new TCHAR[256];
	DWORD dwtype=REG_SZ;
	DWORD sl = 256;

	if(RegOpenKeyEx(HKEY_CURRENT_USER, REG_APP, NULL, KEY_ALL_ACCESS, &hKey)!=ERROR_SUCCESS)
	{
		RegCreateKey(HKEY_CURRENT_USER, REG_APP, &hKey);
	}

	if(RegQueryValueEx(hKey,key,NULL,&dwtype,(LPBYTE)sz,&sl)==ERROR_SUCCESS)
	{ 
		RegCloseKey(hKey); 
		if(wcscmp(sz,L"ISNULL")==0) return L"";
		return sz;
	}
 
	RegCloseKey(hKey);

	return L"";
}

bool Utils::setRegistry(wchar_t *key,wchar_t *value)
{ 
	HKEY hKey;
 
    if (RegOpenKeyEx(HKEY_CURRENT_USER,REG_APP,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS)
    {
        RegCreateKey(HKEY_CURRENT_USER,REG_APP,&hKey);
    }

	if(wcscmp(value,L"") == 0)
	{
		value = L"ISNULL";
	}

    if(RegSetValueEx(hKey, key, NULL, REG_SZ, (BYTE *)value, sizeof(value)*wcslen(value))==ERROR_SUCCESS)
	{ 
		RegCloseKey(hKey);
		return true;
	}

	return  false;
}

bool Utils::delRegistry(wchar_t *key)
{
	HKEY hKey;
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_APP, 0, KEY_ALL_ACCESS, &hKey))
	{
		RegCreateKey(HKEY_CURRENT_USER,REG_APP,&hKey);
	} 
	
	if(RegDeleteValue(hKey,key))
	{
		RegCloseKey(hKey);
		return true;
	}
	return false;
}

bool Utils::deleteTree(HKEY RootKey, const wchar_t *pSubKey)
{
    HKEY hKey;
    DWORD nRet;
    DWORD NameCnt,NameMaxLen;
    DWORD KeyCnt,KeyMaxLen,MaxDateLen;
    static char sFormat[256] = "";
    strcat(sFormat, "----");
    static int dwDeep = -1;
    dwDeep++;
    nRet=RegOpenKeyEx(RootKey,pSubKey,0,KEY_ALL_ACCESS,&hKey);
    if(nRet!=ERROR_SUCCESS)
    {        
        return false;
    }
    nRet = RegQueryInfoKey(hKey,NULL,NULL,NULL,&KeyCnt,&KeyMaxLen,NULL,&NameCnt,
                            &NameMaxLen,&MaxDateLen,NULL,NULL);
    if(nRet == ERROR_SUCCESS)
    {
        for(int dwIndex = KeyCnt - 1; dwIndex >= 0; dwIndex--)  
        {
            wchar_t sKeyName[256] = L"";
            RegEnumKey(hKey, dwIndex, sKeyName, sizeof(sKeyName));
            HKEY hKeySub;
            DWORD KeyCntSub;
            wchar_t pSubKeyTemp[256] = L"";
            wcscpy(pSubKeyTemp, pSubKey);
            wcscat(pSubKeyTemp, L"\\");
            wcscat(pSubKeyTemp, sKeyName);
            
            nRet = RegOpenKeyEx(RootKey,pSubKeyTemp,0,KEY_ALL_ACCESS,&hKeySub);
            if(nRet == ERROR_SUCCESS)
            {
                nRet = RegQueryInfoKey(hKeySub,NULL,NULL,NULL,&KeyCntSub,&KeyMaxLen,NULL,&NameCnt,
                                        &NameMaxLen,&MaxDateLen,NULL,NULL);
                if(nRet == ERROR_SUCCESS)
                {
                    if (KeyCntSub != 0)
                    {
                        deleteTree(RootKey, pSubKeyTemp);
                    }
                    RegCloseKey(hKeySub);
                }
            }
 
            RegDeleteKey(RootKey ,pSubKeyTemp);
        }
        RegCloseKey(hKey);
    }
 
    if (dwDeep == 0)
    {
        RegDeleteKey(RootKey ,pSubKey);
    }
    return true;
}

bool Utils::addStartup()
{
	HKEY hKey;
 
	wchar_t *value=makePath(L"\\Webbox.exe");
	 
    if (RegOpenKeyEx(HKEY_CURRENT_USER,REG_RUN,0,KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS)
    {
        RegCreateKey(HKEY_CURRENT_USER,REG_APP,&hKey);
    }

    if(RegSetValueEx(hKey, L"Webbox", NULL, REG_SZ, (BYTE *)value, sizeof(value)*wcslen(value))==ERROR_SUCCESS)
	{ 
		RegCloseKey(hKey);
		return true;
	}

	return  false;
}

bool Utils::delStartup()
{
	HKEY hKey;
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_APP, 0, KEY_ALL_ACCESS, &hKey))
	{
		RegCreateKey(HKEY_CURRENT_USER,REG_APP,&hKey);
	} 
	
	if(RegDeleteValue(hKey,L"Webbox"))
	{
		RegCloseKey(hKey);
		return true;
	}
	return false;
}


bool Utils::restore()
{
	return setRegistry(L"runonce",L"0");
}