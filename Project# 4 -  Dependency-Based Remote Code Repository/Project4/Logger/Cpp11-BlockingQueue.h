#ifndef CPP11_BLOCKINGQUEUE_H
#define CPP11_BLOCKINGQUEUE_H
///////////////////////////////////////////////////////////////////////////
// Cpp11-BlockingQueue.h - Thread-safe Blocking Queue					 //
// ver 1.1																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,    	 //
///////////////////////////////////////////////////////////////////////////
/*
 
*
*/

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
class BlockingQueue {
public:
  BlockingQueue() {}
  BlockingQueue(const BlockingQueue<T>&);
  BlockingQueue<T>& operator=(const BlockingQueue<T>&);
  T deQ();
  void enQ(const T& t);
  size_t size();
private:
  std::queue<T> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

template<typename T>
BlockingQueue<T>::BlockingQueue(const BlockingQueue<T>& bq) : q_(bq.q_)
{

}

template<typename T>
BlockingQueue<T>& BlockingQueue<T>::operator=(const BlockingQueue<T>& bq)
{
  if (this == &bq) return *this;
  q_ = bq.q_;
  
  return *this;
}

template<typename T>
T BlockingQueue<T>::deQ()
{
  std::unique_lock<std::mutex> l(mtx_);
  if(q_.size() > 0)
  {
    T temp = q_.front();
    q_.pop();
    return temp;
  }
  

  while (q_.size() == 0)
    cv_.wait(l, [this] () { return q_.size() > 0; });
  T temp = q_.front();
  q_.pop();
  return temp;
}

template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
  {
    std::lock_guard<std::mutex> l(mtx_);
    q_.push(t);
  }
  cv_.notify_one();
}

template<typename T>
size_t BlockingQueue<T>::size()
{
  std::lock_guard<std::mutex> l(mtx_);
  return q_.size();
}

#endif
