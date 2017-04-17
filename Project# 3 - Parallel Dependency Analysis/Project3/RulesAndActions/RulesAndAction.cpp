//////////////////////////////////////////////////////////////////////
//  RulesAndAction.cpp - declares new parsing rules and actions     //
//  ver 3.0                                                         //
//  Language:      C++ Visual Studio 2015                           //
//  Application:   Prototype for CSE687 Pr1, Sp09                   //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//      Source:        Jim Fawcett, CST 4-187, Syracuse University  //
//                     (315) 443-3948, jfawcett@twcny.rr.com        //
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
rules and actions for various scopes




*/
//



#ifdef TEST_ACTIONSANDRULES

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ActionsAndRules class\n "
		<< std::string(30, '=') << std::endl;

	try
	{
		std::queue<std::string> resultsQ;
		PreprocToQ ppq(resultsQ);
		PreprocStatement pps;
		pps.addAction(&ppq);

		FunctionDefinition fnd;
		PrettyPrintToQ pprtQ(resultsQ);
		fnd.addAction(&pprtQ);

		Toker toker("../RulesAndAction.h");
		SemiExp se(&toker);
		Parser parser(&se);
		parser.addRule(&pps);
		parser.addRule(&fnd);
		while (se.get())
			parser.parse();
		size_t len = resultsQ.size();
		for (size_t i = 0; i<len; ++i)
		{
			std::cout << "\n  " << resultsQ.front().c_str();
			resultsQ.pop();
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
}
#endif
