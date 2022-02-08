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

/**
*	@function: PostMessage: Handle sending message request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param sender(string): username of sender
*	@param receiver(string): username of receiver
*	@param message(string) : message sender sends to receiver
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
**/
void PostMessage(map<account, SocketInfo> &session,string sender, string receiver, string message, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: getListUser: Handle getting list of current users request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param sender(string): username of sender
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
**/
void getListUser(map<account, SocketInfo> &session, string sender, CRITICAL_SECTION sessionCriticalSection);

/**
*	@function: createGroup: Handle creating a new group request from client
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param member(vector<string>): list group members 
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
**/
void createGroup(vector<Group> &groups, map<account, SocketInfo> &session, vector<string> member, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);

/**
*	@function: getListGroup: Handle getting list of current groups request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param sender(string): username of sender
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
**/
void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);

/**
*	@function: sendGroupMessage: Handle sending message to a group request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param groupName: name of group that client sends message to
*	@param sender(string): username of sender
*	@param payload(string): message from sender to group
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
**/
void sendGroupMessage(map<account, SocketInfo> &session,vector<Group> &groups, string groupName, string sender, string payload, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);

/**
*	@function: addGroupMember: Handle adding new members to a group request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param message(vector<string): all part of request from client
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
**/
void addGroupMember(map<account, SocketInfo> &session, vector<Group> &groups, vector<string> message, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);

/**
*	@function: leaveGroup: Handle leaving group request from client
*	@param session(map<account, SocketInfo>): a map stores account's information and manages login session
*	@param groups(vector<Group>): a vector manages all groups' information
*	@param groupName: name of group that client want to leave
*	@param sender(string): username of sender
*	@param sessionCriticalSection(CRITICAL_SECTION): Critical Variable represents login session
*	@param groupCriticalSection(CRITICAL_SECTION): Critical Variable represents groups' information
**/
void leaveGroup(map<account, SocketInfo> &session, vector<Group> &groups, string groupName, string sender, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection);
#endif 

