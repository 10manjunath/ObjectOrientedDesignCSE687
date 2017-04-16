#ifndef AbstractSyntaxTree_H
#define AbstractSyntaxTree_H
///////////////////////////////////////////////////////////////////////////
//  AbstractSyntaxTree.h - Define Tree for parsing                       //
//          ver:       3.0                                               //
//     Language:       C++, Visual Studio 2015                           //
//  Application:       Abstract Syntax Tree				                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 440-1435, mababu@syr.edu                    //
//       Source:       Jim Fawcett, CST 4-187, Syracuse University       //
//                     (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module defines ops to get AST and display AST

Build Process:
==============
Required files
- AbstractSyntaxTree.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
*/

#include <queue>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

class TreeNode
{
public:
	TreeNode() {}
	TreeNode(const std::string type, const std::string name, size_t startLineCount, size_t endLineCount)
	{
		this->name = name;
		this->type = type;
		this->startLineCount = startLineCount;
		this->endLineCount = endLineCount;
	}

	std::vector<TreeNode*>& getchildren()
	{
		return _children;
	}

	void addChild(TreeNode* pNode)
	{
		_children.push_back(pNode);
	}

	void setname(std::string name)
	{
		this->name = name;
	};

	std::string getname()
	{ 
		return name;
	}

	void settype(std::string type)
	{ 
		this->type = type; 
	};

	std::string gettype() 
	{ 
		return type; 
	}

	void setstartlinecount(size_t startCount)
	{ 
		this->startLineCount = startCount;
	}

	size_t getstartlinecount()
	{
		return startLineCount;
	}

	void setendlinecount(size_t endCount) 
	{ 
		this->endLineCount = endCount;
	}

	size_t getendlinecount()
	{ 
		return endLineCount;
	}

	void setcomplexity(size_t comp)
	{ 
		this->complexityCount = comp;
	}

	size_t getcomplexity()
	{ 
		return complexityCount;
	}

	std::string show();

private:
	size_t startLineCount;
	size_t endLineCount;
	size_t complexityCount = 1;
	std::vector<TreeNode*> _children;
	std::string type;
	std::string name;
};

class AbstractSyntaxTree
{
private:
	TreeNode* root;
public:
	AbstractSyntaxTree()
	{
		root = new TreeNode("namespace", "global namespace", 1, 1);
	}
	TreeNode* getRootElem();
	bool addTreeElemAST(TreeNode* elem);
	TreeNode* DisplayAbstractSyntaxTree(TreeNode* root);
};
#endif