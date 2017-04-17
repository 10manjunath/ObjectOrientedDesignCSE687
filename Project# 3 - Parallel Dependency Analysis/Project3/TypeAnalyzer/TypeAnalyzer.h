#ifndef TYPEANALYZER_H
#define TYPEANALYZER_H
//////////////////////////////////////////////////////////////////////
//  TypeAnalyzer.h - calculate the type table for each file		    //
//  ver 1.0                                                         //
//     Language:       C++, Visual Studio 2015                      //
//  Application:	   Parallel Dependency Analysis				    //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//       Source:       Jim Fawcett, Syracuse University, CST 4-187  //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Calculate the type analyzing in every file



*/
//
#include<vector>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include"../Parser/Parser.h"
#include"../Parser/ConfigureParser.h"
#include"../TypeTable/TypeTable.h"

class TypeAnalClass
{
public:
	MainTypeTable<TypeTableValData>* AnalyzeFileTypes(std::string file, bool &status_file); //anlzFunc the types for file
};
#endif