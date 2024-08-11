///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Platform:    MacBook Pro, Intel Core i5, 2.7 Ghz	             //
// Author:		Manjunath Babu. mababu@syr.edu. SUID: 515114647	     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <algorithm>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
using namespace Scanner;

// Constructor of SemiExp Class
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get(bool clear)
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	clearToken();

	while (true)
	{
		std::string token = _pToker->getTok();
		tokLocalVecVar.push_back(token);

		if (token == "") {
			break;
		}
		for (int i = 0; i < tokLocalVecVar.size(); i++)
		{
			if (token[0] == '/' || (token[1] == '/' || token[1] == '*'))
			{
				remove(token);
			}
		}
		
		if (token == "\n")// added for header files
		{
			newLineRemove();
			for (int i = tokLocalVecVar.size() - 1; i >= 0; i--)
			{
				if (tokLocalVecVar[i] == "#")
					return true;
			}
		}
		if (token == "for")
		{
			do
			{
				token = _pToker->getTok();
				tokLocalVecVar.push_back(token);
			} while (token != ")");

			return true;
		}
		if (token == ":")
		{
			if (tokLocalVecVar[tokLocalVecVar.size() - 2] == "protected" || tokLocalVecVar[tokLocalVecVar.size() - 2] == "private" || tokLocalVecVar[tokLocalVecVar.size() - 2] == "public")
				return true;
		}
		if (token == "{" || token == ";" || token == "}")
			return true;
	}
	return false;
}

bool SemiExp::endConditionCheck(std::string tok)
{

	if (tok == "{" || tok == ";" || tok == "}")
		return true;
}

Token SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= tokLocalVecVar.size())
		throw(std::invalid_argument("index out of range"));
	return tokLocalVecVar[n];
}

size_t SemiExp::length()
{
	return tokLocalVecVar.size();
}

void SemiExp::show()
{
	std::cout << "\n  ";
	toLower();
	for (auto token : tokLocalVecVar)
		if (token != "\n")
			std::cout << token << " ";
	std::cout << "\n";
}

size_t SemiExp::find(const std::string& token) 
{
	for (size_t i = 0; i<length(); ++i)
		if (token == tokLocalVecVar.at(i))
			return i;
	return length();
}

void SemiExp::push_back(const std::string& token)
{
	tokLocalVecVar.push_back(token);
}

bool SemiExp::remove(const std::string& token)
{
	std::vector<std::string>::iterator iterator;
	iterator = std::find(tokLocalVecVar.begin(), tokLocalVecVar.end(), token);
	if (iterator != tokLocalVecVar.end())
	{
		tokLocalVecVar.erase(iterator);
		return true;
	}
	return false;
}

bool SemiExp::remove(size_t index)
{
	if (index < 0 || index> tokLocalVecVar.size())
	{
		return false;
	}
	std::vector<std::string>::iterator iterator = tokLocalVecVar.begin();
	tokLocalVecVar.erase(iterator + index);
}

void SemiExp::toLower()
{
	int i, j;
	for (i = 0; i<tokLocalVecVar.size(); i++)
	{
		for (j = 0; j <((tokLocalVecVar)[i]).length(); j++)
		{
			((tokLocalVecVar)[i])[j] = tolower(((tokLocalVecVar)[i])[j]);
		}
	}
}

void SemiExp::newLineRemove()
{
	while (length() >= 1)
	{
		if (tokLocalVecVar.at(0) == "\n")
		{
			remove(0);

		}
		else
		{
			break;
		}
	}
}
// clear token vector
void SemiExp::clearToken()
{

	tokLocalVecVar.clear();

}

#ifdef TEST_SEMIEXP

int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/test.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

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

	getchar(); // manju TODO: Fix this soon
	return 0;
}
#endif // DEBUG