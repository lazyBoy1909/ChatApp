#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include "Protocol.h"
#include "MessageHandling.h"
using namespace std;

string getListUser(string username)
{
	string ret = "LISTUSER";
	map<account, SocketInfo>::iterator it;
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->second.clientSock != INVALID_SOCKET)
		{
			ret += SPACE_2;
			ret += it->first.username;
		}
	}
	ret += SPACE_1;
	return ret;
}

string getListGroup(string username)
{
	string ret = "LISTGROUP";
	for (int i = 0; i < groups.size(); i++)
	{
		for (int j = 0; j < groups[i].userMember.size(); j++)
		{
			if ((groups[i].userMember)[j] == username)
			{
				ret += SPACE_2;
				ret += groups[i].name;
				//add all other members of the group that has username as a member
				for (int k = 0; k < groups[i].userMember.size(); k++)
				{
					if ((groups[i].userMember)[k] != username)
					{
						ret += SPACE_3;
						ret += (groups[i].userMember)[k];
					}
				}
			}
		}
	}
	ret += SPACE_1;
	return ret;
}

string forwardUserMessage(string receiver, string message)
{
	string ret = "POST";
	ret += SPACE_2;
	SocketInfo dest = findUserInfo(receiver);
	char tempStr[BUFF_SIZE];
	int leftBytes = message.length();
	int index = 0;
	//send until no bytes message left
	while (leftBytes != 0)
	{
		strcpy_s(tempStr, leftBytes - index + 1, message.c_str());
		int ret1 = send(dest.clientSock, tempStr, strlen(tempStr), 0);
		if (ret1 <=0)
		{
			cout << "Failed to forward message to " << receiver.c_str() << endl;
			ret += POST_FAIL_OFFLINE;
			return ret;
		}
		else
		{
			leftBytes -= ret1;
			index += ret1;
		}
	}
	ret += POST_SUCCESS;
	return ret;
}

void forwardGroupMessage(string groupname)
{
	for (int i = 0; i < groups.size(), i++)
	{
		if (groups[i].name == groupname)
		{

		}
	}
}