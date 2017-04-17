#ifndef RECEIVER_H
#define RECEIVER_H
///////////////////////////////////////////////////////////////////////////
//  Reciever.h - receieve messages from the socket						 //
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
This module receives the messages and requested files from the server

*/
//Receiver class

#define MAX_DATE 12
#include "../XMLUtil/XmlParser.h"
#include <unordered_map>
#include <time.h>
#include <string>
#include <iostream>
#include <thread>
#include <math.h>      
#include <algorithm>
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../StaticBlockingQueue/StaticBlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include"../XMLUtil/XmlDocument.h"


using namespace XmlProcessing;
class Receiver
{
public:
	Receiver(){	}
	Receiver(int tport,std::string taddr,bool tisClient,std::string tpath) 
		: port(tport),addr(taddr),isClient(tisClient),path(tpath){};
	~Receiver() {};
	void operator()(Socket socket);

	void getPkgList(HttpMessageFunc msg);
	
	HttpMessageFunc clientMessage(Socket& socket);
	HttpMessageFunc clientMessage2(HttpMessageFunc msg, Socket & socket);	
	std::string contentExtract(HttpMessageFunc msg, Socket & socket);
	void contentExtracter(std::string packageName);
	HttpMessageFunc serverMsg(Socket& socket);
	HttpMessageFunc serverMsg1(HttpMessageFunc msg, Socket & socket);
	void serverMsg2(HttpMessageFunc msg, std::string filename, Socket & socket);
	void CheckOutNonDepPackage(HttpMessageFunc msg);
	std::pair<std::string, std::string> GetPackageAndVersionNumber(std::string packageName);
	void CheckOutPackageDep(HttpMessageFunc msg, std::vector<std::string>* visitedPackages);
	std::vector<std::string> GetDependenciesForPackage(std::string fname, std::string pNa, std::string vNa);
	bool GetCurrentVersionNumber(std::string depFileName, std::string & VersionNo, bool & isClosed);
	std::string testElement(XmlDocument & doc, std::string testTag);
	std::string testElements(XmlDocument& doc, std::string testTag);
	
	std::string readFileStream(const std::string & filename, size_t fileSize, Socket & socket, int cmd, int versionNo, std::string pkgName);
	std::string readFileStreams(const std::string & filename, size_t fileSize, Socket & socket, int cmd, int versionNo, std::string pkgName);
	std::string readFileDependency(HttpMessageFunc msg, Socket & socket, int cmd, int versionNo);
	std::string MergeDepsForCurrentVersion(std::string fileName, HttpMessageFunc msg);
	std::string MergeDepsForNewVersion(std::string fileName, HttpMessageFunc msg, int versionNo);	
	XmlProcessing::XmlDocument BuildXMLFile(HttpMessageFunc msg);
private:
	BlockingQueue<HttpMessageFunc> &recQueue = StaticBlockingQueue<HttpMessageFunc,1>().getQueue();	
	bool connectionClosed_;
	using Show = StaticLogger<1>;
	int port;
	std::string addr;
	std::string path;
	bool isClient;	
};
#endif