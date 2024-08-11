/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../RulesAndActions/RulesAndAction.h"
#include "ConfigureParser.h"
#include "./../TypeTable/TypeTable.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  //delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pHandleLoop;
  delete pClassStruct;
  delete pSpecialKeys;
  delete pLambdaDetect;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{

  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;  

  pRepo->setCurrFileName(name);
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    pToker = new Toker;pToker->returnComments(false); pSemi = new SemiExp(pToker); pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);
    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pFunctionDefinition->addAction(pPushFunction);
	pFunctionDefinition->addAction(new PushFunctionTypeTable(pRepo));
    pParser->addRule(pFunctionDefinition);
    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
	pDeclaration->addAction(new AddTypeDefToType(pRepo));
    pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
	pParser->addRule(pExecutable);
	pHandleLoop = new HandleLoop;
	pHandleLoop->addAction(new HandleLoopAction(pRepo));pParser->addRule(pHandleLoop);
	pClassStruct = new HandleClassStruct;
	pClassStruct->addAction(new HandleClassStructAction(pRepo));	
	pClassStruct->addAction(new HandleClsStrucActTypeTable(pRepo));
	pParser->addRule(pClassStruct);	pSpecialKeys = new HandleSpecialKeywords;
	pSpecialKeys->addAction(new HandleSpecialKeywordsAction(pRepo));
	pParser->addRule(pSpecialKeys);pLambdaDetect = new LambdaDetect;
	pLambdaDetect->addAction(new LambdaDetectAction(pRepo));
	pParser->addRule(pLambdaDetect);
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}


Parser* ConfigParseToConsole::BuildForDependency(MainTypeTable<TypeTableValData>* pMainTable)
{
	try
	{
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);		
		pRepo->SetTypeTable(pMainTable);
		pRuleForDependencyCheck = new RuleForDependencyCheck();
		pRuleForDependencyCheck->addAction(new ActionForDependencyCheck(pRepo));
		pParser->addRule(pRuleForDependencyCheck);
		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pParser->addRule(pBeginningOfScope);
		pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);
		pFunctionDefinition = new FunctionDefinition;
		pPushFunction = new PushFunction(pRepo);  // no action
		pFunctionDefinition->addAction(pPushFunction);
		pFunctionDefinition->addAction(new PushFunctionTypeTable(pRepo));
		pParser->addRule(pFunctionDefinition);
		pDeclaration = new Declaration;pShowDeclaration = new ShowDeclaration;
		pDeclaration->addAction(pShowDeclaration);		
		pParser->addRule(pDeclaration);pExecutable = new Executable;
		pShowExecutable = new ShowExecutable;
		pExecutable->addAction(pShowExecutable);
		pParser->addRule(pExecutable);pHandleLoop = new HandleLoop;
		pHandleLoop->addAction(new HandleLoopAction(pRepo));pParser->addRule(pHandleLoop);
		pClassStruct = new HandleClassStruct;
		pClassStruct->addAction(new HandleClassStructAction(pRepo));
		pClassStruct->addAction(new HandleClsStrucActTypeTable(pRepo));
		pParser->addRule(pClassStruct);pSpecialKeys = new HandleSpecialKeywords;
		pSpecialKeys->addAction(new HandleSpecialKeywordsAction(pRepo));
		pParser->addRule(pSpecialKeys);pLambdaDetect = new LambdaDetect;
		pLambdaDetect->addAction(new LambdaDetectAction(pRepo));pParser->addRule(pLambdaDetect);
		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}

TreeNode * ConfigParseToConsole::DisplayAbstractSyntaxTree()
{
	return pRepo->DisplayAbstractSyntaxTree(pRepo->GetASTRoot());
}

MainTypeTable<TypeTableValData>* ConfigParseToConsole::GetTypeTable()
{
	return pRepo->GetTypeTable();
}

std::pair<std::string, std::vector<std::string>>* ConfigParseToConsole::getDepPair() {
	return pRepo->getCurrFileDependecy();
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>
int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;
  if(argc < 2)		  // collecting tokens from files, named on the command line
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }
  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');
    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }      
      while(pParser->next())	// now that parser is built, use it
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}
#endif
