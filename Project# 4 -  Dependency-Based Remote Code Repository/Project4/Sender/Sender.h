#ifndef SENDER_H
#define SENDER_H
///////////////////////////////////////////////////////////////////////////
//  Sender.h - Sends files and messages.								 //
//  ver 1.0																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
///////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module is used to send files and messages. Acts as sender to both client and server

Public Interface:
=================
bool uploadFunction(HttpMessage msg, Socket& socket);
void sendMessage(HttpMessage & msg, Socket & socket);

Build Process:
==============
Required files
- Sockets.h, FileSystem.h, HttpMessage.h
Build commands (either one)
- devenv Project4.sln
Maintenance History:
====================
ver 1.0 : 9 Apr 16
- first release

*/
//
//Sender class

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>
#include <unordered_map>
class Sender
{
public:
	Sender(std::string addr, int portNo) : portNo(portNo), addr(addr) {};
	~Sender() {};
	std::string getFileName(const std::string & s);
	bool uploadData(HttpMessageFunc msg, Socket& socket);
	void sendMsg(HttpMessageFunc & msg, Socket & socket);
private:
	int portNo;
	std::string addr;
};
#endif