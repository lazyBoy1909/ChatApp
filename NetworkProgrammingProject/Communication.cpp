#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include "Protocol.h"
#include"MessageHandling.h"
#include<map>
using namespace std;
//vector[0] for sender, vector[1] for receiver
void PostMessage(map<account, SocketInfo> &session,string sender, string receiver, string message)
{
	string temp1 = "POST";
	 temp1+= SPACE_2;
	 string temp2 = "MESSAGEUSER";
	 temp2 += SPACE_2;
	 temp2 += sender;
	 temp2 += SPACE_2;
	 temp2 += message;
	 SOCKET sock = findUserInfo(session, receiver).clientSock;
	 if (sock == INVALID_SOCKET)
	 {
		 temp1 += POST_FAIL_OFFLINE;
	 }
	 else
	 {
		 temp1 += POST_SUCCESS;
		 sendMessage(findUserInfo(session, receiver).clientSock, temp2);
	 }
	 sendMessage(findUserInfo(session, sender).clientSock, temp1);
}
void getListUser(map<account, SocketInfo> &session, string sender)
{
	string res = "LISTUSER";
	map<account, SocketInfo>::iterator it;
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->second.clientSock != INVALID_SOCKET)
		{
			res += SPACE_2;
			res += it->first.username;
		}
	}
	sendMessage(findUserInfo(session, sender).clientSock, res);
}

void createGroup(vector<Group> &groups, map<account, SocketInfo> &session, vector<string> message)
{
	string res = "GROUP";
	res += SPACE_2;
	for (int i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == message[1])
		{
			res += GROUP_FAIL_NAME;
			sendMessage(findUserInfo(session, message[2]).clientSock, res);
			return;
		}
	}
	res += GROUP_SUCCESS;
	Group newGroup;
	newGroup.userMember.clear();
	newGroup.name = message[1];
	for (int i = 2; i < message.size(); i++)
	{
		newGroup.userMember.push_back(message[i]);
	}
	groups.push_back(newGroup);
	sendMessage(findUserInfo(session, message[2]).clientSock, res);
}

void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender)
{
	string res = "LISTGROUP";
	for (int i=0; i < groups.size(); i++)
	{
		res += SPACE_2;
		res += groups[i].name;
		for (int k = 0; k < groups[i].userMember.size(); k++)
		{
			res += SPACE_3;
			res += groups[i].userMember[k];
		}
	}
	sendMessage(findUserInfo(session, sender).clientSock, res);
}

void sendGroupMessage(map<account, SocketInfo> &session,vector<Group> &groups, string groupName, string sender, string payload)
{
	string response = "MESSAGEGROUP";
	response += SPACE_2;
	response += groupName;
	response += SPACE_2;
	response += sender;
	response += SPACE_2;	
	response += payload;
	for (int i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == groupName)
		{
			for (int k = 0; k < groups[i].userMember.size(); k++)
			{
				sendMessage(findUserInfo(session, groups[i].userMember[k]).clientSock, response);
			}
		}
	}
}