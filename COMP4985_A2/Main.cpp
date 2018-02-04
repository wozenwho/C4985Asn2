#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Header.h"

//Global window variables
char programName[] = "COMP 4985 Assignment 2";
HWND hwnd;
HWND ipInputField;
HWND packSizeInputField;
HWND numPacksInputField;
HWND radioButtonUDP;
HWND radioButtonTCP;
HWND radioButtonClient;
HWND radioButtonServer;
HWND sendButton;

//Global buffer variables
char inputBuffer[MAX_BUFFER_LENGTH];

//Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lspszCmdParam, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX Wcl;
	PAINTSTRUCT paintstruct;
	HDC hdc = BeginPaint(hwnd, &paintstruct);

	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm = NULL;
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wcl.lpszClassName = programName;

	Wcl.lpszMenuName = "Menu";
	Wcl.cbClsExtra = 0;
	Wcl.cbWndExtra = 0;
	if (!RegisterClassEx(&Wcl))
		return 0;

	//Program Window
	hwnd = CreateWindow(programName, programName, WS_OVERLAPPEDWINDOW, 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	hdc = GetDC(hwnd);

	//Program mode select field
	TextOut(hdc, xCoord_descrip, yCoord_mode + yCoord_textOffset, textField_mode, strlen(textField_mode));
	radioButtonClient = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Client",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,
		xCoord_client, yCoord_mode, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	radioButtonServer = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Server",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xCoord_server, yCoord_mode, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	//Protocol select field
	TextOut(hdc, xCoord_descrip, yCoord_protocol + yCoord_textOffset, textField_protocol, strlen(textField_protocol));
	radioButtonTCP = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "TCP",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,
		xCoord_TCP, yCoord_protocol, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	radioButtonUDP = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "UDP",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xCoord_UDP, yCoord_protocol, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL);


	//IP Address input field
	TextOut(hdc, xCoord_descrip, yCoord_ip + yCoord_textOffset, textField_ip, strlen(textField_ip));
	ipInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT, 
		xCoord_ip, yCoord_ip, ipFieldWidth, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	//Packet Size input field
	TextOut(hdc, xCoord_descrip, yCoord_packSize + yCoord_textOffset, textField_packSize, strlen(textField_packSize));
	packSizeInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
		xCoord_packSize, yCoord_packSize, packSizeFieldWidth, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	//Num. Packets input field
	TextOut(hdc, xCoord_descrip, yCoord_numPacks + yCoord_textOffset, textField_numPacks, strlen(textField_numPacks));
	numPacksInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
		xCoord_numPacks, yCoord_numPacks, numPacksFieldWidth, textBoxFieldHeight, hwnd, NULL, NULL, NULL);

	//Send button
	sendButton = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Send",
	BS_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xCoord_send, yCoord_send, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL);


	
	


	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	WSACleanup();
	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);
	PAINTSTRUCT paintstruct;
	DWORD threadId;
	DWORD dwBytesRead = 0;

	switch (Message)
	{
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}