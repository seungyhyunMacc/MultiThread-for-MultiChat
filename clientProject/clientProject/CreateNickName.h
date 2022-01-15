#pragma once
#define SERVERPORT 9000
#define SERVERIP   "122.34.48.193"
#define BUFSIZE    512
#include <list> 
#include <atlstr.h>
#include <iostream>
// CreateNickName 대화 상자입니다.

struct EveryThingSend {	// 메세지 구조체
	int type;
	char name[40];
	char message[500];
	char NickNames[50][BUFSIZE];
};


class CreateNickName : public CDialogEx
{
	DECLARE_DYNAMIC(CreateNickName)

public:
	CreateNickName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CreateNickName();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	void OnButton1Off();
	SOCKET sock;
	EveryThingSend status;
	CButton EnterChat;



};
