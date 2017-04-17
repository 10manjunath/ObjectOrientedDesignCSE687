#include "Receiver.h"
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
#include <unordered_map>
using namespace XmlProcessing;
using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
//----< this defines processing to frame messages >------------------


HttpMessageFunc Receiver::clientMessage(Socket& socket)
{
	try
	{
		connectionClosed_ = false;
		HttpMessageFunc msg;
		while (true)
		{
			std::string attribString = socket.recvString('\n');
			if (attribString.size() > 1)
			{
				HttpMessageFunc::Attribute attrib = HttpMessageFunc::parseAttribute(attribString);
				msg.addAttribute(attrib);
			}
			else
				break;
		}
		
		if (msg.attributes().size() == 0)
		{
			connectionClosed_ = true;
			return msg;
		}
		HttpMessageFunc msg1 = clientMessage2(msg, socket);
		return msg1;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
		HttpMessageFunc statmsg;
		return statmsg;
	}
}

HttpMessageFunc Receiver::clientMessage2(HttpMessageFunc msg, Socket& socket)
{
	if (msg.findValue("Command") == "CheckIn")
	{
	
		std::string filename = msg.findValue("PackageName");
		if (filename != "")
		{
			std::cout << "checkedin succssfully";
			recQueue.enQ(msg);
		}
	}
	else if (msg.findValue("Command") == "Extract")
	{
		std::string stat = contentExtract(msg, socket);
		std::cout << "\nSuccess in extraction\n";
		msg.addAttribute(HttpMessageFunc::Attribute("DownloadLocation", stat));
		recQueue.enQ(msg);
	}
	else if (msg.findValue("Command") == "TestMessage")
	{
		std::cout << "TestMessage received at client"; //Test message sent
		std::string message = msg.findValue("Message");
		std::cout << "Test message receieztved at Client : " + message + "\n";
		msg.addAttribute(HttpMessageFunc::Attribute("TestMessageStat", "Success"));
		recQueue.enQ(msg);
	}
	else if (msg.findValue("Command") == "GetPackageList")
	{
		std::cout << "fileList received at client";		
		std::string packageList = msg.findValue("GetPackageList");		
		msg.addAttribute(HttpMessageFunc::Attribute("DependencyList", packageList));
		recQueue.enQ(msg);
	}
	return msg;
}
//used to extract contents from a message and from files, and create a repository and from this get the requested files
std::string Receiver::contentExtract(HttpMessageFunc msg, Socket& socket)
{	
	std::string filename = msg.findValue("FileName");
	std::string packageName = msg.findValue("PackageName");
	if (filename != "")
	{
		size_t contentSize;
		std::string sizeString = msg.findValue("content_length");
		if (sizeString != "")
			contentSize = Utilities::Converter<size_t>::toValue(sizeString);
		else
			return false;		
		std::string c = FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName; //Check if direc exists		
		contentExtracter(packageName);
		FileSystem::File file(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName + "\\" + filename );
		file.open(FileSystem::File::out, FileSystem::File::binary);
		if (!file.isGood())
		{
			Show::write("\n\n  can't open file " + FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName + "\\" + filename + ".rec");
			return false;
		}
		const size_t BlockSize = 2048;
		Socket::byte buffer[BlockSize];
		size_t bytesToRead;
		while (true)
		{
			if (contentSize > BlockSize)
				bytesToRead = BlockSize;
			else
				bytesToRead = contentSize;
			socket.recv(bytesToRead, buffer);
			FileSystem::Block blk;
			for (size_t i = 0; i < bytesToRead; ++i)
				blk.push_back(buffer[i]);
			file.putBlock(blk);
			if (contentSize < BlockSize)
				break;
			contentSize -= BlockSize;
		}
		file.close();
		std::cout << "downloaded files onto the client : ";
		std::cout<< FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName + "\\";
		return FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\";
	}
	return "";
}
void Receiver::contentExtracter(std::string packageName)
{
	if (!FileSystem::Directory::exists(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName))
	{
		if (!FileSystem::Directory::exists(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository"))
		{
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository");
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName);
		}
		else
		{
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + "ClientRepository" + "\\" + packageName);
		}
	}
}

HttpMessageFunc Receiver::serverMsg(Socket& socket)
{
	try
	{
		connectionClosed_ = false;
		HttpMessageFunc msg;
		while (true) 	
		{
			std::string attribString = socket.recvString('\n');
			if (attribString.size() > 1)
			{
				HttpMessageFunc::Attribute attrib = HttpMessageFunc::parseAttribute(attribString);
				msg.addAttribute(attrib);
			}
			else
			{
				break;
			}
		}
		if (msg.attributes().size() == 0) 
		{
			connectionClosed_ = true;
			return msg;
		}

		if (msg.attributes()[0].second == "CheckIn")
		{
			
			return	serverMsg1(msg, socket);
		}
		else if (msg.findValue("Command") == "Extract" && msg.findValue("IsDepNeeded") == "True") //dependent files required
		{
		
			std::vector<std::string> *visitedPack = new std::vector<std::string>();
			CheckOutPackageDep(msg, visitedPack);
		}
		else if (msg.findValue("Command") == "Extract" && msg.findValue("IsDepNeeded") != "True") //dependent files not required
		{
			CheckOutNonDepPackage(msg);
		}
		else if (msg.findValue("Command") == "GetPackageList")
		{
			getPkgList(msg);
		}
		else if (msg.findValue("Command") == "TestMessage")
		{
			recQueue.enQ(msg);
		}
		return msg;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
		HttpMessageFunc statmsg;
		return statmsg;
	}
}


HttpMessageFunc Receiver::serverMsg1(HttpMessageFunc msg,Socket& socket)
{
	std::string filename = msg.findValue("FileName");
	if (filename != "")
	{
		size_t contentSize;
		std::string sizeString = msg.findValue("content_length");
		if (sizeString != "")
			contentSize = Utilities::Converter<size_t>::toValue(sizeString);
		else
			return msg;
		
		bool isClosed; std::string versionNo; std::string packageDepName;
		std::cout << "Path here " << this->path;
		packageDepName = FileSystem::Path::getFullFileSpec(this->path) + "\\" + msg.findValue("PackageName") + "\\" + "DependencyFile" + ".xml";
		bool isFileExist = GetCurrentVersionNumber(packageDepName, versionNo, isClosed);
		std::string serverFileName, fileDepLoc;
		int newVersionNo;
		if (isFileExist == false) 
		{
			newVersionNo = 1;
			serverFileName = readFileStream(filename, contentSize, socket, 1, newVersionNo, msg.findValue("PackageName")); 
			fileDepLoc = readFileDependency(msg, socket, 1, newVersionNo);
		}
		else if (isClosed == true && isFileExist == true)
		{
			newVersionNo = stoi(versionNo) + 1;
			serverFileName = readFileStream(filename, contentSize, socket, 2, newVersionNo, msg.findValue("PackageName")); //update the filename back to the server location copy
			fileDepLoc = readFileDependency(msg, socket, 2, newVersionNo);
		}
		else 
		{
			newVersionNo = stoi(versionNo);
			msg.removeAttribute("Version");
			msg.addAttribute(HttpMessageFunc::Attribute("Version", versionNo));
			serverFileName = readFileStream(filename, contentSize, socket, 3, newVersionNo, msg.findValue("PackageName")); //update the filename back to the server location copy
			fileDepLoc = readFileDependency(msg, socket, 3, newVersionNo);
		}
		msg.removeAttribute("FileName");
		msg.addAttribute(HttpMessageFunc::Attribute("FileName", serverFileName));
	}
	
	serverMsg2(msg,filename,socket);
	return msg;
}

void Receiver::serverMsg2(HttpMessageFunc msg,std::string filename,Socket& socket)
{
	if (filename != "")
	{
		msg.removeAttribute("content_length");  
		std::string bodyString = "<file>" + msg.findValue("FileName") + "stored successfully in server" "</file>";
		std::string sizeString = Utilities::Converter<size_t>::toString(bodyString.size());
		msg.addAttribute(HttpMessageFunc::Attribute("content_length", sizeString));
		msg.addBody(bodyString);
	}
	else
	{
		
		size_t numBytes = 0;
		size_t pos = msg.findAttribute("content_length");
		if (pos < msg.attributes().size())
		{
			numBytes = Utilities::Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}
	HttpMessageFunc sendMsg;
	sendMsg.addAttribute(HttpMessageFunc::attribute("Command", "CheckIn"));
	sendMsg.addAttribute(HttpMessageFunc::attribute("ToAddr", msg.findValue("ToAddr")));
	sendMsg.addAttribute(HttpMessageFunc::attribute("FromAddr", msg.findValue("FromAddr")));
	sendMsg.addAttribute(HttpMessageFunc::attribute("Author", msg.findValue("Author")));
	sendMsg.addAttribute(HttpMessageFunc::attribute("PackageName", msg.findValue("PackageName")));
	recQueue.enQ(sendMsg);	
}
 
void Receiver::CheckOutNonDepPackage(HttpMessageFunc msg)
{
	std::string packageName = msg.findValue("PackageName");
	std::vector<std::string> indDep = Utilities::StringHelper::splitUsingHyphen(packageName);
	std::string pNa, vNa; bool isDone = false;
	for (std::string i : indDep)
	{
		if (isDone == false)
		{
			pNa = i;
			isDone = true;
		}
		else
		{
			vNa = i;
			break;
		}
	}
	std::string fullPath = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pNa + "\\" + vNa + "\\";
	
	std::vector<std::string> files = FileSystem::Directory::getFiles(fullPath, "*.*");
	for (auto a : files) {
		HttpMessageFunc sendMsg;
		sendMsg.addAttribute(HttpMessageFunc::attribute("Command", "Extract"));
		sendMsg.addAttribute(HttpMessageFunc::attribute("ToAddr", msg.findValue("ToAddr")));
		sendMsg.addAttribute(HttpMessageFunc::attribute("FromAddr", msg.findValue("FromAddr")));
		sendMsg.addAttribute(HttpMessageFunc::attribute("Author", msg.findValue("Author")));
		sendMsg.addAttribute(HttpMessageFunc::attribute("PackageName", msg.findValue("PackageName")));
		sendMsg.addAttribute(HttpMessageFunc::attribute("FileName", fullPath  + a));
		sendMsg.addAttribute(HttpMessageFunc::attribute("IsDepNeeded", msg.findValue("IsDepNeeded")));
		recQueue.enQ(sendMsg);
	}
}


std::pair<std::string,std::string> Receiver::GetPackageAndVersionNumber(std::string packageName)
{
	std::vector<std::string> indDep = Utilities::StringHelper::splitUsingHyphen(packageName);
	std::string pNa, vNa; bool isDone = false;
	for (std::string i : indDep)
	{
		if (isDone == false)
		{
			pNa = i;
			isDone = true;
		}
		else
		{
			vNa = i;
			break;
		}
	}
	std::pair<std::string, std::string> g = { pNa,vNa };
	return g;
}


void Receiver::CheckOutPackageDep(HttpMessageFunc msg,std::vector<std::string>* visitedPackages)
{
	std::string packageName = msg.findValue("PackageName");
	bool isDone = false;
	std::string pNa, vNa; 
	std::pair<std::string,std::string> p = GetPackageAndVersionNumber(packageName);
	pNa = p.first;
	vNa = p.second;
	std::string fullPath = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pNa + "\\" + vNa + "\\";
	
	std::vector<std::string> files = FileSystem::Directory::getFiles(fullPath, "*.*");
	if (files.size() > 0)
	{
		for (auto a : files) {
			HttpMessageFunc sendMsg;
			sendMsg.addAttribute(HttpMessageFunc::attribute("Command", "Extract"));
			sendMsg.addAttribute(HttpMessageFunc::attribute("ToAddr", msg.findValue("ToAddr")));
			sendMsg.addAttribute(HttpMessageFunc::attribute("FromAddr", msg.findValue("FromAddr")));
			sendMsg.addAttribute(HttpMessageFunc::attribute("Author", msg.findValue("Author")));
			sendMsg.addAttribute(HttpMessageFunc::attribute("PackageName", msg.findValue("PackageName")));
			sendMsg.addAttribute(HttpMessageFunc::attribute("FileName", fullPath + a));
			sendMsg.addAttribute(HttpMessageFunc::attribute("IsDepNeeded", msg.findValue("IsDepNeeded")));
			recQueue.enQ(sendMsg);
		}
		
		visitedPackages->push_back(pNa);
		
		std::string fqname = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pNa + "\\" + "DependencyFile" + ".xml";
		std::vector<std::string> depList = GetDependenciesForPackage(fqname, pNa, vNa);
		
		for (std::string s : depList)
		{
			std::string pNa, vNa;
			std::pair<std::string, std::string> p = GetPackageAndVersionNumber(s);
			pNa = p.first;
			vNa = p.second;
			if (std::find(visitedPackages->begin(), visitedPackages->end(), pNa) != visitedPackages->end()) //package already visited
			{
				std::cout << "package already visited.Hence ignore\n";
			}
			else	
			{
				msg.removeAttribute("PackageName");
				msg.addAttribute(HttpMessageFunc::attribute("PackageName", s));
				CheckOutPackageDep(msg, visitedPackages);
			}
		}
	}
}


std::vector<std::string> Receiver::GetDependenciesForPackage(std::string fname, std::string pNa, std::string vNa)
{
	
	std::vector<std::string> depe;	
	std::unordered_map<int, std::vector<std::string>> mapColl;
	int count = 0;
	
	XmlDocument doc2(fname, XmlDocument::file);
	
	std::cout << doc2.toString();
	std::string &vname = vNa;	
		title("testing global DFS on XmlDocument - printing tags");
		std::function<void(AbstractXmlElement&)> f;
		f = [vNa,pNa,&mapColl,&count](AbstractXmlElement& Elem) {
			if (Elem.tag().size() > 0)
				if (Elem.value() == vNa)
				{
					std::cout << "\n  " << Elem.tag();
					std::vector<sPtr> pkgV = Elem.children().at(1).get()->children();
					std::vector<std::string> dep;
					for (sPtr s : pkgV)
					{
						std::vector<sPtr> pkgV = s.get()->children();
						std::string d  =  pkgV[0].get()->value();
						dep.push_back(Utilities::StringHelper::trim(d));
					}	
					mapColl[count] = dep;
					//mapColl.insert({ count, dep });
					count++;
				}				
		};

	DFS(doc2, f);
	size_t countMap = mapColl.size();
	depe = mapColl[(int)countMap-1];

	return depe;
}


bool Receiver::GetCurrentVersionNumber(std::string depFileName, std::string &VersionNo, bool &isClosed)
{
	std::string fn1 = depFileName;
	FileSystem::FileInfo fi1(fn1);
	if (fi1.good())
	{
		XmlParser parser(fn1);
		XmlDocument* pDoc = parser.buildDocument();
		pDoc->toString();
		std::string x = testElement(*pDoc, "Version");
	
		std::string d = testElement(*pDoc, "IsClosed");
		if (d == "True")
		{
			isClosed = true;
			VersionNo = std::to_string(std::stoi(x));
		}
		else
		{
			isClosed = false;
			VersionNo = x;
		}
		
		return true;
	}
	else
		return false;
}


std::string Receiver::testElement(XmlDocument &doc, std::string testTag)
{
	std::string x = "\"" + testTag + "\"";
	std::string s;
	using sPtr = std::shared_ptr < AbstractXmlElement >;
	std::vector<sPtr> found = doc.elementsAtEnd(testTag).select();
	if (found.size() > 0)
	{	
		for (auto pElem : found)
		{
			return Utilities::StringHelper::trim(pElem->value());
		}
	}
	else
	{		
		return "";
	}
	std::cout << "\n";
	return "";
}


std::string Receiver::testElements(XmlDocument& doc, std::string testTag)
{
	using sPtr = std::shared_ptr < AbstractXmlElement >;
	std::string send;
	std::vector<sPtr> found = doc.elements(testTag).select();
	if (found.size() > 0)
	{
		for (auto pElem : found)
		{
			if(pElem->tag() == "Dep")
			{					
				send = send + Utilities::StringHelper::trim(pElem->children()[0]->value()) + ";";				
			}
		}
	}
	else
		std::cout << "\n  couldn't find child elements";
	return send;
}


std::string Receiver::readFileStream(const std::string& filename, size_t fileSize, Socket& socket,int cmd,int versionNo,std::string pkgName)
{
	std::string fqname;	
	fqname = readFileStreams(filename,fileSize,socket,cmd,versionNo,pkgName);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
		return false;
	}
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;
		socket.recv(bytesToRead, buffer);
		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);
		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return file.name();
}

std::string Receiver::readFileStreams(const std::string& filename, size_t fileSize, Socket& socket, int cmd, int versionNo, std::string pkgName)
{
	std::string fqname;
	if (cmd == 1)
	{
	
		fqname = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo) + "\\" + filename;
	}
	else if (cmd == 2)
	{
		fqname = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo) + "\\" + filename;
	}
	else if (cmd == 3)
	{
		fqname = FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo) + "\\" + filename;
	}
	//Check if direc exists
	if (!FileSystem::Directory::exists(FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo)))
	{
		if (!FileSystem::Directory::exists(FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName))
		{
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName);
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo));
		}
		else
		{
			FileSystem::Directory::create(FileSystem::Path::getFullFileSpec(this->path) + "\\" + pkgName + "\\" + "Version" + std::to_string(versionNo));
		}
	}
	return fqname;
}

// reads file dependencies
std::string Receiver::readFileDependency(HttpMessageFunc msg, Socket& socket, int cmd, int versionNo)
{
	std::string packageName, fqname;
	packageName = msg.findValue("PackageName");	
	fqname = FileSystem::Path::getFullFileSpec(this->path) + "\\" + packageName + "\\" + "DependencyFile" + ".xml";
	XmlProcessing::XmlDocument doc;
	if (cmd == 1)
	{
		//creating for the first time
		msg.addAttribute(HttpMessageFunc::attribute("Version", std::to_string(versionNo)));
		doc  = BuildXMLFile(msg);
		std::cout << doc.toString();
		FileSystem::File file(fqname);
		file.open(FileSystem::File::out, FileSystem::File::binary);
		std::string input = Utilities::StringHelper::trim(doc.toString());
		std::ofstream out(fqname);
		out << input;
		out.close();
		file.close();
		return file.name();
	}
	else if (cmd == 2)
	{
		//old version had closed = true hence update the dependency file with new version and other details
		msg.addAttribute(HttpMessageFunc::attribute("Version", std::to_string(versionNo)));
		//add the new version depen struct in the depn xml
		std::string newdep = MergeDepsForNewVersion(fqname, msg,versionNo);
		msg.removeAttribute("DepName");
		msg.addAttribute(HttpMessageFunc::Attribute("DepName", newdep));
		//doc = BuildXMLFile(msg);
		return "";
	}
	else if (cmd == 3)
	{
		//just update the existing dep files keeping the old version itself
		//also update the depn merge old and new dep get the existing dep from the xml merge with the new dep
		std::string newdep = MergeDepsForCurrentVersion(fqname, msg);
		msg.removeAttribute("DepName");
		msg.addAttribute(HttpMessageFunc::Attribute("DepName", newdep));
		return "";
	}
	return "";
}

//Merge only the current dependencies to the existing version file
std::string Receiver::MergeDepsForCurrentVersion(std::string fileName,HttpMessageFunc msg)
{
	std::string fn1 = fileName;
	using sPtr = std::shared_ptr <AbstractXmlElement>;
	FileSystem::FileInfo fi1(fn1);
	std::vector<std::string> indDep;
	std::string out;
	if (fi1.good())
	{
		XmlParser parser(fn1);
		XmlDocument* pDoc = parser.buildDocument();std::string ver = msg.findValue("Version");		
		indDep = Utilities::StringHelper::splitUsingDecimalDot(ver);		
		std::string newInc = indDep.back();std::string decim;
		std::size_t found = ver.find(".");
		if (found == std::string::npos) { decim = "1"; }
		else { decim = std::to_string(stoi(newInc) + 1); }
		sPtr pRoot = makeTaggedElement("Version" + newInc +"."+ decim);
		std::string x = testElements(*pDoc, "Deps");
		sPtr child1 = makeTaggedElement("Name");
		child1->addChild(makeTextElement(msg.findValue("PackageName")));
		sPtr child6 = makeTaggedElement("Version");
		child6->addChild(makeTextElement(newInc + "." + decim));
		sPtr child3 = makeTaggedElement("Deps");
		std::string depColl = msg.findValue("DepName");		//add the children elements to the dependency
		indDep.clear();
		indDep = Utilities::StringHelper::splitUsingSemiColon(depColl);
		for (std::string i : indDep)
		{
			sPtr grandChild22 = makeTaggedElement("Dep");
			grandChild22->addChild(makeTextElement(Utilities::StringHelper::trim( i)));
			child3->addChild(grandChild22);
		}
		sPtr child4 = makeTaggedElement("Author");
		child4->addChild(makeTextElement(msg.findValue("Author")));
		sPtr child5 = makeTaggedElement("IsClosed");
		child5->addChild(makeTextElement(msg.findValue("ClosedProp")));
		pRoot->addChild(child1);pRoot->addChild(child3);pRoot->addChild(child4);
		pRoot->addChild(child5);pRoot->addChild(child6);
		pDoc->xmlRoot()->addChild(pRoot);
		FileSystem::File file(fileName);		//store the doc in server
		file.open(FileSystem::File::out, FileSystem::File::binary);
		std::string input = Utilities::StringHelper::trim(pDoc->toString());
		std::ofstream out(fileName);out << input;
		out.close();file.close();
	}
	for (std::string d : indDep)
		out = out + d + ";";
	return out;
}


std::string Receiver::MergeDepsForNewVersion(std::string fileName, HttpMessageFunc msg,int versionNo)
{
	std::string fn1 = fileName;
	using sPtr = std::shared_ptr <AbstractXmlElement>;
	FileSystem::FileInfo fi1(fn1);
	std::vector<std::string> indDep;
	std::string out;
	if (fi1.good())
	{
		XmlParser parser(fn1);
		XmlDocument* pDoc = parser.buildDocument();
		sPtr pRoot = makeTaggedElement("Version"+std::to_string (versionNo));
		std::string x = testElements(*pDoc, "Deps");				
		sPtr child1 = makeTaggedElement("Name");
		child1->addChild(makeTextElement(msg.findValue("PackageName")));
		sPtr child6 = makeTaggedElement("Version");
		child6->addChild(makeTextElement(std::to_string(versionNo)));
		sPtr child3 = makeTaggedElement("Deps");
		std::string depColl = msg.findValue("DepName");		//add the children elements to the dependency
		indDep = Utilities::StringHelper::splitUsingSemiColon(depColl);
		for (std::string i : indDep)
		{
			sPtr grandChild22 = makeTaggedElement("Dep");
			grandChild22->addChild(makeTextElement(Utilities::StringHelper::trim(i)));
			child3->addChild(grandChild22);
		}
		sPtr child4 = makeTaggedElement("Author");
		child4->addChild(makeTextElement(msg.findValue("Author")));
		sPtr child5 = makeTaggedElement("IsClosed");
		child5->addChild(makeTextElement(msg.findValue("ClosedProp")));
		pRoot->addChild(child1);
		pRoot->addChild(child3);
		pRoot->addChild(child4);
		pRoot->addChild(child5);
		pRoot->addChild(child6);
		pDoc->xmlRoot()->addChild(pRoot);
		FileSystem::File file(fileName);		//store the doc in server
		file.open(FileSystem::File::out, FileSystem::File::binary);
		std::string input = Utilities::StringHelper::trim(pDoc->toString());
		std::ofstream out(fileName);
		out << input;
		out.close();
		file.close();
	}
	for (std::string d : indDep)
	{
		out = out + d + ";";
	}
	return out;
}


XmlProcessing::XmlDocument Receiver::BuildXMLFile(HttpMessageFunc msg)
{	
	using sPtr = std::shared_ptr <AbstractXmlElement>;
	sPtr pRoot = makeTaggedElement("Metadata");
	XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
	sPtr child9 = makeTaggedElement("Version"+msg.findValue("Version"));	
	sPtr child1 = makeTaggedElement("Name");
	child1->addChild(makeTextElement(msg.findValue("PackageName")));
	sPtr child6 = makeTaggedElement("Version");
	child6->addChild(makeTextElement(msg.findValue("Version")));
	sPtr child3 = makeTaggedElement("Deps");
	std::string depColl = msg.findValue("DepName");
	std::vector<std::string> indDep = Utilities::StringHelper::splitUsingSemiColon(depColl);
	for (std::string i : indDep)
	{
		sPtr grandChild22 = makeTaggedElement("Dep");
		grandChild22->addChild(makeTextElement(Utilities::StringHelper::trim(i)));
		child3->addChild(grandChild22);
	}
	sPtr child4 = makeTaggedElement("Author");
	child4->addChild(makeTextElement(msg.findValue("Author")));
	sPtr child5 = makeTaggedElement("IsClosed");
	child5->addChild(makeTextElement(msg.findValue("ClosedProp")));
	pRoot->addChild(child9);
	child9->addChild(child1);
	child9->addChild(child3);
	child9->addChild(child4);
	child9->addChild(child5);
	child9->addChild(child6);
	return doc;
}
//----< receiver functionality is defined by this function >---------

void Receiver::operator()(Socket socket)
{
	try
	{
		if (this->isClient == false)
			HttpMessageFunc msg = serverMsg(socket);		//for server operations
		else
			HttpMessageFunc msg = clientMessage(socket);      //for client operations
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}


void Receiver::getPkgList(HttpMessageFunc msg)
{
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(FileSystem::Path::getFullFileSpec(this->path), "*.*");	
	std::string outDirs;
	for (std::string s : dirs)
	{
		std::size_t found = s.find("Package");
		if (found != std::string::npos)
		{
			std::vector<std::string> verdirs = FileSystem::Directory::getDirectories(FileSystem::Path::getFullFileSpec(this->path) +"\\"+ s, "*.*");
			for (std::string subdir : verdirs)
			{
				std::size_t subfound = subdir.find("Version");
				if (subfound != std::string::npos)
				{
					outDirs = outDirs + s + "-" + subdir + ";";
				}
			}			
		}
	}
	if(outDirs.size() > 0)
	msg.addAttribute(HttpMessageFunc::attribute("GetPackageList", outDirs));
	recQueue.enQ(msg);
}

#ifdef zTEST_RECEIVER
int main()
{
	using sPtr = std::shared_ptr <AbstractXmlElement>;
	sPtr pRoot = makeTaggedElement("Metadata");
	XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
	sPtr child1 = makeTaggedElement("name");
	child1->addChild(makeTextElement("Package1"));
	sPtr child2 = makeTaggedElement("Deps");
	child2->addChild(makeTextElement("Dep"));
	sPtr grandChild22 = makeTaggedElement("Dep");
	grandChild22->addChild(makeTextElement("File1.h"));
	child2->addChild(grandChild22);
	sPtr child3 = makeTaggedElement("Author");
	child3->addChild(makeTextElement("Client 1"));
	sPtr child4 = makeTaggedElement("IsClosed");
	child4->addChild(makeTextElement("Yes"));
	pRoot->addChild(child1);
	pRoot->addChild(child2);
	pRoot->addChild(child3);
	pRoot->addChild(child4);

}
#endif
