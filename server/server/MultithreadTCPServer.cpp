#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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

std::list<SOCKET> EnteredClient;	// ������ Ŭ���̾�Ʈ�� �ּҸ� �����ϱ� ���� ����Ʈ
std::list<SOCKET>::iterator Entered;	// �ݺ���
std::list<CString> NickNames;		// ������ �г����� �����ϱ� ���� ����Ʈ
std::list<CString>::iterator name;		// �ݺ���
CRITICAL_SECTION cs;
SOCKADDR_IN clientaddr;

struct EveryThingSend {	// �޼��� ����ü
	int type;	// ���� Ŭ���̾�Ʈ�� ����
	char name[40];	// �г���
	char message[500];	// ä�ñ�
	char NickNames[50][BUFSIZE];	// ���� �������ִ� �г��ӵ��� �����ϱ� ����
};


// ���� �Լ� ���� ��� �� ����
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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	EveryThingSend status;

	// Ŭ���̾�Ʈ ���� ���
	int addrlen2 = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen2);	// ������ Ŭ���̾�Ʈ ������ ������ ����


	while (1) {
		// Ŭ���̾�Ʈ���� ������ �ޱ�
		retval = recv(client_sock, (char*)&status, sizeof(status), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		if (status.type == 0) {	// ä�ù濡 ���� ��, �г���â���� �г����� �Է��ϰ� �ߺ�üũ ���� ��
			int boolean = 0;
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), status.name);
			for (name = NickNames.begin(); name != NickNames.end(); name++) {	// �г��� �迭����Ʈ�� ��
				if (strcmp(*name, status.name) == 0) {
					boolean = 1;
					break;
				}
			}
			
			if (boolean) {		// �ߺ��� ���
				status.type = -1;
				retval = send(client_sock, (char*)&status, sizeof(status), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
			else {		// �ߺ��� ���� ���
				NickNames.push_back(status.name);	// �г��Ӹ���Ʈ�� �г��� �߰�
				status.type = 1;
				retval = send(client_sock, (char*)&status, sizeof(status), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
		}
		
		if (status.type == 2) {	// ä�ù濡 ���� ���� ��
			EnteredClient.push_back(client_sock);	// Ŭ���̾�Ʈ ����Ʈ�� �߰�(ä�ù濡 ������ ����) 
			sprintf(status.message, "[%s]���� �����߽��ϴ�.", status.name);

			for (int i = 0; i < 50; i++) {
				status.NickNames[i][0] = '\0';
			}

			int index = 0;
			for (name = NickNames.begin(); name != NickNames.end(); name++) {	// ������ �г����� ����ü �ȿ� �ִ� �г��ӹ迭�� ����
				strcpy(status.NickNames[index++], *name);
			}

			for (Entered=EnteredClient.begin(); Entered!=EnteredClient.end(); Entered++) {
				retval = send(*Entered, (char*)&status, sizeof(status), 0);	// ������ Ŭ���̾�Ʈ�鿡�� ä�ñ��� ����
			}

		}
		

		if (status.type == 3) {	// ä�ù濡 ���� �� ä���� ġ������ ��
			for (Entered = EnteredClient.begin(); Entered != EnteredClient.end(); Entered++) {
				retval = send(*Entered, (char*)&status, sizeof(status), 0);	// ������ Ŭ���̾�Ʈ�鿡�� ä�ñ��� ���� 
			}
		}
		
		
	}

	// ���α׷� ���� ��
	name = NickNames.begin();	// �г��ӹ迭���� �ش� �г����� ����
	while (name != NickNames.end()) {
		if (*name == status.name)
			NickNames.erase(name++);
		else
			++name;
	}

	if (status.type==2 || status.type==3) {	// ���α׷��� �����ϰ� ������ Ŭ���̾�Ʈ�� ����
		Entered = EnteredClient.begin();
		while (Entered != EnteredClient.end()) {
			if (*Entered == client_sock)
				EnteredClient.erase(Entered++);
			else
				++Entered;
		}

		status.type = 4;	// ������ �� Ŭ���̾�Ʈ�鿡�� �����ߴٰ� �˸� 
		sprintf(status.message, "[%s]���� �����߽��ϴ�.", status.name); 
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
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		LeaveCriticalSection(&cs);
		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	DeleteCriticalSection(&cs);
	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
