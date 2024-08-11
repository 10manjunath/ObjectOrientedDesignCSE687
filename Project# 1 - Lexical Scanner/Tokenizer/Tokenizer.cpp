/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Platform:      MacBook Pro, Intel Core i5, 2.7 Ghz	           //
// Author:		Manjunath Babu. mababu@syr.edu. SUID: 515114647	   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
  Helper code that does not attempt to handle all tokenizing
  special cases like escaped characters.
*/
//#define TEST_LOG

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
namespace Scanner
{
	class ConsumeState
	{
	public:
		using Token = std::string;  // manju
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void takeChars() {
			if (_pState == nullptr)
			{
				_pState = _pEatWhitespace; // manju
			}
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		void listOtherTokens(const std::string& commaSeparatedString); // manju
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatSQuote; // manju
		static ConsumeState* _pEatDQuote; // manju
		static ConsumeState* _pEatOtherChars; // manju
		static std::vector<std::string> singleCharTokVec; // manju
		static std::vector<std::string> doubleCharTokVec; // manju
		bool testOneCharTokCondition(Token tok); // manju
		bool testTwoCharTokCondition(Token tok); // manju
		Token charToStringBuilderFunc(int ch); // manju
	};
}

using namespace Scanner;
using Token = std::string;
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatSQuote = nullptr;  // manju
ConsumeState* ConsumeState::_pEatDQuote = nullptr;  // manju
ConsumeState* ConsumeState::_pEatOtherChars = nullptr;  // manju
std::vector<std::string> ConsumeState::singleCharTokVec =
{
	"<", ">", "{", "}", "[", "]", "(", ")", ":", "=", "+", "-", "*", ".","\n"
};  // manju
std::vector<std::string> ConsumeState::doubleCharTokVec =
{
	"<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/="
}; // manju

void testLog(const std::string& msg);

std::vector<std::string> split(const std::string& src)
{
	std::vector<std::string> accum;
	std::string holdVar;
	size_t indexVar = 0;
	do
	{
		while ((isspace(src[indexVar]) || src[indexVar] == ',') && src[indexVar] != '\n')
		{
			indexVar++;
			if (holdVar.size() > 0)
			{
				accum.push_back(holdVar);
				holdVar.clear();
			}
		}
		if (src[indexVar] != '\0')
			holdVar += src[indexVar];
	} while (indexVar++ < src.size());
	if (holdVar.size() > 0)
		accum.push_back(holdVar);
	return accum;
}

void ConsumeState::listOtherTokens(const std::string& csvToks)
{
	std::vector<Token> result = split(csvToks);
	singleCharTokVec.clear();
	doubleCharTokVec.clear();
	for (std::string item : result)
	{
		if (item.size() == 1 || item == "\n")
			singleCharTokVec.push_back(item);
		if (item.size() >= 2)
			doubleCharTokVec.push_back(item);
	}
}

bool ConsumeState::testOneCharTokCondition(Token tokVar) // manju
{
	size_t i;
	for (i = 0; i < singleCharTokVec.size(); i++)
		if (singleCharTokVec.at(i) == tokVar)
			return true;
	return false;
}

bool ConsumeState::testTwoCharTokCondition(Token tokVar) // manju
{
	size_t i;
	for (i = 0; i < doubleCharTokVec.size(); ++i)
		if (doubleCharTokVec[i] == tokVar)
			return true;
	return false;
}

ConsumeState::Token ConsumeState::charToStringBuilderFunc(int ch) // manju
{
	Token tempStringVar;
	tempStringVar += ch;
	return tempStringVar;
}

ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
	{
		return nullptr;
	}
	int chNext = _pIn->peek();
	if (chNext == EOF)
	{
		_pIn->clear();
	}
	if (testOneCharTokCondition(charToStringBuilderFunc(currChar))) // manju
	{
		testLog("state: oneCharToken");
		return _pEatOtherChars;
	}

	if (testTwoCharTokCondition(charToStringBuilderFunc(currChar))) // manju
	{
		testLog("state: twoCharToken");
		return _pEatOtherChars;
	}
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (currChar == '\'' && prevChar != '\\') {
		testLog("state: eatSQuote");   // manju
		return _pEatSQuote;
	}
	if (currChar == '\"' && prevChar != '\\') {
		testLog("state: eatSQuote");   // manju
		return _pEatDQuote;
	}
	if (ispunct(currChar))
	{
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}
	if (!_pIn->good())
	{
		return _pEatWhitespace;
	}
	throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {
			if (!_pIn->good())
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			token += currChar;         // Edited by manjunath
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
	}
};

class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C comment";
		do {
			token += currChar;           // Edited by manjunath
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += "*/";
		_pIn->get();
		currChar = _pIn->get();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar));
	}
};

class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

class EatSQuote : public ConsumeState     // manju TOTO mod
{
public:
	virtual void eatChars()
	{
		do {
			if (!_pIn->good())
				return;		// end of stream

			prevChar = currChar;
			currChar = _pIn->get();

			if (currChar == '\\' && prevChar == '\'') {
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
			}
			if (currChar != '\'')
				token += currChar;
		} while (currChar != '\'');
		currChar = _pIn->get();
	}
};

class EatDQuote : public ConsumeState     // manju TOTO mod
{
public:
	virtual void eatChars()
	{
		do {
			if (!_pIn->good())
				return;		// end of stream

			prevChar = currChar;
			currChar = _pIn->get();

			if (currChar == '\\' && prevChar == '\"') {
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
			}
			if (currChar != '\"')
				token += currChar;
		} while (currChar != '\"');
		currChar = _pIn->get();

	}
};

class EatOtherChars : public ConsumeState  // manju
{
public:
	virtual void eatChars()
	{
		token.clear();
		token += currChar;
		Token temp = charToStringBuilderFunc(currChar) += _pIn->peek();
		if (testTwoCharTokCondition(temp))
		{
			prevChar = currChar;
			token += currChar;
			currChar = _pIn->get();
		}

		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

ConsumeState::ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pState = _pEatWhitespace;
		_pEatSQuote = new EatSQuote();  // manju
		_pEatDQuote = new EatDQuote();  // manju
		_pEatOtherChars = new EatOtherChars(); // manju
	}
}

ConsumeState::~ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatSQuote; // manju
		delete _pEatDQuote; // manju
		delete _pEatOtherChars; // manju
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead()) // End of File Stream
			return "";
		pConsumer->takeChars(); // Attempt to fill token
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok(); // return filled token
}


void Toker::listOtherTokens(const std::string& commaSeparatedTokens)
{
	pConsumer->listOtherTokens(commaSeparatedTokens);
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";
	
	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	in.clear();
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	//std::getchar(); // console key problem. TODO: fix later
	return 0;
}
#endif
