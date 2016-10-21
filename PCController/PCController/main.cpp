#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "Ws2_32.lib")

#include <Windows.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORTSEND "8888"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_PLUS 43
#define KEY_MINUS 45

using namespace std;

class SendData
{
private:
	WSADATA wsaData;
	SOCKET SendSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	char recvbuf[DEFAULT_BUFLEN];

	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

public:
	SendData()
	{

	}

	~SendData()
	{

	}

	int connectSend(char s[15])
	{
		iSendResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iSendResult != 0)
		{
			printf("WSAStartup failed with error: %d\n", iSendResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		iSendResult = getaddrinfo(s, DEFAULT_PORTSEND, &hints, &result);
		if (iSendResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iSendResult);
			WSACleanup();
			return 1;
		}

		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		{

			SendSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (SendSocket == INVALID_SOCKET)
			{
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			iSendResult = connect(SendSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iSendResult == SOCKET_ERROR)
			{
				closesocket(SendSocket);
				SendSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		if (SendSocket == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}
		else
			printf("Connected!\n");
	}

	int sendInput(int i)
	{
		iSendResult = send(SendSocket, (char*)&i, sizeof(int), 0);

		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
			return 1;
		}

		return 0;
	}

	int close()
	{
		printf("Connection closing...\n");

		iSendResult = shutdown(SendSocket, SD_SEND);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
			return 1;
		}

		closesocket(SendSocket);
		WSACleanup();

		return 0;
	}
};

int main()
{
	SendData sd;
	cout << "Connecting to 192.168.43.144.." << endl;
	char c[15] = "192.168.43.144";
	sd.connectSend(c);

	
	for (;;)
	{
		char key;
		int asciiValue;
		key = _getch();
		asciiValue = key;
		switch (asciiValue)
		{
		case KEY_SPACE:
			cout << "Brake" << endl;
			sd.sendInput(0);
			break;
		case KEY_UP:
			cout << "Forwards" << endl;
			sd.sendInput(1);
			break;
		case KEY_DOWN:
			cout << "Backwards" << endl;
			sd.sendInput(2);
			break;
		case KEY_LEFT:
			cout << "Left" << endl;
			sd.sendInput(3);
			break;
		case KEY_RIGHT:
			cout << "Right" << endl;
			sd.sendInput(4);
			break;
		case KEY_ESC:
			sd.sendInput(0);
			sd.sendInput(-1);
			sd.close();
			return 0;
			break;
		case KEY_PLUS:
			sd.sendInput(5);
			cout << "Speed increased" << endl;
			break;
		case KEY_MINUS:
			sd.sendInput(6);
			cout << "Speed decreased" << endl;
			break;
		}
	}
}