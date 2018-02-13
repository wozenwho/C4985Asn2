while (condition)
			{
				SetTimer(hwnd, id_timer, ONE_SECOND, (TIMERPROC) TimerProc);
				wsaResult = WSARecvFrom(socketRecv, &DataBuf, 1, &BytesRecv, &Flags, (SOCKADDR*)&SenderAddr,
					&SenderAddrSize, &Overlapped, NULL);
				if (wsaResult != 0)
				{
					errorUDP = WSAGetLastError();
					if (errorUDP != WSA_IO_PENDING)
					{
						MessageBox(hwnd, "Well, double shit", NULL, NULL);
					}
					else {
						wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
						if (wsaResult == WSA_WAIT_FAILED) {
							wprintf(L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
						}

						wsaResult = WSAGetOverlappedResult(socketRecv, &Overlapped, &BytesRecv, FALSE, &Flags);
						if (wsaResult == FALSE)
						{
							errorUDP = WSAGetLastError();
						}
						else {
							numReceived++;
						}
					}
				}
				else
				{
					numReceived++;
				}
			}


///////////////////////////////
///////////////////////////////
///////////////////////////////
            if (wsaResult != 0)
            	{
            		errorUDP = WSAGetLastError();
            		if (errorUDP != WSA_IO_PENDING)
            		{
            			MessageBox(hwnd, "Well. Shit.", NULL, NULL);
            		}
            		else {
            			wsaResult = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
            			if (wsaResult == WSA_WAIT_FAILED) {
            				wprintf(L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
            			}

            			wsaResult = WSAGetOverlappedResult(socketRecv, &Overlapped, &BytesRecv, FALSE, &Flags);
            			if (wsaResult == FALSE)
            			{
            				errorUDP = WSAGetLastError();
            			}
            			else {
            				numReceived++;
            			}


            		}
            	}
