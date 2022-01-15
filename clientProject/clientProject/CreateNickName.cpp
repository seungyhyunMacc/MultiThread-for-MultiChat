
// CreateNickName.cpp : ���� �����Դϴ�.
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

// CreateNickName ��ȭ �����Դϴ�.
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

// ���� �Լ� ���� ���
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

CreateNickName::CreateNickName(CWnd* pParent /*=NULL*/)	// ������
	: CDialogEx(IDD_DIALOG1, pParent)
{
	int retval;
	sock = socket(AF_INET, SOCK_STREAM, 0);	// ������ �����ϱ� ���� ����(�г��� â������ ���)
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));	// ������ ����
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


// CreateNickName �޽��� ó�����Դϴ�.


void CreateNickName::OnBnClickedButton2()	// ä������
{
	int retval;
	if (EnterSignal == 1) {	// �г��� �ߺ��� Ȯ���ߴٴ� ǥ��

		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);	// �г���â ����
		CclientProjectDlg enterchatdlg;	// ä�ù����� ����
		enterchatdlg.DoModal();
		enterchatdlg.DestroyWindow();
		exit(0);
	}
	
}

void CreateNickName::OnBnClickedButton1()	// �ߺ�üũ
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int checkspace = 0;
	CString WriteMsg;	// �г���
	int retval;
	status.type = 0;	// �г��� â�� ����
	GetDlgItemText(IDC_NAME_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {	// �����̰ų� �г��� �տ� ������ ���� ��츦 �����ϱ� ���ؼ�
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
		MessageBox("�г��� �տ� ������ ������ �ʽ��ϴ�!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else if (checkspace == 2) {
		MessageBox("�г����� 10�� �̳��� �ٿ��ּ���!");
	}
	else {
		strcpy(status.name, WriteMsg.GetBuffer(0));
		retval = send(sock, (char*)&status, sizeof(status), 0);	// �г��� �ߺ� Ȯ���� ���� server�� ����
		if (retval == SOCKET_ERROR) err_quit("send()");


		retval = recv(sock, (char*)&status, sizeof(status), 0);	// server���� �ߺ����� �ƴ��� ������� ����

		if (status.type == -1) {	// status.type�� -1�̸� �ߺ�
			MessageBox("�г����� �ߺ��Դϴ�. �ٽ� �Է��ϼ���!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}

		if (status.type == 1) {	// status.type�� 1�̸� �ߺ�X
			MessageBox("�г��� ��밡���մϴ�!");
			EnterSignal = 1;	//ä�ù� ���� Ŭ�� �̺�Ʈ�� ��������
			GetDlgItem(IDC_CHECK_BTN1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		}
	}
}