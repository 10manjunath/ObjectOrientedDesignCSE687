#include "ParallelDepAnlzr.h"
#include "./../AnalyzeDependencies/AnalyzeDependencies.h"
/////////////////////////////////////////////////////////////////////
//  ParallelDepAnlzr.h - analyzes the dependency for			   //
//	each file asynchronously using Task class					   //
//  ver 1.0                                                        //
//     Language:      C++, Visual Studio 2015                      //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:      Manjunath Babu, MS in Computer Engineering   //
//                    (315) 450-1435, mababu@syr.edu               //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Defines functionality for dependency between files in a specified directory using Tasks Class

*/


void FileHandler::execute(const std::string& fileSpec)
{
	std::function<void()> wiThread = [fileSpec,this]() 
	{
	
		TaskHandlerClass threadVar;
	
		threadVar.getMutex().lock();
	
		std::cout << "\n";
	
		std::cout << "-- " << fileSpec << "\n";
	
		std::cout << "Processing on the thread " << std::this_thread::get_id() << std::endl;
	
		threadVar.getMutex().unlock();
	
		bool status_file_process = false;

		MainTypeTable<TypeTableValData>* x = testVar->getTypeAnalysis()->AnalyzeFileTypes(fileSpec,status_file_process);
	
		if(status_file_process == true)
	
			testVar->getTableQueue().enQ(x); 

	};


	testVar->getTaskInputs().Executor(wiThread);
}


void FileHandlerDep::execute(const std::string& fileSpec)
{
	
	std::function<void()> wi2 = [fileSpec, this]() {
				
		DependencyAnalyzer depAnal(pMainTable);

		if (depAnal.anlzFunc(fileSpec)) 
		{
			std::pair<std::string, std::vector<std::string>>* pPair = depAnal.getPDepTable();
			mt->depQueueVar.enQ(pPair); 
		}
		else 
		{
			std::cout << "Error while file analysis - " << fileSpec;
		}
	};
	TaskHandlerClass t1;
	t1.Executor(wi2);
}


void DirHandler::execute(const std::string& dirSpec)
{
	
}


void DirHandlerDep::execute(const std::string& dirSpec)
{
	
}


void DoneHandler::execute(int numOfFiles) 
{
		std::cout << "\n";

		TaskHandlerClass threadVar;

		threadVar.getMutex().lock();

		std::cout << "No of files processed = " << numOfFiles << "\n";

		threadVar.getMutex().unlock();

		MainTypeTable<TypeTableValData>* lastTableVar = new MainTypeTable<TypeTableValData>;

		TypeTableValData tval;

		std::string key = "last table";
		
		tval.type() = std::to_string(numOfFiles);

		tval.fileName() = "lastFile";

		lastTableVar->addRecordToTable(key, tval);		

		testVar->getTableQueue().enQ(lastTableVar);
}

void DoneHandlerDep::execute(int numOfFiles)
{
	std::pair<std::string, std::vector<std::string>>* pPairPtr = new std::pair<std::string, std::vector<std::string>>();
	std::vector<std::string> x;
	x.push_back(std::to_string(numOfFiles));
	pPairPtr->first = "lastdependencyfile";
	pPairPtr->second = x;
	mt->depQueueVar.enQ(pPairPtr);
}


void ParallelDepAnalysis::Search(std::string path,std::vector<std::string> autoAllVar)
{
	CallObj wi1 = [path,this,autoAllVar]() 
	{		
		std::string fullpath = FileSystem::Path::getFullFileSpec(path);

		IFileMgr* pFileMgrVar = FileMgrFactory::create(fullpath);

		pFileMgrVar->regForFiles(&fh);

		pFileMgrVar->regForDirs(&dh);

		pFileMgrVar->regForDoneEvent(&doneVar);
		for (auto autoVar : autoAllVar)
		{
			pFileMgrVar->addPattern(autoVar);
		}	
		pFileMgrVar->search();
		std::cout << "\n\n";
	};
	thVar.Executor(wi1);
}


void MergeTypeTable::TypeTableProcessing(BlockingQueue<MainTypeTable<TypeTableValData>*>& typetableQueue,std::string path, std::vector<std::string> pattColl)
{
	CallObj wi = [this, &typetableQueue,path,pattColl]() {
		int count = 0;
		while (true)
		{
			MainTypeTable<TypeTableValData>* y = typetableQueue.deQ();
			if (y->find("last table") == y->end())
			{
				mergeDefault(y);
				count = count + 1;
			}
			else
			{
				FileCount = std::stoi(y->find("last table")->second.type());
				if (FileCount == count) 
				{
					break;
				}
				else
				{
					while (count != FileCount)
					{
						MainTypeTable<TypeTableValData>* y = typetableQueue.deQ(); 
						mergeDefault(y);
						count++;
					}
					break;
				}
			}
		}
	    std::cout << "\n" << "Final Type table After Merging All :\n\n"; 
		showTypeTable(*pFinalTableHold);
		fhDep.setPointerToFinalTable(pFinalTableHold);				
		SearchDependency(path,pattColl);	
	};
	TaskHandlerClass tsk;
	tsk.Executor(wi);
}

void MergeTypeTable::StoretoResult(BlockingQueue<std::pair<std::string, std::vector<std::string>>*>& deptableQueue)
{
	CallObj wi = [this, &deptableQueue]() 
	{
		int count = 0;
		while (true)
		{
			std::pair<std::string, std::vector<std::string>>* y = deptableQueue.deQ();
			if (y->first != "lastdependencyfile") 
			{
				AddDependencyRecords(y);
				count++;
			}
			else
			{
				DepFileCount = std::stoi(y->second[0]);				
				if (DepFileCount == count)
				{
					break;
				}
				else 
				{
					while (count != DepFileCount)
					{
						std::pair<std::string, std::vector<std::string>>* yVecVar = deptableQueue.deQ(); 						
						AddDependencyRecords(yVecVar);
						count++;
					}
					break;
				}
			}
		}	
		DisplayDependencyTable();
	};
	TaskHandlerClass tsk;
	tsk.Executor(wi);
}

void MergeTypeTable::mergeDefault(MainTypeTable<TypeTableValData>* pTable) 
{
	for (auto autoPairVar : *pTable) 
	{
		if (pFinalTableHold->find(autoPairVar.first) == pFinalTableHold->end()) 
		{
			pFinalTableHold->addRecordToTable(autoPairVar.first, autoPairVar.second);
		}
		else 
		{ 
			
		}
	}
}

void MergeTypeTable::SearchDependency(std::string path,std::vector<std::string> autoPatternCollection)
{	
	CallObj wi1 = [path, this,autoPatternCollection]() 
	{		
		std::string fullpath = FileSystem::Path::getFullFileSpec(path);

		IFileMgr* pFmgr = FileMgrFactory::create(fullpath);

		pFmgr->regForFiles(&fhDep);

		pFmgr->regForDirs(&dhDep);

		pFmgr->regForDoneEvent(&donehDep);

		for (auto autoPatternVar : autoPatternCollection)
		{
			pFmgr->addPattern(autoPatternVar);
		}	
		pFmgr->search();
		
		StoretoResult(depQueueVar);
	};
	thVar.Executor(wi1);
}


int MergeTypeTable::getFileCount()
{
	return FileCount;
}


void MergeTypeTable::setFileCount(int x)
{
	FileCount = x;
}


void ParallelDepAnalysis::CalculateDependency(std::string path, std::vector<std::string> pattternCollection)
{
	std::cout << "\n";
	std::cout << "Testing Parallel Analyzer Functionality";	
	
	MergeTypeTable *mt = new MergeTypeTable();

	Search(path,pattternCollection);	
	
	mt->TypeTableProcessing(getTableQueue(), path,pattternCollection);		
	
	getTaskInputs().threadColObj->wait();
}



#ifdef PARALLEL_DEPENDENCY_MAIN

int main()
{
	//File manager 
	std::string path = "C:/Users/Manjunath Babu/Documents/OOD/Project 3/";
	ParallelDepAnalysis * testVar = new ParallelDepAnalysis();
	std::vector<std::string> autoAllVar;
	autoAllVar.push_back(".txt");
	testVar->startThreadsFunc();
	testVar->CalculateDependency(path,autoAllVar);	
	return 0;
}
#endif // DEBUG
