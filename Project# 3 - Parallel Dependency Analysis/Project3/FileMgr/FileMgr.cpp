///////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns                //
//             on a specified path                                   //
//      ver 1.1                                                      //
//     Language:       C++, Visual Studio 2015                       //
//  Application:       FileMgr.h								     //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:       Manjunath Babu, MS in Computer Engineering    //
//                     (315) 450-1435, mababu@syr.edu                //
///////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
pattern matching on specified path


*/


#ifdef TEST_FILEMGR
#include "FileMgr.h"
#include <iostream>

using namespace FileManager;

struct FileHandler : IFileEventHandler
{
	void execute(const std::string& fileSpec)
	{
		std::cout << "\n  --   " << fileSpec;
	}
};

struct DirHandler : IDirEventHandler
{
	void execute(const std::string& dirSpec)
	{
		std::cout << "\n  ++ " << dirSpec;
	}
};

int main()
{
	std::cout << "\n  Testing File Managet Package";
	std::cout << "\n =================";

	std::string path = FileSystem::Path::getFullFileSpec("..");
	IFileMgr* pFmgr = FileMgrFactory::create(path);

	FileHandler fh;
	DirHandler dh;

	pFmgr->regForFiles(&fh);
	pFmgr->regForDirs(&dh);

	pFmgr->search();

	std::cout << "\n\n";
	return 0;
}
#endif
