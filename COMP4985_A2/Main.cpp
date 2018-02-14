#include "Header.h"
#include "Common.h"

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

//Global winsock variables
WSADATA wsaData;
WORD wVersionRequested = MAKEWORD(2, 2);
int wsaResult;
SOCKET socketRecv;
SOCKET socketSend;
LPSOCKET_INFORMATION SocketInfoList;

//Global buffer variables
char inputBuffer[MAX_BUFFER_LENGTH];
//char ipBuffer[MAX_IP_LENGTH];
//char numPackBuffer[MAX_NUM_PACKS];
//char packSizeBuffer[MAX_PACK_SIZE];
char fileInputBuffer[MAX_BUFFER_LENGTH];
char fileOutputBuffer[MAX_BUFFER_LENGTH];


//Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int ready(BOOL, BOOL, BOOL, BOOL, char*, int, int);
int setup();
void CreateSocketInformation(SOCKET);

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

	if (!setup())
		return 0;

	wsaResult = WSAStartup(wVersionRequested, &wsaData);
	if (wsaResult != 0)
	{
		wsaResult = WSAGetLastError();
		exit(1);
	}

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
	DWORD dwBytesRead = 0;
	char ipAddr[MAX_IP_LENGTH];
	SOCKET acceptSocket;
	

	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BN_CLICKED:
			BOOL clientChecked = IsDlgButtonChecked(hwnd, id_client);
			BOOL serverChecked = IsDlgButtonChecked(hwnd, id_server);
			BOOL tcpChecked = IsDlgButtonChecked(hwnd, id_tcp);
			BOOL udpChecked = IsDlgButtonChecked(hwnd, id_udp);
			
			GetDlgItemText(hwnd, id_ipaddr, ipAddr, MAX_IP_LENGTH);
			int packSize = GetDlgItemInt(hwnd, id_packSize, NULL, FALSE);
			int numPacks = GetDlgItemInt(hwnd, id_numPacks, NULL, FALSE);

			ready(clientChecked, serverChecked, tcpChecked, udpChecked, ipAddr, packSize, numPacks);
		}
		break;
	case WM_TIMER:
		MessageBox(hwnd, "The fuck.", NULL, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paintstruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case FD_ACCEPT:
		if ((acceptSocket = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
		{
			int err = WSAGETSELECTERROR(lParam);
		}
		CreateSocketInformation(acceptSocket);
		WSAAsyncSelect(acceptSocket, hwnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
		break;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int ready(BOOL client, BOOL server, BOOL tcp, BOOL udp, char* ipAddr, int packSize, int numPacks)
{
	if ((client && server) || (tcp && udp) || packSize < 0 || numPacks < 0)
		return 0;
	if (client)
	{
		if (tcp)
		{
			//MessageBox(hwnd, "Entering Client-TCP", NULL, NULL);
			sendTCP(ipAddr, packSize, numPacks);
		}
		else {
			//MessageBox(hwnd, "Entering Client-UDP", NULL, NULL);
			sendUDP(ipAddr, packSize, numPacks);
		}
	}
	
	else if (server)
	{
		if (tcp)
		{
			//MessageBox(hwnd, "Entering Server-TCP", NULL, NULL);
			recvTCP(ipAddr);
		}
		else {
			//MessageBox(hwnd, "Entering Server -UDP", NULL, NULL);
			recvUDP(ipAddr);
		}
	}
	return 0;
}

int setup() {
	HDC hdc = GetDC(hwnd);

	//Program mode select field
	TextOut(hdc, xCoord_descrip, yCoord_mode + yCoord_textOffset, textField_mode, strlen(textField_mode));

	radioButtonClient = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Client",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,
		xCoord_client, yCoord_mode, width_radio, textBoxFieldHeight, hwnd, (HMENU)id_client, NULL, NULL);

	radioButtonServer = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Server",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xCoord_server, yCoord_mode, width_radio, textBoxFieldHeight, hwnd, (HMENU)id_server, NULL, NULL);

	//Protocol select field
	TextOut(hdc, xCoord_descrip, yCoord_protocol + yCoord_textOffset, textField_protocol, strlen(textField_protocol));

	radioButtonTCP = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "TCP",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,
		xCoord_TCP, yCoord_protocol, width_radio, textBoxFieldHeight, hwnd, (HMENU)id_tcp, NULL, NULL);

	radioButtonUDP = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "UDP",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		xCoord_UDP, yCoord_protocol, width_radio, textBoxFieldHeight, hwnd, (HMENU)id_udp, NULL, NULL);

	//IP Address input field
	TextOut(hdc, xCoord_descrip, yCoord_ip + yCoord_textOffset, textField_ip, strlen(textField_ip));

	ipInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
		xCoord_ip, yCoord_ip, ipFieldWidth, textBoxFieldHeight, hwnd, (HMENU)id_ipaddr, NULL, NULL);

	//Packet Size input field
	TextOut(hdc, xCoord_descrip, yCoord_packSize + yCoord_textOffset, textField_packSize, strlen(textField_packSize));

	packSizeInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
		xCoord_packSize, yCoord_packSize, packSizeFieldWidth, textBoxFieldHeight, hwnd, (HMENU)id_packSize, NULL, NULL);

	//Num. Packets input field
	TextOut(hdc, xCoord_descrip, yCoord_numPacks + yCoord_textOffset, textField_numPacks, strlen(textField_numPacks));

	numPacksInputField = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		xCoord_numPacks, yCoord_numPacks, numPacksFieldWidth, textBoxFieldHeight, hwnd, (HMENU)id_numPacks, NULL, NULL);

	//Send button
	sendButton = CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Send",
		BS_CENTER | WS_CHILD | WS_VISIBLE,
		xCoord_send, yCoord_send, width_radio, textBoxFieldHeight, hwnd, NULL, NULL, NULL); //(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE)

	return (radioButtonClient && radioButtonServer && radioButtonTCP && radioButtonUDP && ipInputField &&
		packSizeInputField && numPacksInputField && sendButton);
}

void CreateSocketInformation(SOCKET s)
{
	LPSOCKET_INFORMATION SI;

	if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
	{
		int err = GetLastError();
	}

	SI->Socket = s;
	SI->RecvPosted = FALSE;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;

	SI->Next = SocketInfoList;
	SocketInfoList = SI;
}