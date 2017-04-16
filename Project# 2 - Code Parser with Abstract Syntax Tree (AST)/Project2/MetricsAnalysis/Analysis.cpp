/////////////////////////////////////////////////////////////////////
//  Analysis.cpp - Metrics Analysis Implementation   		       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//     Platform:   MacBook Pro, Core-i5, Windows 10 on Bootcamp    //
//       Author:   Manjunath Babu, MS in Computer Engineering      //
//                 (315) 440-1435, mababu@syr.edu                  //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Detailed implementation of Metrics Analysis

Maintenance History:
====================
ver 1.0 : 10 Mar 2016
- first release

*/
//
#include "Analysis.h"
#include <iomanip> 
#include"../ASTpkg/AbstractSyntaxTree.h"

void MetricsResults::DisplayMetricsFunction()
{
	std::cout << "\n";
	std::cout.width(80);std::cout << "COMPLEXITY ANALYSIS\n";
	std::cout.width(80); std::cout << "------------------\n";
	int n = 6;
	std::cout.width(20); std::cout << "FUNCTION";
	std::cout.width(20); std::cout << "SIZE";
	std::cout.width(20); std::cout << "COMPLEXITY";
	std::cout.width(20); std::cout << "FILENAME" << "\n";	
	std::vector<TreeNode*> res = getResultsFromVec();
	std::vector <std::string> files = getNames();
	for (int i = 0; i < res.size(); i++)
	{		
		DisplayMetricsHelper(res[i],files[i]);
	}
	res.clear();
}

std::vector<std::string> MetricsResults::getNames()
{
	return fileNames;
}

void MetricsResults::setName(std::string filename)
{
	fileNames.push_back(filename);
}

void MetricsResults::DisplayMetricsHelper(TreeNode* root,std::string file)
{
	
	if (root->gettype() == "function" || root->gettype() == "operator")
	{
		size_t tempVar = root->getendlinecount() - root->getstartlinecount();
		size_t exp = root->getendlinecount() - root->getstartlinecount() - 1;
		if (tempVar == 0 || tempVar == 1) 
		{
			exp = 1;
		}
		else
		{
			exp = tempVar - 1;
		}
		DisplayEachMetric(root->getname(), exp, root->getcomplexity(),file);
	}
	auto iter = root->getchildren().begin();
	while (iter != root->getchildren().end())
	{
		DisplayMetricsHelper(*iter,file);
		++iter;
	}
}

void MetricsResults::DisplayEachMetric(std::string name, size_t size, size_t complexity,std::string filename)
{
	std::cout.width(20); std::cout << name;
	std::cout.width(20); std::cout << size;
	std::cout.width(20); std::cout << complexity;
	std::cout.width(20); std::cout << getPath(filename) << "\n";
}

void MetricsResults::setResultsToVec(TreeNode*  elem, std::string fileName)
{
	fileNames.push_back(fileName);
	holdResultsVec.push_back(elem);
}


std::string MetricsResults::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(pos+1, fileSpec.length());
	return fileSpec;
}

std::vector<TreeNode*> MetricsResults::getResultsFromVec()
{
	return holdResultsVec;
}
#ifdef TEST_ANALYSIS

int main(int argc, char* argv[])
{
	MetricsResults *resultVar = new MetricsResults();
	TreeNode* root = new TreeNode("functionInsert", "textInsert", 34, 40);
	resultVar->setResultsToVec(root, "C:\Users\Manjunath Babu\Documents\OOD\Project 2\M\Project2HelperCode\Test.txt");
	std::vector<TreeNode*> vectres = resultVar->getResultsFromVec();	
	for (int i = 0; i < vectres.size(); i++)
	{
		std::cout << vectres[i] << std::endl;
	}
	vectres.clear();
}
#endif 

