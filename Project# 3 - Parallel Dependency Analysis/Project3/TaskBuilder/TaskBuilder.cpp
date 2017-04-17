//////////////////////////////////////////////////////////////////////
// Task.cpp    - This is the Task which holds the static threadpool //
//  ver 1.0                                                         //
//     Language:      C++, Visual Studio 2015                       //
//  Application:   Parallel Dependency Analysis				        //
//     Platform:      MacBook Pro, Core-i5, Windows 10 on Bootcamp  //
//       Author:      Manjunath Babu, MS in Computer Engineering    //
//                    (315) 450-1435, mababu@syr.edu                //
//////////////////////////////////////////////////////////////////////


/*
Module Operations:
==================
Demonstrate Task class and implement static thread pool.

*/
//

#include "TaskBuilder.h"
ThreadPool *TaskHandlerClass::threadColObj = new ThreadPool();


#ifdef TASK_TEST
int main()
{
	CallObj wi1 = []() {
		std::cout << "\n Threading Package TEST" << std::this_thread::get_id();
		std::this_thread::sleep_for(std::chrono::milliseconds(6060));
	};
	TaskHandlerClass thVar;
	thVar.Executor(wi1);
	thVar.threadColObj.wait();
}
#endif // TASK_TEST

