#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	char login[26] = "Please enter the Username";
	char admin[2] = "e";
	char pass1[43] = "Username Crrect, Please enter the Password";
	char pass[2] = "5";
	char pass2[33] = "Login Successfuly,Welcome, Admin";

	printf("Server is Running,\nWaiting for Connections.\n");

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);
	int r = 0;
	// Receive until the peer shuts down the connection
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) 
		{
			char username[3];
			char test2[6];
			char FailPass[34] = "Wrong Password, Please try again";
			char FailUser[34] = "Wrong Username, Please try again";
			int f = 1;
			int c = 1;
			

			printf("\nConnection received\n\nWaiting for Login\n");
			iSendResult = send(ClientSocket, login, 100, 0);

			do{
				iResult = recv(ClientSocket, username, 1, 0);
				int u = memcmp(username, admin, 1);

				if (f = 1 && u != 0)
				{
					printf("\nLogin Failed, Wrong Username\n");
					iSendResult = send(ClientSocket, FailUser, 100, 0);
				}
				else
				{
					f = 0;
					iSendResult = send(ClientSocket, pass1, 42, 0);
					do{
						iResult = recv(ClientSocket, test2, 1, 0);
						int r = memcmp(test2, pass, 1);

						if (c = 1 && r != 0)
						{
							printf("\nLogin Failed, Wrong Password\n");
							iSendResult = send(ClientSocket, FailPass, 100, 0);
						}
						else
						{
							printf("\nCorrect Username, Waiting for Password\n");
							r = 1;
							c = 0;
							char testBuffer[23] = "Admin Login Successful";
							iSendResult = send(ClientSocket, testBuffer, 22, 0);
							printf("\nAdmin Login Successful\n");
							break;
						}
					} while (c = 1);
					break;
				}
			} while (f = 1);
	}
		if (r = 1 && iResult > 0){
			do{
				iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
				printf("Bytes received: %d\n", iResult);
				//printf("recvbuflen received: %d\n", recvbuflen);
				printf("recvbuf received: %.*s\n", iResult, recvbuf); //usage of %.*s allows me to get only relevent data from my data

				// Echo the buffer back to the sender
				iSendResult = send(ClientSocket, recvbuf, iResult, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d\n", iSendResult);
			} while (r = 1);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}