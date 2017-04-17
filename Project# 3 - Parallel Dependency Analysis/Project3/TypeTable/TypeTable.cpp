#include "TypeTable.h"
using namespace Utilities;
using Utils = StringHelper;
//////////////////////////////////////////////////////////////////////
// TypeTable.cpp - type table to holds the key value                //
//  ver 2.0                                                         //
//     Language:      C++, Visual Studio 2015                       //
//  Application:      Parallel Dependency Analysis			        //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:      Manjunath Babu, MS in Computer Engineering    //
//                    (315) 450-1435, mababu@syr.edu                //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Defines type table structure to hold the data

*/
//
#ifdef TEST_TYPETABLE

void main()
{
	Utils::Title("MT4Q1 - TypeTable");
	putline();
	MainTypeTable<TypeTableValData> table;
	TypeTableValData tval;
	std::string key = "funcA";
	std::string clsname1= "X";
	std::string clsname2 = "Y";
	tval.className().push_back(clsname1); 
	tval.className().push_back(clsname2);
	tval.type() = "MemberFunction";
	tval.fileName() = "X.h";
	tval.addNameSpaceFunc("TypeAnalysis");
	tval.addNameSpaceFunc("MT4");
	tval.addPair();

	tval.addNameSpaceFunc("DependencyAnalysis");
	tval.addNameSpaceFunc("MT5");
	tval.fileName() = "Y.h";
	tval.addPair();
	table.addRecordToTable(key, tval);
	showTypeTable(table);

	std::cout << "=======================" << std::endl;
	TypeTableValData tv2 = table["funcA"];
	showRecord(tv2);
	if (table.find("funcC") == table.end())
	{
		std::cout << "record not present";
	}
}
#endif