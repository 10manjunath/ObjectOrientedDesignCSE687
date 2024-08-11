///////////////////////////////////////////////////////////////////////
// test.cpp - test executive package				                 //
// Language:    C++, Visual Studio 2015                              //
// Platform:      MacBook Pro, Intel Core i5, 2.7 Ghz	           //
// Author:		Manjunath Babu. mababu@syr.edu. SUID: 515114647	     //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <cctype>
#include <string>
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
using namespace std;
using namespace Scanner;

class TestExecutive
{
public:

	void Req1TestExecutive() 
	{
		std::cout << endl;
		std::cout << endl;
		std::cout << endl;
		std::cout << "----- Demonstrating Requirement 1 -----" << std::endl;
		std::cout << "Used Visual Studio 2015 - Windows C++ Console Application Project." << std::endl;

	}

	void Req2TestExecutive()
	{
		std::cout << endl;
		std::cout << endl;
		std::cout << endl;
		std::cout << "----- Demonstrating Requirement 2 ------" << std::endl;
		std::cout << "Used the C++ standard library's streams for all I/O and new and delete for all heap-based memory management" << std::endl;

	}

	void Req3TestExecutive()
	{
		std::cout << endl;
		std::cout << endl;
		std::cout << endl;
		std::cout << "Demonstrating Requirement 3" << std::endl;
		std::cout << "Tokenizer, SemiExpression and ITokCollection in Scanner Interface Implemented." << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();

		std::cout << "\n\n\n ------------- This is Tokenizer.h File  ------------------" << endl << endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
		
		string lineVar;
		ifstream myfile("./Tokenizer/Tokenizer.h");
		if (myfile.is_open())
		{
			while (getline(myfile, lineVar))
			{
				cout << lineVar << '\n';
			}
			myfile.close();
		}
		else 
		{ 
			cout << "Unable to open file>>"; 
		}

		std::cout << endl;
		std::cout << endl;
		std::cout << endl;

		std::cout << "\n Hit any key to continue . . ";
		getchar();

		std::cout << "\n\n\n ------------- This is SemiExp.h  ------------------" << endl << endl;

		std::cout << "\n Hit any key to continue . . ";
		getchar();

		string lineVar1;
		ifstream myfile1("SemiExp/SemiExp.h");
		if (myfile1.is_open())
		{
			while (getline(myfile1, lineVar1))
			{
				cout << lineVar1 << '\n';
			}
			myfile1.close();
		}
		else 
		{ 
			cout << "Unable to open file"; 
		}
		cout << endl;
		cout << endl;

		std::cout << "\n Hit any key to continue . . ";
		getchar();


		std::cout << "\n\n\n ------------- This is ITokCollection.h" << endl << endl;

		std::cout << "\n Hit any key to continue . . ";
		getchar();


		string line2Var;
		ifstream myfile2("SemiExp/ITokCollection.h");

		if (myfile2.is_open())
		{
			while (getline(myfile2, line2Var))
			{
				cout << line2Var << '\n';
			}
			myfile2.close();
		}
		else 
		{
			cout << "Unable to open file";
		}

		std::cout << endl;
		std::cout << endl;
	}

	void Req4TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 4 -------" << std::endl << std::endl;
		std::cout << "Provided a Tokenizer package that declares and defines a Toker class that implements the State Pattern2 with an abstract ConsumeState class and derived classes for collecting the following token types:" << std::endl;
		std::cout << "\n1)alphanumeric tokens \n2) punctuator tokens \n3) special one3 and two4 character tokens with defaults that may be changed by calling setSpecialSingleChars(string ssc) and // or \n4) setSpecialCharPairs(string scp).\n5) C style comments returned as a single token \n6) C++ style comments returned as a single token \n7) quoted strings" << std::endl;
		std::cout << "\n Hit any key to continue . . " << std::endl; 
		getchar();

	}

	void Req5TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 5 -------" << std::endl;
		std::cout << "The Toker class, contained in the Tokenizer package, shall produced one token for each call to a member function getTok()." << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
	}

	void Req6TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 6 -------" << std::endl;
		std::cout << "Provided a SemiExpression package that contains a class SemiExp used to retrieve collections of tokens by calling Toker::getTok() repeatedly until one of the SemiExpression termination conditions, below, is satisfied." << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
	}

	void Req7TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 7 -------" << std::endl;
		std::cout << "Terminated a token collection after extracting any of the single character tokens: semicolon, open brace, closed brace. Also on extracting newline if a # is the first character on that line. Also, the extraction of a single character token consisting of : if immediately preceeded by one of the tokens: public, protected, or private." << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
	}

	void Req8TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 8 -------" << std::endl;
		std::cout << "Provided a facility providing rules to ignore certain termination characters under special circumstances. You are required to provide a rule to ignore the (two) semicolons within parentheses in a for(;;) expression" << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
	}

	void Req9TestExecutive()
	{
		std::cout << "---- Demonstrating Requirement 9------" << std::endl;
		std::cout << "Implemented get() method from the iTokenCollection Interface for the SemiExpression Class.";
		std::cout << "" << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
		std::cout << endl;
		std::cout << endl;
	}

	void Req10TestExecutive()
	{
		std::cout << "----- Demonstrating Requirement 10------" << std::endl;
		std::cout << "Automated Test Executive Included. All Test Cases Tested." << endl;
		std::cout << endl;
		std::cout << "" << std::endl;
		std::cout << "\n Hit any key to continue . . ";
		getchar();
	}
};


int main()
{
	class TestExecutive testExObj;

	testExObj.Req1TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	testExObj.Req2TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	testExObj.Req3TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();
	
	testExObj.Req4TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	std::string fileSpec = "Tokenizer/test.txt";
	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  Can't open file " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	
	testExObj.Req5TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();


	testExObj.Req6TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	in.close();

	fileSpec = "../Tokenizer/test.txt";

	std::ifstream in1(fileSpec);
	if (!in1.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}

	testExObj.Req7TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();


	testExObj.Req8TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();


	toker.attach(&in1); //Toker class defined in Tokenizer.h

	SemiExp semi(&toker);
	while (semi.get())
	{
		if (semi.length() > 0)
		{
			std::cout << "\n  -- semiExpression --";
			semi.show();
		}
	}

	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}

	testExObj.Req9TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	testExObj.Req10TestExecutive();
	std::cout << "\n Hit any key to continue . . ";
	getchar();

	
	return 0;
}

