#include "Receive.h"



int recvTCP(char* ipAddr)
{
	HANDLE sendThread;
	DWORD threadID;
	struct sockaddr_in sender;
	recvThrdParam rtp;
	char buffer[MAX_PACK_SIZE];
	int errorTCP;

	socketRecv = socket(PF_INET, SOCK_STREAM, 0);
	return 0;
}

int recvUDP(char* ipAddr)
{
	HANDLE sendThread;
	DWORD threadID;
	struct sockaddr_in sender;
	recvThrdParam rtp;
	int errorUDP;
	char buffer[MAX_PACK_SIZE];

	memset((char*)&sender, 0, sizeof(sockaddr_in));
	sender.sin_family = AF_INET;
	sender.sin_port = htons(PORT_NO);
	sender.sin_addr.s_addr = inet_addr(ipAddr);
	rtp.sender = sender;

	socketRecv = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socketRecv == INVALID_SOCKET)
	{
		errorUDP = WSAGetLastError();
	}

	//Create thread to handle receiving packets from socket
	//sendThread = CreateThread();

	if (bind(socketRecv, (PSOCKADDR)&sender, sizeof(sockaddr_in)) == 0)
	{
		//ERROR Binding socket
		errorUDP = WSAGetLastError();
		exit(1);
	}

	return 0;
}

DWORD WINAPI recvTCPThread(LPVOID lpParam)
{
	return 0;
}

DWORD WINAPI recvUDPThread(LPVOID lpParam)
{
	char buffer[MAX_BUFFER_LENGTH];
	recvThrdParam* rtp = (recvThrdParam*)lpParam;
	sockaddr_in saFrom;
	int error;
	int errorUDP;
	BOOL condition = true;

	WSAEVENT AcceptEvent;
	SOCKET AcceptSocket;
	WSAEVENT EventArray[1];
	LPSOCKET_INFORMATION SocketInfo;
	DWORD Flags;
	DWORD RecvBytes;


	if (listen(socketRecv, 5))
	{
		errorUDP = WSAGetLastError();
		exit(1);
	}

	if ((AcceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		errorUDP = WSAGetLastError();
		exit(1);
	}

	AcceptSocket = accept(socketRecv, NULL, NULL);
	if ((SocketInfo = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		error = GetLastError();
		exit(1);
	}

	SocketInfo->Socket = AcceptSocket;
	ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));
	SocketInfo->BytesSEND = 0;
	SocketInfo->BytesRECV = 0;
	SocketInfo->DataBuffer.len = MAX_BUFFER_LENGTH;
	SocketInfo->DataBuffer.buf = SocketInfo->Buffer;

	while (condition)
	{
		if (WSARecvFrom(SocketInfo->Socket, &(SocketInfo->DataBuffer), 1, &RecvBytes, &Flags, NULL, NULL,
			&(SocketInfo->Overlapped), NULL) == SOCKET_ERROR)
		{
			errorUDP = WSAGetLastError();
		}
	}
	

}

//TODO: fucking finish this once I know how it works jesus. 
//Completion routine is used to write to file, otherwise pass NULL into WSARecv
void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
	DWORD sendBytes;
	DWORD recvBytes;
	DWORD flags;

	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION) Overlapped;

	if (Error != 0)
	{
		//error things here
	}

	if (BytesTransferred == 0)
	{
		//close socket?
	}

	if (Error != 0 || BytesTransferred == 0)
	{
		closesocket(SI->Socket);
		GlobalFree(SI);
		return;
	}


}