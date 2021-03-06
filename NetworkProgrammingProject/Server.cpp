// WSAEventSelectServer.cpp : Defines the entry point for the console application.
//
#include<iostream>
#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include "MessageHandling.h"
#include "LoginFunction.h"
#include "Protocol.h"
#include"Communication.h"
#include<string>
#include "process.h"
#define PORT 5500
#define BUFF_SIZE 2048
#define SERVER_ADDR "127.0.0.1"
#define MAX_CLIENT 64
using namespace std;
//key of session is username, value of session is struct SocketInfo (null if user hasn't logged in)
map<account, SocketInfo> session;

//list of Group
vector<Group> groups;

CRITICAL_SECTION sessionCriticalSection, groupCriticalSection;

void ServerHandle(SocketInfo &clientManager, SocketInfo* client, DWORD &index, DWORD &nEvents, WSAEVENT* events)
{
	//receive message from clients
	vector<string> message = ReceiveMessage(client, index, nEvents, events);
	if (message.size() == 0) return;
	//handling each type of message
	if (message[0] == "USER")
	{
		account acc;
		acc.username = message[1];
		acc.password = message[2];
		string response = LoginHandling(session, acc, clientManager.clientAddr, clientManager.clientSock,sessionCriticalSection);
		cout << response << endl;
	}
	else if (message[0] == "QUIT")
	{
		sendMessage(clientManager.clientSock, deleteLoginSession(session, groups, clientManager,sessionCriticalSection,groupCriticalSection));
	}
	else if (message[0] == "POST")
	{
		PostMessage(session, message[1], message[2], message[3],sessionCriticalSection);
	}
	else if (message[0] == "LISTUSER")
	{
		getListUser(session, message[1], sessionCriticalSection);
	}
	else if (message[0] == "GROUP")
	{
		createGroup(groups, session, message, sessionCriticalSection, groupCriticalSection);
	}
	else if (message[0] == "LISTGROUP")
	{
		getListGroup(session, groups, message[1], sessionCriticalSection, groupCriticalSection);
	}
	else if (message[0] == "BROADCAST")
	{
		sendGroupMessage(session, groups, message[2], message[1], message[3], sessionCriticalSection, groupCriticalSection);
	}
	else if (message[0] == "ADD")
	{
		addGroupMember(session, groups, message, sessionCriticalSection, groupCriticalSection);
	}
	else if (message[0] == "LEAVE")
	{
		leaveGroup(session, groups, message[2], message[1], sessionCriticalSection, groupCriticalSection);
	}
}

unsigned __stdcall subThread(void *param)
{
	//initialize variables
	SocketInfo client[MAX_CLIENT];
	WSAEVENT	events[WSA_MAXIMUM_WAIT_EVENTS];
	DWORD		nEvents = 0;
	DWORD		index;
	WSANETWORKEVENTS sockEvent;
	char sendBuff[BUFF_SIZE], recvBuff[BUFF_SIZE];
	SOCKET connSock;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	int ret, i;
	int threadState = 0;
	//get listenSock from param
	SOCKET listenSock = ((SOCKET*)param)[0];
	//get connSock from param if exists( not equal INVALID_SOCKET)
	if (((SOCKET*)param)[1] != INVALID_SOCKET)
	{
		SOCKET connSock = ((SOCKET*)param)[1];
		//add connSock to array of client, then create an event and assign it to connSock with reading and closing event
		client[1].clientSock = connSock;
		events[1] = WSACreateEvent();
		cout << "Accept new client from " << endl;
		WSAEventSelect(client[1].clientSock, events[1], FD_READ | FD_CLOSE);
		nEvents++;
	}
	//set first element of client array with listenSock
	client[0].clientSock = listenSock;
	events[0] = WSACreateEvent(); //create new events
	nEvents++;

	// Assign an event types FD_ACCEPT and FD_CLOSE
	// with the listening socket and newEvent   
	WSAEventSelect(client[0].clientSock, events[0], FD_ACCEPT | FD_CLOSE);
	for (i = 1; i < WSA_MAXIMUM_WAIT_EVENTS; i++) {
		if (((SOCKET*)param)[1] != INVALID_SOCKET && i == 1) continue;
		client[i].clientSock = 0;
	}
	HANDLE subthread;
	while (1) {
		//wait for events on all socket
		index = WSAWaitForMultipleEvents(nEvents, events, FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) {
			printf("Error %d: WSAWaitForMultipleEvents() failed\n", WSAGetLastError());
			break;
		}
		//identify the position of client that has event happened
		index = index - WSA_WAIT_EVENT_0;
		//identify type of event happened
		WSAEnumNetworkEvents(client[index].clientSock, events[index], &sockEvent);
		//reset event 
		WSAResetEvent(events[index]);
		//event happened in listenSock
		if (index == 0)
		{
			//accept event happened
			if (sockEvent.lNetworkEvents & FD_ACCEPT) {
				if (sockEvent.iErrorCode[FD_ACCEPT_BIT] != 0) {
					printf("FD_ACCEPT failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}

				if ((connSock = accept(client[index].clientSock, (sockaddr *)&clientAddr, &clientAddrLen)) == SOCKET_ERROR) {
					printf("Error %d: Cannot permit incoming connection.\n", WSAGetLastError());
					continue;
				}
				int i;
				//check if number of events is out of MAXIMUM
				if (nEvents >= WSA_MAXIMUM_WAIT_EVENTS) {
					//check if there was no thread was created before
					if (threadState == 0)
					{
						SOCKET param[2];
						param[0] = listenSock;
						param[1] = connSock;
						printf("\nStart new thread\n");
						subthread= (HANDLE)_beginthreadex(0, 0, subThread, (void*)param, 0, 0);
						threadState = 1;
					}
				}	
				//Add new socket into array of clients
				else
				{
					client[nEvents].clientSock = connSock;
					events[nEvents] = WSACreateEvent();
					client[nEvents].clientAddr = clientAddr;
					cout << "Accept new client from " << endl;
					WSAEventSelect(client[nEvents].clientSock, events[nEvents], FD_READ | FD_CLOSE);
					nEvents++;
				}
			}
		}
		else
		{
			//read event happened
			if (sockEvent.lNetworkEvents & FD_READ) {
				//Receive message from client
				if (sockEvent.iErrorCode[FD_READ_BIT] != 0) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}
				//reset event
				WSAResetEvent(events[index]);
				//call function to handle
				ServerHandle(client[index],client,index,nEvents,events);
			}
			//close event happened
			if (sockEvent.lNetworkEvents & FD_CLOSE) {
				if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0) {
					printf("FD_CLOSE failed with error %d\n", sockEvent.iErrorCode[FD_CLOSE_BIT]);
				}
				//Release socket and event
				if (findUserNameBySocketInfo(session, client[index], sessionCriticalSection) != "")
				{
					deleteLoginSession(session, groups, client[index], sessionCriticalSection, groupCriticalSection);
				}

				closesocket(client[index].clientSock);
				WSACloseEvent(events[index]);
				//delete from array of clients and events
				client[index] = client[nEvents - 1];
				events[index] = events[nEvents - 1];
				client[nEvents - 1].clientSock = 0;
				events[nEvents - 1] = 0;
				nEvents--;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{

	//Step 1: Initiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Winsock 2.2 is not supported\n");
		return 0;
	}

	//Step 2: Construct LISTEN socket	
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);


	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error %d: Cannot associate a local address with server socket.", WSAGetLastError());
		return 0;
	}

	//Step 4: Listen request from client
	if (listen(listenSock, 10)) {
		printf("Error %d: Cannot place server socket in state LISTEN.", WSAGetLastError());
		return 0;
	}
	//read file Account
	int aut= Authenticate(session);
	if (aut == -1)
	{
		cout << "Wrong problem with database!" <<endl;
		return 0;
	}
	printf("Server started!\n");
	//initialize critical variable
	InitializeCriticalSection(&sessionCriticalSection);
	InitializeCriticalSection(&groupCriticalSection);
	SOCKET param[2];
	param[0] = listenSock;
	param[1] = INVALID_SOCKET;
	_beginthreadex(0, 0, subThread, (void*)param, 0, 0);
	while (1)
	{
		_getch();
	}
}