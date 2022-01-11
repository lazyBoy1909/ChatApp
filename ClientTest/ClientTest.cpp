#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<vector>
#include<string>
#include<sstream>
#include "MessageHandling.h"
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
#define SERVER_ADDR "127.0.0.1"
#define	BUFF_SIZE 1024
const string DELIMITER = "\r\n";
int main(int argc, char* argv[])
{
	//convert argv[1](ServerIPAddress to string)
	string ServerIPAddress = argv[1];
	//convert argv[2](Port to unsigned short)
	stringstream cv(argv[2]);
	unsigned short portNumber;
	cv >> portNumber;
	// Step1: Initiate WinSock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	if (WSAStartup(version, &data))
	{
		cout << "Cannot start WinSock! Error code: " << WSAGetLastError() << endl;
		return 0;
	}
	//Step2: Create Client Socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		cout << "Unable to create client socket! Error code: " << WSAGetLastError() << endl;
		return 0;
	}
	//Step 3: Connect to Server
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
	// request to connect to server
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		cout << "Unable to connect to server! Error code: " << WSAGetLastError() << endl;
		return 0;
	}
	cout << "Connected to server!" << endl;
	//Step 4: communicate with server
	string input;
	int  messageLen;
	while (1)
	{
		//input from User
		cout << "Type the message to server: " << endl;
		getline(cin, input);
		if (input.length() == 0) break;
		input += DELIMITER;
		messageLen = input.length();
		char tempStr[BUFF_SIZE];
		int leftBytes = messageLen;
		int index = 0;
		//send sequentially parts of message to server until no bytes message left
		while (leftBytes != 0)
		{
			strcpy_s(tempStr, leftBytes - index + 1, input.c_str());

			int ret1 = send(client, tempStr, strlen(tempStr), 0);
			if (ret1 == SOCKET_ERROR)
			{
				cout << "Cannot send message to Server! Error code: " << WSAGetLastError() << endl;
				break;
			}
			else if (ret1 == 0)
			{
				cout << "No bytes was sent to Server! Error code: " << WSAGetLastError() << endl;
				break;
			}
			else
			{
				leftBytes -= ret1;
				index += ret1;
			}
		}
		char message[BUFF_SIZE];
		//receive result from server
		string answer = "";
		while (1)
		{
			int ret = recv(client, message, BUFF_SIZE, 0);
			if (ret == SOCKET_ERROR)
			{
				cout << "Unable to receive result from server! Error code: " << WSAGetLastError() << endl;
				return 0;
			}
			else if (ret == 0)
			{
				cout << "Receive nothing from server! " << endl;
				return 0;
			}
			else
			{
				message[ret] = 0;
				answer += message;
				//receive from Server until the postfix is delimiter
				if (message[ret - 2] == '\r' && message[ret - 1] == '\n') break;
			}
		}
		// Formar result and display to user's screeen
		vector<string> result, request;
		cout << answer << endl;
	}
}