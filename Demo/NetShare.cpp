/*
 *NetShare.cpp
 *Written by Shijie
 *Modified on 11/3/2013
 *Interface for Srun gateway login/logout
 *(c)2013 Dreamware(R) Inc. All rights reserved.
 */

#include "stdafx.h"
#include "NetShare.h"
#include "Window.h"
#include "Utils.h"

#define NAT_PROTOCOL_TCP 6
#define NAT_PROTOCOL_UDP 17
  
HRESULT NetShare::AddSharingNet()
{
    INetConnection * pNC = NULL; 

    INetSharingEveryConnectionCollection * pNSECC = NULL;
    HRESULT hr = pNSM->get_EnumEveryConnection (&pNSECC);
    if (!pNSECC)
	{
        wprintf (L"failed to get EveryConnectionCollection!\r\n");
	}
    else
    {       
        IEnumVARIANT * pEV = NULL;
        IUnknown * pUnk = NULL;
        hr = pNSECC->get__NewEnum (&pUnk);
        if (pUnk)
        {
            hr = pUnk->QueryInterface (__uuidof(IEnumVARIANT),
                                       (void**)&pEV);
            pUnk->Release();
        }
        if (pEV)
        {
            VARIANT v;
            VariantInit (&v);
            BOOL bFoundIt = FALSE;
            INetSharingConfiguration * pNSC = NULL;
            INetSharingConfiguration * pVWifiNSC = NULL;
            NETCON_PROPERTIES* pVWifi = NULL;

            while (S_OK == pEV->Next (1, &v, NULL))
            {
                if (V_VT (&v) == VT_UNKNOWN)
                {
                    V_UNKNOWN (&v)->QueryInterface (__uuidof(INetConnection),
                                                    (void**)&pNC);
                    if (pNC)
                    {
                        NETCON_PROPERTIES* pNP = NULL;
                        pNC->GetProperties(&pNP);
                        setlocale(LC_ALL, "chs");

                        wprintf(L"pszwName--%s\n",pNP->pszwName);
                        wprintf(L"pszwDeviceName--%s\n",pNP->pszwDeviceName);
                        wprintf(L"Status--%d\n",pNP->Status);
                        wprintf(L"MediaType--%d\n",pNP->MediaType);
                        wprintf(L"dwCharacter--%d\n",pNP->dwCharacter);
                        wprintf(L"clsidThisObject--%d\n",pNP->clsidThisObject);
                        wprintf(L"clsidUiObject--%d\n",pNP->clsidUiObject);
                        wprintf(L"\n");

                        if(pNP->Status == NCS_CONNECTED)
                        {
                            hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
                            hr = pNSC->EnableSharing(ICSSHARINGTYPE_PUBLIC);
                            pNSC->Release();

                        }

						wprintf(L"network name %s\n",pNP->pszwDeviceName);
						
						if(_tcsstr(pNP->pszwDeviceName,L"Microsoft Virtual WiFi Miniport Adapter") != NULL || _tcsstr(pNP->pszwDeviceName,L"ÍÐ¹ÜÍøÂçÐéÄâ")!= NULL  )
						{
							pVWifi = pNP;
                            hr = pNSM->get_INetSharingConfigurationForINetConnection(pNC,&pVWifiNSC);
                            pVWifiNSC->EnableSharing(ICSSHARINGTYPE_PRIVATE);
                            wprintf(L"\nIs this VWifi?--%s\n\n",pVWifi->pszwDeviceName);
							//MessageBox(NULL,pVWifi->pszwDeviceName,L"MSG",MB_OK);
						}						 
                    }
                }
            }

            INetSharingPortMapping * pNSPM = NULL;
            
			if(pVWifi) 
			{
				BSTR pszwName = pVWifi->pszwName;
				wprintf(L"BSTR--------------%s\n",pszwName);
			}
			else 
			{
				wprintf(L"pVMWifi is null\n");
			}

			if(pVWifiNSC) 
			{
				pVWifiNSC->Release();
			}

            if (pNSPM)
            {
                wprintf (L"just added ........................................................!\r\n");

                hr = pNSPM->Enable();
                wprintf (L"just enabled port mapping!\r\n");

                INetSharingPortMappingProps* pMapping = NULL;
                pNSPM->get_Properties(&pMapping);
                BSTR result = NULL;
                long port = 0;
                pMapping->get_Name(&result);
                pMapping->get_ExternalPort(&port);
                wprintf(L"NAMEEEEE::::%s\n",result);
                wprintf(L"exPortttttt::::%d\n",port);

                pNSPM->Release();
            }

        }
    }
    return hr;
}

bool NetShare::checkWifi()
{
	bool isWifi = false;

	INetConnection * pNC = NULL; 

    INetSharingEveryConnectionCollection * pNSECC = NULL;
    HRESULT hr = pNSM->get_EnumEveryConnection (&pNSECC);
    if (!pNSECC)
        wprintf (L"failed to get EveryConnectionCollection!\r\n");
    else
    {       
        IEnumVARIANT * pEV = NULL;
        IUnknown * pUnk = NULL;
        hr = pNSECC->get__NewEnum (&pUnk);
        if (pUnk)
        {
            hr = pUnk->QueryInterface (__uuidof(IEnumVARIANT),
                                       (void**)&pEV);
            pUnk->Release();
        }
        if (pEV)
        {
            VARIANT v;
            VariantInit (&v);
            BOOL bFoundIt = FALSE;
            INetSharingConfiguration * pNSC = NULL;
            INetSharingConfiguration * pVWifiNSC = NULL;
            NETCON_PROPERTIES* pVWifi = NULL;

            while (S_OK == pEV->Next (1, &v, NULL))
            {
                if (V_VT (&v) == VT_UNKNOWN)
                {
                    V_UNKNOWN (&v)->QueryInterface (__uuidof(INetConnection),
                                                    (void**)&pNC);
                    if (pNC)
                    {
                        NETCON_PROPERTIES* pNP = NULL;
                        pNC->GetProperties(&pNP);
                        setlocale(LC_ALL, "chs");

                        wprintf(L"pszwName--%s\n",pNP->pszwName);
                        wprintf(L"pszwDeviceName--%s\n",pNP->pszwDeviceName);
                        wprintf(L"Status--%d\n",pNP->Status);
                        wprintf(L"MediaType--%d\n",pNP->MediaType);
                        wprintf(L"dwCharacter--%d\n",pNP->dwCharacter);
                        wprintf(L"clsidThisObject--%d\n",pNP->clsidThisObject);
                        wprintf(L"clsidUiObject--%d\n",pNP->clsidUiObject);
                        wprintf(L"\n");

                        if(pNP->Status == NCS_CONNECTED)
                        {
                            hr = pNSM->get_INetSharingConfigurationForINetConnection (pNC, &pNSC);
                            hr = pNSC->EnableSharing(ICSSHARINGTYPE_PUBLIC);
                            pNSC->Release();

                        }

						wprintf(L"network name %s\n",pNP->pszwDeviceName);
						
						if(_tcsstr(pNP->pszwDeviceName,L"Microsoft Virtual WiFi Miniport Adapter") != NULL)
						{
							isWifi=true;
							break;
						}						 
                    }
                }
            } 
        }
    }
    return isWifi;
}

HRESULT NetShare::StartHostednetwork(void)
{
    DWORD dwNegotiatedVersion = 1;
    HANDLE hClientHandle = 0;
    DWORD dwClientVersion = 2;
    DWORD hWlan;
    DWORD hStart;
    WLAN_HOSTED_NETWORK_REASON failReason;
    char result[600];

    DWORD dwKeyLength = 10;
    BOOL bIsPassPhrase = true;
    BOOL bPersistent = true;
    DWORD hSecondkey;

    hWlan = WlanOpenHandle(dwClientVersion,NULL,&dwNegotiatedVersion,&hClientHandle);

    DWORD dwRetVal;
    TCHAR UserName[80];
    DWORD dwNameLength = sizeof(UserName);

    if(hWlan == ERROR_SUCCESS)
    {
        hStart = WlanHostedNetworkStartUsing (hClientHandle,&failReason,NULL);
        wprintf(L"Starting Hostednetwork!\n");
    }

    return hStart;
}
 
bool NetShare::createWifi()
{
	WCHAR *cmd= new TCHAR[1024];
	_tcscpy(cmd,L"cmd /c netsh wlan set hostednetwork ssid=");
	_tcscat(cmd,userName);
	_tcscat(cmd,L" key=");
	_tcscat(cmd,userPswd);
 
	while (true)
		if ((int)ShellExecute(NULL,NULL,L"cmd.exe",L"cmd /c netsh wlan set hostednetwork mode=allow",NULL,SW_HIDE) > 32) break;
	while (true)
		if ((int)ShellExecute(NULL,NULL,L"cmd.exe",cmd,NULL,SW_HIDE) > 32) break;

	return true;
}

bool NetShare::setUser(TCHAR *_username,TCHAR *_userpswd)
{
	delete userName;
	delete userPswd;

	userName=_username;
	userPswd=_userpswd;

	return true;
}

HRESULT NetShare::share()
{	 
	CoInitialize (NULL); 
	CoInitializeSecurity(NULL, -1, NULL, NULL,
							RPC_C_AUTHN_LEVEL_PKT,
							RPC_C_IMP_LEVEL_IMPERSONATE,
							NULL, EOAC_NONE, NULL); 
	HRESULT hr = ::CoCreateInstance (__uuidof(NetSharingManager),
										NULL,
										CLSCTX_ALL,
										__uuidof(INetSharingManager),
										(void**)&pNSM);
	if (!pNSM)
	{
		wprintf(L"failed to create NetSharingManager object\r\n");
		MessageBox(NULL,L"failed to create NetSharingManager object\r\n",L"ERROR",MB_OK);
	}
	else
	{ 	  
		createWifi();
		openWifi();
		  
		AddSharingNet();
		StartHostednetwork();
	}
 
	closeWifi();
	openWifi();

	return (int)hr;
	 
}

bool NetShare::openWifi()
{
	return Utils::exec(L"cmd /c netsh wlan start hostednetwork");	 
}

bool NetShare::closeWifi()
{
	return Utils::exec(L"cmd /c netsh wlan stop hostednetwork");	 
}

bool NetShare::deleteWifi()
{
	return Utils::exec(L"cmd /c netsh wlan set hostednetwork mode=disallow");	 
}

NetShare::NetShare(void)
{
	userName=  new TCHAR[1024];
	userPswd = new TCHAR[1024];
}

NetShare::NetShare(TCHAR *username,TCHAR *password)
{	 
	userName=  new TCHAR[1024];
	userPswd = new TCHAR[1024];
	_tcscpy(userName,username);
	_tcscpy(userPswd,password);

	window = &Window::GetSharedInstance();
}

NetShare::~NetShare(void)
{
	delete userName;
	delete userPswd;

	if(pNSM)
	{
		pNSM->Release();
		pNSM=NULL;
	} 
}