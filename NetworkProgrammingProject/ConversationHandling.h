#include<iostream>
#include<vector>
using namespace std;
#ifndef ConversationHandling
#define ConversationHandling
#include"Protocol.h""
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include "MessageHandling.h"
//message is the username of person who sent request
string getListUser(string username);
string getListGroup(string username);
string forwardUserMessage(string receiver, string message);
void forwardGroupMessage(string groupname);
#endif 
