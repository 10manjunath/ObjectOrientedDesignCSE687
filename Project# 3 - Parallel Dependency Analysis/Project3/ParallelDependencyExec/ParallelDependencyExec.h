#ifndef PARALLELDEPENDENCYEXEC_H
#define PARALLELDEPENDENCYEXEC_H
/////////////////////////////////////////////////////////////////////
//  ParallelDependencyExec.h - Test executive for Project 3        //
//  ver 1.0                                                        //
//     Language:      C++, Visual Studio 2015                      //
//  Application:      Parallel Dependency Analysis				   //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:      Manjunath Babu, MS in Computer Engineering   //
//                    (315) 450-1435, mababu@syr.edu               //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Tests requirements of Project 3

*/
//

#include<string>
#include"../FileMgr/FileSystem.h"
#include<iostream>
#include<vector>
#include"../ParallelDepAnlzr/ParallelDepAnlzr.h"


class TestExecutiveClass
{
public:
	ParallelDepAnalysis* testVar;
	TestExecutiveClass()
	{
		testVar = new ParallelDepAnalysis();
	}
	void ShowReq();
};
#endif