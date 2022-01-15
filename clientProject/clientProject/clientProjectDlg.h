
// clientProjectDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#define SERVERPORT 9000
#define SERVERIP   "122.34.48.193"
#define BUFSIZE    512
#include <thread>
#include "CreateNickName.h"


// CclientProjectDlg 대화 상자
class CclientProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	void ReceiveMessage();
	CclientProjectDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	~CclientProjectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	CListBox chat_list;
	afx_msg void OnLbnSelchangeChatlist();
	afx_msg void OnEnChangeChatEdit1();
	CEdit chat_edit;
	CButton send_btn;
	SOCKET client_sock;
	afx_msg void OnBnClickedSendButton1();
	void AddEventString(CString str);
	void AddUserListString(CString srt);
	CListBox chatuser_list;
	afx_msg void OnLbnSelchangeChatuserList1();
	CreateNickName* mParent;	// mparent는 cretenickname클래스(부모)를 가리키는 포인터다
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	CEdit blockuser;
	std::list<CString> blockuserlist;
	std::list<CString>::iterator blocked;
	std::list<CString> userlist;
	std::list<CString>::iterator exist;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedMecroButton4();
	CButton ChatMecro;
	CButton Introduce;
	afx_msg void OnBnClickedIntroduceButton5();
};
