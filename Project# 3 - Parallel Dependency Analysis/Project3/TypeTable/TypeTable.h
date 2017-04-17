#ifndef TYPETABLE_H
#define TYPETABLE_H
/////////////////////////////////////////////////////////////////////
// TypeTable.h - This is the type table which holds the key value  //
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
Defining Type table structure to hold the data 

*/
//
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Utilities/Utilities.h"


class TypeTableValData
{
public:

	using Namespace = std::string;
	using ClassName = std::string;
	using Type = std::string;
	using FileName = std::string;
	using PairNSFile = std::pair<FileName, Namespace>;
	Type& type() { return _type; }
	FileName& fileName() { return _fileName; }
	std::vector<ClassName>& className() { return _classNames; }

	void addNameSpaceFunc(const Namespace& ns)
	{
		if (find(_namespaces.begin(), _namespaces.end(), ns) == _namespaces.end())
			_namespaces.push_back(ns);
	}
	std::vector<Namespace> getNameSpaces()
	{
		return _namespaces;
	}
	void addPair()
	{
		std::pair<std::string, std::vector<std::string>> x(_fileName, _namespaces);
		_pairsNsFiles.push_back(x);
		_namespaces.clear();
	}
	std::vector<std::pair<std::string, std::vector<std::string>>>& GetPairs()
	{
		return _pairsNsFiles;
	}
private:
	Type _type;
	FileName _fileName;
	std::vector<ClassName> _classNames;
	std::vector<Namespace> _namespaces;
	std::vector<std::pair<std::string, std::vector<std::string>>> _pairsNsFiles;
};


template<typename T>
class MainTypeTable
{
public:
	using TypeName = std::string;

	using Value = T;
	using iterator = typename std::unordered_map<TypeName, T>::iterator;
	
	void addRecordToTable(std::string key, T& record)
	{
		varRecordsData[key] = record;
	}
	
	T& operator[](std::string n)
	{
		if (n == "" || varRecordsData.find(n) == varRecordsData.end())
		{
		
		}
		return varRecordsData[n];
	}
	T operator[](std::string n) const
	{
		if (n == "" || varRecordsData.find(n) == varRecordsData.end())
		{

		}
		return varRecordsData[n];
	}

	iterator find(std::string key)
	{
		return varRecordsData.find(key);
	}
	iterator begin() { return varRecordsData.begin(); }
	iterator end() { return varRecordsData.end(); }
	std::unordered_map<TypeName, Value>& record() { return varRecordsData; } 
private:
	std::unordered_map<TypeName, Value> varRecordsData; 
};


template<typename TypeTableRecord>
void showRecord(TypeTableRecord& record)
{	
	std::cout << "Type : " << record.type() << "\n";
	
	bool isClassFound = false;
	for (auto rec : record.className())
	{
		if (rec != "")
		{
			std::cout << "Class Names: ";
			isClassFound = true;
			break;
		}
	}
	for (auto rec : record.className())
	{	if(rec !="")
		std::cout << rec << "; ";
	}	
	if(isClassFound == true)
	std::cout << "\n";
	std::vector<std::pair<std::string, std::vector<std::string>>> pairs = record.GetPairs();
	for (auto pr : pairs)
	{
		std::cout << "FileName: " << pr.first<<"\n";
		std::cout << "Namespaces: ";
		for (auto ns : pr.second)
		{
			if (ns != "" && ns != ";")
			std::cout << ns << "; ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}


template<typename TypeTableRecord>
void showTypeTable(MainTypeTable<TypeTableRecord>& table)
{

	if (table.begin() == table.end())
		std::cout << "No entries in the table" << std::endl;
	else {
		for (auto record : table)
		{
			std::cout << "Key : " << record.first << "\n";
			showRecord(record.second);
		}
	}
}
#endif