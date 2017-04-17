#ifndef RULESANDACTION_H
#define RULESANDACTION_H
//////////////////////////////////////////////////////////////////////
//  RulesAndAction.h - declares new parsing rules and actions       //
//  ver 3.1                                                         //
//     Language:       C++ Visual Studio 2015                       //
//  Application:       Prototype for CSE687 Pr1, Sp09               //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp //
//       Author:       Manjunath Babu, MS in Computer Engineering   //
//                     (315) 450-1435, mababu@syr.edu               //
//       Source:       Jim Fawcett, CST 4-187, Syracuse University  //
//                     (315) 443-3948, jfawcett@twcny.rr.com        //
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================

Define rules and actions for various scopes

*/
//
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include<map>
#include "../Parser/Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include"../AbstractSyntaxTree/AbstractTreeOperations.h"
#include"../TypeTable/TypeTable.h"


class Repository  
{
	ScopeStack<TreeNode*> stack;
	Scanner::Toker* p_Toker;
	AbstractSyntaxTree* ast;	
	MainTypeTable<TypeTableValData>* table;
	std::string currFileName;
public:
	std::pair<std::string, std::vector<std::string>>* pTableDep;	
	std::vector<std::string> dependentFiles;
	std::vector<std::string> scopeNs;
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
		ast = new AbstractSyntaxTree();
		table = new MainTypeTable<TypeTableValData>();
		pTableDep = new std::pair<std::string, std::vector<std::string>>;
		scopeNs.push_back("Global Namespace");
	}
	std::pair<std::string, std::vector<std::string>>* getCurrFileDependecy() {
		pTableDep->first = getCurrFileName();
		pTableDep->second = dependentFiles;
		return pTableDep;
	}
	ScopeStack<TreeNode*>& scopeStack()
	{
		return stack;
	}
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	size_t Repository::startLineCount() 
	{
		return (size_t)(p_Toker->currentLineCount() - 1);
	}
	size_t Repository::endLineCount()
	{
		return (size_t)(p_Toker->currentLineCount() - 1);
	}	
	bool Repository::AddASTNode(TreeNode* elem)
	{
		ast->GetRootElem()->addChildElem(elem);
		return true;
	}
	bool Repository::AddTypeTableEntry(std::string key, TypeTableValData tval)
	{
		table->addRecordToTable(key, tval);
		return true;
	}	
	MainTypeTable<TypeTableValData>*  Repository::GetTypeTable()
	{
		return table;
	}
	void  Repository::SetTypeTable(MainTypeTable<TypeTableValData>* pMainTable) {
		table = pMainTable;
	}
	std::string Repository::getCurrFileName()
	{
		return currFileName;
	}
	void Repository::setCurrFileName(std::string file)
	{
		currFileName = file;
	}
	TreeNode* Repository::GetASTRoot()
	{
		return ast->GetRootElem();
	}
	TreeNode* Repository::DisplayAbstractSyntaxTree(TreeNode* root)
	{
		return ast->DisplayAbstractSyntaxTree(root);
	}
};


class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		
		pTc->trimFront();
		if (pTc->find("{") == 0)
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};


// handle scope stack at end of scope
class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{

		TreeNode* elem = new TreeNode("unknown", "anonymous", p_Repos->startLineCount(), 0);
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};


// detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

// handle scope stack at end of scope
class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->scopeStack().size() == 0)
			return;
		TreeNode* elem = new TreeNode();
		elem = p_Repos->scopeStack().pop();
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->setComplexityLevel(p_Repos->scopeStack().top()->getComplexityLevel() + elem->getComplexityLevel());
		elem->setEndLineNumber(p_Repos->endLineCount());
		if (elem->gettype() == "namespace") {
			p_Repos->scopeNs.erase(p_Repos->scopeNs.end()-1);
		}
	}
};


// detect preprocessor statements
class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};


// print preprocessor statement to console
class PrintPreprocRule : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
	//	std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};


// detect function loops such as for, while, if, switch,catch, else
class HandleLoop : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if","catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

// action to handle loops 
class HandleLoopAction : public IAction
{
	Repository* p_Repos;
public:
	HandleLoopAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		
		std::string name = (*pTc)[pTc->find("(") - 1];
		TreeNode* elem = new TreeNode(name, name, p_Repos->startLineCount(), 0);

		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		
		p_Repos->scopeStack().push(elem);
	}
};


// detect class declaration
class HandleClassStruct : public IRule
{
public:
	bool isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return true;
		}
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if ((tc[tc.length() - 1] == "{") && isKeyWord(tc))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

// handle loops 
class HandleClassStructAction : public IAction
{
	Repository* p_Repos;
public:
	HandleClassStructAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return keys[i];
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		TreeNode* elem = new TreeNode();
		elem->setTypeToElem(isKeyWord(tc));
		if (elem->gettype() == "class")
			elem->setNameToElem((*pTc)[pTc->find("class") + 1]);
		else if(elem->gettype() == "struct")
			elem->setNameToElem((*pTc)[pTc->find("struct") + 1]);
		else
		{
			elem->setNameToElem((*pTc)[pTc->find("enum") + 1]);
		}
		elem->setStartLineNumber(p_Repos->startLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

// handle loops 
class HandleClsStrucActTypeTable : public IAction
{
	Repository* p_Repos;
public:
	HandleClsStrucActTypeTable(Repository* pRepos)
	{
		p_Repos = pRepos;			
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return keys[i];
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (isKeyWord(tc) == "class")
		{			
			TypeTableValData tval;
			std::string key = (*pTc)[pTc->find("class") + 1];
			if ((*p_Repos->GetTypeTable()).find(key) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
		   
			tval.type() = "class";
			tval.fileName() = p_Repos->getCurrFileName();	
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}			
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
		else if (isKeyWord(tc) == "struct")
		{
			TypeTableValData tval;std::string key = (*pTc)[pTc->find("struct") + 1];
			if ((*p_Repos->GetTypeTable()).find(key) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
			
			tval.type() = "struct";tval.fileName() = p_Repos->getCurrFileName();
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}			
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
		else if (isKeyWord(tc) == "enum")
		{
			TypeTableValData tval;std::string key = (*pTc)[pTc->find("enum") + 1];
			if ((*p_Repos->GetTypeTable()).find(key) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
			tval.className().push_back("");tval.type() = "enum";tval.fileName() = p_Repos->getCurrFileName();
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
	}
};


// detect else , try , do while
class HandleSpecialKeywords : public IRule
{
public:
	bool isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"else", "try","do"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
				{
					if (tok[j + 1] != "if")
						return true;
				}
		}
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if ((tc[tc.length() - 1] == "{") && isKeyWord(tc))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

// handle special keywords like do while, else, try 
class HandleSpecialKeywordsAction : public IAction
{
	Repository* p_Repos;
public:
	HandleSpecialKeywordsAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"else", "do","try"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
			{
				if (tok[j] == keys[i])
					return keys[i] == "do" ? "do while" : keys[i];
			}
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		TreeNode* elem = new TreeNode(isKeyWord(tc), isKeyWord(tc), p_Repos->startLineCount(), 0);
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};


// detect lambdas
class LambdaDetect : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		const size_t numKeys = 6;
		const static std::string keys[numKeys] = {
			"[", "]","(",")", "{","Operator"
		};
		if ((tc.find("main")) < pTc->length())
		{
			return true;
		}
		std::map<std::string, int> operMap;
		for (int j = 0; j < tc.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tc[j] == keys[i])
				{
					operMap[keys[i]] = j;
				}
		}
		int size_1 = operMap["]"] - operMap["["];
		int size_2 = operMap[")"] - operMap["("];
		int size_3 = operMap["{"];
		int size_4 = operMap["Operator"];
		if ((size_1 >= 1) && (size_2 >= 1) && (size_3 >= 1) && (size_4 < 1))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

// handle special keywords like do while, else, try 
class LambdaDetectAction : public IAction
{
	Repository* p_Repos;
public:
	LambdaDetectAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
				
		ITokCollection& tc = *pTc;
		TreeNode* elem = new TreeNode();
		std::string name = (*pTc)[pTc->find("=") - 1] = NULL ? "unnamed" : (*pTc)[pTc->find("=") - 1];
		elem->setTypeToElem("Lambda");
		elem->setNameToElem(name);
		elem->setStartLineNumber(p_Repos->startLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};


//  detect function definitions
class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;		
		if ((tc.find("Operator") >= pTc->length()) && (tc.find("[") < pTc->length()) && (tc.find("main") >= pTc->length()))
		{
			return true;
		}
		if ((tc.find("namespace") < pTc->length()) && (tc.find("using") >= pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		if ((tc.find("operator") < pTc->length()) && (tc.find("{") < pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};


//  push function name onto ScopeStack
class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		
		std::string name = (*pTc)[pTc->find("(") - 1];
		TreeNode *elem;
		if (pTc->find("namespace") < pTc->length())
		{	
			pTc->trim();
			elem = new TreeNode("namespace", (*pTc)[pTc->find("{") - 1], p_Repos->startLineCount(), p_Repos->endLineCount());
		}
		else if (pTc->find("operator") < pTc->length())
			elem = new TreeNode("operator", "operator"+name, p_Repos->startLineCount(), p_Repos->endLineCount());
		else
			elem = new TreeNode("function", name, p_Repos->startLineCount(), p_Repos->endLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChildElem(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};


class PushFunctionTypeTable : public IAction
{
	Repository* p_Repos;
public:
	PushFunctionTypeTable(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string name = (*pTc)[pTc->find("(") - 1];TreeNode *elem;
		if (pTc->find("namespace") < pTc->length())
		{
			pTc->trim();
			p_Repos->scopeNs.push_back((*pTc)[pTc->find("{") - 1]);					
		}
		else if (pTc->find("operator") < pTc->length())
		{
			
		}		
		else
		{
			elem = new TreeNode("function", name, p_Repos->startLineCount(), p_Repos->endLineCount());
			std::string type;std::string clsName;size_t func_name_index = pTc->find("(") - 1;
			if (func_name_index >= 1) {
				if ((*pTc)[func_name_index - 1] == "::") {
					type = "MemberFunction";
					clsName = (*pTc)[func_name_index - 2];
				}
				else {
					type = "GlobalFunction";clsName = "";
				}
			}
			else
			{
				type = "GlobalFunction";
				clsName = "";
			}
			if ((*p_Repos->GetTypeTable()).find(name) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
			if (name == "main")
				return;			
			TypeTableValData tval;
			std::string key = name;
			tval.className().push_back(clsName);
			tval.type() = type;
			tval.fileName() = p_Repos->getCurrFileName();
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
	}
};

class Declaration : public IRule          
{                                         
public:                                  
	bool isModifier(const std::string& tok) 
	{                                       
		const size_t numKeys = 22;            
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;		
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);		
	}
	void doTest1(Scanner::SemiExp &tc, Scanner::SemiExp &se)
	{
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (isModifier(tc[i]))
				continue;
			if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
				continue;
			if (tc[i] == "=" || tc[i] == ";")
				break;
			else
				se.push_back(tc[i]);
		}
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i < in.length(); ++i)
			tc.push_back(in[i]);
		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);			
			Scanner::SemiExp se;
			doTest1(tc, se);

			if (se.length() == 2)  
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};


// show declaration
class ShowDeclaration : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;		
		Scanner::SemiExp se; 
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);				
	}
};



//  show declaration
class AddTypeDefToType : public IAction
{
	Repository* p_Repos;
public:
	AddTypeDefToType(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;		
		Scanner::SemiExp se;
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		if (se.find("typedef") < se.length())
		{
			TypeTableValData tval;
			std::string key = se[se.find(";")-1];
			if ((*p_Repos->GetTypeTable()).find(key) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
			
			tval.type() = "typedef";
			tval.fileName() = p_Repos->getCurrFileName();
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}			
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
		else if (se.find("using") < se.length() && se.find("=") < se.length()) {
			TypeTableValData tval;
			std::string key = se[se.find("=") - 1];
			if ((*p_Repos->GetTypeTable()).find(key) != (*p_Repos->GetTypeTable()).end()) {
				return;
			}
			
			tval.type() = "using";
			tval.fileName() = p_Repos->getCurrFileName();
			for (auto a : p_Repos->scopeNs)
			{
				tval.addNameSpaceFunc(a);
			}
			tval.addPair();
			p_Repos->AddTypeTableEntry(key, tval);
		}
	}
};



// rule to detect executable
class Executable : public IRule           
{                                       
public:                                   
	bool isModifier(const std::string& tok) 
	{                                      
		const size_t numKeys = 22;            
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;		
	}

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);		
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);
		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{			
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);			
			Scanner::SemiExp se;	
			doTest2(tc, se);			
			if (se.length() != 2)  
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}

	void doTest2(Scanner::SemiExp &tc, Scanner::SemiExp &se)
	{
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (isModifier(tc[i]))
				continue;
			if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
				continue;
			if (tc[i] == "=" || tc[i] == ";")
				break;
			else
				se.push_back(tc[i]);
		}
	}
};


// show executable
class ShowExecutable : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		}
		
	}
};



// detect beginning of anonymous scope
class RuleForDependencyCheck : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{		
		pTc->trimFront();
			doActions(pTc);			
		return true;
	}
};

// handle loops 
class ActionForDependencyCheck : public IAction
{
	Repository* p_Repos;
public:
	ActionForDependencyCheck(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return keys[i];
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		for (int i = 0; i < tc.length(); i++) {
			if (p_Repos->GetTypeTable()->find(tc[i]) != p_Repos->GetTypeTable()->end()) {
				std::string depFile;
				TypeTableValData tval = (*(p_Repos->GetTypeTable()))[tc[i]];
				depFile = (tval.GetPairs())[0].first;
				bool shouldPush = true;
				
				for (auto file : p_Repos->dependentFiles) {
					if (file == depFile)
						shouldPush = false;
				}				
				if (depFile == p_Repos->getCurrFileName())	
					shouldPush = false;
				
				if (tval.type() == "MemberFunction" && p_Repos->scopeStack().size() > 0 && p_Repos->scopeStack().top()->gettype() != "class")
					shouldPush = false;
				if (shouldPush) {
					p_Repos->dependentFiles.push_back(depFile);
				}
			}
		}
	}
};

#endif
