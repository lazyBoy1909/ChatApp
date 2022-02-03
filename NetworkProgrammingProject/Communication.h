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

void PostMessage(map<account, SocketInfo> &session,string sender, string receiver, string message, CRITICAL_SECTION &sessionCriticalSection);
void getListUser(map<account, SocketInfo> &session, string sender, CRITICAL_SECTION &sessionCriticalSection);
void createGroup(vector<Group> &groups, map<account, SocketInfo> &session, vector<string> member, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
void sendGroupMessage(map<account, SocketInfo> &session,vector<Group> &groups, string groupName, string sender, string payload, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
void addGroupMember(map<account, SocketInfo> &session, vector<Group> &groups, vector<string> message, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
void updateGroup(map<account, SocketInfo> &session,vector<Group> &groups, string nameMember, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
void leaveGroup(map<account, SocketInfo> &session, vector<Group> &groups, string groupName, string sender, CRITICAL_SECTION &sessionCriticalSection, CRITICAL_SECTION &groupCriticalSection);
#endif 

