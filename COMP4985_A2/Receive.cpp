#include "Receive.h"

int recvTCP(char* ipAddr)
{
	socketRecv = socket(PF_INET, SOCK_STREAM, 0);
	return 0;
}

int recvUDP(char* ipAddr)
{
	socketRecv = socket(PF_INET, SOCK_DGRAM, 0);
	return 0;
}