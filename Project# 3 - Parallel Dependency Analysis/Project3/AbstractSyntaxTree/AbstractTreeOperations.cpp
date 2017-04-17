//////////////////////////////////////////////////////////////////////
//  AbstractTreeOperations.cpp - New parsing Rules                  //
//  ver 3.0                                                         //
//  Language:          C++, Visual Studio 2015                      //
//  Application:       Abstract Tree Operations			            //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//       Source:       Jim Fawcett, Syracuse University, CST 4-187  //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Defines operations to generate and display an Abstract Syntax Tree 

*/
//
#include "AbstractTreeOperations.h"

//show the element
std::string TreeNode::show()
{	
	std::ostringstream temp;
	temp << "(";
	temp << gettype();
	temp << ", ";
	temp << getname();
	temp << ", ";
	temp << getStartLineNumber();
	temp << ", ";
	temp << getEndLineNumber();
	temp << ", ";
	temp << getComplexityLevel();
	temp << ")";
	temp << "\n";	
	return temp.str();
}

TreeNode* AbstractSyntaxTree::DisplayAbstractSyntaxTree(TreeNode* root)
{
	static size_t holdIndentLevel = 0;
	std::cout << "\n  " << std::string(2 * holdIndentLevel, ' ') << root->show();
	auto iter = root->getChildren().begin();
	++holdIndentLevel;
	while (iter != root->getChildren().end())
	{
		DisplayAbstractSyntaxTree(*iter);
		++iter;
	}
	--holdIndentLevel;
	return root;
}

TreeNode* AbstractSyntaxTree::GetRootElem()
{
	return root;
}

bool AbstractSyntaxTree::AddToASTRoot(TreeNode* elem)
{
	root->addChildElem(elem);
	return true;
}


#ifdef TEST_AST
int main()
{
	AbstractSyntaxTree* clAst;
	TreeNode* elem;
	elem->type = "namespace";
	elem->name = "global namespace";
	elem->startLineNumber = 0;
	elem->endLineNumber = 0;
	clAst->AddToASTRoot(elem);
	elem.show();
}
#endif 
