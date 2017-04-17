#ifndef THREADPOOL_H
#define THREADPOOL_H

/////////////////////////////////////////////////////////////////////
// ThreadPool.h - A single child thread processes work items	   //
// enqueued by main thread										   //
//  ver 1.0                                                        //
//     Language:      C++, Visual Studio 2015                      //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:      Manjunath Babu, MS in Computer Engineering   //
//                    (315) 450-1435, mababu@syr.edu               //
//     Source:        Dr.Jim Fawcett					   	       //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
A single child thread processes work items enqueued by main thread

*/
//

#include <thread>
#include <functional>
#include "../ThreadPool/Cpp11-BlockingQueue.h"

using CallObj = std::function<void()>;

class ThreadPool
{
public:
	ThreadPool()
	{
		countThreads = 6; 
	}
	ThreadPool(int count)
	{
		countThreads = count;
	}
	void start();
	void doWork(CallObj* pWiVar);
	void wait();
	std::mutex mtx;
	~ThreadPool() {
		for (auto _pThread : pThreadCollection)
		{
			delete _pThread;
		}
	};
private:
	
	std::vector<std::thread*> pThreadCollection;	
	int countThreads;			
	BlockingQueue<CallObj*> itemQueue; 
};


#endif