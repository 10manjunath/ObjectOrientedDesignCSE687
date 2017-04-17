#ifndef ABSTRACTTREEOPERATIONS_H
#define ABSTRACTTREEOPERATIONS_H
////////////////////////////////////////////////////////////////////////
//  AbstractTreeOperations.h - declares new parsing rules and actions //
//      ver 3.5                                                       //
//	   Language:       C++, Visual Studio 2015                        //
//  Application:       Perform Tree Operations				          //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp   //
//       Author:       Manjunath Babu, MS in Computer Engineering     //
//                     (315) 450-1435, mababu@syr.edu                 //
//		 Source:       Jim Fawcett, Syracuse University, CST 4-187    //
////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Operations of an abstract syntax tree

*/
//
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
	TreeNode()
	{
			
	}

	TreeNode(const std::string type, const std::string name , size_t startLineCount, size_t endLineCount)
	{
		this->name = name;
		this->type = type;
		this->startLineNumber = startLineCount;
		this->endLineNumber = endLineCount;
	}
	
	std::vector<TreeNode*>& getChildren() 
	{ 
		return _childrenVec; 
	}

	
	void addChildElem(TreeNode* pNode) 
	{ 
		_childrenVec.push_back(pNode);
	}

	void setNameToElem(std::string name) 
	{ 
		this->name = name; 
	};
	std::string getname() 
	{ 
		return name;
	}
	

	void setTypeToElem(std::string type) 
	{ 
		this->type = type; 
	};

	std::string gettype() 
	{ 
		return type; 
	}

	void setStartLineNumber(size_t startCount)
	{ 
		this->startLineNumber =startCount; 
	}
	size_t getStartLineNumber()
	{ 
		return startLineNumber; 
	}
	

	void setEndLineNumber(size_t endCount) 
	{ 
		this->endLineNumber = endCount; 
	}
	size_t getEndLineNumber()
	{
		return endLineNumber; 
	}
	

	void setComplexityLevel(size_t comp) 
	{ 
		this->complexityLevel = comp;
	}
	size_t getComplexityLevel() 
	{
		return complexityLevel; 
	}
	
	std::string show();

private:

	size_t startLineNumber;
	size_t endLineNumber;
	size_t complexityLevel = 1;
	std::vector<TreeNode*> _childrenVec;
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
		root = new TreeNode("namespace", "global namespace",1,1);		
	}
	TreeNode* GetRootElem();
	bool AddToASTRoot(TreeNode* elem);
	TreeNode* DisplayAbstractSyntaxTree(TreeNode* root);
};
#endif