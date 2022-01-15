#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <list> 
#include <atlstr.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE    512
#define no_init_all

std::list<SOCKET> EnteredClient;	// 접속한 클라이언트의 주소를 저장하기 위한 리스트
std::list<SOCKET>::iterator Entered;	// 반복자
std::list<CString> NickNames;		// 접속한 닉네임을 저장하기 위한 리스트
std::list<CString>::iterator name;		// 반복자
CRITICAL_SECTION cs;
SOCKADDR_IN clientaddr;

struct EveryThingSend {	// 메세지 구조체
	int type;	// 현재 클라이언트의 상태
	char name[40];	// 닉네임
	char message[500];	// 채팅글
	char NickNames[50][BUFSIZE];	// 현재 접속해있는 닉네임들을 전송하기 위한
};


// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
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

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	EveryThingSend status;

	// 클라이언트 정보 얻기
	int addrlen2 = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen2);	// 접속한 클라이언트 소켓의 정보를 얻음


	while (1) {
		// 클라이언트에게 데이터 받기
		retval = recv(client_sock, (char*)&status, sizeof(status), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		if (status.type == 0) {	// 채팅방에 들어가기 전, 닉네임창에서 닉네임을 입력하고 중복체크 했을 때
			int boolean = 0;
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), status.name);
			for (name = NickNames.begin(); name != NickNames.end(); name++) {	// 닉네임 배열리스트와 비교
				if (strcmp(*name, status.name) == 0) {
					boolean = 1;
					break;
				}
			}
			
			if (boolean) {		// 중복일 경우
				status.type = -1;
				retval = send(client_sock, (char*)&status, sizeof(status), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
			else {		// 중복이 없을 경우
				NickNames.push_back(status.name);	// 닉네임리스트에 닉네임 추가
				status.type = 1;
				retval = send(client_sock, (char*)&status, sizeof(status), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}
		
		if (status.type == 2) {	// 채팅방에 입장 했을 때
			EnteredClient.push_back(client_sock);	// 클라이언트 리스트에 추가(채팅방에 접속한 유저) 
			sprintf(status.message, "[%s]님이 입장했습니다.", status.name);

			for (int i = 0; i < 50; i++) {
				status.NickNames[i][0] = '\0';
			}

			int index = 0;
			for (name = NickNames.begin(); name != NickNames.end(); name++) {	// 접속한 닉네임을 구조체 안에 있는 닉네임배열에 복사
				strcpy(status.NickNames[index++], *name);
			}

			for (Entered=EnteredClient.begin(); Entered!=EnteredClient.end(); Entered++) {
				retval = send(*Entered, (char*)&status, sizeof(status), 0);	// 접속한 클라이언트들에게 채팅글을 보냄
			}

		}
		

		if (status.type == 3) {	// 채팅방에 입장 후 채팅을 치고있을 때
			for (Entered = EnteredClient.begin(); Entered != EnteredClient.end(); Entered++) {
				retval = send(*Entered, (char*)&status, sizeof(status), 0);	// 접속한 클라이언트들에게 채팅글을 보냄 
			}
		}
		
		
	}

	// 프로그램 종료 시
	name = NickNames.begin();	// 닉네임배열에서 해당 닉네임을 삭제
	while (name != NickNames.end()) {
		if (*name == status.name)
			NickNames.erase(name++);
		else
			++name;
	}

	if (status.type==2 || status.type==3) {	// 프로그램에 접속하고 종료한 클라이언트를 삭제
		Entered = EnteredClient.begin();
		while (Entered != EnteredClient.end()) {
			if (*Entered == client_sock)
				EnteredClient.erase(Entered++);
			else
				++Entered;
		}

		status.type = 4;	// 나갔을 때 클라이언트들에게 퇴장했다고 알림 
		sprintf(status.message, "[%s]님이 퇴장했습니다.", status.name); 
		for (int i = 0; i < 50; i++) {
			status.NickNames[i][0] = '\0';
		}
		int index = 0;
		for (name = NickNames.begin(); name != NickNames.end(); name++) {
			strcpy(status.NickNames[index++], *name);
		}
		for (Entered = EnteredClient.begin(); Entered != EnteredClient.end(); Entered++) {
			retval = send(*Entered, (char*)&status, sizeof(status), 0);	
		}
	}



	// closesocket()
	LeaveCriticalSection(&cs);
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	int addrlen;
	HANDLE hThread;

	InitializeCriticalSection(&cs);
	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		EnterCriticalSection(&cs);

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		LeaveCriticalSection(&cs);
		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	DeleteCriticalSection(&cs);
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
