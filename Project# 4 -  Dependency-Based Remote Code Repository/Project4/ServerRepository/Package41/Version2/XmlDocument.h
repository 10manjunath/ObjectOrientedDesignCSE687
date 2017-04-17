#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 1.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Public Interface:
* =================
*XmlDocument()								constructs a dockelement and assigns it to pDocelement
*
*parse(std::string, bool type);				To parses the input file or string
*
*BuildTree(std::vector<std::string>&);		Takes Pareser output and builts the XML tree
*
*display();									Returns the Tree structure into an string
*
*elements(const std::string& tag);			Searches for the element that matches tag
*
*descendents(const std::string& tag = "");	Operates on found_ which is processed by elements function, and finds the descendents
*
*select();									Returns the contents of found_ vector
*
*attrsearch(const std::string& attribute);	Populates the found_ with address of the search match nodes of attribute search returns the first found
*
*addChildDoc(std::string, std::string);		Adds a child to the given parent
*
*removeChildDoc(std::string, std::string);	Removes a child from the given parent
*
*addRoot(std::string);						Adds a root to the given DocElement if it doesn't have
*
*getAttributes(std::string);				Returns all atribute value vector pair if the tag found
*
*allChildren();								Returns all children pointer vector if the tag found
*
*addAttribute(const std::string& , const std::string& , const std::string& );	Adds an attribute, value pair if the tag found
*
*removeAttribute(const std::string , std::string );								Removes an attribute, value pair if the tag found that matches the attribute name
*
*writeToFile(std::string &);				Write the built XML into a file
*
*std::vector<std::string> Values(std::vector<sPtr>);	Returns tags for shared pointers
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*  devenv CppCli-WPF-App_original.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.3 : 21 Mar 15
* - modified according to project 2 requirements
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*/

#include <memory>
#include <string>
#include <stack>
#include "../XmlElement/XmlElement.h"

namespace XmlProcessing
{
	/////////////////////////////////////////////////////////////////////////////
	// XmlDocument - a container of XmlElement nodes

	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement > ;

		XmlDocument();
		XmlDocument(XmlDocument&&);
		XmlDocument& operator=(XmlDocument &&);
		std::vector<std::string> parse(std::string, bool);				//To parses the input file or string
		void BuildTree(std::vector<std::string>&);						//Takes Pareser output and builts the XML tree	
		std::string display();											//Returns the Tree structure into an string
		XmlDocument& elements(const std::string&);						//Searches for the element that matches tag          
		XmlDocument& descendents(const std::string& tag = "");			//Operates on found_ which is processed by elements function, and finds the descendents  
		std::vector<sPtr> select();										//Returns the contents of found_ vector                           
		sPtr attrsearch(const std::string&);							//Populates the found_ with address of the search match nodes of attribute search returns the first found
		bool addChildDoc(std::string, std::string);						//Adds a child to the given parent
		bool removeChildDoc(std::string, std::string);					//Removes a child from the given parent
		bool addRoot(std::string);										//Adds a root to the given DocElement if it doesn't have
		std::vector<std::pair<std::string, std::string>> getAttributes(std::string);//Returns all atribute value vector pair if the tag found 
		XmlDocument& allChildren();										//Returns all children pointer vector if the tag found
		bool addAttribute(const std::string&, const std::string&, const std::string&);//Adds an attribute, value pair if the tag found
		bool removeAttribute(std::string, std::string);				//Removes an attribute, value pair if the tag found that matches the attribute name
		bool writeToFile(std::string &);												// Write the built XML into a file
		std::vector<std::string> Values(std::vector<sPtr>);			//returns tags for shared pointers
	private:
		void BuildElement(std::string&);							//Takes the line parser output and builts a element
		void addattribute(sPtr, std::string&);						//Finds the attribute value pair and adds that to corresponding element
		void find_tag(sPtr, const std::string&);					//helps descendents() for populating found_ with descendents
		void find_desc(sPtr);										//helper for the descendents
		void find_attr(sPtr, const std::string&);					//Helps attrsearch() to populate the found_ 
		sPtr makeChild(std::vector<std::string>);					//Helper function for addChildDoc() to make an element upon parsing the string
		void XmlDocument::stackflow(sPtr element, bool flag);		//to check stack underflow condition

		sPtr pDocElement_;											// AST that holds procInstr, comments, XML root, and more comments
		std::vector<std::string> parsed;							//stores the parsed value
		std::vector<sPtr> found_;									// query results
		std::stack<sPtr> stack1;									//stack to build the XML tree that stores the parent
	};
}
#endif
