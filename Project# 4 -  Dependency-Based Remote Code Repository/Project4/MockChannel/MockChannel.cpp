///////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015			 //
// - build as DLL to show how C++\CLI client can use native code channel //
// - MockChannel reads from sendQ and writes to recvQ					 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,        //
/////////////////////////////////////////////////////////////////////////// 
#define IN_DLL
#include "MockChannel.h"
#include "../MsgClient/MsgClient.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>


using BQueue = BlockingQueue < std::string >;


class MockChannel : public IMockChannel
{
public:
	MockChannel(ISenderClass* pSendr, IRecvr* pRecvr);
	void uploadFilesToClient(std::string, IRecvr * pRecvr);	
	void stop();
	void connectOps(std::string msg, IRecvr * pRecvr);
	void startClient();
	void start(std::string senderaddr, std::string path);
private:
	std::thread thread_;
	ISenderClass* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
	HttpMessageFunc message;
	std::string sendaddress;
	std::string path;
	std::string port;
	bool connection = false;
	MsgClient *client;
};



class SenderClass : public ISenderClass
{
public:
  void postMessage(const Message& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void SenderClass::postMessage(const Message& msg)
{	
  sendQ_.enQ(msg);
}

BQueue& SenderClass::queue() { return sendQ_; }


class Recvr : public IRecvr
{
public:
  std::string getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

std::string Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}


//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISenderClass* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::uploadFilesToClient(std::string msg,IRecvr* pRecvr)
{
	BlockingQueue<HttpMessageFunc> &recQueue = StaticBlockingQueue<HttpMessageFunc, 1>().getQueue();	//receiver  queue
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	client->HandleCommonFunction(msg); 	//Call Client Upload functionality
	HttpMessageFunc msg1 = recQueue.deQ();
	if (msg1.findValue("Command") == "CheckIn")
	{
		recvQ.enQ(msg1.findValue("TestMessageStat"));		
	}
	else if (msg1.findValue("Command") == "Extract")
	{
		recvQ.enQ(msg1.findValue("DownloadLocation"));		
	}
	else if (msg1.findValue("Command") == "GetPackageList")
	{
		recvQ.enQ(msg1.findValue("DependencyList"));
	}
}
void MockChannel::startClient()
{
	client->StartClientSenRec();
}
//----< signal server thread to start >---------------------------------------
void MockChannel::start(std::string senderaddr, std::string path)
{ 
  std::cout << "\n  Client Starting up --- Mock channel\n";
  this->sendaddress = senderaddr; 
  FileSystem::Directory::setCurrentDirectory(FileSystem::Path::getFullFileSpec(path));
  std::string p = FileSystem::Directory::getCurrentDirectory();
  client = new MsgClient(senderaddr,path);
  thread_ = std::thread(
    [this] {
    SenderClass* pSendr = dynamic_cast<SenderClass*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
	startClient();
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  Failed to Start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
      std::string msg = sendQ.deQ();  
	  if (msg.find("Connect") != std::string::npos && connection == false) {		  
		  msg = msg + " NoFile"; 
		  connection = true;	
		 
		  connectOps(msg, pRecvr);
	  }	 
	  else if (msg.find("TestMessage") != std::string::npos || msg.find("CheckIn") != std::string::npos || msg.find("GetPackageList") != std::string::npos || msg.find("Extract") != std::string::npos)
	  {
		  uploadFilesToClient(msg,pRecvr);
	  }
     
    }
    std::cout << "\n  Server stopping\n\n";
  });
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< Connect operation for the UI and Native client >-------

void MockChannel::connectOps(std::string msg, IRecvr* pRecvr)
{
	BlockingQueue<std::string> &q1 = StaticBlockingQueue<std::string, 1>().getQueue();
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	bool conn_flag = false;
	if (msg.find("CONNECT") != std::string::npos)conn_flag = true;
	client->HandleCommonFunction(msg);
	
}


//----< factory functions >--------------------------------------------------

ISenderClass* ObjectFactory::createSendr() { return new SenderClass; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISenderClass* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISenderClass* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
