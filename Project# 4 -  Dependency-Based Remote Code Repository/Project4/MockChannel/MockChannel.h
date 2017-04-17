#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
///////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015				 //
// - build as DLL to show how C++\CLI client can use native code channel //
// - MockChannel reads from sendQ and writes to recvQ					 //
//  Language:          C++, Visual Studio 2015							 //
// Application:        Dependency-Based Remote Code Repository,          //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author:    Jim Fawcett                                       //
///////////////////////////////////////////////////////////////////////////

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>

using Message = std::string;
struct ISenderClass
{
  virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start(std::string,std::string) = 0;
  virtual void stop() = 0;
  virtual void startClient() = 0;
};


extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISenderClass* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISenderClass* pISendr, IRecvr* pIRecvr);
  };
}

#endif


