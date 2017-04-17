//////////////////////////////////////////////////////////////////////////////
//  ParallelDependencyExec.cpp - Demonstrate Various features of Project 3  //
//  ver 1.0                                                                 //
//     Language:       C++, Visual Studio 2015                              //
//  Application:       Parallel Dependency Analysis				            //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp         //
//       Author:       Manjunath Babu, MS in Computer Engineering           //
//                     (315) 450-1435, mababu@syr.edu                       //
//////////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Demonstrate Various features of Project 3

*/
//

#include "ParallelDependencyExec.h"
using namespace Utilities;
#ifdef PARALLEL_DEP_EXECNAME

int main(int argc, char* argv[])
{
	// Main entry
	if (argc < 2)
	{
		std::cout << "\n Input name of File or Directory \n\n";
		return 1;
	}
	std::string fileSpec = "";
	TestExecutiveClass testingVar;
	testingVar.ShowReq();
	testingVar.testVar->startThreadsFunc();
	fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
	std::cout << "\n\n";
	std::string msg = "Processing files from the file/directory : " + fileSpec;
	std::cout << msg << "\n";
	std::vector<std::string> pattColl;
	std::cout << "Patterns identified: ";
	for (int i = 2; i < argc; ++i)
	{
		std::cout << argv[i];
		pattColl.push_back(argv[i]);
		std::cout << "; ";
	}
	std::cout << "\n";		
    testingVar.testVar->CalculateDependency(fileSpec,pattColl); 
	return 0;
}


void TestExecutiveClass::ShowReq()
{
	std::cout << "********************* Parallel Dependency Analysis *********************" << "\n\n";
	
	StringHelper::Title("DEMONSTRATING REQUIREMENT 1", '*');
	std::cout << "\nUsed Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs.\n";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 2", '*');
	std::cout << "\nUsed C++ standard library's streams for all I/O and new and delete for all heap-based memory management.\n";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 3", '*');
	std::cout << "\nProvided C++ packages as described in the Purpose section. You are free to alter the names and division of responsibilities as you see fit. However, you are expected to enforce the \"Single Responsiblity Principle\" as illustrated in the package structure described above.";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 4", '*');
	std::cout << "\nProvided a ThreadPool package that declares and defines a ThreadPool class that executes enqueued work items asynchronously";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 5", '*');
	std::cout << "\nProvided a ThreadPool package that declares and defines a ThreadPool class that executes enqueued work items asynchronously";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 6", '*');
	std::cout << "\nProvided TypeAnalysis package that identifies all of the types defined in a specified file.";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 7", '*');
	std::cout << "\nProvide a DependencyAnalysis package that identifies all of the dependencies between files in a specified file collection.";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 8", '*');
	std::cout << "\nProvide ParallelDependencyAnalysis package that finds compilation dependencies between all of the files in a specified file collection using the asynchronous processing facilities provided by the Task package.";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 9", '*');
	std::cout << "\nProvided a ParallelDependencyExecutive that finds all the compilation dependencies between files in a file collection. Each of the files in the collection are found in a directory tree rooted at a specified path and whose names match one or more specified patterns.";
	std::cout << std::endl << std::endl;

	StringHelper::Title("DEMONSTRATING REQUIREMENT 10", '*');
	std::cout << "\nIncluded an automated unit test suite that demonstrates you meet all the requirements of this project";
	std::cout << std::endl << std::endl;

}
#endif // DEBUG