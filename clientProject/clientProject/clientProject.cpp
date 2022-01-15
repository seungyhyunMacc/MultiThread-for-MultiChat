
// clientProject.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "clientProject.h"
#include "clientProjectDlg.h"
#include "CreateNickName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclientProjectApp

BEGIN_MESSAGE_MAP(CclientProjectApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CclientProjectApp 생성

CclientProjectApp::CclientProjectApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CclientProjectApp 개체입니다.

CclientProjectApp theApp;


// CclientProjectApp 초기화

BOOL CclientProjectApp::InitInstance()
{
	CWinApp::InitInstance();
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	CreateNickName dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	WSACleanup();
	return FALSE;
}

