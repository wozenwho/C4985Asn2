#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Header.h"

char programName[] = "COMP 4985 Assignment 2";
HWND hwnd;
HWND ipInputField;
HWND packSizeInputField;
HWND numPacksInputField;

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

	//Wcl.lpszMenuName = "Menu";
	Wcl.cbClsExtra = 0;
	Wcl.cbWndExtra = 0;
	if (!RegisterClassEx(&Wcl))
		return 0;

	//Program Window
	hwnd = CreateWindow(programName, programName, WS_OVERLAPPEDWINDOW, 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	 
	ipInputField = CreateWindow();

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