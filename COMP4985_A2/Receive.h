#pragma once
#include "Header.h"

int recvTCP(char*);
int recvUDP(char*);

DWORD WINAPI recvTCPThread(LPVOID);
DWORD WINAPI recvUDPThread(LPVOID);
DWORD WINAPI acceptThread(LPVOID);