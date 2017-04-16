///////////////////////////////////////////////////////////////////////////
//  AbstractSyntaxTree.cpp - Define Tree for parsing                     //
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
This module defines actions for Abstract Syntax Tree

Maintenance History:
====================
ver 1.0 : 10 Mar 2016
- first release

*/
//
#include "AbstractSyntaxTree.h"

std::string TreeNode::show()
{	
	std::ostringstream temp;
	temp << "(";
	temp << gettype();
	temp << ", ";
	temp << getname();
	temp << ", ";
	temp << getstartlinecount();
	temp << ", ";
	temp << getendlinecount();
	temp << ", ";
	temp << getcomplexity();
	temp << ")";
	temp << "\n";	
	return temp.str();
}

TreeNode* AbstractSyntaxTree::DisplayAbstractSyntaxTree(TreeNode* root)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << root->show();
	auto iter = root->getchildren().begin();
	++indentLevel;
	while (iter != root->getchildren().end())
	{
		DisplayAbstractSyntaxTree(*iter);
		++iter;
	}
	--indentLevel;
	return root;
}


TreeNode* AbstractSyntaxTree::getRootElem()
{
	return root;
}

bool AbstractSyntaxTree::addTreeElemAST(TreeNode* elem)
{
	root->addChild(elem);
	return true;
}


#ifdef TEST_AST
int main()
{
	AbstractSyntaxTree* clAst;
	TreeNode* elem;
	elem->type = "namespace";
	elem->name = "global namespace";
	elem->startLineCount = 0;
	elem->endLineCount = 0;
	clAst->addTreeElemAST(elem);
	elem.show();
}
#endif 
