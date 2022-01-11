
#include<iostream>
#include<vector>
#include<string>
using namespace std;
#ifndef MessageHandling
#define MessageHandling

/**
*	@function: DivideMessage: Split a long message(contains many small message and separated by DELIMITER) into a vector of small message
*	@param res: a vector of string that'll contain the results
*	@param message: a long message received from sender(may be client or server)
*
*	return 1 if no errors
**/
int DivideMessage(vector<string>&res, string message);

/**
*	@function: checkMessage: Check if a string can convert to int or not and calculate the sum of its digit if possible
*	@param s: a string that is a message from client
*
*	return a string: sum of message's digits if it can be converted
*					"-Failed: String contains non-number character!" if it contains non-number character
**/
string checkMessage(string s);

/**
*	@function: FormatResponse: Format the result of response and display it to user's screen
*	@param request: list of string that client sends to server
*	@param response: list of string that are responses from server for each request.
*
*	return a string: none
**/
void FormatResponse(vector<string>request, vector<string>response);
#endif
#pragma once