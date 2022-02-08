#include<iostream>
#include<vector>
using namespace std;
#ifndef LoginFunction
#define LoginFunction
#include"Protocol.h"
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<time.h>

/**
*	@function: Authenticate: Load database from file .txt
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*
*	return 0 if success, -1 if error
**/
int Authenticate(map<account, SocketInfo> &session);

/**
*	@function: LoginHandling: Handle log in request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param acc(account): username and password received from client
*	@param clientAddr(sockaddr_in): address of client
*	@param clientSock(SOCKET): Socket of client
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	return a string contains response'll be sent to client
**/
string LoginHandling(map<account, SocketInfo> &session, account acc,sockaddr_in clientAddr, SOCKET clientSock, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: LogoutHandling: Handle log out request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param username(string): username received from client
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	return a string contains response'll be sent to client
**/
string LogoutHandling(map<account, SocketInfo> &session,string username, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: deleteLoginSession: Delete all information of client's log in session
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param client(SocketInfo): Socket information of client
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
*	return a string contains response'll be sent to client
**/
string deleteLoginSession(map<account, SocketInfo> &session, vector<Group> &groups, SocketInfo &client, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);
#endif 
