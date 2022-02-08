#include<iostream>
#include<string.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#include<vector>
#include<map>
using namespace std;
#ifndef Protocol
#define Protocol
#define BUFF_SIZE 4096
#define SPACE_1 "\r\n"
#define SPACE_2 char(6)
#define SPACE_3 char(17)
#define LOGIN_SUCCESS "10"
#define LOGIN_NO_ACCOUNT "11"
#define LOGIN_SERVER_ERROR "12"
#define LOGIN_YET "13"
#define POST_SUCCESS "20"
#define POST_FAIL_OFFLINE "21"
#define LOGOUT_SUCCESS "50"
#define GROUP_SUCCESS "30"
#define GROUP_FAIL_NAME "31"
#define ADD_SUCCESS "40"
#define ADD_FAIL_OFFLINE "41"
#define LEAVE_SUCCESS "60"
#define LEAVE_ALONE "61"
#define WSA_MAXIMUM_WAIT_EVENT 3
struct SocketInfo
{
	SOCKET clientSock;
	sockaddr_in clientAddr;
};
struct account
{
	string username;
	string password;
	bool operator < (const account &other) const
	{
		return username<other.username;
	}
};
struct Group
{
	//name of group
	string name;
	//name of group members
	vector<string> userMember;
};
#endif 