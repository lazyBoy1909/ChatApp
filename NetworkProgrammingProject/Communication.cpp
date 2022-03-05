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
void getListGroup(map<account, SocketInfo> &session, vector<Group> &groups, string sender, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection)
{
	string res = "LISTGROUP";
	EnterCriticalSection(&groupCriticalSection);
	//get all groups that have sender as a member
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
	LeaveCriticalSection(&groupCriticalSection);
	//send result to sender
	sendMessage(findUserInfo(session, sender,sessionCriticalSection).clientSock, res);
}
void PostMessage(map<account, SocketInfo> &session,string sender, string receiver, string message, CRITICAL_SECTION sessionCriticalSection)
{
	string temp1 = "POST";
	 temp1+= SPACE_2;
	 string temp2 = "MESSAGEUSER";
	 temp2 += SPACE_2;
	 temp2 += sender;
	 temp2 += SPACE_2;
	 temp2 += message;
	 //get receiver's socket information
	 SOCKET sock = findUserInfo(session, receiver, sessionCriticalSection).clientSock;
	 //receiver is offline
	 if (sock == INVALID_SOCKET)
	 {
		 temp1 += POST_FAIL_OFFLINE;
	 }
	 else
	 {
		 temp1 += POST_SUCCESS;
		 sendMessage(findUserInfo(session, receiver, sessionCriticalSection).clientSock, temp2);
	 }
	 //send message to receiver
	 sendMessage(findUserInfo(session, sender, sessionCriticalSection).clientSock, temp1);
}
void getListUser(map<account, SocketInfo> &session, string sender, CRITICAL_SECTION sessionCriticalSection)
{
	string res = "LISTUSER";
	EnterCriticalSection(&sessionCriticalSection);
	map<account, SocketInfo>::iterator it;
	//get list of online users
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->second.clientSock != INVALID_SOCKET)
		{
			res += SPACE_2;
			res += it->first.username;
		}
	}
	LeaveCriticalSection(&sessionCriticalSection);
	sendMessage(findUserInfo(session, sender, sessionCriticalSection).clientSock, res);
}

void createGroup(vector<Group> &groups, map<account, SocketInfo> &session, vector<string> message, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection)
{
	string res = "GROUP";
	res += SPACE_2;
	EnterCriticalSection(&groupCriticalSection);
	//check if group's name is existed
	for (int i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == message[1])
		{
			res += GROUP_FAIL_NAME;
			sendMessage(findUserInfo(session, message[2],sessionCriticalSection).clientSock, res);
			return;
		}
	}
	//create new group with group's name
	res += GROUP_SUCCESS;
	Group newGroup;
	newGroup.userMember.clear();
	newGroup.name = message[1];
	//add all online members to new groups
	for (int i = 2; i < message.size(); i++)
	{
		if(findUserInfo(session,message[i],sessionCriticalSection).clientSock != INVALID_SOCKET)
		newGroup.userMember.push_back(message[i]);
	}
	//add new group to list of current groups
	groups.push_back(newGroup);
	LeaveCriticalSection(&groupCriticalSection);
	int siz = groups.size();
	//send message to notify all members in new group
	for (int k = 0; k< groups[siz-1].userMember.size(); k++)
	{
		getListGroup(session, groups, groups[siz-1].userMember[k],sessionCriticalSection, groupCriticalSection);
	}
	//send response to request's sender
	sendMessage(findUserInfo(session, message[2], sessionCriticalSection).clientSock, res);
}


void sendGroupMessage(map<account, SocketInfo> &session,vector<Group> &groups, string groupName, string sender, string payload, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection)
{
	string response = "MESSAGEGROUP";
	response += SPACE_2;
	response += groupName;
	response += SPACE_2;
	response += sender;
	response += SPACE_2;	
	response += payload;
	EnterCriticalSection(&groupCriticalSection);
	//find group with group's name
	for (int i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == groupName)
		{
			for (int k = 0; k < groups[i].userMember.size(); k++)
			{
				//send message to all members of groups except sender
				if (groups[i].userMember[k] != sender)
				{
					sendMessage(findUserInfo(session, groups[i].userMember[k],sessionCriticalSection).clientSock, response);
				}
			}
		}
	}
	LeaveCriticalSection(&groupCriticalSection);
}
void addGroupMember(map<account, SocketInfo> &session, vector<Group> &groups, vector<string> message, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection)
{
	string res = "ADD";
	res += SPACE_2;
	res += ADD_SUCCESS;
	int i, k;
	EnterCriticalSection(&groupCriticalSection);
	//find group with group's name
	for (i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == message[2])
		{
			for (k = 3; k < message.size(); k++)
			{
				// if guest is online
				if (findUserInfo(session, message[k], sessionCriticalSection).clientSock != INVALID_SOCKET)
				{
					//if guest is not in group then add
					vector<string>::iterator it = find(groups[i].userMember.begin(), groups[i].userMember.end(), message[k]);
					if (it == groups[i].userMember.end())
					{
						//add to group
						groups[i].userMember.push_back(message[k]);
						res += SPACE_2;
						res += message[k];
					}
				}
			}			
			break;
		}
	}
	LeaveCriticalSection(&groupCriticalSection);
	sendMessage(findUserInfo(session, message[1],sessionCriticalSection).clientSock, res);
	//send message update group's infor to all members
	for (int j = 0; j < (groups[i].userMember).size(); j++)
	{
		getListGroup(session, groups, (groups[i].userMember)[j],sessionCriticalSection,groupCriticalSection);
	}
}

void leaveGroup(map<account, SocketInfo> &session, vector<Group> &groups, string groupName, string sender, CRITICAL_SECTION sessionCriticalSection, CRITICAL_SECTION groupCriticalSection)
{
	string res = "LEAVE";
	res += SPACE_2;
	int i;
	EnterCriticalSection(&groupCriticalSection);
	//find group with group's name
	for (i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == groupName) break;
	}
	//if not found group
	if (i == groups.size()) return;
	//find sender in list of members in that group
	vector<string>::iterator it = find(groups[i].userMember.begin(), groups[i].userMember.end(), sender);
	//no member
	if (groups[i].userMember.size() == 0)
	{
		return;
	}
	//sender is the last member in the group
	else if (groups[i].userMember.size() == 1 ||(groups[i].userMember.size() == 2 && (groups[i].userMember[0] == sender || groups[i].userMember[1] == sender)))
	{
		//delete that group
		groups[i].userMember.erase(it, it + 1);
		groups.erase(groups.begin() + i);
		res += LEAVE_SUCCESS;
		res += SPACE_2;
		res += groupName;
		//send response to request's sender
		sendMessage(findUserInfo(session, sender,sessionCriticalSection).clientSock, res);
		return;
	}
	else
	{
		groups[i].userMember.erase(it, it + 1);
		res += LEAVE_SUCCESS;
		res += SPACE_2;
		res += groupName;
	}
	LeaveCriticalSection(&groupCriticalSection);
	//send response to request's sender
	sendMessage(findUserInfo(session, sender, sessionCriticalSection).clientSock, res);
	//send list group to all member left in that group
	for (int k = 0; k < groups[i].userMember.size(); k++)
	{
		printf("%s\n", groups[i].userMember[k]);
		getListGroup(session, groups, groups[i].userMember[k], sessionCriticalSection, groupCriticalSection);
	}
}