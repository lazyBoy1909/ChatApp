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
void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender)
{
	string res = "LISTGROUP";
	for (int i = 0; i < groups.size(); i++)
	{
		vector<string> list = groups[i].userMember;
		vector<string>::iterator it = find(list.begin(), list.end(), sender);
		if (it != list.end())
		{
			res += SPACE_2;
			res += groups[i].name;
			for (int k = 0; k < list.size(); k++)
			{
				res += SPACE_3;
				res += list[k];
			}
		}
	}
	sendMessage(findUserInfo(session, sender).clientSock, res);
}
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
	int siz = groups.size();
	for (int k = 0; k< groups[siz-1].userMember.size(); k++)
	{
		getListGroup(session, groups, groups[siz-1].userMember[k]);
	}
	//sendMessage(findUserInfo(session, message[2]).clientSock, res);
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
void updateGroup(map<account, SocketInfo> &session,vector<Group> &groups, string nameMember)
{
	for (int i = 0; i < groups.size(); i++)
	{
		vector<string> list = groups[i].userMember;
		vector<string>::iterator it;
		it = find(list.begin(), list.end(), nameMember);
		if (it != list.end())
		{
			for (int k = 0; k<list.size(); k++)
			{
				getListGroup(session, groups, list[k]);
			}
		}
	}
}
void addGroupMember(map<account, SocketInfo> &session, vector<Group> &groups, vector<string> message)
{
	string res = "ADD";
	res += SPACE_2;
	res += ADD_SUCCESS;
	int i, k;
	for (i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == message[2])
		{
			for (k = 3; k < message.size(); k++)
			{
				// if guest is online
				if (findUserInfo(session, message[k]).clientSock != INVALID_SOCKET)
				{
					//if guest is not in group then add
					vector<string>::iterator it = find(groups[i].userMember.begin(), groups[i].userMember.end(), message[k]);
					if (it == groups[i].userMember.end())
					{
						//add to group
						groups[i].userMember.push_back(message[k]);
						res += message[k];
					}
				}
			}			
			break;
		}
	}
	//sendMessage(findUserInfo(session, message[1]).clientSock, res);
	//send message update group's infor to all members
	for (int j = 0; j < groups[i].userMember.size(); j++)
	{
		getListGroup(session, groups, groups[i].userMember[j]);
	}
}

void leaveGroup(map<account, SocketInfo> &session, vector<Group> &groups, string groupName, string sender)
{
	string res = "LEAVE";
	res += SPACE_2;
	res += LEAVE_SUCCESS;
	int i;
	for (i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == groupName) break;
	}
	vector<string>::iterator it = find(groups[i].userMember.begin(), groups[i].userMember.end(), sender);
	groups[i].userMember.erase(it, it + 1);
	sendMessage(findUserInfo(session, sender).clientSock, res);
	for (int k = 0; k < groups[i].userMember.size(); k++)
	{
		printf("%s\n", groups[i].userMember[k]);
		getListGroup(session, groups, groups[i].userMember[k]);
	}
}