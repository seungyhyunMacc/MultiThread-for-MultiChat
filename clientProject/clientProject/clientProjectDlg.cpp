// clientProjectDlg.cpp : ���� ����

#include "stdafx.h"
#include "clientProject.h"
#include "clientProjectDlg.h"
#include "afxdialogex.h"
#include "CreateNickName.h"

#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int checkchatlength(const char *chat_string) {
	int count = 0;
	while (*chat_string) {
		if (*chat_string >> 7)
			chat_string++;
		chat_string++;
		count++;
	}
	return count;
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
void err_quitt(char *msg)
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
void err_displayy(char *msg)
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

void CclientProjectDlg::ReceiveMessage()
{

	EveryThingSend status;	// �޼��� ����ü
	int retval;
	int len;
	CString WriteMsg;	// MFC ä�ÿ� �̿�
	client_sock = mParent->sock;

	while (1) {
		retval = recv(client_sock, (char*)&status, sizeof(status), 0);	// server���� �޼��� ����
		if (retval == SOCKET_ERROR) {
			err_displayy("recv()");
			break;
		}
		else if (retval == 0)
			break;

		if (status.type == 2) {		// status.type�� 2�� ���������� �����ߴٴ� �� 
			WriteMsg = (LPCSTR)(LPSTR)status.message;
			AddEventString(WriteMsg);		// [~]���� �����߽��ϴ� ���
			chatuser_list.ResetContent();	// �������ִ� ��������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �ֽ�ȭ
			userlist.clear();				// ��������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �ֽ�ȭ

			for (int i = 0; i< 50; i++) {	// ��������Ʈ�� �г����� ������� 
				if (status.NickNames[i][0] == '\0')	// �г����� �� ����־����� �ǹ�
					break;
				userlist.push_back(status.NickNames[i]);
				WriteMsg = (LPCSTR)(LPSTR)status.NickNames[i];
				AddUserListString(WriteMsg);	
			}

		}

		if (status.type == 3) {		// status.type�� 3�̸� �����ϰ� ä���� �ϰ� �ִ� ���� ����
			char msg[500];
			sprintf(msg, "[%s]: %s", status.name, status.message);	// [~]:~~~ �г��Ӱ� ä�ñ��� ���� ����ϰ� �;� sprintf���
			int isBlocked = 0;

			for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {	// ������ �������� �ƴ��� �Ǵ�
				if (strcmp(status.name, *blocked) == 0) {	// ���ܵ� ����Ʈ�� ������ 
					isBlocked = 1;	
					break;
				}
			}

			if (!isBlocked) {	// ���ܵ� ����Ʈ�� ���� ��
				if (strcmp(status.message, "!�λ�") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�ȳ��ϼ��� (�ޡԣ�)/");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!����") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�޾ƶ� ��������-!!��( @��@)< =====@");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!�ְ�") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�ְ��Դϴ�! d=(�����)");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!����") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�� ���� ������ �� (��v��)v");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!�ٽ�") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�� �� ������ϴ�? (�� �� ��)?");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!�Ϸ�") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "���õ� ���� �Ϸ�Ǽ��� o(OvO)o ��");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else if (strcmp(status.message, "!����") == 0) {	//ä�ø�ũ��
					sprintf(msg, "[%s]: %s", status.name, "�̸� �����ڽ��ϴ� (_ _)");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
				else {
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring ��ȯ �� 
					AddEventString(WriteMsg);		// ä��â�� ���
				}
			}
		}

		if (status.type == 4) {		// status.type�� 4�̸� ������ ���´ٴ� ��
			WriteMsg = (LPCSTR)(LPSTR)status.message;	
			AddEventString(WriteMsg);		// [~]���� �����߽��ϴ� ���
			chatuser_list.ResetContent();	// �������ִ� ��������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �ֽ�ȭ
			userlist.clear();				// ��������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �ֽ�ȭ

			for (int i = 0; i < 50; i++) {
				if (status.NickNames[i][0] == '\0')
					break;
				userlist.push_back(status.NickNames[i]);
				WriteMsg = (LPCSTR)(LPSTR)status.NickNames[i];
				AddUserListString(WriteMsg);
			}
		}
	}

	// closesocket()
	closesocket(client_sock);

}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CclientProjectDlg ��ȭ ����



CclientProjectDlg::CclientProjectDlg(CWnd* pParent /*=NULL*/)	// ������
	: CDialogEx(IDD_CLIENTPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CclientProjectDlg::~CclientProjectDlg() {		// �Ҹ���
	
}

void CclientProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_LIST1, chat_list);	// ä��â
	DDX_Control(pDX, IDC_CHAT_EDIT1, chat_edit);	// ä�� ���� ĭ
	DDX_Control(pDX, IDC_SEND_BUTTON1, send_btn);	// ���۹�ư
	DDX_Control(pDX, IDC_CHATUSER_LIST1, chatuser_list);	// ä�ÿ� �������ִ� ��������Ʈ
	DDX_Control(pDX, IDC_EDIT1, blockuser);
	DDX_Control(pDX, IDC_Mecro_BUTTON4, ChatMecro);
	DDX_Control(pDX, IDC_Introduce_BUTTON5, Introduce);
}
BEGIN_MESSAGE_MAP(CclientProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_CHAT_LIST1, &CclientProjectDlg::OnLbnSelchangeChatlist)
	ON_BN_CLICKED(IDC_SEND_BUTTON1, &CclientProjectDlg::OnBnClickedSendButton1)
	ON_EN_CHANGE(IDC_CHAT_EDIT1, &CclientProjectDlg::OnEnChangeChatEdit1)
	ON_LBN_SELCHANGE(IDC_CHATUSER_LIST1, &CclientProjectDlg::OnLbnSelchangeChatuserList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CclientProjectDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CclientProjectDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CclientProjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CclientProjectDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_Mecro_BUTTON4, &CclientProjectDlg::OnBnClickedMecroButton4)
	ON_BN_CLICKED(IDC_Introduce_BUTTON5, &CclientProjectDlg::OnBnClickedIntroduceButton5)
END_MESSAGE_MAP()


// CclientProjectDlg �޽��� ó����

BOOL CclientProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.


	mParent = (CreateNickName *)::AfxGetMainWnd();	// �г����� �θ�, �ڽ� ����ϴ� ������ �������� ���� ���
	
	int len;
	int retval;
	std::thread create_thread(&CclientProjectDlg::ReceiveMessage, this);	// server���� �޼����� �޴� �����带 ����
	create_thread.detach();	// �����带 ���������� ���డ���ϰ� 
	
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Sleep(300);
	EveryThingSend status;	// �޼��� ����ü
	status.type = 2;	// ä�ù濡 �����ߴٰ� type = 2�� server�� ����
	strcpy(status.name, mParent->status.name);	//[~]���� �����߽��ϴٸ� ���������� ���� �г����� ����ü�� �ְ� ���� ����
	retval=send(client_sock, (char*)&status, sizeof(status), 0);	// �������� ���� 

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CclientProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CclientProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CclientProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CclientProjectDlg::OnLbnSelchangeChatlist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


void CclientProjectDlg::OnEnChangeChatEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CclientProjectDlg::OnBnClickedSendButton1()	// ���۹�ư
{
	EveryThingSend status;	// �޼��� ����ü
	CString WriteMsg;		// ä�� ��
	int retval;
	int checkspace = 0;
	GetDlgItemText(IDC_CHAT_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {
		if (WriteMsg.IsEmpty()) {	// �����ϴ� �г���â�� ������ �ִ��� Ȯ�� 
			checkspace = 1;
			break;
		}
		if (WriteMsg.Find(" ") == 0) {
			checkspace = 1;
			break;
		}
		if (checkchatlength(WriteMsg) > 40) {
			checkspace = 2;
			break;
		}
		
	}

	if (checkspace == 1) {
		MessageBox("���� ���ּžߵǿ�!!");
	}
	else if (checkspace == 2) {
		int n = checkchatlength(WriteMsg) - 40;
		CString strText;
		strText.Format(_T("%d"), n);
		MessageBox(strText + "�ڰ� �ʰ��Ǿ����!\n�ٽý��ּ���!");
	}
	else {
		SetDlgItemText(IDC_CHAT_EDIT1, "");
		strcpy(status.message, WriteMsg.GetBuffer(0));	// message�� WriteMsg�� �����Ѵ�.
		strcpy(status.name, mParent->status.name);	// �θ����� �ִ� status�� �̸��� �����´�.
		status.type = 3;	// ä�ù濡 ���������� �����Ͽ� ���ڸ� ������ ���̹Ƿ� type=3�� ������
		retval = send(client_sock, (char*)&status, sizeof(status), 0);	// server���� ����
	}
}

void CclientProjectDlg::AddEventString(CString str) {	// ä��â
	while (chat_list.GetCount() > 300) {	// ä�� 300���� �Ѿ��
		chat_list.DeleteString(0);			// ������ ä�ñۺ��� �����Ѵ�
	}
	// �ֱ� ���� ���ڿ��� ������ �߰��� �� �ְ� 
	int index = chat_list.InsertString(-1, str);
	chat_list.SetCurSel(index);
}

void CclientProjectDlg::AddUserListString(CString str) {	// ä�ù濡 ������ ��������Ʈ
	while (chatuser_list.GetCount() > 300) {
		chatuser_list.DeleteString(0);
	}
	int index = chatuser_list.InsertString(-1, str);
	chatuser_list.SetCurSel(index);
}

void CclientProjectDlg::OnLbnSelchangeChatuserList1()
{
	

}


void CclientProjectDlg::OnBnClickedButton1()	// �����ϱ�
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int blockcheck = 0;
	CString WriteMsg;
	int checkspace = 0;

	GetDlgItemText(IDC_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {
		if (WriteMsg.IsEmpty()) {	// �����ϴ� �г���â�� ������ �ִ��� Ȯ�� 
			checkspace = 1;
			break;
		}
		if (WriteMsg.Find(" ") == 0) {
			checkspace = 1;
			break;
		}
	}
	if (checkspace == 1) {
		MessageBox("�г��� �տ� ������ �ֽ��ϴ�! �ٽ� ���ּ���!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else {
		if (strcmp(mParent->status.name, WriteMsg) == 0) {	// �ڱ� �ڽ����ܺҰ�
			MessageBox("�ڱ��ڽ��� �����ϴ� ���� �Ұ��մϴ�!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}
		else {
			for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {
				if (strcmp(WriteMsg, *blocked) == 0) {	// ������������Ʈ�� ��
					MessageBox("���ܸ���Ʈ�� �����մϴ�!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
					blockcheck = 2;
					break;
				}
			}
			if (blockcheck != 2) {	// ������������Ʈ�� ���� ���
				for (exist = userlist.begin(); exist != userlist.end(); exist++) {	// ���� ���� �����ϴ� �г������� Ȯ��
					if (strcmp(WriteMsg, *exist) == 0) {
						blockuserlist.push_back(WriteMsg);
						MessageBox(WriteMsg + "���� ���ܿϷ�Ǿ����ϴ�!");
						SetDlgItemText(IDC_NAME_EDIT1, "");
						blockcheck = 1;
						break;
					}
				}
				if (blockcheck == 0){
					MessageBox("���� �����Դϴ�.");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
			}
		}
	}
}


void CclientProjectDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CclientProjectDlg::OnBnClickedButton2()	// ����Ǯ��
{
	int checkspace = 0;
	int blockcheck = 0;
	CString WriteMsg;
	GetDlgItemText(IDC_EDIT1, WriteMsg);

	for (int i = 0; i < sizeof(WriteMsg); i++) {
		if (WriteMsg.IsEmpty()) {
			checkspace = 1;
			break;
		}
		if (WriteMsg.Find(" ") == 0) {
			checkspace = 1;
			break;
		}
	}

	if (checkspace == 1) {
		MessageBox("�г��� �տ� ������ �ֽ��ϴ�! �ٽ� ���ּ���!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else {
		if (strcmp(mParent->status.name, WriteMsg) == 0) {
			MessageBox("�ڱ��ڽ��� ���������ϴ� ���� �Ұ��մϴ�!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}
		else {
			blocked = blockuserlist.begin();
			while (blocked != blockuserlist.end()) {
				if (*blocked == WriteMsg) {
					blockuserlist.erase(blocked++);
					MessageBox(WriteMsg + "���� ���������Ǿ����ϴ�!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
				else {
					++blocked;
					MessageBox("���� �����Դϴ�!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
			}
		}
	}
}


void CclientProjectDlg::OnBnClickedButton3()	// ������������Ʈ
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString WriteMsg;
	int index = 0;
	for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {
		WriteMsg += *blocked;
		WriteMsg += '\n';
	}
	MessageBox("ä�����ܸ���Ʈ\n\n"+WriteMsg);
}


void CclientProjectDlg::OnBnClickedMecroButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	MessageBox("ĢĢ���常�� ä�ø�ũ�θ� ����غ�����!!\n�Ʒ� ��ɾ ġ�ø�˴ϴ�!!\n\n!�λ� = �ȳ��ϼ��� (�ޡԣ�)/\n\n!���� = �޾ƶ� ��������-!!��( @��@)< =====@\n\n!�ְ� = �ְ��Դϴ�! d=(�����)\n\n!���� = �� ���� ������ �� (��v��)v\n\n!�ٽ� = �� �� ������ϴ�? (�� �� ��)?\n\n!�Ϸ� = ���õ� ���� �Ϸ�Ǽ��� o(OvO)o ��\n\n!���� = �̸� �����ڽ��ϴ� (_ _)");
}


void CclientProjectDlg::OnBnClickedIntroduceButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	MessageBox("�ȳ��ϼ��� ĢĢ������ ���� �����ڵ��Դϴ�.\n������ �Ƕ������� �� ������� ������ ������ ä�����α׷��Դϴ�.\n\n��հ� �̿����ֽð� ��ſ��Ϸ纸������!!");
}
