#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include "Protocol.h"
#include "LoginFunction.h"
#include<map>
using namespace std;
vector<string> DivideMessage(string message)
{
	vector<string> res;
	int pos;
	for (int i = 0; i<message.length(); i++)
	{
		if (message[i] == SPACE_2)
		{
			string temp = message.substr(0, i);
			res.push_back(temp);
			message.erase(0, i + 1);
			i = 0;
		}
		if (message[i] == '\r' && message[i+1] == '\n')
		{
			string temp = message.substr(0, i);
			res.push_back(temp);
			break;
		}
	}
	return res;
}
vector<string> ReceiveMessage(SocketInfo* client, DWORD &index, DWORD &nEvents, WSAEVENT *events)
{
	string answer = "";
	char buff[BUFF_SIZE];
	while (1)
	{
		int ret = recv(client[index].clientSock, buff, BUFF_SIZE, 0);
		if (ret <= 0)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				cout << "Client disconnected! " << endl;
				closesocket(client[index].clientSock);
				WSACloseEvent(events[index]);
				client[index] = client[nEvents - 1];
				events[index] = events[nEvents - 1];
				client[nEvents - 1].clientSock = 0;
				events[nEvents - 1] = 0;
				nEvents--;
				break;
			}
		}
		else
		{
			buff[ret] = 0;
			answer += buff;
			if (buff[ret - 2] == '\r' && buff[ret - 1] == '\n') break;
		}
	}
	cout << answer << endl;
	vector<string> res;
	res= DivideMessage(answer);
	return res;
}

int sendMessage(SOCKET connectionSocket, string message)
{
	message += (string)SPACE_1;
	char tempStr[BUFF_SIZE];
	int leftBytes = message.length();
	int index = 0;
	//send until no bytes message left
	while (leftBytes != 0)
	{
		strcpy_s(tempStr, leftBytes - index + 1, message.c_str());
		int ret1 = send(connectionSocket, tempStr, strlen(tempStr), 0);
		if (ret1 == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				cout << "Cannot send message to Client! Error code: " << WSAGetLastError() << endl;
				return 0;
			}
		}
		else if (ret1 == 0)
		{
			cout << "No bytes was sent to Client! Error code: " << WSAGetLastError() << endl;
			return 0;
		}
		else
		{
			leftBytes -= ret1;
			index += ret1;
		}
	}
	return 1;
}
//find Socket information are loggin with username
SocketInfo& findUserInfo(map<account, SocketInfo> &session, string username, CRITICAL_SECTION sessionCriticalSection)
{
	SocketInfo tmp;
	tmp.clientSock == INVALID_SOCKET;
	EnterCriticalSection(&sessionCriticalSection);
	map<account, SocketInfo>::iterator it;
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->first.username == username)
		{
			return it->second;
		}
	}
	LeaveCriticalSection(&sessionCriticalSection);
	return tmp;
}

string findUserNameBySocketInfo(map<account, SocketInfo>&session, SocketInfo& client, CRITICAL_SECTION sessionCriticalSection)
{
	EnterCriticalSection(&sessionCriticalSection);
	map<account, SocketInfo>::iterator it;
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->second.clientSock == client.clientSock)
		{
			return it->first.username;
		}
	}
	LeaveCriticalSection(&sessionCriticalSection);
	return "";
}
Group getGroupByName(vector<Group> groups, string name)
{
	for (int i = 0; i < groups.size(); i++)
	{
		if (groups[i].name == name) return groups[i];
	}
	Group tmp;
	tmp.name = "";
}
