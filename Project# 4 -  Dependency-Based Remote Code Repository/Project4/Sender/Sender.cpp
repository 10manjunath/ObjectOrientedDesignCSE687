///////////////////////////////////////////////////////////////////////////
//  Sender.cpp - Sends files and messages.								 //
//  ver 1.0															     //
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



*/
//

#include "Sender.h"


std::string Sender::getFileName(const std::string& s) {
	char sep = '/';

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	else
	{
		char sep = '\\';
		size_t i = s.rfind(sep, s.length());
		if (i != std::string::npos) {
			return(s.substr(i + 1, s.length() - i));
		}
	}
	return("");
}


bool Sender::uploadData(HttpMessageFunc msg, Socket& socket) {
	
	std::string fqname = msg.findValue("FileName");
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	std::string x = getFileName(msg.findValue("FileName"));
	msg.removeAttribute("FileName");
	msg.removeAttribute("content_length");
	msg.addAttribute(HttpMessageFunc::attribute("content_length", std::to_string(fileSize)));
	msg.addAttribute(HttpMessageFunc::attribute("FileName", x));
	sendMsg(msg, socket);

	
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;

};



void Sender::sendMsg(HttpMessageFunc& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

