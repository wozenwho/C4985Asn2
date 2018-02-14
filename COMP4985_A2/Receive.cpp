#include "Receive.h"

SOCKET AcceptSocket;
int condition;

int recvTCP()
{
	struct sockaddr_in clientAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	char *ip;
	HANDLE threadHandle;
	DWORD threadID;
	recvThrdParam* rtp = (recvThrdParam*) malloc(sizeof(recvThrdParam));

	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT_NO);
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	rtp->sender = clientAddr;
	rtp->receiver = localAddr;

	CreateThread(NULL, 0, recvTCPThread, (LPVOID)rtp, NULL, &threadID);
	return 0;
}

int recvUDP()
{
	struct sockaddr_in clientAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	char *ip;
	HANDLE threadHandle;
	DWORD threadID;
	recvThrdParam* rtp = (recvThrdParam*) malloc(sizeof(recvThrdParam));
	
	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT_NO);
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	rtp->sender = clientAddr;
	rtp->receiver = localAddr;

	CreateThread(NULL, 0, recvUDPThread, (LPVOID) rtp, NULL, &threadID);
	return 0;
}

DWORD WINAPI recvTCPThread(LPVOID lpParam)
{
	struct sockaddr_in recvAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	struct sockaddr acceptAddr;
	SOCKET acceptSocket;
	WSADATA wsd;
	WSABUF DataBuf;
	DWORD RecvBytes, Flags;
	WSAOVERLAPPED Overlapped;
	//SOCKET ConnSocket = INVALID_SOCKET;
	char recvBuffer[MAX_BUFFER_LENGTH];
	char* ip;
	int acceptLen;
	int error;
	int errorTCP;
	int numPacksReceived = 0;
	int totalBytes = 0;

	recvThrdParam* rtp = (recvThrdParam*)lpParam;
	recvAddr = rtp->sender;
	localAddr = rtp->receiver;
	DataBuf.len = MAX_BUFFER_LENGTH;
	DataBuf.buf = recvBuffer;


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

	int initialLoop = 1;
	while (1)
	{
		Flags = 0;
		wsaResult = WSARecv(acceptSocket, &DataBuf, 1, NULL, &Flags, &Overlapped, NULL);
		if ((wsaResult == SOCKET_ERROR) && (WSA_IO_PENDING != (errorTCP = WSAGetLastError()))) {
			break;
		}
		numPacksReceived++;
		//totalBytes += RecvBytes;

		if (initialLoop)
		{
			wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, FALSE, 10000, FALSE);
			if (wsaResult == WSA_WAIT_FAILED)
			{
				errorTCP = WSAGetLastError();
			}
			initialLoop = 0;
		}

		wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
		if (wsaResult == WSA_WAIT_FAILED)
		{
			errorTCP = WSAGetLastError();
		}
		if (wsaResult == WSA_WAIT_TIMEOUT)
		{
			errorTCP = WSAGetLastError();
			break;
		}

		wsaResult = WSAGetOverlappedResult(acceptSocket, &Overlapped, &RecvBytes, FALSE, &Flags);
		if (wsaResult == FALSE) {
			errorTCP = WSAGetLastError();
		}
		totalBytes += RecvBytes;
		if (RecvBytes == 0)
			break;
	}

	return totalBytes;
}



DWORD WINAPI recvUDPThread(LPVOID lpParam)
{
	struct sockaddr_in RecvAddr;
	struct sockaddr_in SenderAddr;
	WSADATA wsaData;
	WSABUF DataBuf;
	DWORD BytesRecv = 0;
	DWORD Flags = 0;
	WSAOVERLAPPED Overlapped;
	char recvBuffer[MAX_BUFFER_LENGTH];
	int errorUDP;
	int numPacksReceived = 0;
	int initialLoop = 1;

	//socketRecv = INVALID_SOCKET;

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

	wsaResult = bind(socketRecv, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
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

	while (1)
	{
		wsaResult = WSARecvFrom(socketRecv, &DataBuf, 1, &BytesRecv, &Flags, (SOCKADDR*)&SenderAddr,
			&SenderAddrSize, &Overlapped, NULL);
		if (wsaResult != 0)
		{
			errorUDP = WSAGetLastError();
			if (errorUDP != WSA_IO_PENDING)
			{
				errorUDP = WSAGetLastError();
			}
			else {
				if (initialLoop) {
					wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, 10000, TRUE);
					initialLoop = 0;
				}
				
				wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, 1000, TRUE);
				if (wsaResult == WSA_WAIT_FAILED) {
					errorUDP = WSAGetLastError();
				}
				if (wsaResult == WSA_WAIT_TIMEOUT)
				{ 
					break;
				}

				wsaResult = WSAGetOverlappedResult(socketRecv, &Overlapped, &BytesRecv, FALSE, &Flags);
				if (wsaResult == FALSE)
				{
					errorUDP = WSAGetLastError();
				}
				else {
					numPacksReceived++;
				}
			}
		}
		else
		{
			numPacksReceived++;
		}
	}
	return numPacksReceived;
}
