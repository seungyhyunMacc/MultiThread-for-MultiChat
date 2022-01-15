// clientProjectDlg.cpp : 구현 파일

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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
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

// 소켓 함수 오류 출력
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

	EveryThingSend status;	// 메세지 구조체
	int retval;
	int len;
	CString WriteMsg;	// MFC 채팅에 이용
	client_sock = mParent->sock;

	while (1) {
		retval = recv(client_sock, (char*)&status, sizeof(status), 0);	// server한테 메세지 받음
		if (retval == SOCKET_ERROR) {
			err_displayy("recv()");
			break;
		}
		else if (retval == 0)
			break;

		if (status.type == 2) {		// status.type이 2는 안정적으로 접속했다는 뜻 
			WriteMsg = (LPCSTR)(LPSTR)status.message;
			AddEventString(WriteMsg);		// [~]님이 입장했습니다 출력
			chatuser_list.ResetContent();	// 접속해있는 유저리스트를 초기화하고 다시 최신화
			userlist.clear();				// 유저리스트도 초기화하고 다시 최신화

			for (int i = 0; i< 50; i++) {	// 유저리스트에 닉네임을 집어넣음 
				if (status.NickNames[i][0] == '\0')	// 닉네임을 다 집어넣었음을 의미
					break;
				userlist.push_back(status.NickNames[i]);
				WriteMsg = (LPCSTR)(LPSTR)status.NickNames[i];
				AddUserListString(WriteMsg);	
			}

		}

		if (status.type == 3) {		// status.type이 3이면 접속하고 채팅을 하고 있는 중을 뜻함
			char msg[500];
			sprintf(msg, "[%s]: %s", status.name, status.message);	// [~]:~~~ 닉네임과 채팅글을 같이 출력하고 싶어 sprintf사용
			int isBlocked = 0;

			for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {	// 차단한 유저인지 아닌지 판단
				if (strcmp(status.name, *blocked) == 0) {	// 차단된 리스트에 있을시 
					isBlocked = 1;	
					break;
				}
			}

			if (!isBlocked) {	// 차단된 리스트에 없을 시
				if (strcmp(status.message, "!인사") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "안녕하세요 (＾∇＾)/");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!공격") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "받아라 에너지파-!!「( @∇@)< =====@");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!최고") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "최고입니다! d=(´▽｀)");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!으쓱") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "이 정도 가지고 뭘 (˘v˘)v");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!다시") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "잘 못 들었습니다? (º ∀ º)?");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!하루") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "오늘도 좋은 하루되세요 o(OvO)o ♪");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else if (strcmp(status.message, "!퇴장") == 0) {	//채팅매크로
					sprintf(msg, "[%s]: %s", status.name, "이만 가보겠습니다 (_ _)");
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
				else {
					WriteMsg = (LPCSTR)(LPSTR)msg;	//Cstring 변환 후 
					AddEventString(WriteMsg);		// 채팅창에 출력
				}
			}
		}

		if (status.type == 4) {		// status.type이 4이면 접속을 끝냈다는 뜻
			WriteMsg = (LPCSTR)(LPSTR)status.message;	
			AddEventString(WriteMsg);		// [~]님이 퇴장했습니다 출력
			chatuser_list.ResetContent();	// 접속해있는 유저리스트를 초기화하고 다시 최신화
			userlist.clear();				// 유저리스트도 초기화하고 다시 최신화

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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CclientProjectDlg 대화 상자



CclientProjectDlg::CclientProjectDlg(CWnd* pParent /*=NULL*/)	// 생성자
	: CDialogEx(IDD_CLIENTPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CclientProjectDlg::~CclientProjectDlg() {		// 소멸자
	
}

void CclientProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_LIST1, chat_list);	// 채팅창
	DDX_Control(pDX, IDC_CHAT_EDIT1, chat_edit);	// 채팅 쓰는 칸
	DDX_Control(pDX, IDC_SEND_BUTTON1, send_btn);	// 전송버튼
	DDX_Control(pDX, IDC_CHATUSER_LIST1, chatuser_list);	// 채팅에 접속해있는 유저리스트
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


// CclientProjectDlg 메시지 처리기

BOOL CclientProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	mParent = (CreateNickName *)::AfxGetMainWnd();	// 닉네임을 부모, 자식 계승하는 것으로 가져오기 위해 사용
	
	int len;
	int retval;
	std::thread create_thread(&CclientProjectDlg::ReceiveMessage, this);	// server에게 메세지를 받는 스레드를 생성
	create_thread.detach();	// 스레드를 독립적으로 실행가능하게 
	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	Sleep(300);
	EveryThingSend status;	// 메세지 구조체
	status.type = 2;	// 채팅방에 입장했다고 type = 2를 server에 보냄
	strcpy(status.name, mParent->status.name);	//[~]님이 입장했습니다를 보내기위해 유저 닉네임을 구조체에 넣고 같이 보냄
	retval=send(client_sock, (char*)&status, sizeof(status), 0);	// 서버에게 전송 

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CclientProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CclientProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CclientProjectDlg::OnLbnSelchangeChatlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CclientProjectDlg::OnEnChangeChatEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CclientProjectDlg::OnBnClickedSendButton1()	// 전송버튼
{
	EveryThingSend status;	// 메세지 구조체
	CString WriteMsg;		// 채팅 글
	int retval;
	int checkspace = 0;
	GetDlgItemText(IDC_CHAT_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {
		if (WriteMsg.IsEmpty()) {	// 차단하는 닉네임창에 공백이 있는지 확인 
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
		MessageBox("뭐라도 써주셔야되요!!");
	}
	else if (checkspace == 2) {
		int n = checkchatlength(WriteMsg) - 40;
		CString strText;
		strText.Format(_T("%d"), n);
		MessageBox(strText + "자가 초과되었어요!\n다시써주세요!");
	}
	else {
		SetDlgItemText(IDC_CHAT_EDIT1, "");
		strcpy(status.message, WriteMsg.GetBuffer(0));	// message를 WriteMsg에 복사한다.
		strcpy(status.name, mParent->status.name);	// 부모한테 있는 status의 이름을 가져온다.
		status.type = 3;	// 채팅방에 안정적으로 입장하여 문자를 보내는 것이므로 type=3를 보낸다
		retval = send(client_sock, (char*)&status, sizeof(status), 0);	// server에게 전송
	}
}

void CclientProjectDlg::AddEventString(CString str) {	// 채팅창
	while (chat_list.GetCount() > 300) {	// 채팅 300줄이 넘어가면
		chat_list.DeleteString(0);			// 최초의 채팅글부터 삭제한다
	}
	// 최근 들어온 문자열이 밑으로 추가될 수 있게 
	int index = chat_list.InsertString(-1, str);
	chat_list.SetCurSel(index);
}

void CclientProjectDlg::AddUserListString(CString str) {	// 채팅방에 입장한 유저리스트
	while (chatuser_list.GetCount() > 300) {
		chatuser_list.DeleteString(0);
	}
	int index = chatuser_list.InsertString(-1, str);
	chatuser_list.SetCurSel(index);
}

void CclientProjectDlg::OnLbnSelchangeChatuserList1()
{
	

}


void CclientProjectDlg::OnBnClickedButton1()	// 차단하기
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int blockcheck = 0;
	CString WriteMsg;
	int checkspace = 0;

	GetDlgItemText(IDC_EDIT1, WriteMsg);
	for (int i = 0; i < sizeof(WriteMsg); i++) {
		if (WriteMsg.IsEmpty()) {	// 차단하는 닉네임창에 공백이 있는지 확인 
			checkspace = 1;
			break;
		}
		if (WriteMsg.Find(" ") == 0) {
			checkspace = 1;
			break;
		}
	}
	if (checkspace == 1) {
		MessageBox("닉네임 앞에 공백이 있습니다! 다시 써주세요!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else {
		if (strcmp(mParent->status.name, WriteMsg) == 0) {	// 자기 자신차단불가
			MessageBox("자기자신을 차단하는 것은 불가합니다!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}
		else {
			for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {
				if (strcmp(WriteMsg, *blocked) == 0) {	// 차단유저리스트와 비교
					MessageBox("차단리스트에 존재합니다!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
					blockcheck = 2;
					break;
				}
			}
			if (blockcheck != 2) {	// 차단유저리스트에 없는 경우
				for (exist = userlist.begin(); exist != userlist.end(); exist++) {	// 서버 내에 존재하는 닉네임인지 확인
					if (strcmp(WriteMsg, *exist) == 0) {
						blockuserlist.push_back(WriteMsg);
						MessageBox(WriteMsg + "님이 차단완료되었습니다!");
						SetDlgItemText(IDC_NAME_EDIT1, "");
						blockcheck = 1;
						break;
					}
				}
				if (blockcheck == 0){
					MessageBox("없는 유저입니다.");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
			}
		}
	}
}


void CclientProjectDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CclientProjectDlg::OnBnClickedButton2()	// 차단풀기
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
		MessageBox("닉네임 앞에 공백이 있습니다! 다시 써주세요!");
		SetDlgItemText(IDC_NAME_EDIT1, "");
	}
	else {
		if (strcmp(mParent->status.name, WriteMsg) == 0) {
			MessageBox("자기자신을 차단해제하는 것은 불가합니다!");
			SetDlgItemText(IDC_NAME_EDIT1, "");
		}
		else {
			blocked = blockuserlist.begin();
			while (blocked != blockuserlist.end()) {
				if (*blocked == WriteMsg) {
					blockuserlist.erase(blocked++);
					MessageBox(WriteMsg + "님이 차단해제되었습니다!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
				else {
					++blocked;
					MessageBox("없는 유저입니다!");
					SetDlgItemText(IDC_NAME_EDIT1, "");
				}
			}
		}
	}
}


void CclientProjectDlg::OnBnClickedButton3()	// 차단유저리스트
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString WriteMsg;
	int index = 0;
	for (blocked = blockuserlist.begin(); blocked != blockuserlist.end(); blocked++) {
		WriteMsg += *blocked;
		WriteMsg += '\n';
	}
	MessageBox("채팅차단리스트\n\n"+WriteMsg);
}


void CclientProjectDlg::OnBnClickedMecroButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox("칙칙톡톡만의 채팅매크로를 사용해보세요!!\n아래 명령어를 치시면됩니다!!\n\n!인사 = 안녕하세요 (＾∇＾)/\n\n!공격 = 받아라 에너지파-!!「( @∇@)< =====@\n\n!최고 = 최고입니다! d=(´▽｀)\n\n!으쓱 = 이 정도 가지고 뭘 (˘v˘)v\n\n!다시 = 잘 못 들었습니다? (º ∀ º)?\n\n!하루 = 오늘도 좋은 하루되세요 o(OvO)o ♪\n\n!퇴장 = 이만 가보겠습니다 (_ _)");
}


void CclientProjectDlg::OnBnClickedIntroduceButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox("안녕하세요 칙칙톡톡을 만든 개발자들입니다.\n저희의 피땀눈물이 모여 만들어진 굉장히 소중한 채팅프로그램입니다.\n\n재밌게 이용해주시고 즐거운하루보내세요!!");
}
