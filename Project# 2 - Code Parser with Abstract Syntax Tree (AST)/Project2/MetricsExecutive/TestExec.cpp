/////////////////////////////////////////////////////////////////////
// TestExec.cpp - Implement all tests here		                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
//    Platform:   MacBook Pro, Core-i5, Windows 10 on Bootcamp     //
//      Author:   Manjunath Babu, MS in Computer Engineering       //
//                (315) 440-1435, mababu@syr.edu                   //
/////////////////////////////////////////////////////////////////////

/*
Test all requirements for the project 2.
*/

/*
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2016
* - first release of new design
*
*/

#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include "../FileManager/FileMgr.h"
#include"../FileManager/DataStore.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../RulesAndActions/RulesAndAction.h"
#include "../Parser/ConfigureParser.h"
#include "TestExec.h"
#include"../MetricsAnalysis/Analysis.h"
#define Util StringHelper

using namespace Utilities;


class TestExecutiveClass
{
public:
	TestExecutiveClass();
	~TestExecutiveClass();
	void Req1();
	void Req2();
	
	void testReq1();
	void testReq2();
	void testReq3();
	void testReq4();
	void testReq5();
	void testReq6();
	void testReq7();
	void testReq8();
	void testReq9();
	void testReq10();
	bool ExtractFiles(std::string, std::vector<std::string>);
private:
	MetricsResults *resultVar;
};

TestExecutiveClass::TestExecutiveClass()
{
	resultVar = new MetricsResults();
}

TestExecutiveClass::~TestExecutiveClass()
{

}

void TestExecutiveClass::testReq1() 
{
	std::cout.width(70); 
	std::cout << "Project 2" << std::endl;
	std::cout.width(70); 
	std::cout << "Code Parser with Abstract Syntax Tree" << std::endl;
	std::cout.width(70); 
	std::cout << "---------------------------------------------\n";
	
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 1\n";
	std::cout << "*****************************\n";
	
	std::cout << "Used Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs.\n";
}


void TestExecutiveClass::testReq2() 
{
	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 2\n";
	std::cout << "*****************************\n";

	std::cout << "Used the C++ standard library's streams for all I/O and new and delete for all heap-based memory management";

}

void TestExecutiveClass::testReq3()
{

	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 3 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided C++ packages for analyzing function size and complexity metrics for a set of specified packages. These packages will use the Tokenizer and SemiExpression packages developed in Project #1." << std::endl;
}

void TestExecutiveClass::testReq4()
{

	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 4 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided a Parser package with a Parser class that is a container for Rules and that provides the interfaces IRule and IAction for rules contained in the Parser and actions contained in each rule." << std::endl;
}

void TestExecutiveClass::testReq5()
{

	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 5 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided an associated RulesAndActions package that has rules to detect: \n - global functions and static and non-static member function definitions3. \n - beginning and end of all C++ scopes. \nand actions for each rule that support building the Abstract Syntax Tree (AST) that represents a single file's scope structure and identifies the type and line number extend of each scope." << std::endl;
}


void TestExecutiveClass::testReq6()
{

	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 6 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided a facility for building an abstract syntax tree that provides an interface for adding scope nodes to the tree and an methods to analyze the contents of the tree." << std::endl;
}

//  Requirement 3,4,5,6 is displayed here  
void TestExecutiveClass::Req1()
{
	std::cout.width(50); std::cout << "Code Parser with Abstract Syntax Tree (AST)" << std::endl;
	std::cout.width(80); std::cout << "-------------------------------------------\n";
	std::cout << "Demonstrating Requirement #3" << "\n";
	std::cout << "SemiExpression package and TestExecutiveClass packages have been used from Project 1 as can be seen in Visual Studio" << "\n\n";
	std::cout << "Demonstrating Requirement #4" << "\n";
	std::cout << "Parser packages has been used in solution file with option for IRule and IAction interfaces Line#86 ,#97 in Parser.h " << "\n\n";
	std::cout << "Demonstrating Requirement #5" << "\n";
	std::cout << "Rules packages has been used in solution file with name as RulesAndActions package. Please refer RulesAndAction.h for more details on\n";
	std::cout<<  "the Rules and actions defined for global,static and non-static functions and scopes" << "\n";
	std::cout << "Line #85 - RulesAndActions.h Also supports actions for each rule that support building the Abstract Syntax Tree (AST)\n";
	std::cout << "that represents a single file's scope structure and identifies the type and line number extend of each scope." << "\n\n";
	std::cout << "Demonstrating Requirement #6" << "\n";	
}



void TestExecutiveClass::testReq7()
{
	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 7 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided a FileMgr package that supports finding files and/or directories in a directory tree rooted at a specified path." << std::endl;
}


void TestExecutiveClass::testReq8()
{
	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 8 \n";
	std::cout << "*****************************\n";

	std::cout << "Provided a MetricsAnalysis package for evaluating and displaying the size and complexity of all global functions, static member functions, and non-static member functions in each of a set of specified packages." << std::endl;

	std::cout << "\n Results are displayed below.\n";

	resultVar->DisplayMetricsFunction();

}

void TestExecutiveClass::testReq9()
{
	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 9 \n";
	std::cout << "*****************************\n";

	std::cout << "Provide a MetricsExecutive package that enables collecting metrics for all the packages with names that match a specified pattern in a directory tree rooted at a specified path. Please define the path and file patterns on the MetricsExecutive command line." << std::endl;
}

void TestExecutiveClass::testReq10()
{
	std::cout << std::endl << std::endl;
	std::cout << "*****************************\n";
	std::cout << "DEMONSTRATING REQUIREMENT 10 \n";
	std::cout << "*****************************\n";

	std::cout << "Included an automated unit test suite that exercises all of the packages provided and met all requirements" << std::endl;
}

//  Requirement 7,8,9,10 is displayed here  
void TestExecutiveClass::Req2()
{	
	std::cout << "Demonstrating Requirement #7" << "\n";
	std::cout << "File Manager package has been created where we support finding files and/or directories in a directory tree rooted at a specified path.\n\n";
	std::cout << "Demonstrating Requirement #8" << "\n";
	std::cout << "Metric Analysis package details and the results are displayed below.\n";
	resultVar->DisplayMetricsFunction();
	std::cout << "\n\nDemonstrating Requirement #9" << "\n";
	std::cout << "Metrics Executive package has been created and used where the file paramters such as file name and patterns can been passed from the command line arguments\n\n";
	std::cout << "Demonstrating Requirement #10" << "\n";
	std::cout << "TestStub has been created for all packages\n\n";
}

//Extract files
bool TestExecutiveClass::ExtractFiles(std::string filespec, std::vector<std::string> pattColl)
{
	try
	{
		DataStore ds;FileMgr fm(filespec, ds);
		for (auto patt : pattColl)
			fm.addPattern("*" + patt + "*");		//add pattern to the file
		fm.search();	//search the files pattern 	
		DataStore::iterator iter = ds.begin();bool isFound = false;
		for (auto fs : ds)
		{
			isFound = true;
			ConfigParseToConsole configure;std::cout << "\n\nFile processed : " << "\n";
			std::cout << "-----------------------\n";std::cout << fs << "\n";
			Parser* pParser = configure.Build();
			try
			{
				if (pParser)
				{
					if (!configure.Attach(fs))
						std::cout << "\n  could not open file " << filespec << std::endl;
				}
				else
					std::cout << "\n\n  Parser not built\n\n";				//return 1;
				while (pParser->next()) 			// now that parser is built, use it
					pParser->parse();
				std::cout << "\n";std::cout << "\n\nTREE STRUCTURE DISPLAY" << "\n";
				std::cout << "-------------------";
				TreeNode* root = configure.DisplayAbstractSyntaxTree(); //Display AST
				resultVar->setResultsToVec(root, fs); //add the tree results into metrics analysis
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
		}
		if (isFound == false)
		{
			std::cout << "Files with the specified pattern not found: \n";
			for (auto patt : pattColl)
				std::cout << "Pattern : " << patt << std::endl;
		}
		std::cout << "\n\n";
		return isFound;
	}
	catch (std::exception ex)
	{
		std::cout << "Error in TestExecutiveClass" << std::endl;
	}
}


#ifdef  TEST_METRICSEXEC
int main(int argc, char* argv[])
{
	//main entry for the program
	//collecting tokens from files, named on the command line
	if (argc < 2)
	{
		std::cout<< "\n Please enter name of file/directory to process on command line\n\n";
		return 1;
	}
	std::string fileSpec = "";
	TestExecutiveClass test;
	test.testReq1();
	test.testReq2();
	test.testReq3();
	test.testReq4();
	test.testReq5();
	test.testReq6();
	//test.Req1();
	fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
	std::string msg = "Processing files from the file/directory : " + fileSpec;
	Util::title(msg);
	std::vector<std::string> pattColl;
	for (int i = 2; i < argc; ++i)
	{
		pattColl.push_back(argv[i]);	    		
		putline();
	}
	bool fileStat = test.ExtractFiles(fileSpec,pattColl);
	if (fileStat)
	{
		test.testReq7();
		test.testReq8();
		test.testReq9();
		test.testReq10();
	}
	std::getchar();
	return 0;
}

#endif //  TEST_METRICSEXEC