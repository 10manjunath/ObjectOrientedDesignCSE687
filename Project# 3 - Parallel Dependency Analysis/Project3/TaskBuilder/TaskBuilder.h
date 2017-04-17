#ifndef TASKBUILDER_H
#define TASKBUILDER_H
/////////////////////////////////////////////////////////////////////////////
//  TaskBuilder.h     - This is the Task which holds the static threadpool //
//  ver 1.0                                                                //
//     Language:      C++, Visual Studio 2015                              //
//  Application:      Parallel Dependency Analysis	                       //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp         //
//       Author:      Manjunath Babu, MS in Computer Engineering           //
//                    (315) 450-1435, mababu@syr.edu                       //
/////////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
Defines TASK class holding static threadpool instance

*/
//
#include "../ThreadPool/ThreadPool.h"
class TaskHandlerClass
{
public:
	TaskHandlerClass() {
		
	}
	
	void StartThreadPool()
	{
	
		threadColObj->start();
	}
	std::mutex& getMutex() { return threadColObj->mtx; }
	void Executor(CallObj& co) 
	{
		TaskHandlerClass::threadColObj->doWork(&co);   
	}
	static ThreadPool *threadColObj; 
	
};


#endif