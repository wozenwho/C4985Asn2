#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Setup.h"

//Window size constants
#define WINDOW_WIDTH			400
#define WINDOW_HEIGHT			600
#define xOffset					15
#define yOffset					25

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

//Static UI Text
#define xCoord_descrip			5
#define yCoord_textOffset		3
#define textField_ip			"IP Address:"
#define textField_protocol		"Protocol:"
#define textField_mode			"Program Mode:"
#define textField_numPacks		"Num. Packets:"	
#define textField_packSize		"Packet size:"

