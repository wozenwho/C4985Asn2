#include "Send.h"



int sendTCP(char* ipAddr, int packSize, int numPacks)
{
	HANDLE sendThread;
	DWORD threadID;
	struct sockaddr_in receiver;
	sendThrdParam stp;
	char defaultBuffer[MAX_PACK_SIZE];
	int errorTCP;


	socketSend = socket(PF_INET, SOCK_STREAM, 0);
	if (socketSend == INVALID_SOCKET)
	{
		errorTCP = WSAGetLastError();
		MessageBox(hwnd, "socket init error in sendTCP", NULL, NULL);
	}

	memset((char*) &receiver, 0, sizeof(sockaddr_in));
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(PORT_NO);
	receiver.sin_addr.s_addr = inet_addr(ipAddr);


	sendThread = CreateThread(NULL, 0, sendTCPThread, NULL, 0, &threadID);


	//CreateEvent();
	return 0;
}

int sendUDP(char* ipAddr, int packSize, int numPacks)
{
	WSADATA wsaData;
	WSABUF DataBuf;
	WSAOVERLAPPED Overlapped;
	struct sockaddr_in recvAddr;
	struct sockaddr_in localAddr;
	int RecvAddrSize = sizeof(recvAddr);
	int LocalAddrSize = sizeof(localAddr);

	struct hostent *localHost;
	char *ip;

	char *targetip;
	char *targetport;

	char SendBuf[1024] = "Data buffer to send";
	int BufLen = 1024;
	DWORD BytesSent = 0;
	DWORD Flags = 0;

	HANDLE sendThread;
	DWORD threadID;
	sendThrdParam* stp = (sendThrdParam*) malloc(sizeof(sendThrdParam));

	int error;
	int retval = 0;

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
	sendThrdParam* stp = (sendThrdParam*) lpParam;
	char defaultBuffer[MAX_PACK_SIZE];

	for (int i = 0; i < stp->numPacks; i++)
	{
		memset(defaultBuffer, (char)65, stp->packSize);
		sendto(socketSend, defaultBuffer, stp->packSize, 0, (struct sockaddr*)&(stp->receiver), sizeof(stp->receiver));
	}

	//createevent?
	return 0;
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
			WSASendTo(socketSend, &DataBuf, 1, &BytesSent, Flags, (SOCKADDR *)&threadInfo.receiver, RecvAddrSize, &Overlapped, NULL);
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
	return 0;
}