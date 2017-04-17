///////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Simple one-way HTTP style messaging and file transfer //								 //
//																		 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,        //
///////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*/
/*
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/
#include "MsgServer.h"

using Show = StaticLogger<1>;
using namespace Utilities;


void MsgServer::ServerSender()
{
	try
	{
		SocketConnecter si;
		Sender send(this->addr, this->port);

		HttpMessageFunc popValue;
		BlockingQueue<HttpMessageFunc> &q1 = StaticBlockingQueue<HttpMessageFunc, 1>().getQueue();
		while (true)
		{
			popValue = q1.deQ();
			
			std::string s = popValue.findValue("FromAddr");
			std::string delimiter = ":";
			this->addr = s.substr(0, s.find(delimiter));
			std::string d = s.substr(s.find(delimiter) + 1, s.size());
		
			if (connectSocketStatus(this->addr, stoi(d), si))
			{
				if (popValue.findValue("Command") == "GetPackageList")
					send.sendMsg(popValue, si);
				else if (popValue.findValue("Command") == "TestMessage")
				{
					send.sendMsg(popValue, si);
				}
				else if (popValue.findValue("Command") == "CheckIn")
				{
					send.sendMsg(popValue, si);
				}
				else 
					send.uploadData(popValue, si);
			}
			
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

void MsgServer::ServerReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);
		Receiver cp(port, addr, false, path);
		sl.start(cp);
		std::cin.get();
		std::cout << "received message";
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

bool MsgServer::connectSocketStatus(std::string addr, int portNo, SocketConnecter& si)
{
	try
	{
		
		int noOfTries = 0;
		while (!si.connect(addr, portNo))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100); noOfTries++;
			if (noOfTries == 3) { return false; }
		}
		Show::write("\n server connected to Address:" + addr + "PortNo:" + std::to_string(portNo) + "\n");
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	return true;
}

//----< test stub >--------------------------------------------------

int main(int argc, char* argv[])
{
	try
	{
		MsgServer ms(argv[1], argv[2]);
		std::cout << "Server is up and running";
		std::thread Sender(&MsgServer::ServerSender, ms);
		std::thread Receiver(&MsgServer::ServerReceiver, ms); 
		Sender.join();
		Receiver.join();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Server Response Failed.\n ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}