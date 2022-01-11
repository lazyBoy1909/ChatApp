#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include "Protocol.h"
using namespace std;
#ifndef MessageHandling
#define MessageHandling
vector<string> DivideMessage(string message);
SocketInfo& findUserInfo(map<account, SocketInfo> &session, string username);
string findUserNameBySocketInfo(map<account, SocketInfo>&session, SocketInfo& client);
vector<string> ReceiveMessage(SocketInfo* client, DWORD &index, DWORD &nEvents, WSAEVENT *events);
int sendMessage(SOCKET connectionSocket, string message);
#endif // !MessageHandling

