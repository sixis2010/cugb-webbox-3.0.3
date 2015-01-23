#pragma once

#include "OSVersion.h"

#define REG_APP  L"Software\\Dreamware\\Webbox"
#define REG_RUN  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

namespace Utils
{  
wchar_t* charTowchar(const char *chr,wchar_t *wchar,int sz);
char*    wcharTochar(const wchar_t*wchr,char *chr,int len);
wchar_t *c2w(const char		*chr);
char    *w2c(const wchar_t	*wchr); 
wchar_t *getPath();
wchar_t *makePath(wchar_t *dir);
bool     saveFile(char *filename,char *text);
void     fileFilter(char *text);
char	*str_replace(char *orig, char *rep, char *with);
wchar_t *makeSentence(wchar_t *text,int sz);

bool     exec(wchar_t *cmdline);
wchar_t *getRegistry(wchar_t *key);
bool     setRegistry(wchar_t *key,wchar_t *value);
bool     delRegistry(wchar_t *key);
bool     deleteTree(HKEY RootKey, const wchar_t *pSubKey);
bool     addStartup();
bool     delStartup();
bool     restore();

}