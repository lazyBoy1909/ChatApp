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


int Authenticate(map<account, SocketInfo> &session);

string LoginHandling(map<account, SocketInfo> &session, account acc,sockaddr_in clientAddr, SOCKET clientSock, CRITICAL_SECTION &sessionCriticalSection);

string LogoutHandling(map<account, SocketInfo> &session,string username, CRITICAL_SECTION &sessionCriticalSection);

string deleteLoginSession(map<account, SocketInfo> &session, vector<Group> &groups, SocketInfo &client, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
#endif 
