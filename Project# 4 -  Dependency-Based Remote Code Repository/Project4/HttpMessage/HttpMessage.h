#pragma once
///////////////////////////////////////////////////////////////////// /////
// HttpMessage.h - Build and parse Http style messages					 //
//  ver 1.0																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository,                 //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original Author: Jim Fawcett, CSE687 - Object Oriented Design,        //
///////////////////////////////////////////////////////////////////////////
//
/*
Module Operations:
==================
This module is used to build and parse Http style messages


*/

#include <vector>
#include <string>
#include <array>

class HttpMessageFunc
{
public:
  using byte = char;
  using Name = std::string;
  using Value = std::string;
  using Attribute = std::pair<Name, Value>;
  using Attributes = std::vector<Attribute>;
  using Terminator = std::string;
  using Body = std::vector<byte>;

  
  void addAttribute(const Attribute& attrib);
  Value findValue(const Name& name);
  size_t findAttribute(const Name& name);
  bool removeAttribute(const Name& name);
  Attributes& attributes();
  static std::string attribString(const Attribute& attrib);
  static Attribute attribute(const Name& name, const Value& value);
  static Attribute parseAttribute(const std::string& src);


  void setBody(byte buffer[], size_t Buflen);
  size_t getBody(byte buffer[], size_t& Buflen);
  void addBody(const Body& body);
  void addBody(const std::string& body);
  void addBody(size_t numBytes, byte* pBuffer);
  Body& body();
  size_t bodyLength();

  std::string headerString() const;
  std::string bodyString() const;
  std::string toString() const;
  std::string toIndentedHeaderString() const;
  std::string toIndentedBodyString() const;
  std::string toIndentedString() const;


  void clear();
  static void fillBuffer(byte buffer[], size_t BufSize, byte fill = '\0');
  
  HttpMessageFunc makeMessage(std::string comm, std::string sendAddr, std::string recAddr,int contentLength,std::string body);
private:

  Attributes attributes_;
  Terminator term_ = "\n";
  Body body_;
};