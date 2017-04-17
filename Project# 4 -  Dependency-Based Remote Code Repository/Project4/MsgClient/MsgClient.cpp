///////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple one-way HTTP messaging			 //
// ver: 1.0																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository, CSE687          //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,        //
///////////////////////////////////////////////////////////////////////////
/*
Module Operations:
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*/


#include"MsgClient.h"

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;


class ClientCounter
{
public:
	ClientCounter() { ++clientCount; }
	size_t count() { return clientCount; }
private:
	static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;



void MsgClient::ClientSender()
{
	try
	{
		SocketConnecter si;
		Sender send(this->addr, this->port);
		
		HttpMessageFunc popValue;	
		while (true)
		{
			popValue = sendQueue.deQ();
			std::string s = popValue.findValue("ToAddr");
			std::string delimiter = ":";
			this->addr = s.substr(0, s.find(delimiter));
			std::string d = s.substr(s.find(delimiter) + 1, s.size());
					
			if (popValue.findValue("Command") == "CheckIn")
			{
				//Upload files
				if (connectSocketStatus(this->addr,stoi(d), si))
				{
					send.uploadData(popValue, si);
				}
			}
			
			else if(popValue.findValue("Command") == "Extract" || popValue.findValue("Command") == "GetPackageList" || popValue.findValue("Command") == "TestMessage")
			{
				if (connectSocketStatus(this->addr, stoi(d), si))
				{
					send.sendMsg(popValue, si);
				}
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

bool MsgClient::connectSocketStatus(std::string addr, int portNo, SocketConnecter& si)
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

void MsgClient::HandleCommonFunction(std::string msg)
{
	std::istringstream lstream(msg);
	std::string cmd,serverport;
	lstream >> cmd;
	lstream >> serverport;
	if (cmd == "CheckIn")
	{
		HandleCheckIn(msg,cmd);
	}
	else if( cmd == "Extract")
	{
		std::string packageName, isDepNeeded, author;
		lstream >> author;
		lstream >> packageName;
		lstream >> isDepNeeded; HttpMessageFunc httpmsg;
		httpmsg.addAttribute(HttpMessageFunc::attribute("Command", cmd));		
		httpmsg.addAttribute(HttpMessageFunc::attribute("ToAddr", serverport));
		httpmsg.addAttribute(HttpMessageFunc::attribute("FromAddr", this->addr+":" + std::to_string(this->port)));
		httpmsg.addAttribute(HttpMessageFunc::attribute("Author", author));
		httpmsg.addAttribute(HttpMessageFunc::attribute("PackageName", packageName));
		httpmsg.addAttribute(HttpMessageFunc::attribute("IsDepNeeded", isDepNeeded));
		sendQueue.enQ(httpmsg);
	}
	else if (cmd == "GetPackageList")
	{		
		HttpMessageFunc httpmsg;
		httpmsg.addAttribute(HttpMessageFunc::attribute("Command", cmd));
		httpmsg.addAttribute(HttpMessageFunc::attribute("ToAddr", serverport));
		httpmsg.addAttribute(HttpMessageFunc::attribute("FromAddr", this->addr + ":" + std::to_string(this->port)));
		sendQueue.enQ(httpmsg);
	}
	else if (Utilities::StringHelper::trim(cmd) == "TestMessage")
	{
		std::string rpackageName, isDepNeeded, testMsg;
		lstream >> testMsg;
		HttpMessageFunc httpmsg;
		httpmsg.addAttribute(HttpMessageFunc::attribute("Command", cmd));
		httpmsg.addAttribute(HttpMessageFunc::attribute("ToAddr", serverport));
		httpmsg.addAttribute(HttpMessageFunc::attribute("FromAddr", this->addr + ":" + std::to_string(this->port)));
		httpmsg.addAttribute(HttpMessageFunc::attribute("Message", testMsg));
		sendQueue.enQ(httpmsg);	
	}
}

void MsgClient::HandleCheckIn(std::string msg,std::string cmd)
{
	std::istringstream lstream(msg);
	std::string fileName,serverport, typeOfComm, dependencyNames, author, packageName, closedProp;
	lstream >> cmd;
	lstream >> serverport;
	lstream >> author;
	lstream >> packageName;
	lstream >> closedProp; 
	lstream >> typeOfComm;
	lstream >> fileName;
	lstream >> dependencyNames;
	HttpMessageFunc httpmsg;
	httpmsg.addAttribute(HttpMessageFunc::attribute("Command", cmd));
	httpmsg.addAttribute(HttpMessageFunc::attribute("ToAddr", serverport));
	httpmsg.addAttribute(HttpMessageFunc::attribute("FromAddr", this->addr + ":" + std::to_string(this->port)));
	httpmsg.addAttribute(HttpMessageFunc::attribute("Author", author));
	httpmsg.addAttribute(HttpMessageFunc::attribute("PackageName", packageName));
	httpmsg.addAttribute(HttpMessageFunc::attribute("ClosedProp", closedProp));
	httpmsg.addAttribute(HttpMessageFunc::attribute("Mode", typeOfComm));
	httpmsg.addAttribute(HttpMessageFunc::attribute("FileName", fileName));
	if(dependencyNames !="")
	httpmsg.addAttribute(HttpMessageFunc::attribute("DepName", dependencyNames));
	if (fileName.size() > 0)
	{
		httpmsg.addAttribute(HttpMessageFunc::attribute("content_length", Converter<size_t>::toString(fileName.size())));
	}
	sendQueue.enQ(httpmsg);
}


void MsgClient::StartClientSenRec()
{
	std::thread Sender(&MsgClient::ClientSender, this);	
	std::thread Receiver(&MsgClient::ClientReceiver, this);
	Sender.detach();
	Receiver.detach();
}


void MsgClient::ClientReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);
		Receiver cp(port, this->addr, true, path);
		sl.start(cp);
		std::cin.get();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----< entry point - runs two clients each on its own thread >------
#ifdef TEST_MSGCLIENT
int main()
{
	::SetConsoleTitle(L"Clients Running on Threads");
	Show::title("Demonstrate two HttpMessage Clients each running on a child thread");
	using CallObj = std::function<void()>;
	BlockingQueue<CallObj*> _workItemQueue; //blocking queue to hold the work items
	CallObj* p3 = new CallObj;
	_workItemQueue.enQ(p3);
}
#endif