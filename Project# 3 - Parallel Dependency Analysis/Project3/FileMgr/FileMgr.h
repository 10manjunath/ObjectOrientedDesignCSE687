#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////////////////////
// FileMgr.h - Search files for specified matching pattern in a path //
//      ver 1.0                                                      //
//     Language:       C++, Visual Studio 2015                       //
//  Application:       FileMgr.h								     //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:       Manjunath Babu, MS in Computer Engineering    //
//                     (315) 450-1435, mababu@syr.edu                //
///////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Search files for specified matching pattern in a path

*/

#include "IFileMgr.h"
#include "FileSystem.h"
#include<iostream>
namespace FileManager
{
	class FileMgr : public IFileMgr
	{
	public:
		using patterns = std::vector<std::string>;
		using fileSubscribers = std::vector<IFileEventHandler*>;
		using dirSubscribers = std::vector<IDirEventHandler*>;
		using doneSubscribers = std::vector<IDoneEventHandler*>;

		FileMgr(const std::string& path) : path_(path)
		{
			patterns_.push_back("*.*");
		}

		void addPattern(const std::string& patt)
		{
			if (patterns_.size() == 1 && patterns_[0] == "*.*")
				patterns_.pop_back();
			patterns_.push_back(patt);
		}

		void search()
		{
			find(path_);
			for (auto pEvtHandler : doneSubscribers_) {
				pEvtHandler->execute(fileCount);
			}
		}

		void find(const std::string& path)
		{			
			std::string fpath = FileSystem::Path::getFullFileSpec(path);
			for (auto pEvtHandler : dirSubscribers_)
			{
				pEvtHandler->execute(fpath);
			}
			for (auto patt : patterns_)
			{
				std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);			
				for (auto f : files)
				{
					++fileCount;
					for (auto pEvtHandler : fileSubscribers_)
					{
						pEvtHandler->execute(fpath + "\\" + f);
					}
				}
			}
			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
			for (auto d : dirs)
			{
				if (d == "." || d == "..")
					continue;
				std::string dpath = fpath + "\\" + d;
				find(dpath);
			}
		}

		void regForFiles(IFileEventHandler* pHandler)
		{
			fileSubscribers_.push_back(pHandler);
		}

		void regForDirs(IDirEventHandler* pHandler)
		{
			dirSubscribers_.push_back(pHandler);
		}

		void regForDoneEvent(IDoneEventHandler* pHandler) {
			doneSubscribers_.push_back(pHandler);
		}

	private:
		std::string path_;
		patterns patterns_;
		fileSubscribers fileSubscribers_;
		dirSubscribers dirSubscribers_;
		doneSubscribers doneSubscribers_;
		int fileCount;
	};

	inline IFileMgr* FileMgrFactory::create(const std::string& path)
	{
		return new FileMgr(path);
	}
}
#endif
