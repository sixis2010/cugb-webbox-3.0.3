#pragma once

#include "resource.h"
#include "stdafx.h"
#include "NetGateway.h"
#include "NetShare.h"
#include "NetWallover.h"
#include "NetUpdate.h"
#include "NetworkAdapter.h"

#define IDB_TAB1		101
#define IDB_TAB2		102
#define IDB_TAB3		103
#define IDB_TAB4		104
#define IDB_TAB5		105
 
#define ID_PAGE1		3001
#define ID_PAGE2		3002
#define ID_PAGE3		3003
#define ID_PAGE4		3004
#define ID_PAGE5		3005  

#define ID_BT_LOGIN		4003
#define ID_BT_LOGOUT	4004 
#define ID_EDIT_SID		4006
#define ID_EDIT_PSWD	4007
#define ID_BT_START		4009
#define ID_BT_DEL		4010
#define ID_BT_WALLOVER	4011
#define ID_AUTOLOGIN	4015
#define ID_REM			4016
#define	ID_IPV6			4014
#define	ID_IPV4			4013
#define ID_STATIC2		4012
#define ID_STATIC5		4017 
#define ID_STATIC6		4018 
#define ID_STATIC7		4019         
#define ID_DOT_L		4020 
#define ID_DOT_R		4021 
#define ID_BT_SVC		4022 

#define ID_SSID			5013
#define ID_PSW			5014
#define ID_AUTOSTARTUP  5015
#define ID_STATIC8      5016
#define ID_STATIC9      5017
#define ID_AUTOLOGIN2   5018
#define ID_BT_RESTORE   5019 
#define ID_STATIC10     5020 
#define ID_BT_MODIFY    5021
#define ID_BT_RESET     5022
#define ID_EDIT_ORIG    5023
#define ID_EDIT_NEW     5024
#define ID_EDIT_D         5025
#define ID_EDIT_H         5026
#define ID_EDIT_M         5027
#define ID_STATICD         5028
#define ID_STATICH         5029
#define ID_STATICM         5030
#define ID_BT_STYLE         5031
#define ID_BT_SHUTDOWN         5032 
#define ID_BT_CANCEL         5033 
#define ID_COPYRIGHT         5034 
#define ID_RIB          5035
#define ID_USERINFO     5555  

class Window : public CXEventMsg
{
public:
	Window();
	virtual ~Window();

public:
	void CreateMainUI();
	void CreateSkin();
    void Message(wchar_t *text,wchar_t *caption=L"");
	void UpdateUserInfo(wchar_t *text);
	void Update();
	void Run();
 
	static Window &GetSharedInstance()
	{
		static Window window;
		return window;
	}

protected:

	BOOL   OnEventListViewSelect(HELE hEle,HELE hEventEle,int groupIndex,int itemIndex);
	BOOL   OnWndKillFocus(HWINDOW hwnd);
	BOOL   OnMyEventTabClick(HELE hEle,HELE hEventEle) ;
	TCHAR *GetEditText(HELE hEle);
	BOOL   OnStartWifi(HELE hEle,HELE hEventEle) ;
	BOOL   OnWallover(HELE hEle,HELE hEleEvent) ;
	void   GetAllElements();

	BOOL   OnEventBtnClick_Close(HELE hEle,HELE hEleEvent) ;
	BOOL   OnEventBtnClick_Minimize(HELE hEle,HELE hEleEvent) ;
	BOOL   OnDotL(HELE hEle,HELE hEleEvent) ;
	BOOL   OnDotR(HELE hEle,HELE hEleEvent) ;
	
	BOOL   OnMacAddressSelected(HELE hEle,HELE hEventEle,int id);
	BOOL   OnMacAddressPopup(HELE hEle,HELE hEventEle,HWINDOW hWindow,HELE hListBox);
	BOOL   OnEventBtnClick_ChangeSkin(HELE hEle,HELE hEleEvent);
	BOOL   OnMsgOk(HELE hEle,HELE hEleEvent);
	BOOL   OnNotify(HWINDOW hWnd,WPARAM wParam,LPARAM lParam);
	BOOL   OnBtMacChange(HELE hEle,HELE hEleEvent);
	BOOL   OnBtMacReset(HELE hEle,HELE hEleEvent);
	BOOL   OnBtShutdown(HELE hEle,HELE hEleEvent);
	BOOL   OnBtCancel(HELE hEle,HELE hEleEvent);
	BOOL   OnBtStyle(HELE hEle,HELE hEleEvent);
	BOOL   OnBtRestore(HELE hEle,HELE hEleEvent);
	void   OnRunOnce();

	void   initCheck(HELE hEle);
	int    RefreshAdapter();
	void   ResetColor();
	BOOL   OnMenu(HELE hEle,UINT flags,POINT *pPt);
	BOOL   OnMenuSelect(HWINDOW hWindow,int id);

	BOOL   OnCheckIPV4(HELE hEle,HELE hEleEvent);
	BOOL   OnCheckIPV6(HELE hEle,HELE hEleEvent);
	BOOL   OnCheckAutoWifi(HELE hEle,HELE hEleEvent);
	BOOL   OnCheckShowPsw(HELE hEle,HELE hEleEvent);
	BOOL   OnCheckAutoLogin(HELE hEle,HELE hEleEvent);
	BOOL   OnCheckAutoStartup(HELE hEle,HELE hEleEvent);
	
	BOOL   OnLogin(HELE hEle,HELE hEleEvent);
	BOOL   OnLogout(HELE hEle,HELE hEleEvent);
	BOOL   OnService(HELE hEle,HELE hEleEvent);
	BOOL   OnInput(HELE hEle,HELE hEleEvent);
	void   OnUpdate();
	void   DotPageSwitch(bool isLeft);
	void   HideDotPage();
	bool   VerifyMacID(CString strMac);
public:
 
	bool  ipv4;
	bool  ipv6; 
 	bool  autowifi ;
	bool  showpassword ;
	bool  autostartup ;
	bool  autologin ;
	bool  autologout;
	bool  brunonce;

	bool  isWifiOn;

	wchar_t *path_default_img;  

	HIMAGE  hThemeBackground;   
	HIMAGE  hThemeBorder; 
	HIMAGE  himg;
	HELE	hPicLogo;
	int     SkinIndex;    

	HELE    hStatic1,hStatic2,hStatic3,hStatic4;
	HELE    hStaticD,hStaticH,hStaticM;
	HELE    hEditD,hEditH,hEditM;
	HELE    hBtShutdown,hBtCancel,hBtStyle,hBtRestore;;

	HIMAGE  hImage_check_leave;
	HIMAGE  hImage_check_stay;
	HIMAGE  hImage_check_down;
	
	HFONT   hFont;
	HELE    hIpv4,hIpv6,hAuto,hRem;
	HMENUX  hMenu;
	HELE    hBottomText;
	int     bottomText_width;

	HELE    hBtnClose;
	HELE    hBtnMax;
	HELE    hBtnMin;
	HELE    hBtnMenu;
	HELE    hBtnSkin;
	
	HELE    hStatic10;
	HELE    hBtModify;
	HELE    hBtReset;
	HELE    hEditMacOrig;
	HELE    hEditMacNew;
	HELE    hAutoStartup;
	HELE    hStatic8;
	HELE    hStatic9;
	HELE    hAutoLogin;
	HELE    hBtOK;
	HELE    hDotL,hDotR;
	HELE    hBtLogout,hBtSvc;
	HELE	hComboBox;
	HELE    hTab1,hTab2,hTab3,hTab4,hPage1,hPage2,hPage3,hPage4,hPage5;
	HELE    hBtStartWifi,hBtDelWifi ,hEditSsid,hEditPsw,hSSID,hPSW,hWallover,hBtLogin;
	HELE    hListView;
	HELE	hCurSelTab ; 
	HELE    hRib;

	HELE    hHead ;		 
	HELE    hHead2 ;  
	HELE    hUserinfo;

	HWINDOW hWindow,hSkinWindow,hMsgWindow;

	HFONTX  hFontX;
	HFONTX  hFontEdit; 
	HFONTX  hFont14; 
	HFONTX  hFontH;
	HFONTX  hFontS;
	
	HELE    hCopyright; 

public:
	
	NetUpdate *update;
	Network    *network;
	NetShare   *netshare; 
	NetGateway *netgateway;
	NetWallover *netwallover;
	ADAPTERINFO* m_pAdapters;
};