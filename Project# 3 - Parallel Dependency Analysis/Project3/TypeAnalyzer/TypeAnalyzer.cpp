//////////////////////////////////////////////////////////////////////
//  TypeAnalyzer.cpp - Compute type table for every file.  	        //
//   ver 1.0                                                        //
//   Language:      C++, Visual Studio 2015                         //
//  Application:   Parallel Dependency Analysis				        //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//       Source:       Jim Fawcett, Syracuse University, CST 4-187  //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Calculate type analysis for every file

*/
//
#include "TypeAnalyzer.h"

MainTypeTable<TypeTableValData>* TypeAnalClass::AnalyzeFileTypes(std::string file,bool& status_file)
{
	try
	{
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build(); 
		try
		{
			if (pParser)
			{
				if (!configure.Attach(file))
					std::cout << "\n  Error: file cant be opened " << file << std::endl;
			}
			else
				std::cout << "\n\n  Parser building failed\n\n";
			while (pParser->next())
				pParser->parse();
		
			MainTypeTable<TypeTableValData>* table = configure.GetTypeTable();
			status_file = true;
			
			return table;		
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		return new MainTypeTable<TypeTableValData>;
	}
	catch (std::exception ex)
	{
		std::cout << "Error!!" << std::endl;
	}
	return new MainTypeTable<TypeTableValData>;
}

#ifdef TEST_TYPE_ANALYZER
int main(int argc, char* argv[])
{
	std::string fileName = "C:/Users/Manjunath Babu/Documents/OOD/Project 3/File1.txt";
	std::cout << fileName;
	TypeAnalClass *tan = new TypeAnalClass();
	bool status = false;
	tan->AnalyzeFileTypes(fileName,status);
	if (status == true)
		std::cout << "Processed success\n";
}
#endif


