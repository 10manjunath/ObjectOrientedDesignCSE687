#ifndef MSG_SERVER_H
#define MSG_SERVER_H

///////////////////////////////////////////////////////////////////////////
// MsgServer.h - Demonstrates simple one-way HTTP style messaging		 //
//                 and file transfer									 //
//																		 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687                                  //
///////////////////////////////////////////////////////////////////////////
/*

*/

#include"../Receiver/Receiver.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Sender/Sender.h"
#include <string>
#include <iostream>
class MsgServer
{
public:
	
	void ServerSender();
	void ServerReceiver();
	bool connectSocketStatus(std::string addr, int portNo, SocketConnecter& si);
	MsgServer(std::string taddr = "localhost:8081", std::string tpath = "ServerRepository\\")
	{
		addr = taddr.substr(0, taddr.find(':'));
		port = std::stoi(taddr.substr(taddr.find(':') + 1, taddr.size()));
		path = tpath;
		FileSystem::Directory::setCurrentDirectory(path);
	}
	
private:
	std::string addr;
	BlockingQueue<HttpMessageFunc> &sendQueue = StaticBlockingQueue<HttpMessageFunc, 4>().getQueue();
	int port;
	std::string path;
};
#endif 

