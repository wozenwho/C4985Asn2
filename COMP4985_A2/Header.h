#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <windowsx.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Send.h"
#include "Receive.h"
#include "Common.h"

//Connection constants
#define PORT_NO					9999

//Window size constants
#define WINDOW_WIDTH			400
#define WINDOW_HEIGHT			600
#define xOffset					15
#define yOffset					25

//Window IDs
#define id_client				100
#define id_server				101
#define id_tcp					102
#define id_udp					103
#define id_ipaddr				104
#define id_packSize				105
#define id_numPacks				106
#define id_send					107
#define id_timer				200

//Timer constants
#define ONE_SECOND				1000

//Radio button constants
#define width_radio				90
#define xCoord_client			110
#define xCoord_server			xCoord_client + width_radio
#define yCoord_mode				5
#define xCoord_TCP				110
#define xCoord_UDP				xCoord_TCP + width_radio
#define yCoord_protocol			30

//IP field constants
#define xCoord_ip				110
#define yCoord_ip				70
#define ipFieldWidth			150
#define textBoxFieldHeight		20

//Packet Option field constants
#define xCoord_packSize			110
#define yCoord_packSize			95
#define packSizeFieldWidth		100
#define xCoord_numPacks			110
#define yCoord_numPacks			120
#define numPacksFieldWidth		100

//Send button constants
#define xCoord_send				5
#define yCoord_send				145

//Buffer sizes
#define MAX_BUFFER_LENGTH		65536
#define MAX_IP_LENGTH			16
#define MAX_PACK_SIZE			99999
#define MAX_NUM_PACKS			99999

//Static UI Text
#define xCoord_descrip			5
#define yCoord_textOffset		3
#define textField_ip			"IP Address:"
#define textField_protocol		"Protocol:"
#define textField_mode			"Program Mode:"
#define textField_numPacks		"Num. Packets:"	
#define textField_packSize		"Packet size:"

#define WM_SOCKET				(WM_USER + 1)

//Thread struct
typedef struct {
	struct sockaddr_in receiver;
	struct sockaddr_in sender;
	int packSize;
	int numPacks;
	char* filePtr;
} sendThrdParam;

typedef struct {
	sockaddr_in receiver;
	sockaddr_in sender;
} recvThrdParam;

typedef struct _SOCKET_INFORMATION {
	OVERLAPPED Overlapped;
	SOCKET Socket;
	CHAR Buffer[MAX_BUFFER_LENGTH];
	WSABUF DataBuffer;
	DWORD BytesSEND;
	DWORD BytesRECV;
	BOOL RecvPosted;
	_SOCKET_INFORMATION *Next;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;