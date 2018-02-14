#include "Send.h"



int sendTCP(char* ipAddr, int packSize, int numPacks)
{
	struct sockaddr_in recvAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	char* ip;
	HANDLE sendThread;
	DWORD threadID;
	sendThrdParam* stp = (sendThrdParam*)malloc(sizeof(sendThrdParam));

	int error;

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(PORT_NO);
	recvAddr.sin_addr.s_addr = inet_addr(ipAddr);

	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	stp->receiver = recvAddr;
	stp->sender = localAddr;
	stp->packSize = packSize;
	stp->numPacks = numPacks;
	stp->filePtr = NULL;

	sendThread = CreateThread(NULL, 0, sendTCPThread, (LPVOID)stp, 0, &threadID);
	
	return 0;
}

int sendUDP(char* ipAddr, int packSize, int numPacks)
{
	struct sockaddr_in recvAddr;
	struct sockaddr_in localAddr;
	struct hostent *localHost;
	char *ip;
	HANDLE sendThread;
	DWORD threadID;
	sendThrdParam* stp = (sendThrdParam*) malloc(sizeof(sendThrdParam));

	int error;

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(PORT_NO);
	recvAddr.sin_addr.s_addr = inet_addr(ipAddr);

	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	stp->receiver = recvAddr;
	stp->sender = localAddr;
	stp->numPacks = numPacks;
	stp->packSize = packSize;
	stp->filePtr = NULL;

	sendThread = CreateThread(NULL, 0, sendUDPThread, (LPVOID)stp, 0, &threadID);
	if (sendThread == NULL)
	{
		error = GetLastError();
	}

	return 0;
}

DWORD WINAPI sendTCPThread(LPVOID lpParam)
{
	sendThrdParam* stp = (sendThrdParam*)lpParam;
	WSADATA wsaData;
	WSABUF wsaBuf;

	WSAOVERLAPPED Overlapped;
	WSABUF DataBuf;

	struct sockaddr_in localAddr;
	int localAddrSize = sizeof(localAddr);

	char sendBuf[MAX_BUFFER_LENGTH];
	DWORD BytesSent = 0;
	DWORD Flags = 0;

	int errorTCP;
	int error;

	ZeroMemory((PVOID)&Overlapped, sizeof(WSAOVERLAPPED));
	Overlapped.hEvent = WSACreateEvent();
	if (Overlapped.hEvent == WSA_INVALID_EVENT)
	{
		errorTCP = WSAGetLastError();
	}
	
	socketSend = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socketSend == INVALID_SOCKET)
	{
		errorTCP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		WSACleanup();
	}

	if (connect(socketSend, (sockaddr*)&(stp->receiver), sizeof(stp->receiver)) == -1)
	{
		errorTCP = WSAGetLastError();
	}

	DataBuf.len = MAX_BUFFER_LENGTH;
	DataBuf.buf = sendBuf;

	for (int i = 0; i < stp->numPacks; i++)
	{
		wsaResult = WSASend(socketSend, &DataBuf, 1, &BytesSent, Flags, &Overlapped, NULL);
		if (wsaResult == SOCKET_ERROR)
		{
			errorTCP = WSAGetLastError();
		}
		wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
		if (wsaResult == WSA_WAIT_FAILED)
		{
			errorTCP = WSAGetLastError();
		}
		wsaResult = WSAGetOverlappedResult(socketSend, &Overlapped, &BytesSent, FALSE, &Flags);
		
		if (wsaResult == FALSE)
		{
			errorTCP = WSAGetLastError();
		}
		WSAResetEvent(Overlapped.hEvent);
	}

	return BytesSent;
}

DWORD WINAPI sendUDPThread(LPVOID lpParam)
{
	WSADATA wsaData;
	WSABUF DataBuf;

	WSAOVERLAPPED Overlapped;

	struct sockaddr_in RecvAddr;
	struct sockaddr_in localAddr;
	int RecvAddrSize = sizeof(RecvAddr);
	int LocalAddrSize = sizeof(localAddr);

	struct hostent *localHost;
	char* ip;

	sendThrdParam threadInfo;

	char sendBuf[MAX_BUFFER_LENGTH];
	DWORD BytesSent = 0;
	DWORD Flags = 0;

	int errorUDP;
	int error;
	int retval = 0;
	int numSent = 0;
		
	threadInfo  = *(sendThrdParam*)lpParam;

	localHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORT_NO);
	localAddr.sin_addr.s_addr = inet_addr(ip);

	ZeroMemory((PVOID)&Overlapped, sizeof(WSAOVERLAPPED));
	Overlapped.hEvent = WSACreateEvent();
	if (Overlapped.hEvent == WSA_INVALID_EVENT)
	{
		errorUDP = WSAGetLastError();
	}

	socketSend = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socketSend == INVALID_SOCKET)
	{
		errorUDP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		WSACleanup();
	}

	wsaResult = bind(socketSend, (struct sockaddr*)&localAddr, LocalAddrSize);
	if (wsaResult == SOCKET_ERROR)
	{
		errorUDP = WSAGetLastError();
		WSACloseEvent(Overlapped.hEvent);
		closesocket(socketSend);
		WSACleanup();
	}

	if (threadInfo.filePtr == NULL)
	{
		DataBuf.len = threadInfo.packSize;
		DataBuf.buf = sendBuf;
		memset(DataBuf.buf, 'A', threadInfo.packSize);
		for (int i = 0; i < threadInfo.numPacks; i++)
		{
			wsaResult = WSASendTo(socketSend, &DataBuf, 1, &BytesSent, Flags, (SOCKADDR *)&threadInfo.receiver, RecvAddrSize, &Overlapped, NULL);
			if ((wsaResult == SOCKET_ERROR) && (WSA_IO_PENDING != (error = WSAGetLastError()))) {
				WSACloseEvent(Overlapped.hEvent);
				closesocket(socketSend);
				WSACleanup();
				exit(1);
			}
			wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, 1, TRUE);
			if (wsaResult == WSA_WAIT_FAILED) {
				errorUDP = WSAGetLastError();
			}
			wsaResult = WSAGetOverlappedResult(socketSend, &Overlapped, &BytesSent, FALSE, &Flags);
			if (wsaResult == FALSE) {
				errorUDP = WSAGetLastError();
			}
			else
				numSent++;
		}
	}

	free(lpParam);


	//sendThrdParam* stp = (sendThrdParam*)lpParam;
	//char defaultBuffer[MAX_PACK_SIZE];

	//for (int i = 0; i < stp->numPacks; i++)
	//{
	//	memset(defaultBuffer, (char)65, stp->packSize);
	//	sendto(socketSend, defaultBuffer, stp->packSize, 0, (struct sockaddr*)&(stp->receiver), sizeof(stp->receiver));
	//}
	////createevent?
	return numSent;
}