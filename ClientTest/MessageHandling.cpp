#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include"MessageHandling.h"
//More details comments in MessageHandling.h
int DivideMessage(vector<string>&res, string message)
{
	int len = message.length();
	int pos = 0;
	for (int i = 0; i < len - 1; i++)
	{
		if (message[i] == '\r' && message[i + 1] == '\n')
		{
			string temp = message.substr(0, i);
			res.push_back(temp);
			message.erase(0, i + 2);
			i = 0;
			len = message.length();
		}
	}
	return 1;
}
string checkMessage(string s)
{
	int sum = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] <'0' || s[i] > '9') return "-Failed: String contains non-number character!";
		else
		{
			sum += ((int)s[i]) - 48;
		}
	}
	stringstream ss;
	ss << sum;
	string res = ss.str();
	res.insert(res.begin(), '+');
	return res;
}
void FormatResponse(vector<string>request, vector<string>response)
{
	int n = response.size();
	for (int i = 0; i < n; i++)
	{
		if (response[i][0] == '+')
		{
			response[i].erase(0, 1);
			cout << "Convert " << request[i] << " to " << response[i] << endl;
		}
		else if (response[i][0] == '-')
		{
			response[i].erase(0, 1);
			cout << "Convert " << request[i] << ": " << response[i] << endl;
		}
	}
}