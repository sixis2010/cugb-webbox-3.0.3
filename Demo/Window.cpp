#include "Window.h"
#include "Utils.h"

#include <fstream>
#include <vector>

DWORD color(RGB(255,255,255));

Window::Window()
{
	hMsgWindow =NULL;
	hSkinWindow=NULL;
	 
	hFontX =XFont_Create2(L"Microsoft Yahei",19,false);
	hFontEdit = XFont_Create2(L"Microsoft Yahei",21,true); 
	hFont14 = XFont_Create2(L"Microsoft Yahei",14,false,true); 
	hFontH  = XFont_Create2(L"Microsoft Yahei",20,false);
	hFontS  = XFont_Create2(L"Microsoft Yahei",19,true,1);
	path_default_img=Utils::makePath(L"/image/wallpaper.png");

	brunonce =false;
	isWifiOn = false;
}

Window::~Window()
{  
}

BOOL Window::OnMyEventTabClick(HELE hEle,HELE hEventEle) 
{ 
	HideDotPage();
	XEle_ShowEle(hComboBox,FALSE);
	if(hEle!=hEventEle)  return FALSE;
	if(hEle==hCurSelTab) return FALSE;

	HELE hOldPage=(HELE)XEle_GetUserData(hCurSelTab);
	XEle_ShowEle(hOldPage,FALSE);
	XBtn_SetCheck(hCurSelTab,FALSE);
	
	HELE hCurSelPage=(HELE)XEle_GetUserData(hEle);
	XEle_ShowEle(hCurSelPage,TRUE);
	XBtn_SetCheck(hEle,TRUE);
	hCurSelTab=hEle;
	
	if(hCurSelPage == hPage3)
	{ 
		int x=0;
		for(;x<1600;x+=80)
		{
			XBtn_AddAnimationFrame(hWallover,XImage_LoadFileRect(Utils::makePath(L"/image/anim.png"),x,0,80,80),50); //x+=80;
		} 

		XBtn_EnableAnimation(hWallover,TRUE,false); 
	}
	else 
	{
		XBtn_EnableAnimation(hWallover,FALSE,FALSE);
		if(hCurSelPage == hPage2)
		{
			
		}
		else if(hCurSelPage == hPage4)
		{
			XEle_ShowEle(hComboBox,TRUE);
		}
		else if(hCurSelPage == hPage1)
		{
			XEle_ShowEle(hDotR,TRUE);
			XEle_ShowEle(hDotL,TRUE);
		}
	}

	XWnd_RedrawWnd(XEle_GetHWindow(hEle));

	return FALSE;
}

TCHAR *Window::GetEditText(HELE hEle)
{ 
	wchar_t *text;
	int len = XEdit_GetTextLength(hEle);
	text=new wchar_t[len+1];
	XEdit_GetText(hEle,text,len);
	text[len]=L'\0';
	return text;
}

BOOL Window::OnStartWifi(HELE hEle,HELE hEventEle) 
{   
	HideDotPage();

	if(Utils::IsXP())
	{
		Message(L"亲，请升级系统吧，XP暂不支持");	
	}

	if(XEdit_IsEmpty(hSSID))
	{
		return false;
	}
  
	if(isWifiOn)
	{
		if(netshare->closeWifi())
		{
			XBtn_SetText(hBtStartWifi,L"热点己停止");
			XBtn_SetImageLeave(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b2.png"),1));

			isWifiOn = false;
		} 
	}
	else 
	{ 
		netshare->setUser(GetEditText(hSSID),GetEditText(hPSW));
		if(netshare->share()==0)
		{
			XBtn_SetText(hBtStartWifi,L"热点己启动");
			XBtn_SetImageLeave(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b5.png"),1));
			isWifiOn = true;

			Utils::setRegistry(L"wifiname",GetEditText(hSSID));
			Utils::setRegistry(L"wifipswd",GetEditText(hPSW));
		}  
	}

	return true;
}

void Window::DotPageSwitch(bool isLeft)
{
	if(isLeft)
	{
		XBtn_SetImageLeave(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot.png"),1));  
		XBtn_SetImageLeave(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot2.png"),1)); 
		XBtn_SetImageStay(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot2.png"),1)); 
		
		XEle_ShowEle(hPage1,TRUE); 
		XEle_ShowEle(hPage5,FALSE);
 
		XWnd_RedrawWnd(hWindow);
	}
	else 
	{		 
		XBtn_SetImageLeave(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot.png"),1)); 
		XBtn_SetImageLeave(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot2.png"),1));
		XBtn_SetImageStay(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot2.png"),1)); 
		
		XEle_ShowEle(hPage1,FALSE); 
		XEle_ShowEle(hPage5,TRUE);

		XWnd_RedrawWnd(hWindow);
	}
}

BOOL Window::OnDotL(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent)  return FALSE;

	DotPageSwitch(TRUE);

	return TRUE;
}

BOOL Window::OnDotR(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent)  return FALSE;

	if(!netgateway->userinfo)
	{
		netgateway->Login(GetEditText(hEditSsid),GetEditText(hEditPsw),0);  
		netgateway->getUserInfo();
	}

	DotPageSwitch(FALSE);

	return TRUE;
}

void Window::GetAllElements()
{
	hTab1=XWnd_GetEle(hWindow,IDB_TAB1);
	hTab2=XWnd_GetEle(hWindow,IDB_TAB2);
	hTab3=XWnd_GetEle(hWindow,IDB_TAB3);
	hTab4=XWnd_GetEle(hWindow,IDB_TAB4);
	 
	hBtLogin = XWnd_GetEle(hWindow,ID_BT_LOGIN);

	hAuto = XWnd_GetEle(hWindow, ID_AUTOLOGIN  ); 
    hRem = XWnd_GetEle(hWindow,ID_REM  );
    hIpv6 =	XWnd_GetEle(hWindow,ID_IPV6  );
    hIpv4 =	XWnd_GetEle(hWindow,ID_IPV4  );
    hAutoStartup=	XWnd_GetEle(hWindow,ID_AUTOSTARTUP  );
    hStatic8	=	XWnd_GetEle(hWindow,ID_STATIC8  );
    hStatic9	=	XWnd_GetEle(hWindow,ID_STATIC9  );
	hAutoLogin  =   XWnd_GetEle(hWindow,ID_AUTOLOGIN2);

	hPage1=XWnd_GetEle(hWindow,ID_PAGE1);
	hPage2=XWnd_GetEle(hWindow,ID_PAGE2);
	hPage3=XWnd_GetEle(hWindow,ID_PAGE3);
	hPage4=XWnd_GetEle(hWindow,ID_PAGE4);
	hPage5=XWnd_GetEle(hWindow,ID_PAGE5);
   
	hBtLogout = XWnd_GetEle(hWindow,ID_BT_LOGOUT);
	hBtSvc   =  XWnd_GetEle(hWindow,ID_BT_SVC);

	hBtStartWifi = XWnd_GetEle(hWindow,ID_BT_START);
	hBtDelWifi   = XWnd_GetEle(hWindow,ID_BT_DEL);

	hEditSsid = XWnd_GetEle(hWindow,ID_EDIT_SID);
	hEditPsw  = XWnd_GetEle(hWindow,ID_EDIT_PSWD);
	
	hSSID		= XWnd_GetEle(hWindow,ID_SSID);
	hPSW		= XWnd_GetEle(hWindow,ID_PSW);
	hBtRestore	= XWnd_GetEle(hWindow,ID_BT_RESTORE);
	hStatic10	= XWnd_GetEle(hWindow,ID_STATIC10);
	hBtModify	= XWnd_GetEle(hWindow,ID_BT_MODIFY);
	hBtReset	= XWnd_GetEle(hWindow,ID_BT_RESET);
 
	hDotL = XWnd_GetEle(hWindow,ID_DOT_L);
	hDotR = XWnd_GetEle(hWindow,ID_DOT_R);

	hWallover=XWnd_GetEle(hWindow,ID_BT_WALLOVER);
	
	hStatic1 = XWnd_GetEle(hWindow,ID_STATIC2);
	hStatic2 = XWnd_GetEle(hWindow,ID_STATIC5);
	hStatic3 = XWnd_GetEle(hWindow,ID_STATIC6);
	hStatic4 = XWnd_GetEle(hWindow,ID_STATIC7); 
	hUserinfo= XWnd_GetEle(hWindow,ID_USERINFO);
	hEditMacOrig = XWnd_GetEle(hWindow,ID_EDIT_ORIG);
	hEditMacNew  = XWnd_GetEle(hWindow,ID_EDIT_NEW);

	hCopyright =   XWnd_GetEle(hWindow,ID_COPYRIGHT);
	hStaticD  =		XWnd_GetEle(hWindow,ID_STATICD);
	hStaticH =		XWnd_GetEle(hWindow,ID_STATICH);
	hStaticM  =		XWnd_GetEle(hWindow,ID_STATICM);
	hEditD =		XWnd_GetEle(hWindow,ID_EDIT_D);
	hEditH =		XWnd_GetEle(hWindow,ID_EDIT_H);
	hEditM =		XWnd_GetEle(hWindow,ID_EDIT_M);
	hBtShutdown =	XWnd_GetEle(hWindow,ID_BT_SHUTDOWN);
	hBtCancel =		XWnd_GetEle(hWindow,ID_BT_CANCEL);
	hBtStyle =		XWnd_GetEle(hWindow,ID_BT_STYLE);

	hRib    =       XWnd_GetEle(hWindow,ID_RIB);

	XEle_SetBkTransparent(XSView_GetView(hEditMacOrig),1);
	XEle_SetBkTransparent(XSView_GetView(hEditMacNew),1);

	XEle_SetTextColor(hUserinfo,RGB(100,100,255));
	
	XEle_SetFont(hBtLogout,hFontH);
	XEle_SetFont(hBtSvc,hFontH);
	XEle_SetFont(hEditMacOrig,hFontX);
	XEle_SetFont(hEditMacNew,hFontX);
	XEle_SetFont(hBtModify,hFontX);
	XEle_SetFont(hBtReset,hFontX);
	XEle_SetFont(hBtRestore,hFontX);
	XEle_SetFont(hBtShutdown,hFontX);

	XEle_SetFont(hEditD,hFontX);
	XEle_SetFont(hEditH,hFontX);
	XEle_SetFont(hEditM,hFontX);
	XEle_SetFont(hStaticD,hFontX);
	XEle_SetFont(hStaticH,hFontX);
	XEle_SetFont(hStaticM,hFontX);
	XEle_SetFont(hCopyright,hFontX);
	XEle_SetFont(hBtStyle,hFontX);
	XEle_SetFont(hBtCancel,hFontX);

	XEle_SetBkTransparent(XSView_GetView(hEditD),1);
	XEle_SetBkTransparent(XSView_GetView(hEditH),1);
	XEle_SetBkTransparent(XSView_GetView(hEditM),1);
	XEle_SetTextColor(hEditD,color);
	XEle_SetTextColor(hEditM,color);
	XEle_SetTextColor(hEditH,color);
	XEle_SetTextColor(hStaticD,color);
	XEle_SetTextColor(hStaticH,color);
	XEle_SetTextColor(hStaticM,color);
	XEle_SetTextColor(hEditMacOrig,color);
	XEle_SetTextColor(hEditMacNew,color);
	XEle_SetTextColor(hStatic8,color);
	XEle_SetTextColor(hStatic9,color);
	XEle_SetTextColor(hCopyright,color);

	XWnd_SetImageNC(hWindow,XImage_LoadFile(Utils::makePath(L"/image/wallpaper.png")));

	XPic_SetImage(hPage1,XImage_LoadFile(Utils::makePath(L"/image/bg1.png")));
	XPic_SetImage(hPage2,XImage_LoadFile(Utils::makePath(L"/image/bg1.png")));
	XPic_SetImage(hPage3,XImage_LoadFile(Utils::makePath(L"/image/bg4.png")));
	//XPic_SetImage(hPage4,XImage_LoadFile(Utils::makePath(L"/image/bg1.png")));
	XPic_SetImage(hPage5,XImage_LoadFile(Utils::makePath(L"/image/bg1.png")));
	XPic_SetImage(hRib,XImage_LoadFile(Utils::makePath(L"/image/ribbon3.png")));

	XBtn_SetIcon(hTab1,XImage_LoadFile(Utils::makePath(L"/image/dribble.png")));
	XBtn_SetImageLeave(hTab1,XImage_LoadFile(Utils::makePath(L"/image/transparent.png")));
	XBtn_SetImageStay(hTab1,XImage_LoadFile(Utils::makePath(L"/image/toolbar_hover.png")));
	XBtn_SetImageDown(hTab1,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	XBtn_SetImageCheck(hTab1,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	
	XBtn_SetIcon(hTab2,XImage_LoadFile(Utils::makePath(L"/image/radar.png")));
	XBtn_SetImageLeave(hTab2,XImage_LoadFile(Utils::makePath(L"/image/transparent.png")));
	XBtn_SetImageStay(hTab2,XImage_LoadFile(Utils::makePath(L"/image/toolbar_hover.png")));
	XBtn_SetImageDown(hTab2,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	XBtn_SetImageCheck(hTab2,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	
	XBtn_SetIcon(hTab3,XImage_LoadFile(Utils::makePath(L"/image/sheet.png")));
	XBtn_SetImageLeave(hTab3,XImage_LoadFile(Utils::makePath(L"/image/transparent.png")));
	XBtn_SetImageStay(hTab3,XImage_LoadFile(Utils::makePath(L"/image/toolbar_hover.png")));
	XBtn_SetImageDown(hTab3,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	XBtn_SetImageCheck(hTab3,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	
	XBtn_SetIcon(hTab4,XImage_LoadFile(Utils::makePath(L"/image/settings.png")));
	XBtn_SetImageLeave(hTab4,XImage_LoadFile(Utils::makePath(L"/image/transparent.png")));
	XBtn_SetImageStay(hTab4,XImage_LoadFile(Utils::makePath(L"/image/toolbar_hover.png")));
	XBtn_SetImageDown(hTab4,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
	XBtn_SetImageCheck(hTab4,XImage_LoadFile(Utils::makePath(L"/image/toolBar_pushed.png")));
 
	XBtn_SetImageLeave(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot2.png")));
	XBtn_SetImageStay(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot.png")));
	XBtn_SetImageDown(hDotL,XImage_LoadFile(Utils::makePath(L"/image/dot2.png")));

	XBtn_SetImageLeave(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot.png")));
	XBtn_SetImageStay(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot.png")));
	XBtn_SetImageDown(hDotR,XImage_LoadFile(Utils::makePath(L"/image/dot2.png")));

	XBtn_SetImageLeave(hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b3.png")));
	XBtn_SetImageStay(hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b7.png")));
	XBtn_SetImageDown(hBtLogin,XImage_LoadFile(Utils::makePath(L"/image/b3.png")));

	XBtn_SetImageLeave(hBtLogout,XImage_LoadFile(Utils::makePath(L"/image/btL.png")));
	XBtn_SetImageStay(hBtLogout,XImage_LoadFile(Utils::makePath(L"/image/btL3.png")));
	XBtn_SetImageDown(hBtLogout,XImage_LoadFile(Utils::makePath(L"/image/btL3.png")));

	XBtn_SetImageLeave(hBtSvc,XImage_LoadFile(Utils::makePath(L"/image/btR.png")));
	XBtn_SetImageStay(hBtSvc,XImage_LoadFile(Utils::makePath(L"/image/btR3.png")));
	XBtn_SetImageDown(hBtSvc,XImage_LoadFile(Utils::makePath(L"/image/btR3.png")));

	XBtn_SetImageLeave(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b3.png")));
	XBtn_SetImageStay(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b7.png")));
	XBtn_SetImageDown(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b3.png")));

	XBtn_SetImageLeave(hBtRestore,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtRestore,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtRestore,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));

	XBtn_SetImageLeave(hBtStyle,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtStyle,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtStyle,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));

	XBtn_SetImageLeave(hBtModify,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtModify,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtModify,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));

	XBtn_SetImageLeave(hBtReset,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtReset,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtReset,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));

	XBtn_SetImageLeave(hBtShutdown,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtShutdown,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtShutdown,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
 
	XBtn_SetImageLeave(hBtCancel,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));
	XBtn_SetImageStay(hBtCancel,XImage_LoadFile(Utils::makePath(L"/image/btR5.png")));
	XBtn_SetImageDown(hBtCancel,XImage_LoadFile(Utils::makePath(L"/image/btR4.png")));

	DotPageSwitch(TRUE);

	XCGUI_RegEleEvent(hDotL,XE_BNCLICK,&Window::OnDotL);
	XCGUI_RegEleEvent(hDotR,XE_BNCLICK,&Window::OnDotR);
	XCGUI_RegEleEvent(hTab1,XE_BNCLICK,&Window::OnMyEventTabClick);
	XCGUI_RegEleEvent(hTab2,XE_BNCLICK,&Window::OnMyEventTabClick);
	XCGUI_RegEleEvent(hTab3,XE_BNCLICK,&Window::OnMyEventTabClick);
	XCGUI_RegEleEvent(hTab4,XE_BNCLICK,&Window::OnMyEventTabClick);
	XCGUI_RegEleEvent(hBtLogin,XE_BNCLICK,&Window::OnLogin);
	XCGUI_RegEleEvent(hBtLogout,XE_BNCLICK,&Window::OnLogout);
	XCGUI_RegEleEvent(hBtSvc,XE_BNCLICK,&Window::OnService);
	XCGUI_RegEleEvent(hBtStartWifi,XE_BNCLICK,&Window::OnStartWifi);
	XCGUI_RegEleEvent(hWallover,XE_BNCLICK,&Window::OnWallover);
	XCGUI_RegEleEvent(hIpv4,XE_BNCLICK,&Window::OnCheckIPV4);
	XCGUI_RegEleEvent(hIpv6,XE_BNCLICK,&Window::OnCheckIPV6);
	XCGUI_RegEleEvent(hAuto,XE_BNCLICK,&Window::OnCheckAutoWifi);
	XCGUI_RegEleEvent(hRem,XE_BNCLICK,&Window::OnCheckShowPsw);
	XCGUI_RegEleEvent(hAutoStartup,XE_BNCLICK,&Window::OnCheckAutoStartup);
	XCGUI_RegEleEvent(hAutoLogin,XE_BNCLICK,&Window::OnCheckAutoLogin);
	XCGUI_RegEleEvent(hBtModify,XE_BNCLICK,&Window::OnBtMacChange);
	XCGUI_RegEleEvent(hBtReset,XE_BNCLICK,&Window::OnBtMacReset);
	XCGUI_RegEleEvent(hBtStyle,XE_BNCLICK,&Window::OnBtStyle);
	XCGUI_RegEleEvent(hBtShutdown,XE_BNCLICK,&Window::OnBtShutdown);
	XCGUI_RegEleEvent(hBtCancel,XE_BNCLICK,&Window::OnBtCancel);
	XCGUI_RegEleEvent(hBtRestore,XE_BNCLICK,&Window::OnBtRestore);
}

void Window::ResetColor()
{
	if(color == RGB(255,255,255))
	{
		color = RGB(65,105,225);
	}
	else 
	{
		color = RGB(255,255,255);
	}

	XEle_SetTextColor(hEditD,color);
	XEle_SetTextColor(hEditM,color);
	XEle_SetTextColor(hEditH,color);
	XEle_SetTextColor(hStaticD,color);
	XEle_SetTextColor(hStaticH,color);
	XEle_SetTextColor(hStaticM,color);
	XEle_SetTextColor(hEditMacOrig,color);
	XEle_SetTextColor(hEditMacNew,color);
	XEle_SetTextColor(hStatic8,color);
	XEle_SetTextColor(hStatic9,color);
	XEle_SetTextColor(hComboBox,color);
	XEle_SetTextColor(hCopyright,color);
	 
	//HANDLE hThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)&MyMenu,0,0,0);
   

	XWnd_RedrawWnd(hWindow);
}

BOOL Window::OnBtRestore(HELE hEle,HELE hEleEvent)
{
	Utils::restore();
	OnRunOnce();
	return false;
}

BOOL Window::OnBtShutdown(HELE hEle,HELE hEleEvent)
{
	int d,h,m;

	wchar_t *d_str,*h_str,*m_str;
	d_str = GetEditText(hEditD);
	h_str = GetEditText(hEditH);
	m_str = GetEditText(hEditM);

	d = _wtoi(d_str);
	h = _wtoi(h_str);
	m = _wtoi(m_str);

	int sec = d*24*60*60+h*60*60+m*60;

	wchar_t *cmdline = new wchar_t[1024];
	wsprintf(cmdline,L"shutdown /s /f /t %d",sec);
	 
	return Utils::exec(cmdline);

}

BOOL Window::OnBtCancel(HELE hEle,HELE hEleEvent)
{
	XEdit_SetText(hEditD,L"");
	XEdit_SetText(hEditH,L"");
	XEdit_SetText(hEditM,L"");
	return Utils::exec(L"shutdown -a");
}

BOOL Window::OnBtStyle(HELE hEle,HELE hEleEvent)
{
	ResetColor();
	return true;
}

BOOL Window::OnWallover(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent) return FALSE;
	 
	if(netwallover->isWallover)
	{
		int x=0;
		for(;x<1600;x+=80)
		{
			XBtn_AddAnimationFrame(hWallover,XImage_LoadFileRect(Utils::makePath(L"/image/anim.png"),x,0,80,80),50); //x+=80;
		} 

		XBtn_EnableAnimation(hWallover,TRUE,false); 
		netwallover->delProxy();
	}
	else 
	{
		netwallover->setProxy(L"127.0.0.1",L"8087");
		
		XBtn_EnableAnimation(hWallover,false);
		XBtn_SetImageLeave(hWallover,XImage_LoadFile(Utils::makePath(L"/image/safari5.png"),1));  
		XBtn_SetImageStay(hWallover,XImage_LoadFile(Utils::makePath(L"/image/safari3.png"),1));  
		XBtn_SetImageDown(hWallover,XImage_LoadFile(Utils::makePath(L"/image/safari2.png"),1));  
	}

	return FALSE;
}

bool Window::VerifyMacID(CString strMac)
{
	bool bRet = false;
	strMac.MakeUpper().Remove(_T(' '));		
	if(strMac.GetLength() == 12)
	{			
		for(int i=0; i<12; i++)
		{
			if( !( ( (strMac[i] >= _T('0')) && (strMac[i] <= _T('9')) )
				|| ( (strMac[i] >= _T('A')) && (strMac[i] <= _T('F')) ) ) )
			{				
				break;
			}
			if(i==11)
				bRet = true;
		}		
	}
	return bRet;
} 

BOOL Window::OnBtMacChange(HELE hEle,HELE hEleEvent)
{
	int index = XComboBox_GetSelectItem(hComboBox);
	ADAPTERINFO *pAdInfo = &m_pAdapters[index];
	wchar_t *mac_addr = GetEditText(hEditMacNew);
	if(!VerifyMacID(mac_addr))
	{
		Message(L"MAC地址不合法");
	}
	if(UpdateRegistry(pAdInfo->InstanceId, mac_addr))
	{
		if(Reset(pAdInfo))
		{ 
			Message(L"MAC地址设置成功"); 
		}
		else
		{
			Message(L"MAC地址设置失败"); 
		}
		int count = RefreshAdapter();
		XEdit_SetText(hEditMacOrig,m_pAdapters[index].MAC.AllocSysString());
		XEdit_SetText(hEditMacNew,L"");
	}
	return false;
}

BOOL Window::OnBtMacReset(HELE hEle,HELE hEleEvent)
{
	int index = XComboBox_GetSelectItem(hComboBox);
	ADAPTERINFO *pAdInfo = &m_pAdapters[index];
	if(UpdateRegistry(pAdInfo->InstanceId))
	{
		if(Reset(pAdInfo))
		{
			int count = RefreshAdapter();
			XEdit_SetText(hEditMacOrig,m_pAdapters[index].MAC.AllocSysString());
			XEdit_SetText(hEditMacNew,L"");
			Message(L"MAC地址重置成功"); 
		}
		else
		{
			Message(L"MAC地址重置失败"); 
		}
	}	
	return false;
}

BOOL Window::OnEventBtnClick_Close(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent) return FALSE;

	NOTIFYICONDATA tnid; 
	tnid.cbSize=sizeof(NOTIFYICONDATA); 
	tnid.hWnd=XWnd_GetHWnd(hWindow); 
	tnid.uID=IDI_ICON2; 
	Shell_NotifyIcon(NIM_DELETE,&tnid);

	netwallover->delProxy();
	if(autologout)
	{  
		netgateway->Logout(GetEditText(hEditSsid),GetEditText(hEditPsw),0);
	} 

	XWnd_CloseWindow(hWindow);
	return FALSE;
}

BOOL Window::OnEventBtnClick_Minimize(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent) return FALSE;
	ShowWindow(XWnd_GetHWnd(hWindow),SW_HIDE );
	return FALSE;
}

void Window::OnUpdate()
{	 
	if(netgateway->ipv4Connected)
	{ 
		if(update->checkVersion(NULL))
		{
			Message(L"检测到新版本，将执行自动升级");
			update->update();
		}
	} 
}

BOOL Window::OnLogin(HELE hEle,HELE hEleEvent) 
{
	if(hEle!=hEleEvent) return FALSE;	 

	Utils::setRegistry(L"username",GetEditText(hEditSsid));
	Utils::setRegistry(L"password",GetEditText(hEditPsw));
	
	MessageBox(NULL,L"",L"",0);

	netgateway->Login(GetEditText(hEditSsid),GetEditText(hEditPsw),0);
	MessageBox(NULL,L"",L"",0);
	if(brunonce)
		network->goOnline(GetEditText(hEditSsid),GetEditText(hEditPsw));
	MessageBox(NULL,L"",L"",0);
	/*jump to next page*/
	DotPageSwitch(FALSE);
	/*show user info*/
	netgateway->getUserInfo();
	 
	return TRUE;
}

void Window::UpdateUserInfo(wchar_t *text)
{
	XStatic_SetText(hUserinfo, text );
}
 
BOOL Window::OnLogout(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE;
	netgateway->Logout(GetEditText(hEditSsid),GetEditText(hEditPsw),0);
	UpdateUserInfo(L"");
	DotPageSwitch(TRUE);
	return TRUE;
}

BOOL Window::OnService(HELE hEle,HELE hEleEvent)
{
	netgateway->Service(GetEditText(hEditSsid),GetEditText(hEditPsw));
	return TRUE;
}

BOOL Window::OnEventBtnClick_ChangeSkin(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	CreateSkin();
	return FALSE;
}

BOOL  Window::OnCheckIPV4(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	if(value) {
		Utils::setRegistry(L"ipv4",L"1");
	} else {
		Utils::setRegistry(L"ipv4",L"0");
	}

	return TRUE;
}
BOOL  Window::OnCheckIPV6(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	if(value) {
		Utils::setRegistry(L"ipv6",L"1");
	} else {
		Utils::setRegistry(L"ipv6",L"0");
	}

	return TRUE;  
}

BOOL  Window::OnCheckAutoWifi(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	if(value) {
		Utils::setRegistry(L"autowifi",L"1");
	} else {
		Utils::setRegistry(L"autowifi",L"0");
	}

	return TRUE;  
}
BOOL  Window::OnCheckShowPsw(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	if(value) {
		XEdit_EnablePassBox(hPSW,false);
		Utils::setRegistry(L"showpassword",L"1");
	} else {
		XEdit_EnablePassBox(hPSW,true);
		Utils::setRegistry(L"showpassword",L"0");
	}

	XWnd_RedrawWnd(hWindow,true);

	return TRUE;  
}
 
BOOL  Window::OnCheckAutoStartup(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	if(value) {
		Utils::setRegistry(L"autostartup",L"1");
	} else {
		Utils::setRegistry(L"autostartup",L"0");
	}

	return TRUE;  

}

BOOL  Window::OnCheckAutoLogin(HELE hEle,HELE hEleEvent)
{
	if(hEle!=hEleEvent) return FALSE; 
	
	BOOL prev_value = XBtn_IsCheck(hEleEvent);
	BOOL value = !prev_value;

	XBtn_SetCheck(hEleEvent,value);

	autologout = value;

	if(value) {
		Utils::setRegistry(L"autologout",L"1");
	} else {
		Utils::setRegistry(L"autologout",L"0");
	}

	return TRUE;  
}
  
std::vector<std::wstring> imgList;

void Window::CreateSkin()
{	
	int width=400;
	int height=490;

	RECT rect;
	XEle_GetRect(hBtnClose,&rect);

	POINT pt;
	pt.x=rect.right+5;
	pt.y=rect.bottom-28;

	HWND hMainWnd=XWnd_GetHWnd(hWindow);
	
	ClientToScreen(hMainWnd,&pt);

	hSkinWindow=XWnd_CreateWindow(pt.x,pt.y,width,height,L"Skin",hMainWnd,XC_SY_ROUND);
    XWnd_EnableDragWindow(hSkinWindow,TRUE);
	XWnd_SetRoundSize(hSkinWindow,9);
    XWnd_SetTransparentFlag(hSkinWindow,XC_WIND_TRANSPARENT_SHADOW);

	XWnd_SetImageNC(hSkinWindow,XImage_LoadFile(path_default_img,1));
	 
	HELE hPicTitle=XPic_Create(10,10,360,25,hSkinWindow);
	XEle_SetBkTransparent(hPicTitle,TRUE);
	XPic_SetImage(hPicTitle,XImage_LoadFile(Utils::makePath(L"/image/skinDlg/titleBG.png"),1));
	
	HELE hStaticTitle=XStatic_Create(10,5,100,20,L"更换皮肤",hPicTitle);
	XEle_SetBkTransparent(hStaticTitle,TRUE);
	XEle_SetTextColor(hStaticTitle,color);

	hListView=XListView_Create(20,45,360,430,hSkinWindow);
	XListView_SetIconSize(hListView,97,62);
	XListView_SetItemBorderSpacing(hListView,3,3,3,3);
	XListView_SetViewLeftAlign(hListView,0);
	XListView_SetViewTopAlign(hListView,0);
	XEle_SetTextColor(hListView,color);
	XEle_EnableBorder(hListView,FALSE);
	XEle_SetBkTransparent(hListView,TRUE);
	XEle_SetBkTransparent(XSView_GetView(hListView),TRUE);

	HXCGUI hImageList=XImageList_Create(97,62);
  
	imgList.clear();
	
	WIN32_FIND_DATA fd;
 
	std::wstring path = Utils::getPath();
	path.append(L"\\image\\skin\\*.*"); 
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	std::wstring filename,postfix;
	int pDot=0;
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{ 
			filename = fd.cFileName; 
			pDot = filename.find_last_of(L"."); 
			postfix =filename.substr(pDot,filename.size()-pDot); 
			if(wcscmp( postfix.c_str(),L".png") == 0 || wcscmp( postfix.c_str(),L".jpg") == 0)
			{
				imgList.push_back(filename);  
			}
		}
		while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	} 

	for(int i=0;i<imgList.size();++i)
	{
		path =  Utils::getPath();
		path.append(L"\\image\\Skin\\");
		 
		XImageList_AddImage(hImageList,XImage_LoadFile((wchar_t *)path.append(imgList[i]).c_str()));
	}
	 

	XListView_SetImageList(hListView,hImageList);

	for(int i=0;i<imgList.size();++i)
	{
		XListView_AddItem(hListView,(wchar_t *)imgList[i].c_str(),i);		 
	}
 
	HIMAGE  hImageStay=XImage_LoadFile(Utils::makePath(L"/image/skinDlg/listView_stay.png"));    
	HIMAGE  hImageSelect=XImage_LoadFile(Utils::makePath(L"/image/skinDlg/listView_select.png")); 

	for (int i=0;i<7;i++)
	{
		XListView_SetItemImageStay(hListView,-1,i,hImageStay);
		XListView_SetItemImageSelect(hListView,-1,i,hImageSelect);
	}

	XListView_SetSelectItem(hListView,-1,SkinIndex,TRUE);

	HELE hScrollBar=XSView_GetVScrollBar(hListView);
	XEle_SetBkTransparent(hScrollBar,TRUE);
	XSBar_EnableScrollButton2(hScrollBar,FALSE);
	XSBar_SetImageLeaveSlider(hScrollBar,XImage_LoadFileAdaptive(Utils::makePath(L"/image/skinDlg/ScrollBar_leave.png"),1,14,10,40));
	XSBar_SetImageStaySlider(hScrollBar,XImage_LoadFileAdaptive(Utils::makePath(L"/image/skinDlg/ScrollBar_stay.png"),1,14,10,40));
	XSBar_SetImageDownSlider(hScrollBar,XImage_LoadFileAdaptive(Utils::makePath(L"/image/skinDlg/ScrollBar_stay.png"),1,14,10,40));

	XCGUI_RegEleEvent(hListView,XE_LISTVIEW_SELECT,&Window::OnEventListViewSelect);
	XCGUI_RegWndMessage(hSkinWindow,WM_KILLFOCUS,&Window::OnWndKillFocus);
	
	XWnd_ShowWindow(hSkinWindow,SW_SHOW);
}

BOOL Window::OnEventListViewSelect(HELE hEle,HELE hEventEle,int groupIndex,int itemIndex)
{
	if(itemIndex<0) return FALSE;

	if(SkinIndex!=itemIndex)  
	{
		SkinIndex=itemIndex;
		std::wstring path =  Utils::getPath();
		path.append(L"\\image\\Skin\\");
		 
		hThemeBackground =XImage_LoadFile((wchar_t *)path.append(imgList[itemIndex]).c_str());
		wchar_t *tmp = new wchar_t[path.size()];
		wsprintf(tmp,L"%s",path.c_str());
		path_default_img = tmp;
		XImage_SetDrawType(hThemeBackground,XC_IMAGE_TILE);

		XWnd_SetImageNC(hWindow,hThemeBackground);
		 
		XWnd_SetImageNC(hSkinWindow,hThemeBackground);

		XWnd_RedrawWnd(hWindow);
		XWnd_RedrawWnd(hSkinWindow);
	}
	return FALSE;
}

BOOL Window::OnWndKillFocus(HWINDOW hwnd)
{
	HWND hWnd=XWnd_GetHWnd(hwnd);	 
	ShowWindow(hWnd,SW_HIDE); 
	::DestroyWindow(hWnd);	 
	return TRUE;
}

BOOL Window::OnMsgOk(HELE hEle,HELE hEleEvent)
{
	HWND hWnd=XEle_GetHWnd(hEle);
 
	ShowWindow(hWnd,SW_HIDE);
	return TRUE;
}

void Window::Message(wchar_t *text,wchar_t *caption)
{ 
	POINT pt;
 
	int width=320;
	int height=480;

	pt.x=0;
	pt.y=0;
 
	HWND hMainWnd=XWnd_GetHWnd(hWindow);
 
	ClientToScreen(hMainWnd,&pt);
		 
	hMsgWindow=XWnd_CreateWindow(pt.x,pt.y,width,height,L"Webbox 消息",hMainWnd,XC_SY_ROUND);
	XWnd_EnableDragWindow(hMsgWindow,FALSE);
	XWnd_SetRoundSize(hMsgWindow,9);
	XWnd_SetCaptionHeight(hMsgWindow,0);
	//XWnd_SetTransparentFlag(hMsgWindow,XC_WIND_TRANSPARENT_SHAPED);
	himg =XImage_LoadFile(path_default_img,0);
     
	XWnd_SetImageNC(hMsgWindow,himg); 

	HELE hPicLogo=XPic_Create(5,5,25,25,hMsgWindow);
	XEle_SetBkTransparent(hPicLogo,TRUE);
	XPic_SetImage(hPicLogo,XImage_LoadFile(Utils::makePath(L"/image/head.png"),1));
	XEle_EnableMouseThrough(hPicLogo,TRUE);

	/*Title*/
	HELE hHead = XStatic_Create(30, 5, 320, 50,L"来自Srun的消息",hMsgWindow);
	XEle_SetBkTransparent(hHead,TRUE);
	XEle_SetTextColor(hHead,color);
	XEle_SetFont(hHead,hFontEdit);
	XEle_EnableMouseThrough(hHead,TRUE);
	XStatic_SetTextAlign(hHead,TA_CENTER);
	XStatic_AdjustSize(hHead); 

	/*Display message*/
	HELE hHead2=XStatic_Create(40, 150, 230, 200,L"",hMsgWindow);		
	XEle_EnableBorder(hHead2,FALSE);
	XEle_SetBkTransparent(hHead2,TRUE);	
	XEle_SetFont(hHead2,hFontEdit);		 
	XEle_SetTextColor(hHead2,color);
	  
	XStatic_SetText(hHead2,Utils::makeSentence(text,15));
	XEle_RedrawEle(hHead2,TRUE);
		 
	/*Close button*/
	hBtOK=XBtn_Create(59,360,200,50,NULL,hMsgWindow);
	XEle_SetBkTransparent(hBtOK,TRUE);
	XEle_EnableFocus(hBtOK,FALSE);
	XBtn_SetImageLeave(hBtOK,XImage_LoadFile(Utils::makePath(L"/image/b6.png"),1));
	XBtn_SetImageStay(hBtOK,XImage_LoadFile(Utils::makePath(L"/image/b2.png"),1));
	XBtn_SetImageDown(hBtOK,XImage_LoadFile(Utils::makePath(L"/image/b4.png"),1));
	XBtn_SetText(hBtOK,L"知道啦");
	XEle_SetFont(hBtOK,hFontX);
	XEle_SetTextColor(hBtOK,color);

	XCGUI_RegWndMessage(hMsgWindow,WM_KILLFOCUS,&Window::OnWndKillFocus);
	XCGUI_RegEleEvent(hBtOK,XE_BNCLICK,&Window::OnMsgOk); 
  
	XWnd_EnableDragWindow(hMsgWindow,FALSE);

	XWnd_ShowWindow(hMsgWindow,SW_SHOW);

}

void Window::HideDotPage()
{
	XEle_ShowEle(hPage1,FALSE); 
	XEle_ShowEle(hPage5,FALSE);
	XEle_ShowEle(hDotR,FALSE);
	XEle_ShowEle(hDotL,FALSE);
}

void Window::initCheck(HELE hEle)
{
	XCheck_SetImageLeave_UnCheck(hEle,XImage_LoadFile(Utils::makePath(L"/image/off2.png"),true));  
    XCheck_SetImageStay_UnCheck(hEle,XImage_LoadFile(Utils::makePath(L"/image/off2.png"),true));
    XCheck_SetImageLeave_Check(hEle,XImage_LoadFile(Utils::makePath(L"/image/On2.png"),true));
    XCheck_SetImageStay_Check(hEle,XImage_LoadFile(Utils::makePath(L"/image/On2.png"),true));
    XCheck_SetImageDown_UnCheck(hEle,XImage_LoadFile(Utils::makePath(L"/image/off2.png"),true));
    XCheck_SetImageDown_Check(hEle,XImage_LoadFile(Utils::makePath(L"/image/On2.png"),true));
}
 
BOOL Window::OnMenu(HELE hEle,UINT flags,POINT *pPt)//创建菜单过程
{
	
	RECT rect;
	XEle_GetRect(hBtnMenu,&rect);

	POINT pt;
	pt.x=rect.left;
	pt.y=rect.bottom;

	 hMenu=XMenu_Create();
	 XMenu_AddItem(hMenu,1,L"访问官网");
	 XMenu_AddItem(hMenu,2,L"技术支持");	 
	 XMenu_AddItem(hMenu,3,L"关于");
	 XMenu_AddItem(hMenu,4,L"退出");

	 XMenu_SetBGImage(hMenu,XImage_LoadFile(Utils::makePath(L"/image\b2.png"),0));

	 HWND hMainWnd=XWnd_GetHWnd(hWindow);
	 ClientToScreen(hMainWnd,&pt);
	 XMenu_Popup(hMenu,XWnd_GetHWnd(hWindow),pt.x,pt.y);

	return false;
}

BOOL Window::OnMenuSelect(HWINDOW hWindow,int id)  //菜单选择
{
    switch(id)
	{
	case 1:
		netgateway->openURL(L"http://www.objcoder.com:8088");
		break;
	case 2:
		Message(L"若有技术问题或者想拥有定制版本\n请联系\n：dreamware@outlook.com\nQQ:827275102\n(c)2013 Dream X Studio.\nAll rights reserved");
		break;
	case 3:
		Message(L"Webbox 地大快车\n版本：Build.X1.13\n程序设计:石杰\n美术设计:左佳伟，石杰\n让地大学子生活更美好");
		break;
	case 4:
		SendMessage(XWnd_GetHWnd(hWindow),WM_CLOSE,NULL,NULL);
		break;
	}
    return FALSE;
}

void Window::CreateMainUI()
{
	XInitXCGUI(); 
 
	MessageBox(NULL,L"",L"",0);

	HXMLRES  hRes=XXmlRes_Load(Utils::makePath(L"/layout/mainFrame.res"));

	if(NULL==hRes)
	{
		MessageBox(NULL,L"Resource file is missing, please reinstall the app",L"Webbox message",0);
		return ;
	} 
 
	hWindow=XC_LoadXML_Window(Utils::makePath(L"/layout/mainFrame.xml"),hRes);  
  
	if(NULL==hWindow)
	{
		MessageBox(NULL,L"界面文件丢失，请重新安装软件",L"错误",0);
		return ;
	}

	hBtnClose=XBtn_Create(270,0,47,22,NULL,hWindow);
	XEle_SetBkTransparent(hBtnClose,TRUE);
	XEle_EnableFocus(hBtnClose,FALSE); 
	XBtn_SetImageLeave(hBtnClose,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_close.png"),0,0,47,22));
	XBtn_SetImageStay(hBtnClose,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_close.png"),47,0,47,22));
	XBtn_SetImageDown(hBtnClose,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_close.png"),94,0,47,22));
 
	hBtnMin=XBtn_Create(237,0,33,22,NULL,hWindow);
	XEle_SetBkTransparent(hBtnMin,TRUE);
	XEle_EnableFocus(hBtnMin,FALSE);
	XBtn_SetImageLeave(hBtnMin,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_min.png"),0,0,33,22));
	XBtn_SetImageStay(hBtnMin,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_min.png"),33,0,33,22));
	XBtn_SetImageDown(hBtnMin,XImage_LoadFileRect(Utils::makePath(L"/image/sys_button_min.png"),66,0,33,22));

	hBtnMenu=XBtn_Create(204,0,33,22,NULL,hWindow);
	XEle_SetBkTransparent(hBtnMenu,TRUE);
	XEle_EnableFocus(hBtnMenu,FALSE);
	XBtn_SetImageLeave(hBtnMenu,XImage_LoadFileRect(Utils::makePath(L"/image/title_bar_menu.png"),0,0,33,22));
	XBtn_SetImageStay(hBtnMenu,XImage_LoadFileRect(Utils::makePath(L"/image/title_bar_menu.png"),33,0,33,22));
	XBtn_SetImageDown(hBtnMenu,XImage_LoadFileRect(Utils::makePath(L"/image/title_bar_menu.png"),66,0,33,22));
	
	hBtnSkin=XBtn_Create(180,0,22,18,NULL,hWindow);
	XEle_SetBkTransparent(hBtnSkin,TRUE);
	XEle_EnableFocus(hBtnSkin,FALSE);
	XBtn_SetImageLeave(hBtnSkin,XImage_LoadFileRect(Utils::makePath(L"/image/SkinButtom.png"),0,0,22,18));
	XBtn_SetImageStay(hBtnSkin,XImage_LoadFileRect(Utils::makePath(L"/image/SkinButtom.png"),22,0,22,18));
	XBtn_SetImageDown(hBtnSkin,XImage_LoadFileRect(Utils::makePath(L"/image/SkinButtom.png"),44,0,22,18));

	hPicLogo=XPic_Create(5,5,25,25,hWindow);
	XEle_SetBkTransparent(hPicLogo,TRUE);
	XPic_SetImage(hPicLogo,XImage_LoadFile(Utils::makePath(L"/image/head.png"),1));
	XEle_EnableMouseThrough(hPicLogo,TRUE);

	hThemeBackground=XImage_LoadFile(Utils::makePath(L"/image/skin/f.png"));
	//HANDLE hThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)&MyMenu,0,0,0);

	XCGUI_RegEleEvent(hBtnClose,XE_BNCLICK,&Window::OnEventBtnClick_Close);
	XCGUI_RegEleEvent(hBtnSkin,XE_BNCLICK,&Window::OnEventBtnClick_ChangeSkin);
	XCGUI_RegEleEvent(hBtnMin,XE_BNCLICK,&Window::OnEventBtnClick_Minimize);
	XCGUI_RegEleEvent(hBtnMenu,XE_BNCLICK,&Window::OnMenu);
	 
	HELE hHead = XStatic_Create(32, 5, 100, 25,L"WebBox",hWindow);
	XEle_SetBkTransparent(hHead,TRUE);
	XEle_SetTextColor(hHead,color);
	XEle_SetFont(hHead,hFontEdit);
	XEle_EnableMouseThrough(hHead,TRUE);

#ifdef __GIRL__

#define TITLE L"girl"
#define SEX   L"♀"

#else

#define TITLE L"boy"
#define SEX   L"♂"

#endif

	HELE hHead2 = XStatic_Create(100, 2, 60, 25,TITLE,hWindow); //♂ ♀
	XEle_SetBkTransparent(hHead2,TRUE);
	XEle_SetTextColor(hHead2,color);
	XEle_SetFont(hHead2,hFont14);
	XEle_EnableMouseThrough(hHead2,TRUE);
 
	HELE hHead3 = XStatic_Create(120, 3, 25, 25,SEX,hWindow); //♂ ♀
	XEle_SetBkTransparent(hHead3,TRUE);
	XEle_SetTextColor(hHead3,color);
	XEle_SetFont(hHead3,hFontS);
	XEle_EnableMouseThrough(hHead3,TRUE);

 
	 hMenu=XMenu_Create();
	 XMenu_AddItem(hMenu,1,L"访问官网");
	 XMenu_AddItem(hMenu,2,L"技术支持");	 
	 XMenu_AddItem(hMenu,3,L"关于");
	 XMenu_AddItem(hMenu,4,L"退出");
	 XMenu_SetAutoDestroy(hMenu,false);
	 XCGUI_RegWndMessage(hWindow,XWM_MENUSELECT,&Window::OnMenuSelect);

	 XMenu_SetBGImage(hMenu,XImage_LoadFile(Utils::makePath(L"/image/b2.png"),0));

	XWnd_SetTransparentFlag(hWindow,XC_WIND_TRANSPARENT_SHADOW);
	XWnd_EnableDragBorder(hWindow,false);
 
	XC_LoadXML_Page(hWindow,Utils::makePath(L"/layout/mainPage2.xml"),hRes);
	XC_LoadXML_Page(hWindow,Utils::makePath(L"/layout/mainPage3.xml"),hRes);
	XC_LoadXML_Page(hWindow,Utils::makePath(L"/layout/mainPage4.xml"),hRes);
	XC_LoadXML_Page(hWindow,Utils::makePath(L"/layout/mainPage1.xml"),hRes);
 
	if(FALSE==XC_LoadXML_Style(hWindow,Utils::makePath(L"/layout/mainFrame.css") ,hRes))	 
	{
		MessageBox(NULL,L"CSS file is missing! Please Reinstall the application",L"Webbox Error",0);
		return ;
	} 

	SendMessage(XWnd_GetHWnd(hWindow),WM_SETICON,IDI_ICON1,(LPARAM )LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON2)));

	GetAllElements();

    XEle_SetFont(hPSW,  hFontX);
	XEle_SetFont(hSSID, hFontX);
	
	XEdit_EnablePassBox(hPSW,true);
	XWnd_SetTransparentAlpha(hWindow,250);

	initCheck(hIpv4);
	initCheck(hIpv6);
	initCheck(hAuto);
	initCheck(hRem);
	initCheck(hIpv4); 
	initCheck(hAutoStartup);
	initCheck(hAutoLogin);
	
	XEle_SetFont(hStatic8,hFontEdit);
	XEle_SetFont(hStatic9,hFontEdit);
	XEle_SetFont(hTab1,hFontX);
	XEle_SetFont(hTab2,hFontX);
	XEle_SetFont(hTab3,hFontX);
	XEle_SetFont(hTab4,hFontX);
	XWnd_SetFont(hWindow,hFontX);
	XEle_SetFont(hUserinfo,hFontEdit);
	
	XEle_SetFont(hBtStartWifi,hFontH);	 
	XEdit_EnablePassBox(hEditPsw,true);
	 
	XEle_SetFont(hEditSsid,hFontX);
	XEle_SetFont(hEditPsw,hFontX);
	XEle_SetFont(hBtLogin,hFontH); 

	XEle_SetBkTransparent(hEditSsid,TRUE);
	XEle_SetBkTransparent(XSView_GetView(hEditSsid),TRUE);
	XEle_SetBkTransparent(hEditPsw,TRUE);
	XEle_SetBkTransparent(XSView_GetView(hEditPsw),TRUE);
 
	XEle_SetBkTransparent(hSSID,TRUE);
	XEle_SetBkTransparent(XSView_GetView(hSSID),TRUE);
 
	XEle_SetBkTransparent(hPSW,TRUE);
	XEle_SetBkTransparent(XSView_GetView(hPSW),TRUE);
  
	XEle_SetFont(hStatic1,hFontX);
	XEle_SetFont(hStatic2,hFontX);
	XEle_SetFont(hStatic3,hFontX);
	XEle_SetFont(hStatic4,hFontX);
 
	XWnd_SetBorderSize(hWindow,0,0,0,0);
	XWnd_EnableDragWindow(hWindow,TRUE);
	 
	XEle_SetUserData(hTab1,(int)hPage1);
	 
	XEle_SetUserData(hTab2,(int)hPage2);
	XEle_ShowEle(hPage2,FALSE);

	XEle_SetUserData(hTab3,(int)hPage3);
	XEle_ShowEle(hPage3,FALSE);
 
	XEle_SetUserData(hTab4,(int)hPage4);
	XEle_ShowEle(hPage4,FALSE);

	XEle_SetUserData(hDotR,(int)hPage5);
	XEle_ShowEle(hPage5,FALSE);

	XEle_SetUserData(hDotL,(int)hPage1);
		 
	hCurSelTab=hTab1; 

	XWnd_EnableMaxButton(hWindow,FALSE);	 
	XWnd_ShowWindow(hWindow,SW_SHOW); 
	XXmlRes_Destroy(hRes);  

	int count = RefreshAdapter();

	hComboBox = XComboBox_Create(20,158,280,30,hWindow);
	XComboBox_EnableEdit(hComboBox,false);  
	XComboBox_SetListHeight(hComboBox,20*count+7); 
	XComboBox_SetItemHeight(hComboBox,20);
			
	XEle_SetFont(hComboBox,hFontX);

	for(int i=0;i<count;++i)
	{
		wchar_t *item = new wchar_t[41];
		wcsncpy(item,(wchar_t *)m_pAdapters[i].Description.AllocSysString(),40);
		item[40]=L'\0';
		XComboBox_AddString(hComboBox,item);
	}

	XComboBox_SetSelectItem(hComboBox,0);  
	XEle_SetBkTransparent(hComboBox,1);
	XEle_SetBkTransparent(XEle_GetChildByIndex(hComboBox,0),1);
	XEle_SetTextColor(hComboBox,color);
	XEdit_SetText(hEditMacOrig, m_pAdapters[0].MAC.AllocSysString());
	XCGUI_RegEleEvent(hComboBox,XE_COMBOBOX_SELECT,&Window::OnMacAddressSelected);
	XCGUI_RegEleEvent(hComboBox,XE_COMBOBOX_POPUP_LIST,&Window::OnMacAddressPopup);
	XEle_ShowEle(hComboBox,FALSE);

}

BOOL Window::OnInput(HELE hEle,HELE hEleEvent)
{ 
	return TRUE;
}

void Window::Update()
{
	wchar_t *name = new wchar_t[256];
	wsprintf(name,L"%s",Utils::getRegistry(L"username"));
	wchar_t *pswd = Utils::getRegistry(L"password");
 
	if(wcslen(name)>0&&wcslen(pswd)>0)
	{
		XEdit_SetText(hEditSsid,name);
		XEdit_SetText(hEditPsw,pswd);
	} 
 
	ipv4= wcscmp(Utils::getRegistry(L"ipv4"),L"1")?true:false;
	ipv6= wcscmp(Utils::getRegistry(L"ipv6"),L"1")?true:false;			 
   
	Utils::getRegistry(L"ipv4");
		
	wchar_t *wifiname=Utils::getRegistry(L"wifiname");
	wchar_t *wifipswd=Utils::getRegistry(L"wifipswd");

	if(wcslen(wifiname)>0)
	{
		XEdit_SetText(hSSID,wifiname);
		XEdit_SetText(hPSW,wifipswd);
	}
	else 
	{
		if(wcslen(GetEditText(hEditSsid))>0)
		{
			wifiname = GetEditText(hEditSsid);
			XEdit_SetText(hSSID,wifiname);			 
		}
		else 
		{
			DWORD n = sizeof(wifiname)*wcslen(wifiname);
			GetComputerName(wifiname,&n);
			XEdit_SetText(hSSID,wifiname);		
		}
	}
		
	autowifi     =	wcscmp(Utils::getRegistry(L"autowifi"),L"1")==0?true:false;
	showpassword =	wcscmp(Utils::getRegistry(L"showpassword"),L"1")==0?true:false;
	autostartup  =	wcscmp(Utils::getRegistry(L"autostartup"),L"1")==0?true:false;
	autologout    =	wcscmp(Utils::getRegistry(L"autologout"),L"1")==0?true:false;
	 
	XBtn_SetCheck(hAuto,autowifi);
	XBtn_SetCheck(hRem,showpassword);
	XBtn_SetCheck(hAutoStartup,autostartup);
	XBtn_SetCheck(hAutoLogin,autologout);

	if(autostartup)
	{ 
		Utils::addStartup();
	}
	else 
	{
		Utils::delStartup();
	}

	if(showpassword)
	{
		XEdit_EnablePassBox(hPSW,false);
	}
	else 
	{
		XEdit_EnablePassBox(hPSW,true);
	}

	if(autologout)
	{
		//netgateway->Login(GetEditText(hEditSsid),GetEditText(hEditPsw),0);  
		 
		//DotPageSwitch(FALSE);
		 
		//netgateway->getUserInfo(); 
	}

	if(autowifi)
	{ 
		if(!XEdit_IsEmpty(hSSID) )
		{
			if(!isWifiOn)
			{
				netshare->setUser(GetEditText(hSSID),GetEditText(hPSW));
				if(netshare->share()==0)
				{
					XBtn_SetText(hBtStartWifi,L"热点己启动");
					XBtn_SetImageLeave(hBtStartWifi,XImage_LoadFile(Utils::makePath(L"/image/b5.png"),1));
			
					isWifiOn = true;
				}
			}
		} 
	} 
}

void Window::OnRunOnce()
{
	wchar_t *runonce = Utils::getRegistry(L"runonce");

	if( runonce == NULL || wcscmp(runonce,L"0")==0 || wcslen(runonce)==0)  
	{
		brunonce = true;

		Utils::setRegistry(L"runonce",L"1");

		Utils::setRegistry(L"ipv4",L"1");
		Utils::setRegistry(L"ipv6",L"1");
		Utils::setRegistry(L"username",L"ISNULL");
		Utils::setRegistry(L"password",L"ISNULL");
		Utils::setRegistry(L"wifiname",L"ISNULL");
		Utils::setRegistry(L"wifipswd",L"ISNULL");
		Utils::setRegistry(L"autowifi",L"ISNULL");
		Utils::setRegistry(L"showpassword",L"0");
		Utils::setRegistry(L"autostartup",L"1");
		Utils::setRegistry(L"autologin",L"0");
		Utils::setRegistry(L"autologout",L"0");
		Utils::setRegistry(L"location",Utils::getPath());
	} 

	Update(); 
}

int Window::RefreshAdapter()
{
	CNetworkAdapterList nal;
	
	if(nal.IsValid())
	{
		int count = nal.GetCount();
		if(count>0)
		{
			m_pAdapters = new ADAPTERINFO[count];
			nal.GetAdapters(m_pAdapters); 
		} 
		return count;
	}
	return 0;
}


void Window::Run()
{ 
	CreateMainUI();
  
	MessageBox(NULL,L"",L"",0);

	network = new Network();	
	update = new NetUpdate();
	netshare = new NetShare();
	netgateway = new NetGateway();
	netwallover = new NetWallover();
	 
	MessageBox(NULL,L"",L"",0);

	OnRunOnce();
  
	MessageBox(NULL,L"",L"",0);

	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA); 
	tnd.hWnd=XWnd_GetHWnd(hWindow); 
	tnd.uID=IDI_ICON2; 
	tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	tnd.uCallbackMessage=WM_USER_NOTIFYICON; 
	tnd.hIcon=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON2)); 
	XCGUI_RegWndMessage(hWindow,WM_USER_NOTIFYICON,&Window::OnNotify);
	wcscpy(tnd.szTip,L"Webbox正在悄悄地运行"); 
	Shell_NotifyIcon(NIM_ADD,&tnd); 
   
	MessageBox(NULL,L"",L"",0);

	XRunXCGUI(); 
}

BOOL Window::OnNotify(HWINDOW hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT p;
	::GetCursorPos(&p);
	
	 hMenu=XMenu_Create();
	 XMenu_AddItem(hMenu,1,L"访问官网");
	 XMenu_AddItem(hMenu,2,L"技术支持");	 
	 XMenu_AddItem(hMenu,3,L"关于");
	 XMenu_AddItem(hMenu,4,L"退出");

	if(lParam==WM_LBUTTONDOWN)
	{
		RECT rec;
		::SetForegroundWindow(::GetDesktopWindow());
		XMenu_CloseMenu(hMenu);
		XWnd_GetWindowRect(hWindow,&rec);
		XWnd_SetWindowPos(hWindow,HWND_TOP,rec.left,rec.top,rec.right-rec.left,rec.bottom-rec.top,SWP_SHOWWINDOW);
		XWnd_ShowWindow(hWindow,SW_SHOW);	
	}
	else if(lParam==WM_RBUTTONDOWN)
	{   
		XMenu_CloseMenu(hMenu);
	}
	else if(lParam==WM_RBUTTONUP)
	{
		::SetForegroundWindow(XWnd_GetHWnd(hWindow));
		XMenu_Popup(hMenu,XWnd_GetHWnd(hWindow),p.x,p.y-12);
	}

	return false;
}

BOOL Window::OnMacAddressSelected(HELE hEle,HELE hEventEle,int id)
{
	if(id>-1)
    {
		XEdit_SetText(hEditMacOrig, m_pAdapters[id].MAC.AllocSysString());
    }
    return true; 
}

BOOL Window::OnMacAddressPopup(HELE hEle,HELE hEventEle,HWINDOW hWindow,HELE hListBox)   
{  
	return TRUE;
} 

