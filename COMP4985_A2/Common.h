#pragma once
#include "Header.h"

extern HWND hwnd;
extern HWND ipInputField;
extern HWND packSizeInputField;
extern HWND numPacksInputField;
extern HWND radioButtonUDP;
extern HWND radioButtonTCP;
extern HWND radioButtonClient;
extern HWND radioButtonServer;
extern HWND sendButton;

extern int wsaResult;

extern SOCKET socketRecv;
extern SOCKET socketSend;

extern size_t numPackSent;
extern size_t totalDataSent;
extern size_t numPackRecv;
extern size_t totalDataRecv;