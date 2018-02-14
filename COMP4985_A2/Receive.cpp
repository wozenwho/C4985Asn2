#include "Receive.h"

SOCKET AcceptSocket;
int condition;

int recvTCP(char* ipAddr)
{
	WSADATA wsd;
	struct sockaddr_in recvAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	char* ip;
	//HANDLE sendThread;
	//DWORD threadID;

	WSAOVERLAPPED Overlapped;
	SOCKET ConnSocket = INVALID_SOCKET;
	WSABUF DataBuf;
	DWORD RecvBytes, Flags;
	char recvBuffer[MAX_BUFFER_LENGTH];

	SOCKET acceptSocket;
	struct sockaddr acceptAddr;
	int acceptLen;

	int error;
	int errorTCP;

	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	ZeroMemory((PVOID)&Overlapped, sizeof(WSAOVERLAPPED));
	Overlapped.hEvent = WSACreateEvent();
	if (Overlapped.hEvent == WSA_INVALID_EVENT)
	{
		errorTCP = WSAGetLastError();
	}

	socketRecv = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socketRecv == INVALID_SOCKET)
	{
		errorTCP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		WSACleanup();
	}

	//wsaResult = WSAAsyncSelect(socketRecv, hwnd, WM_SOCKET, FD_ACCEPT);
	//if (wsaResult == SOCKET_ERROR)
	//{
	//	errorTCP = WSAGetLastError();
	//}

	wsaResult = bind(socketRecv, (struct sockaddr*) &localAddr, sizeof(localAddr));
	if (wsaResult == SOCKET_ERROR)
	{
		errorTCP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		closesocket(socketSend);
		WSACleanup();
	}

	wsaResult = listen(socketRecv, 2);
	if (wsaResult == SOCKET_ERROR)
	{
		errorTCP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		closesocket(socketSend);
		WSACleanup();
	}

	acceptSocket = accept(socketRecv, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		errorTCP = WSAGetLastError();
		closesocket(acceptSocket);
		closesocket(socketRecv);
	}

	DataBuf.len = MAX_BUFFER_LENGTH;
	DataBuf.buf = recvBuffer;

	while (1)
	{
		Flags = 0;
		wsaResult = WSARecv(acceptSocket, &DataBuf, 1, &RecvBytes, &Flags, &Overlapped, NULL);
		if ((wsaResult == SOCKET_ERROR) && (WSA_IO_PENDING != (errorTCP = WSAGetLastError()))) {
			break;
		}

		wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
		if (wsaResult == WSA_WAIT_FAILED)
		{
			errorTCP = WSAGetLastError();
		}

		wsaResult = WSAGetOverlappedResult(acceptSocket, &Overlapped, &RecvBytes, FALSE, &Flags);
		if (wsaResult == FALSE) {
			errorTCP = WSAGetLastError();
		}

		if (RecvBytes == 0)
			break;
	}

	return RecvBytes;
}

int recvUDP(char* ipAddr)
{
	WSADATA wsaData;
	WSABUF DataBuf;
	WSAOVERLAPPED Overlapped;
	socketRecv = INVALID_SOCKET;
	struct sockaddr_in RecvAddr;
	struct sockaddr_in SenderAddr;
	DWORD BytesRecv = 0;
	DWORD Flags = 0;
	int errorUDP;
	char recvBuffer[MAX_BUFFER_LENGTH];
	int numReceived = 0;
	condition = 1;

	int SenderAddrSize = sizeof(SenderAddr);
	ZeroMemory(&Overlapped, sizeof(Overlapped));
	Overlapped.hEvent = WSACreateEvent();
	if (Overlapped.hEvent == NULL)
	{
		errorUDP = WSAGetLastError();
	}
	socketRecv = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socketRecv == INVALID_SOCKET)
	{
		errorUDP = WSAGetLastError();
	}
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT_NO);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	wsaResult = bind(socketRecv, (SOCKADDR*) &RecvAddr, sizeof(RecvAddr));
	if (wsaResult != 0)
	{
		errorUDP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		closesocket(socketRecv);
		WSACleanup();
	}

	DataBuf.len = MAX_BUFFER_LENGTH;
	DataBuf.buf = recvBuffer;
	wsaResult = WSARecvFrom(socketRecv, &DataBuf, 1, &BytesRecv, &Flags, (SOCKADDR*)&SenderAddr,
		&SenderAddrSize, &Overlapped, NULL);
	
	
	while (condition)
	{
		//SetTimer(hwnd, id_timer, 50, TimerProc);
		wsaResult = WSARecvFrom(socketRecv, &DataBuf, 1, &BytesRecv, &Flags, (SOCKADDR*)&SenderAddr,
			&SenderAddrSize, &Overlapped, NULL);
		if (wsaResult != 0)
		{
			errorUDP = WSAGetLastError();
			if (errorUDP != WSA_IO_PENDING)
			{
				MessageBox(hwnd, "Well, double shit", NULL, NULL);
			}
			else {
				wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
				if (wsaResult == WSA_WAIT_FAILED) {
					wprintf(L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
				}

				wsaResult = WSAGetOverlappedResult(socketRecv, &Overlapped, &BytesRecv, FALSE, &Flags);
				if (wsaResult == FALSE)
				{
					errorUDP = WSAGetLastError();
				}
				else {
					numReceived++;
					KillTimer(hwnd, id_timer);
				}
			}
		}
		else
		{
			numReceived++;
			KillTimer(hwnd, id_timer);
		}
	}
	return 0;
}

DWORD WINAPI recvTCPThread(LPVOID lpParam)
{
	return 0;
}



DWORD WINAPI recvUDPThread(LPVOID lpParam)
{
	return 0;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hwnd, id_timer);
	condition = 0;
}

//TODO: fucking finish this once I know how it works jesus. ///Completion routine is used to write to file, otherwise pass NULL into WSARecv
//void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
//{
//	DWORD sendBytes;
//	DWORD recvBytes;
//	DWORD flags;
//
//	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION)Overlapped;
//
//	if (Error != 0)
//	{
//		//error things here
//	}
//
//	if (BytesTransferred == 0)
//	{
//		//close socket?
//	}
//
//	if (Error != 0 || BytesTransferred == 0)
//	{
//		closesocket(SI->Socket);
//		GlobalFree(SI);
//		return;
//	}
//
//
//}