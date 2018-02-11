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
	HANDLE sendThread;
	DWORD threadID;
	struct sockaddr_in receiver;
	sendThrdParam stp;
	char defaultBuffer[MAX_PACK_SIZE];
	int errorUDP;

	size_t numPackRecv = 0;
	size_t numPackSent = 0;
	 
	socketSend = socket(PF_INET, SOCK_DGRAM, 0);
	if (socketSend == INVALID_SOCKET)
	{
		errorUDP = WSAGetLastError();
	}

	memset((char*)&receiver, 0, sizeof(sockaddr_in));
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(PORT_NO);
	receiver.sin_addr.s_addr = inet_addr(ipAddr);

	stp.receiver = receiver;
	stp.packSize = packSize;
	stp.numPacks = numPacks;
	stp.filePtr = NULL;

	sendThread = CreateThread(NULL, 0, sendTCPThread, (LPVOID) &stp, 0, &threadID);

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
	return 0;
}