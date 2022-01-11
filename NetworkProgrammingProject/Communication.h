#include<iostream>
#include<vector>
using namespace std;
#ifndef Communication
#define Communication
#include"Protocol.h"
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<WinSock2.h>
#include<WS2tcpip.h>

void PostMessage(map<account, SocketInfo> &session,string sender, string receiver, string message);
void getListUser(map<account, SocketInfo> &session, string sender);
void createGroup(vector<Group> &groups, map<account, SocketInfo> &session, vector<string> member);
void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender);
void sendGroupMessage(map<account, SocketInfo> &session,vector<Group> &groups, string groupName, string sender, string payload);
#endif 

