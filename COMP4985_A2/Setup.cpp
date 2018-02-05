#include "Setup.h"
#include "Common.h"

int setup() {
	HDC hdc = GetDC(hwnd);

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

	return (radioButtonClient && radioButtonServer && radioButtonTCP && radioButtonUDP && ipInputField &&
		packSizeInputField && numPacksInputField && sendButton);
}