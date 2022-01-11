#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<vector>
#include<string>
#include<sstream>
#include "process.h"
#include<map>
#include<fstream>
#include "LoginFunction.h"
#include "MessageHandling.h"
#pragma comment(lib,"Ws2_32.lib")
#define SERVER_ADDR "127.0.0.1"
#define	BUFF_SIZE 1024
#define DELIMITER "\r\n"
using namespace std;

//must be called when server starts
int Authenticate(map<account, SocketInfo> &session)
{
	// read from files
	int returnVal=0;
	ifstream logFile;
	logFile.open("account.txt");
	if (!logFile.is_open()) {
		return -1;
	}
	string username,password;
	while (!logFile.eof()) {
		logFile >> username;
		logFile >> password;
		//init first value of SocketInfo
		SocketInfo temp;
		temp.clientSock = INVALID_SOCKET;
		session.insert(pair<account, SocketInfo>({ username,password }, temp));
	}
	logFile.close();
	return returnVal;
}

string LoginHandling(map<account, SocketInfo>&session,account acc,sockaddr_in clientAddr, SOCKET clientSock)
{
	//no DB- no account exist
	string ans = "USER";
	ans += SPACE_2;
	int check = 0;
	map<account, SocketInfo>::iterator it;
	for (it = session.begin(); it != session.end(); it++)
	{
		if (it->first.username == acc.username && it->first.password == acc.password)
		{
			check = 1;
			break;
		}
	}
	if (check)
	{
		if (it->second.clientSock == INVALID_SOCKET)
		{
			it->second.clientSock = clientSock;
			it->second.clientAddr = clientAddr;
			//login success
			ans+= LOGIN_SUCCESS;
		}
		//this account had been logged in before
		else ans+= LOGIN_YET;
	}
	//account not existed
	else ans+= LOGIN_NO_ACCOUNT;
	sendMessage(clientSock, ans);
	return ans;
}

string LogoutHandling(map<account, SocketInfo> &session, string username)
{
	string ans = "QUIT";
	ans += SPACE_2;
	findUserInfo(session, username).clientSock = INVALID_SOCKET;
	ans+=LOGOUT_SUCCESS;
	return ans;
}
string deleteLoginSession(map<account, SocketInfo> &session, vector<Group> &groups, SocketInfo &client)
{
	string userName = findUserNameBySocketInfo(session, client);
	string res = LogoutHandling(session, userName);
	for (int i = 0; i < groups.size(); i++)
	{
		for (int k = 0; k < groups[i].userMember.size(); k++)
		{
			if (groups[i].userMember[k] == userName)
			{
				groups[i].userMember.erase(groups[i].userMember.begin() + k, groups[i].userMember.begin() + k + 1);
			}
		}
	}
	return res;
}