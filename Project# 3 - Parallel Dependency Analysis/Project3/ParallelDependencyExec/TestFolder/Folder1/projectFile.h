#ifndef SCOPESTACK_H
#define SCOPESTACK_H
/////////////////////////////////////////////////////////////////////////////
// ScopeStack.h - implements template stack holding specified element type //
// ver 2.0                                                                 //
// Language:      Visual C++ 2010, SP1                                     //
// Platform:      Dell Precision T7400, Win 7 Pro SP1                      //
// Application:   Code Analysis Research                                   //
// Author:        Jim Fawcett, CST 4-187, Syracuse University              //
//                (315) 443-3948, jfawcett@twcny.rr.com                    //
/////////////////////////////////////////////////////////////////////////////
/*
  Package Operations:
  ===================
  This package contains one class: ScopeStack<element>.  The element type
  is an application specific type designed to hold whatever information you
  need to stack.

  Public Interface:
  =================
  ScopeStack<element> stack;
  // define some element type
  element elem;
  stack.push(elem);
  element popped = stack.pop();

  Required Files:
  ===============
  ScopeStack.h, ScopeStack.cpp

  Build Command:
  ==============
  devenv ScopeStack.sln /rebuild debug

  Maintenance History:
  ====================
  ver 2.1 : 02 Jun 11
  - changed ScopeStack type to single template argument intended to be
    an application specific type (usually a struct that holds whatever
    is needed)
  ver 2.0 : 02 Feb 11
  - fixed bug in pop() method found by Drumil Bhattad
  ver 1.0 : 31 Jan 11
  - first release
*/
#include <list>
// element is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.

template<typename element>
class ScopeStack {
public:
  typename typedef std::list<element>::iterator iterator;
  void push(const element& item);
  element pop();
  element top();
  size_t size();
  iterator begin();
  iterator end();
private:
  std::list<element> stack;
};

template<typename element>
typename inline size_t ScopeStack<element>::size() { return stack.size(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::begin() { return stack.begin(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::end() { return stack.end(); }

template<typename element>
void ScopeStack<element>::push(const element& item)
{
  stack.push_back(item);
}

template<typename element>
element ScopeStack<element>::pop()
{
  element item = stack.back();
  stack.pop_back();
  return item;
}

template<typename element>
element ScopeStack<element>::top()
{
  return stack.back();
}

template<typename element>
void showStack(ScopeStack<element>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    element temp = *iter;
    std::cout << "\n  " << strIndent << temp->show();
    ++iter;
  }
}

template<typename element>
void showStack(ScopeStack<element*>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element*>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    std::cout << "\n  " << strIndent << ((*iter)->show());
    ++iter;
  }
}
#endif
  - Added displayResults to display Ast and function details
  ver 1.4 : 15 Feb 16
  - removed all folding rules code
  - changed solution folder layout which caused changes in many of the
    include paths
  - small modifications to one of the derived actions
  - now preface (new) Toker and SemiExp with Scanner namespace
  ver 1.3 : 02 Jun 11
  - added use of folding rules in get();
  ver 1.2 : 17 Jan 09
  - modified to accept pointer to interface ITokCollection instead of
    SemiExpression
  ver 1.1 : 17 Sep 07
  - cosmetic modifications to comments on this page
  ver 1.0 : 12 Jan 06
  - first release
*/

#include <string>
#include <iostream>
#include <vector>
#include "../SemiExp/itokcollection.h"

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action
class Parser;

class IBuilder
{
public:
  virtual ~IBuilder() {}
  virtual Parser* Build()=0;
};

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action

class IAction
{
public:
  virtual ~IAction() {}
  virtual void doAction(ITokCollection*& pTc)=0;
};

///////////////////////////////////////////////////////////////
// abstract base class for parser language construct detections
//   - rules are registered with the parser for use

class IRule
{
public:
  virtual ~IRule() {}
  void addAction(IAction* pAction);
  void doActions(ITokCollection*& pTc);
  virtual bool doTest(ITokCollection*& pTc)=0;
protected:
  std::vector<IAction*> actions;
};

//Parser class that contains all the rules and current token
class Parser
{
public:
  Parser(ITokCollection* pTokCollection);
  ~Parser();
  void addRule(IRule* pRule);
  bool parse();
  void displayResults();
  bool next();
private:
  ITokCollection* pTokColl;
  std::vector<IRule*> rules;
};

inline Parser::Parser(ITokCollection* pTokCollection) : pTokColl(pTokCollection) {}

inline Parser::~Parser() {}
#endif
