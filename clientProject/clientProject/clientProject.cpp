
// clientProject.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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


// CclientProjectApp ����

CclientProjectApp::CclientProjectApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CclientProjectApp ��ü�Դϴ�.

CclientProjectApp theApp;


// CclientProjectApp �ʱ�ȭ

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

