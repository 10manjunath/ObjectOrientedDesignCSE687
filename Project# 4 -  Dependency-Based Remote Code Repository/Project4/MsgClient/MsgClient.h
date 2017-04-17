#ifndef MSGCLIENT_H
#define MSGCLIENT_H
///////////////////////////////////////////////////////////////////////////
// MsgClient.h - Demonstrates simple one-way HTTP messaging				 //
// ver: 1.0																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,        //
///////////////////////////////////////////////////////////////////////////
/*

*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Sender/Sender.h"
#include"../StaticBlockingQueue/StaticBlockingQueue.h"
#include <string>
#include <iostream>
#include <thread>
#include "../Receiver/Receiver.h"

class MsgClient
{
public:
	
	void ClientSender();
	bool connectSocketStatus(std::string addr, int portNo, SocketConnecter & si);
	void ClientReceiver();
	MsgClient(std::string taddr = "localhost:8080", std::string tpath = "ClientRepository\\" )
	{
		addr = taddr.substr(0, taddr.find(':'));
		port = std::stoi(taddr.substr(taddr.find(':') + 1, taddr.size()));		
		path = tpath;
	}	
	void HandleCommonFunction(std::string msg);
	void HandleCheckIn(std::string msg, std::string cmd);
	void StartClientSenRec();
private:
	std::string addr;	
	BlockingQueue<HttpMessageFunc> &sendQueue = StaticBlockingQueue<HttpMessageFunc, 4>().getQueue();	//store the static sending blocking queue
	int port;
	std::string path;
};
#endif