
// CreateNickName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "clientProject.h"
#include "CreateNickName.h"
#include "clientProjectDlg.h"
#include "afxdialogex.h"

int EnterSignal = 0;

int checklength(const char *ap_string) {
	int count = 0;
	while (*ap_string) {
		if (*ap_string >> 7)
			ap_string++;
		ap_string++;
		count++;
	}
	return count;
}

// CreateNickName 대화 상자입니다.
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

IMPLEMENT_DYNAMIC(CreateNickName, CDialogEx)

CreateNickName::CreateNickName(CWnd* pParent /*=NULL*/)	// 생성자
	: CDialogEx(IDD_DIALOG1, pParent)
{
	int retval;
	sock = socket(AF_INET, SOCK_STREAM, 0);	// 서버에 연결하기 위한 소켓(닉네임 창에서만 사용)
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));	// 서버에 연결
	if (retval == SOCKET_ERROR) err_quit("connect()");

}

CreateNickName::~CreateNickName()
{
}

void CreateNickName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, EnterChat);
}


BEGIN_MESSAGE_MAP(CreateNickName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CreateNickName::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CreateNickName::OnBnClickedButton1)
END_MESSAGE_MAP()


// CreateNickName 메시지 처리기입니다.


void CreateNickName::OnBnClickedButton2()	// 채팅접속
{
	int retval;
	if (EnterSignal == 1) {	// 닉네임 중복을 확인했다는 표시

		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);	// 닉네임창 끄기
		CclientProjectDlg enterchatdlg;	// 채팅방으로 입장
		enterchatdlg.DoModal();
		enterchatdlg.DestroyWindow();
		exit(0);
	}
	
}

void CreateNickName::OnBnClickedButton1()	// 중복체크
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int checkspace = 0;
	CString WriteMsg;	// 닉네임
	int retval;
	status.type = 0;	// 닉네임 창에 입장
	GetDlgItemText(IDC_NAME_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {	// 공백이거나 닉네임 앞에 공백이 있을 경우를 방지하기 위해서
		if (WriteMsg.IsEmpty()) {
			checkspace = 1;
			break;
		}
		if (WriteMsg.Find(" ")== 0) {
			checkspace = 1;
			break;
		}
		if (checklength(WriteMsg) > 10) {
			checkspace = 2;
			break;
		}
	}

	if (checkspace == 1) {
		MessageBox("닉네임 앞에 공백이 허용되지 않습니다!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else if (checkspace == 2) {
		MessageBox("닉네임을 10자 이내로 줄여주세요!");
	}
	else {
		strcpy(status.name, WriteMsg.GetBuffer(0));
		retval = send(sock, (char*)&status, sizeof(status), 0);	// 닉네임 중복 확인을 위해 server에 보냄
		if (retval == SOCKET_ERROR) err_quit("send()");


		retval = recv(sock, (char*)&status, sizeof(status), 0);	// server에서 중복인지 아닌지 결과물을 받음

		if (status.type == -1) {	// status.type이 -1이면 중복
			MessageBox("닉네임이 중복입니다. 다시 입력하세요!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}

		if (status.type == 1) {	// status.type이 1이면 중복X
			MessageBox("닉네임 사용가능합니다!");
			EnterSignal = 1;	//채팅방 입장 클릭 이벤트가 가능해짐
			GetDlgItem(IDC_CHECK_BTN1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		}
	}
}