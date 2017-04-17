#ifndef PARALLELDEPANLZR_H
#define PARALLELDEPANLZR_H
//////////////////////////////////////////////////////////////////////
//  ParallelDepAnlzr.h -								            //
//					using task, analyze dependency for files asynch //
//  ver 1.0                                                         //
//     Language:      C++, Visual Studio 2015                       //
//  Application:      Parallel Dependency Analysis			        //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:      Manjunath Babu, MS in Computer Engineering    //
//                    (315) 450-1435, mababu@syr.edu                //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Operations for calculating dependencies between files

*/
//

#include"../ThreadPool/Cpp11-BlockingQueue.h"
#include"../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
#include <functional>
#include <iostream>
#include "../TaskBuilder/TaskBuilder.h"
#include"../TypeAnalyzer/TypeAnalyzer.h"



class ParallelDepAnalysis;

using namespace FileManager;

class FileHandler : public IFileEventHandler
{
public:
	void execute(const std::string& fileSpec); 
	ParallelDepAnalysis* testVar;
	FileHandler(ParallelDepAnalysis* pAnal) : testVar(pAnal) { } 

};

class DirHandler : public IDirEventHandler 
{
	void execute(const std::string& dirSpec);
};

class DoneHandler : public IDoneEventHandler {	
public:
	void execute(int numOfFiles);
	ParallelDepAnalysis* testVar;
	DoneHandler(ParallelDepAnalysis* pAnal) : testVar(pAnal) { } 
};


class ParallelDepAnalysis
{
	FileHandler fh;
	DirHandler dh;
	DoneHandler doneVar;
	TypeAnalClass * tan;
	TaskHandlerClass thVar;	
	BlockingQueue<MainTypeTable<TypeTableValData>*> tableQueue;
public:
	void startThreadsFunc() { 
		thVar.StartThreadPool();
	}

	ParallelDepAnalysis() : fh(this), doneVar(this) 
	{
		tan = new TypeAnalClass();
	}

	TaskHandlerClass& getTaskInputs()
	{
		return thVar;
	}

	TypeAnalClass* getTypeAnalysis()
	{
		return tan;
	}

	BlockingQueue<MainTypeTable<TypeTableValData>*>& getTableQueue()
	{
		return tableQueue;
	}
	void Search(std::string path, std::vector<std::string>); 
	
	void CalculateDependency(std::string, std::vector<std::string>);
};

class MergeTypeTable;

class FileHandlerDep : public IFileEventHandler	
{
public:
	void execute(const std::string& fileSpec);
	MergeTypeTable* mt;
	MainTypeTable<TypeTableValData>* pMainTable;
	FileHandlerDep(MergeTypeTable* mVar) : mt(mVar) 
	{
		pMainTable = nullptr;
	}
	
	void setPointerToFinalTable(MainTypeTable<TypeTableValData>* pFinal) { pMainTable = pFinal; }	//set pointer to final table
};
class DirHandlerDep : public IDirEventHandler
{
	void execute(const std::string& dirSpec);	 
};

class DoneHandlerDep : public IDoneEventHandler { 
public:
	void execute(int numOfFiles);
	MergeTypeTable* mt;
	DoneHandlerDep(MergeTypeTable* m) : mt(m) { }
};

class MergeTypeTable	
{
	int FileCount = 0;
	FileHandlerDep fhDep;
	DirHandlerDep dhDep;
	DoneHandlerDep donehDep;
	TaskHandlerClass thVar;
	int DepFileCount = 0;
public:
	BlockingQueue<std::pair<std::string, std::vector<std::string>>*> depQueueVar;
	MainTypeTable<TypeTableValData>* pFinalTableHold;	
	std::vector<std::pair<std::string, std::vector<std::string>>>* pDependencyTableHold;
	void TypeTableProcessing(BlockingQueue<MainTypeTable<TypeTableValData>*>&, std::string, std::vector<std::string>);	
	int getFileCount();

	TaskHandlerClass& getTaskInputs()
	{
		return thVar;
	}
	void setFileCount(int);

	MergeTypeTable() : fhDep(this), donehDep(this) 
	{
		pFinalTableHold = new MainTypeTable<TypeTableValData>;
		pDependencyTableHold = new std::vector<std::pair<std::string, std::vector<std::string>>>;
	}
	void mergeDefault(MainTypeTable<TypeTableValData>* pTable); 

	void SearchDependency(std::string path, std::vector<std::string>);	

	void StoretoResult(BlockingQueue<std::pair<std::string, std::vector<std::string>>*>&); 

	void AddDependencyRecords(std::pair<std::string, std::vector<std::string>>* record)
	{
		pDependencyTableHold->push_back(*record);
	}
	void DisplayDependencyTable()
	{
		std::cout << "*****************************************\n";
		std::cout << "************ Dependency Table ***********\n";
		std::cout << "*****************************************\n\n";
		int i = 1;
		for (auto rec : *pDependencyTableHold)
		{
			std::cout << "\n" << std::string(60, '#') << std::endl;
			std::cout << i++ << std::endl;
			std::cout << "File Name: " << rec.first << std::endl << std::endl;
			if (rec.second.size() == 0)
			{
				std::cout << "No of Dependencies " << std::endl;
			}
			else
			{
				std::cout << "Dependencies are: " << std::endl;
				for (auto a : rec.second) {
					std::cout << a << std::endl;
				}
			}
			std::cout << "\n" << std::string(60, '#') << std::endl;
			//std::cout << "\n";
		}
	}
};

#endif