#ifndef ANALYZEDEPENDENCIES
#define ANALYZEDEPENDENCIES
//////////////////////////////////////////////////////////////////////
//  AnalyzeDependencies.h - Dependency between different Typetables	//
//      ver 1.0                                                     //
//     Language:       C++, Visual Studio 2015                      //						        //
//  Application:       Parallel Dependency Analysis				    //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Defines all operations to computer dependency between every file in the assigned directory.

*/
//

#include <string>
#include "./../TypeTable/TypeTable.h"
#include <unordered_map>
#include <vector>		


class DependencyAnalyzer 
{
	MainTypeTable<TypeTableValData>* pMainTable;
	std::pair<std::string, std::vector<std::string>>* pTableDep = nullptr; 
public:
	DependencyAnalyzer(MainTypeTable<TypeTableValData>* pFinalTableHold) : pMainTable(pFinalTableHold) 
	{
		pTableDep = new std::pair<std::string, std::vector<std::string>>;
	}

	bool anlzFunc(std::string fileSpec);

	std::pair<std::string, std::vector<std::string>>* getPDepTable()
	{
		return pTableDep;
	}
	MainTypeTable<TypeTableValData>* getPMainTable()
	{
		return pMainTable;
	}
};

#endif