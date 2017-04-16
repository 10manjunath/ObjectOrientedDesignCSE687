#ifndef ANALYSIS_H
#define ANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  Analysis.h -    Interface for Metrics Analysis  		       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Application:   Prototype for CSE687 Pr1                        //
//     Platform:   MacBook Pro, Core-i5, Windows 10 on Bootcamp    //
//       Author:   Manjunath Babu, MS in Computer Engineering      //
//                 (315) 440-1435, mababu@syr.edu                  //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Metric Analysis to display functions

Maintenance History:
====================
ver 1.0 : 10 Mar 2016
- first release

*/
//
#include"../ASTpkg/AbstractSyntaxTree.h"
class MetricsResults
{
private:
	std::vector<TreeNode*> holdResultsVec; 
	std::vector<std::string> fileNames; 
public:
	void setResultsToVec(TreeNode*,std::string);
	std::vector<TreeNode*> getResultsFromVec();	
	void DisplayMetricsFunction();
	void DisplayMetricsHelper(TreeNode*,std::string);
	void DisplayEachMetric(std::string, size_t,size_t,std::string);
	std::vector<std::string> getNames();
	void setName(std::string);
	std::string getPath(const std::string & fileSpecVar);
};
#endif