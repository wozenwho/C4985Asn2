#pragma once
#include "Header.h"



int sendTCP(char*, int, int);
int sendUDP(char*, int, int);

DWORD WINAPI sendTCPThread(LPVOID);
DWORD WINAPI sendUDPThread(LPVOID);
VOID CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);