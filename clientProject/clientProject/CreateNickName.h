#pragma once
#define SERVERPORT 9000
#define SERVERIP   "122.34.48.193"
#define BUFSIZE    512
#include <list> 
#include <atlstr.h>
#include <iostream>
// CreateNickName ��ȭ �����Դϴ�.

struct EveryThingSend {	// �޼��� ����ü
	int type;
	char name[40];
	char message[500];
	char NickNames[50][BUFSIZE];
};


class CreateNickName : public CDialogEx
{
	DECLARE_DYNAMIC(CreateNickName)

public:
	CreateNickName(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CreateNickName();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	void OnButton1Off();
	SOCKET sock;
	EveryThingSend status;
	CButton EnterChat;



};
