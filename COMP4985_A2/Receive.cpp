#include "Receive.h"

SOCKET AcceptSocket;

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
	
	if (wsaResult != 0)
	{
		errorUDP = WSAGetLastError();
		if (errorUDP != WSA_IO_PENDING)
		{
			MessageBox(hwnd, "Well. Shit.", NULL, NULL);
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
			}
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

//TODO: fucking finish this once I know how it works jesus. 
//Completion routine is used to write to file, otherwise pass NULL into WSARecv
void CALLBACK WorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
	DWORD sendBytes;
	DWORD recvBytes;
	DWORD flags;

	LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION)Overlapped;

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