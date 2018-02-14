#pragma once
#include "Header.h"

int recvTCP();
int recvUDP();

DWORD WINAPI recvTCPThread(LPVOID);
DWORD WINAPI recvUDPThread(LPVOID);