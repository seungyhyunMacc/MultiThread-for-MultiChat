
// clientProject.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CclientProjectApp:
// �� Ŭ������ ������ ���ؼ��� clientProject.cpp�� �����Ͻʽÿ�.
//

class CclientProjectApp : public CWinApp
{
public:
	CclientProjectApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CclientProjectApp theApp;