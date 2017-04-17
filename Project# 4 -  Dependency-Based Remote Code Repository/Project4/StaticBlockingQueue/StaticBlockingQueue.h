#ifndef  STATIC_BLOCKING_QUEUE
#define STATIC_BLOCKING_QUEUE

///////////////////////////////////////////////////////////////////////////
//  StaticBlockingQueue.h - This supports enqueuing of requests			 //
//  ver 1.0																 //
//  Language:          C++, Visual Studio 2015					         //
// Application:        Dependency-Based Remote Code Repository,          //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
///////////////////////////////////////////////////////////////////////////

/*


*/
#include"../MockChannel/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"

template <typename T, int n>
class StaticBlockingQueue
{
public:
	BlockingQueue<T>& getQueue() {
		return queue;
	}
private:
	static BlockingQueue<T> queue;
};

template<typename T, int n>
BlockingQueue<T> StaticBlockingQueue<T,n>:: queue;
#endif 

