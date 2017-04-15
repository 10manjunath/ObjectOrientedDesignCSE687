#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
//  Platform:      MacBook Pro, Intel Core i5, 2.7 Ghz	             //
// Author:		Manjunath Babu. mababu@syr.edu. SUID: 515114647	     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.0 : 11 Jun 2014
 * - first release of new design
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;

  class Toker
  {
  public:
    Toker();
    Toker(const Toker&) = delete;
    ~Toker();
    Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
	void listOtherTokens(const std::string& commaSeparatedString);
  private:
    ConsumeState* pConsumer;
  };
}
#endif
#define a