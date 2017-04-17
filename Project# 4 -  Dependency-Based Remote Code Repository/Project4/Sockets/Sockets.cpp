/////////////////////////////////////////////////////////////////////////
// Sockets.cpp - C++ wrapper for Win32 socket api                        //
// ver 5.1                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com //
//---------------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                      //
// All rights granted provided this copyright notice is retained       //
//---------------------------------------------------------------------//
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Provides four classes that wrap the Winsock API:
*  Socket:
*  - provides all the functionality necessary to handle server clients
*  - created by SocketListener after accepting a request
*  - usually passed to a client handling thread
*  SocketConnecter:
*  - adds the ability to connect to a server
*  SocketListener:
*  - adds the ability to listen for connections on a dedicated thread
*  - instances of this class are the only ones influenced by ipVer().
*    clients will use whatever protocol the server provides.
*  SocketSystem:
*  - Loads and unloads winsock2 library.
*  - Declared once at beginning of execution
*
*  Required Files:
*  ---------------
*  Sockets.h, Sockets.cpp,
*  Logger.h, Logger.cpp,
*  Utilities.h, Utililties.cpp,
*  WindowsHelpers.h, WindowsHelpers.cpp
*
*  Maintenance History:
*  --------------------
*  ver 5.1 : 10 Apr 16
*  - Added sendStream and recvStream to support sending and receiving
*    file streams.  These simply wrap the native sockets send and recv.
*  ver 5.0 : 04 Mar 16
*  - Fixed bugs in Socket test stub, essentially stealing fixes from
*    ClientTest.cpp and ServerTest.cpp
*  - Didn't change any code in the Socket library itself
*  ver 4.9 : 04 Mar 16
*  - Added a single write statement in Socket::Listener::accept()
*  ver 4.8 : 22 Feb 16
*  - Replaced verbose I/O with Logger I/O
*  - Replaced ApplicationHelpers package with Utilities package
*  ver 4.7 : 04 Apr 15
*  - removed testBlockHandling declaration from Socket.cpp ClientHandler.
*    The implementation had already been removed, I just forgot the declaration.
*  - added test for INVALID_SOCKET in Socket::recvString.  The omission was
*    reported by Huanming Fang.  Thanks Huanming.
*  ver 4.6 : 30 Mar 15
*  - minor modification to comments, above, and in Socket class implem.
*  ver 4.5 : 30 Mar 15
*  - moved SocketListener::start(...) from cpp to h file since it is a
*    template method.
*  - renamed ClientProc to ClientHandler
*  - removed Block operations to avoid binding Socket package to
*    FileSystem package.  Will add buffer operations to the
*    FileSystem::File class to match the Socket buffer operations.
*  - gave ClientHandler a command interpreter to select a test process
*    - test string tranfers
*    - test buffer transfers
*    - client sends a string to select test mode
*    - test modes are (string, buffer, and stop)
*  - Created a Verbose class in AppHelpers package that locks stream io.
*    That helps to keep server and client io text from intermingling.
*    You can turn verbose mode off which silences output that isn't
*    marked "always".
*  - Fixed again the bug which prevented communicating with anything other
*    than the loopback by adding hints.ai_flags = AI_PASSIVE to
*    SocketListener member data.
*  - added more testing
*  ver 4.4 : 27 Mar 15
*  - minor changes to comments
*  - moved ClientHandler into test stub
*  ver 4.3 : 26 Mar 15
*  - fixed bug noticed by Tarun Rajput
*  - used '0' as terminator.  Should have been '\0'
*  ver 4.2 : 26 Mar 15
*  - several small changes to the Socket class interface
*  ver 4.1 : 25 Mar 15
*  - fixed connection bug that prevented connecting to anything
*    other than a loopback (localhost, 127.0.0.1, ::1) by
*    adding winsock code to SocketConnecter().
*  - removed low-level code from ClientProc
*    (server's client handler callable object)
*    replaced with code written to Socket interface
*  Ver 4.0 : 24 Mar 15
*  - first release of total redesign - had a known bug (see ver 4.1)
*/
/*
* ToDo:
* - make SocketSystem a reference counted instance of Socket
* - write buffered recv which efficiently returns string or line
*   - reads and concatenates everything available into circular buffer
*   - parses out first string or line and moves start of buffer pointer
*     to begining of next
* -----------------------------------------------------------------------
*  Wait for The next items until Students have submitted their code
* -----------------------------------------------------------------------
* - build front end, e.g., Sender and Receiver classes
* - implement message facility: message class, sendMsg and recvMsg
* - Test and Display packages
*/

#include "Sockets.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <functional>
#include <exception>
#include "../Utilities/Utilities.h"

using Util = Utilities::StringHelper;
template<typename T>
using Conv = Utilities::Converter<T>;
using Show = StaticLogger<1>;

/////////////////////////////////////////////////////////////////////////////
// SocketSystem class members

//----< constructor starts up sockets by loading winsock lib >---------------

SocketSystem::SocketSystem()
{
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    Show::write("\n  WSAStartup failed with error = " + Conv<int>::toString(iResult));
  }
}
//-----< destructor frees winsock lib >--------------------------------------

SocketSystem::~SocketSystem()
{
  int error = WSACleanup();
  Show::write("\n  Socket System cleaning up");
}

/////////////////////////////////////////////////////////////////////////////
// Socket class members

//----< constructor sets TCP protocol and Stream mode >----------------------

Socket::Socket(IpVer ipver) : ipver_(ipver)
{
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
}
//----< promotes Win32 socket to Socket >------------------------------------
/*
*  You have to set ip version if you want IP6 after promotion, e.g.:
*     s.ipVer() = IP6;
*/
Socket::Socket(::SOCKET sock) : socket_(sock)
{
  ipver_ = IP4;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
}
//----< transfer socket ownership with move constructor >--------------------

Socket::Socket(Socket&& s)
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
}
//----< transfer socket ownership with move assignment >---------------------

Socket& Socket::operator=(Socket&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  return *this;
}
//----< get, set IP version >------------------------------------------------
/*
*  Note: 
*    Only instances of SocketListener are influenced by ipVer().
*    Clients will use whatever protocol the server supports.
*/
Socket::IpVer& Socket::ipVer()
{
  return ipver_;
}
//----< close connection >---------------------------------------------------

void Socket::close()
{
  if (socket_ != INVALID_SOCKET)
    ::closesocket(socket_);
}
//----< tells receiver there will be no more sends from this socket >--------

bool Socket::shutDownSend()
{
  ::shutdown(socket_, SD_SEND);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;
}

//----< tells receiver this socket won't call receive anymore >--------------

bool Socket::shutDownRecv()
{
  ::shutdown(socket_, SD_RECEIVE);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;
}
//----< tells receiver there will be no more sends or recvs >----------------

bool Socket::shutDown()
{
  ::shutdown(socket_, SD_BOTH);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;

}
//----< destructor closes socket handle >------------------------------------

Socket::~Socket() {
  shutDown();
  close();
}
//----< send buffer >--------------------------------------------------------
/*
*  - bytes must be less than or equal to the size of buffer
*  - doesn't return until requested number of bytes have been sent
*/
bool Socket::send(size_t bytes, byte* buffer)
{
  size_t bytesSent = 0, bytesLeft = bytes;
  byte* pBuf = buffer;
  while (bytesLeft > 0)
  {
      bytesSent = ::send(socket_, pBuf, (int)bytesLeft, 0);	  
    if (socket_ == INVALID_SOCKET || bytesSent == 0)
      return false;
    bytesLeft -= bytesSent;
    pBuf += bytesSent;
  }
  return true;
}
//----< recv buffer >--------------------------------------------------------
/*
*  - bytes must be less than or equal to the size of buffer
*  - doesn't return until buffer has been filled with requested bytes
*/
bool Socket::recv(size_t bytes, byte* buffer)
{
  size_t bytesRecvd = 0, bytesLeft = bytes;
  byte* pBuf = buffer;
  while (bytesLeft > 0)
  {
    bytesRecvd = ::recv(socket_, pBuf, (int)bytesLeft, 0);
    if (socket_ == INVALID_SOCKET || bytesRecvd == 0)
      return false;
    bytesLeft -= bytesRecvd;
    pBuf += bytesRecvd;
  }
  return true;
}
//----< sends a terminator terminated string >-------------------------------
/*
 *  Doesn't return until entire string has been sent
 *  By default terminator is '\0'
 */
bool Socket::sendString(const std::string& str, byte terminator)
{
  size_t bytesSent, bytesRemaining = str.size();
  const byte* pBuf = &(*str.begin());
  while (bytesRemaining > 0)
  {
    bytesSent = ::send(socket_, pBuf, (int)bytesRemaining, 0);
    if (bytesSent == INVALID_SOCKET || bytesSent == 0)
      return false;
    bytesRemaining -= bytesSent;
    pBuf += bytesSent;
  }
  ::send(socket_, &terminator, 1, 0);
  return true;
}
//----< receives terminator terminated string >------------------------------
/*
 * Doesn't return until a terminator byte as been received.
 *
 * ToDo:
 * - needs reads of one byte to be replaced by bulk reads into a
 *   stream buffer to improve efficiency.
 * - That will require building a circular buffer.
 * - performance seems acceptable, so won't do this now
 */
std::string Socket::recvString(byte terminator)
{
  static const int buflen = 1;
  char buffer[1];
  std::string str;
  while (true)
  {
    iResult = ::recv(socket_, buffer, buflen, 0);
    if (iResult == 0 || iResult == INVALID_SOCKET)
      break;
    if (buffer[0] == terminator)
      break;
    str += buffer[0];
  }
  return str;
}
//----< attempt to send specified number of bytes, but may not send all >----
/*
 * returns number of bytes actually sent
 */
size_t Socket::sendStream(size_t bytes, byte* pBuf)
{
  return  ::send(socket_, pBuf, (int)bytes, 0);
}
//----< attempt to recv specified number of bytes, but may not send all >----
/*
* returns number of bytes actually received
*/
size_t Socket::recvStream(size_t bytes, byte* pBuf)
{
  return ::recv(socket_, pBuf, (int)bytes, 0);
}
//----< returns bytes available in recv buffer >-----------------------------

size_t Socket::bytesWaiting()
{
  unsigned long int ret;
  ::ioctlsocket(socket_, FIONREAD, &ret);
  return (size_t)ret;
}
//----< waits for server data, checking every timeToCheck millisec >---------

bool Socket::waitForData(size_t timeToWait, size_t timeToCheck)
{
  size_t MaxCount = timeToWait / timeToCheck;
  static size_t count = 0;
  while (bytesWaiting() == 0)
  {
    if (++count < MaxCount)
      ::Sleep((DWORD)timeToCheck);
    else
      return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
// SocketConnector class members

//----< constructor inherits its base Socket's Win32 socket_ member >--------

SocketConnecter::SocketConnecter() : Socket()
{
  hints.ai_family = AF_UNSPEC;
}
//----< move constructor transfers ownership of Win32 socket_ member >-------

SocketConnecter::SocketConnecter(SocketConnecter&& s) : Socket()
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
}
//----< move assignment transfers ownership of Win32 socket_ member >--------

SocketConnecter& SocketConnecter::operator=(SocketConnecter&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  return *this;
}
//----< destructor announces destruction if Verbose(true) >------------------

SocketConnecter::~SocketConnecter()
{
  Show::write("\n  SocketConnecter instance destroyed");
}
//----< request to connect to ip and port >----------------------------------

bool SocketConnecter::connect(const std::string& ip, size_t port)
{
  size_t uport = htons((u_short)port);
  std::string sPort = Conv<size_t>::toString(uport);  
  const char* pTemp = ip.c_str();
  iResult = getaddrinfo(pTemp, sPort.c_str(), &hints, &result);  // was DEFAULT_PORT
  if (iResult != 0) {
    Show::write("\n\n  -- getaddrinfo failed with error: " + Conv<int>::toString(iResult));
    return false;
  }  
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    char *ipver;
    if (ptr->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    inet_ntop(ptr->ai_family, addr, ipstr, sizeof ipstr);
    printf("\n  %s: %s", ipver, ipstr);    
    socket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (socket_ == INVALID_SOCKET) {
      int error = WSAGetLastError();
      Show::write("\n\n  -- socket failed with error: " + Conv<int>::toString(error));
      return false;
    }
    iResult = ::connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      socket_ = INVALID_SOCKET;
      int error = WSAGetLastError();
      Show::write("\n  WSAGetLastError returned " + Conv<int>::toString(error));
      continue;
    }
    break;
  }
  freeaddrinfo(result);
  if (socket_ == INVALID_SOCKET) {
    int error = WSAGetLastError();
    Show::write("\n\n  -- unable to connect to server, error = " + Conv<int>::toString(error));
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
// SocketListener class members

//----< constructs SocketListener, specifying type of protocol to use >------

SocketListener::SocketListener(size_t port, IpVer ipv) : Socket(ipv), port_(port)
{
  socket_ = INVALID_SOCKET;
  ZeroMemory(&hints, sizeof(hints));
  if (ipv == Socket::IP6)
    hints.ai_family = AF_INET6;       // use this if you want an IP6 address
  else
    hints.ai_family = AF_INET;        // this gives IP4 address
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
}
//----< move constructor transfers ownership of Win32 socket_ member >-------

SocketListener::SocketListener(SocketListener&& s) : Socket()
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  hints.ai_flags = s.hints.ai_flags;
}
//----< move assignment transfers ownership of Win32 socket_ member >--------

SocketListener& SocketListener::operator=(SocketListener&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  hints.ai_flags = s.hints.ai_flags;
  return *this;
}
//----< destructor announces destruction if Verbal(true) >-------------------

SocketListener::~SocketListener()
{
  Show::write("\n  SocketListener instance destroyed");
}
//----< binds SocketListener to a network adddress on local machine >--------

bool SocketListener::bind()
{
  // Resolve the server address and port

  size_t uport = ::htons((u_short)port_);
  std::string sPort = Conv<size_t>::toString(uport);
  iResult = getaddrinfo(NULL, sPort.c_str(), &hints, &result);
  if (iResult != 0) {
    Show::write("\n\n  -- getaddrinfo failed with error: " + Conv<int>::toString(iResult));
    return false;
  }

  // Iterate through all results and bind to first available

  for (auto pResult = result; pResult != NULL; pResult = pResult->ai_next)
  {
    // Create a SOCKET for connecting to server
   
    socket_ = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);
    if (socket_ == INVALID_SOCKET) {
      int error = WSAGetLastError();
      Show::write("\n\n  -- socket failed with error: " + Conv<int>::toString(error));
      continue;
    }
    Show::write("\n  server created ListenSocket");

    // Setup the TCP listening socket

    iResult = ::bind(socket_, pResult->ai_addr, (int)pResult->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      int error = WSAGetLastError();
      Show::write("\n\n  -- bind failed with error: " + Conv<int>::toString(error));
      socket_ = INVALID_SOCKET;
      continue;
    }
    else
    {
      break;
    }
  }
  freeaddrinfo(result);
  return true;
}
//----< put SocketListener in listen mode, doesn't block >-------------------

bool SocketListener::listen()
{
  iResult = ::listen(socket_, SOMAXCONN);
  if (iResult == SOCKET_ERROR) {
    int error = WSAGetLastError();
    Show::write("\n\n  -- listen failed with error: " + Conv<int>::toString(error));
    socket_ = INVALID_SOCKET;
    return false;
  }
  Show::write("\n  server TCP listening socket setup complete");
  return true;
}
//----< accepts incoming requrests to connect - blocking call >--------------

Socket SocketListener::accept()
{
  ::SOCKET sock = ::accept(socket_, NULL, NULL);
  Socket clientSocket = sock;    // uses Socket(::SOCKET) promotion ctor
  if (!clientSocket.validState()) {
    acceptFailed_ = true;
    int error = WSAGetLastError();
    Show::write("\n\n  -- server accept failed with error: " + Conv<int>::toString(error));
    Show::write(
      "\n  -- this occurs when application shuts down while listener thread is blocked on Accept call"
    );
    return clientSocket;
  }
  return clientSocket;
}
//----< request SocketListener to stop accepting connections >---------------

void SocketListener::stop()
{
  stop_.exchange(true);
  sendString("Stop!");
}

#ifdef TEST_SOCKETS

//----< test stub >----------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// Server's client handler class
// - must be callable object so we've built as a functor
// - passed to SocketListener::start(CallObject& co)
// - Client handling thread starts by calling operator()

class ClientHandler
{
public:
  void operator()(Socket& socket_);
  bool testStringHandling(Socket& socket_);
  bool testBufferHandling(Socket& socket_);
};

//----< Client Handler thread starts running this function >-----------------

void clearBuffer(Socket::byte* buffer, size_t BufLen)
{
  for (size_t i = 0; i < BufLen; ++i)
    buffer[i] = '\0';
}

void ClientHandler::operator()(Socket& socket_)
{

  // we get here if command isn't requesting a test, e.g., "TEST_STOP"

  Show::write("\n");
  Show::write("\n  ClientHandler socket connection closing");
  socket_.shutDown();
  socket_.close();
  Show::write("\n  ClientHandler thread terminating");
}

//----< test string handling >-----------------------------------------------
/*
*   Creates strings, sends to server, then reads strings server echos back.
*/
bool ClientHandler::testStringHandling(Socket& socket_)
{
  Show::title("String handling test on server");

  while (true)
  {
    std::string str = socket_.recvString();
    if (socket_ == INVALID_SOCKET)
      return false;
    if (str.size() > 0)
    {
      //Show::write("\n  bytes recvd at server: " + toString(str.size() + 1));
      Show::write("\n  server rcvd : " + str);

      if (socket_.sendString(str))
      {
        Show::write("\n  server sent : " + str);
      }
      else
      {
        return false;
      }
      if (str == "TEST_END")
        break;
    }
    else
    {
      break;
    }
  }
  //socket_.sendString("TEST_STRING_HANDLING_END");
  Show::write("\n  End of string handling test");
  return true;
}

//----< test buffer handling >-----------------------------------------------
/*
*   Creates buffers, sends to server, then reads buffers server echos back.
*/
bool ClientHandler::testBufferHandling(Socket& socket_)
{
  Show::title("Buffer handling test on server");
  const size_t BufLen = 20;
  Socket::byte buffer[BufLen];
  bool ok;

  while (true)
  {
    ok = socket_.recv(BufLen, buffer);
    if (socket_ == INVALID_SOCKET)
      return false;
    if (ok)
    {
      std::string temp;
      for (size_t i = 0; i < BufLen; ++i)
        temp += buffer[i];
      //Show::write("\n  bytes recvd at server: " + toString(BufLen));
      Show::write("\n  server rcvd : " + temp);

      if (socket_.send(BufLen, buffer))
      {
        Show::write("\n  server sent : " + std::string(buffer));
      }
      else
      {
        Show::write("\n  server send failed");
        return false;
      }
      if (temp.find("TEST_END") != std::string::npos)
        break;
    }
    else
    {
      break;
    }
  }
  Show::write("\n  End of buffer handling test");
  return true;
}

//----< test string handling - server echos back client sent string >--------

void clientTestStringHandling(Socket& si)
{
  std::string command = "TEST_STRING_HANDLING";
  si.sendString(command);
  Show::write("\n  client sent : " + command);

  for (size_t i = 0; i < 5; ++i)
  {
    std::string text = "Hello World " + std::string("#") + Conv<size_t>::toString(i + 1);
    si.sendString(text);
    Show::write("\n  client sent : " + text);
  }
  command = "TEST_END";
  si.sendString(command);
  Show::write("\n  client sent : " + command);

  while (true)
  {
    std::string str = si.recvString();
    if (str.size() == 0)
    {
      Show::write("\n  client detected closed connection");
      break;
    }
    Show::write("\n  client recvd: " + str);
    if (str == "TEST_END")
    {
      Show::write("\n  End of string handling test");
      break;
    }
  }
}
//----< test buffer handling - server echos back client sent buffer >--------

void clientTestBufferHandling(Socket& si)
{
}
//----< demonstration >------------------------------------------------------

int main(int argc, char* argv[])
{
  Show::attach(&std::cout);
  Show::start();
  Show::title("Testing Sockets", '=');

  try
  {
    SocketSystem ss;
    SocketConnecter si;
    SocketListener sl(9070, Socket::IP6);
    ClientHandler cp;
    sl.start(cp);
    while (!si.connect("localhost", 9070))
    {
      Show::write("\n  client waiting to connect");
      ::Sleep(100);
    }

    Show::title("Starting string test on client");
    clientTestStringHandling(si);
    Show::title("Starting buffer test on client");
    clientTestBufferHandling(si);

    si.sendString("TEST_STOP");

    Show::write("\n\n  client calling send shutdown\n");
    si.shutDownSend();
    sl.stop();
  }
  catch (std::exception& ex)
  {
    std::cout << "\n  Exception caught:";
    std::cout << "\n  " << ex.what() << "\n\n";
  }
}

#endif
