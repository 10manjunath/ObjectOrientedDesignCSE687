//////////////////////////////////////////////////////////////////////
//  AnalyzeDependencies.cpp - analyzes the dependency			    //
//	between the typetables										    //
//  ver 1.0                                                         //
//	   Language:       C++, Visual Studio 2015                      //
//  Application:       Parallel Dependency Analysis Implementation  //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //     							    //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module defines ops to calculate the dependency between files in a directory


*/
//
#include"AnalyzeDependencies.h"
#include "./../Parser/ConfigureParser.h"
#include <vector>
#include <string>


bool DependencyAnalyzer::anlzFunc(std::string fileSpec) {

	pTableDep = nullptr;
	ConfigParseToConsole configureParser;
	Parser* pParser = configureParser.BuildForDependency(pMainTable);
	try
	{
		if (pParser)
		{
			if (!configureParser.Attach(fileSpec))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		while (pParser->next())

			pParser->parse();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		return false;
	}	
	
	pTableDep = configureParser.getDepPair();
	return true;
}

#ifdef TEST_MAIN_DEP_ANA_SINGLE
int main()
{
	MainTypeTable<TypeTableValData>* pMainTable;
	DependencyAnalyzer depAnal(pMainTable);
	std::string fileSpec = "C:/Users/Manjunath Babu/Documents/OOD/Project 3/File1.txt";
	
	std::function<void()> wi2 = [fileSpec, this]() {		
		DependencyAnalyzer depAnal(pMainTable);
		if (depAnal.anlzFunc(fileSpec)) {
			std::pair<std::string, std::vector<std::string>>* pPair = depAnal.pTableDep;
			std::cout << "Dependency table is equal to:\n";
			std::cout << "File: " << pPair->first << std::endl;
			std::cout << "Depends on following files: " << std::endl;
			for (auto a : pPair->second) {
				std::cout << a << std::endl;
			}
			std::cout << "\n\n";
		}
		else {
			std::cout << "Could not analyze file - " << fileSpec;
		}
	};
	TaskHandlerClass t1;
	t1.Executor(wi2);
}
#endif // DEBUG
