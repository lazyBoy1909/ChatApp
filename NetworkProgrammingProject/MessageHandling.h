#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include "Protocol.h"
using namespace std;
#ifndef MessageHandling
#define MessageHandling

/**
*	@function: DivideMessage: Split a long message(contains many small message and separated by DELIMITER) into a vector of small message
*	@param message(string): a long message received from sender(may be client or server)
*
*	return a vector of string contain all parts of message after being divided
**/
vector<string> DivideMessage(string message);

/**
*	@function: findUserInfo: Find the information(SocketInfo) of a user
*	@param session(map<account, SocketInfo>): a map that manages login session
*	@param username(string): Name of user need to find socket information
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*
*	return a SocketInfo value(clientSock is INVALID_SOCKET if not found)
**/
SocketInfo& findUserInfo(map<account, SocketInfo> &session, string username, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: findUserNameBySocketInfo: Find the information(name of user) of a user by SocketInfo
*	@param session(map<account, SocketInfo>): a map that manages login session
*	@param client(SocketInfo): Socket Information of user that need to find username
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*
*	return a string contains that username ("" if not found)
**/
string findUserNameBySocketInfo(map<account, SocketInfo>&session, SocketInfo& client, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: ReceiveMessage: Receive message from client
*	@param client(SocketInfo*): An array manages clients'information in current thread
*	@param index(DWORD) : position of client that sends message in array client
*	@param nEvents(DWORD): number of Events current thread manages
*	@param events(WSAEVENT*): an array manages events in current thread
*
*	return a vector of String contains all parts of message received from client
**/
vector<string> ReceiveMessage(SocketInfo* client, DWORD &index, DWORD &nEvents, WSAEVENT *events);

/**
*	@function: sendMessage: send message to client
*	@param connectionSocket(SOCKET) : Socket connected with the client'll be sent
*	@param message(string): message'll be sent to client
*
*	return 1 if success, 0 if error
**/
int sendMessage(SOCKET connectionSocket, string message);
#endif // !MessageHandling

