//////////////////////////////////////////////////////////////////////
// ThreadPool.cpp- A single child thread processes work items	    //
// enqueued by main thread									        //
//      ver 1.0                                                     //
//     Language:      C++, Visual Studio 2015                       //
//  Application:      Parallel Dependency Analysis			        //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:      Manjunath Babu, MS in Computer Engineering    //
//                    (315) 450-1435, mababu@syr.edu                //
//     Source:        Dr.Jim Fawcett						        //
//////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
A single child thread processes work items enqueued by main thread



*/
//
#include <string>
#include <iostream>
#include <conio.h>
#include "../ThreadPool/ThreadPool.h"
#include "../Utilities/Utilities.h"

//----< demonstrate ProcessWorkItem class >----------------------------
void ThreadPool::start()
{
//	std::cout << "**** going to start threads";
	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			CallObj* pWi = itemQueue.deQ();
			CallObj* p2 = new CallObj;
			*p2 = *pWi;
			if (pWi == nullptr)
			{
				std::cout << "\n  shutting down the work item in processing";
				return;
			}
			(*p2)();
			delete p2;
		}
	};
	for (int i = 0; i < countThreads; i++)
	{
		pThreadCollection.push_back(new std::thread(threadProc));
	}
}
//wait on the threads
void ThreadPool::wait()
{
	for (auto _pThread : pThreadCollection)
	{
		_pThread->join();
	}
}
//do work on the threads 
void ThreadPool::doWork(CallObj* pWi)
{
	CallObj* p3 = new CallObj;
	*p3 = *pWi;
	itemQueue.enQ(p3);
	//_workItemQueue.enQ(pWi);
}
using Util = Utilities::StringHelper;
using WorkResult = std::string;

#ifdef TEST_THREADPOOL
int main()
{
	Util::Title("Enqueued Work Items");
	std::cout << "\n  main thread id = " << std::this_thread::get_id();
	ThreadPool processor(4);
	processor.start();
	{
		CallObj wi1 = []() {
			std::cout << "\n  wi1 working on thread " << std::this_thread::get_id();
			std::this_thread::sleep_for(std::chrono::milliseconds(6000));
			return "Hello from wi1";
		};
		processor.doWork(&wi1);
	}
	CallObj wi2 = []()
	{
		std::cout << "\n  wi2 working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		std::this_thread::sleep_for(std::chrono::milliseconds(6000));
		return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
	};
	processor.doWork(&wi2);

	CallObj wi3 = []()
	{
		std::cout << "\n  wi3 working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		std::this_thread::sleep_for(std::chrono::milliseconds(6000));
		return "wi3 result = " + Utilities::Converter<size_t>::toString(sum);
	};
	processor.doWork(&wi3);

	processor.wait();
	std::cout << "\n\n";
}
#endif // TEST_THREADPOOL