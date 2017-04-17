#pragma once

/////////////////////////////////////////////////////////////////////////////
// IFileMgr.h - Interface for FileMgr.h					                   //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provides classes, File, FileInfo, Path, Directory, and
* FileSystemSearch.
*
* The File class supports opening text and binary files for either input
* or output.  File objects have names, get and put lines of text, get and
* put blocks of bytes if binary, can be tested for operational state,
* cleared of errors, and output File objects can be flushed to their streams.
*
* FileInfo class accepts a fully qualified filespec and supports queries
* concerning name, time and date, size, and attributes.  You can compare
* FileInfo objects by name, date, and size.
*
* Path class provides static methods to turn a relative filespec into an
* absolute filespec, return the path, name, or extension of the filespec,
* and build filespecs from path and name.  Path also provides methods to
* convert filespecs to uppercase and to lowercase.
*
* The Directory class supports getting filenames or directories from a
* fully qualified filespec, e.g., path + filename pattern using static
* methods.  It also provides non-static methods to get and set the current
* directory.
*
* Public Interface:
* =================
* File f(filespec,File::in,File::binary);
* File g(filespec,File::out,File::binary);
* while(f.isGood()) {
*   block b = f.getBlock();
*   g.putBlock(b);
* }
* FileInfo fi("..\foobar.txt");
* if(fi.good())
*   ...
* std::string filespec = "..\temp.txt";
* std::string fullyqualified = Path::getFullFileSpec(filespec);
* std::string path = Path::getPath(fullyqualified);
* std::string name = Path::getName(fullyqualified);
* std::string extn = Path::getExt(fullyqualified);
*
* Directory d;
* std::string dir = d.getCurrentDirectory();
* d.setCurrentDirectory(dir);
* std::vector<std::string> files = Directory::getFiles(path, pattern);
* std::vector<std::string> dirs = Directory::getDirectories(path);
*
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILESYSTEM FileSystem.cpp
*
* Maintenance History:
* ====================
* ver 2.2 : 23 Feb 13
* - fixed bug in Path::getExt(...) discovered by Yang Zhou and Kevin Kong
* ver 2.1 : 07 Jun 12
* - moved FileSystemSearch to cpp file to make private
* - added bool withExt parameter to Path::getName
* - made Directory's getCurrentDirectory and setCurrentDirectory static
* - added exists, create, and remove to Directory class
* ver 2.0 : 04 Jun 12
* - made a few small changes to provide same interface as
*   Linux version.
* ver 1.0 : 31 Jan 09
* - first release
*/
#include <string>

namespace FileManager
{
	struct IFileEventHandler
	{
		virtual void execute(const std::string& fileSpec) = 0;
	};

	struct IDirEventHandler
	{
		virtual void execute(const std::string& dirSpec) = 0;
	};

	struct IDoneEventHandler {
		virtual void execute(int numOfFiles) = 0;
	};

	struct IFileMgr
	{
		virtual ~IFileMgr() {}
		virtual void addPattern(const std::string& patt) = 0;
		virtual void search() = 0;
		virtual void find(const std::string& path) = 0;
		virtual void regForFiles(IFileEventHandler* pEventHandler) = 0;
		virtual void regForDirs(IDirEventHandler* pEventHandler) = 0;
		virtual void regForDoneEvent(IDoneEventHandler* pEventHandler) = 0;
	};

	struct FileMgrFactory
	{
		static IFileMgr* create(const std::string& path);
	};
}