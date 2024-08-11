///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "XmlDocument.h"
#include "../XML Paresr/XmlElementParts/xmlElementParts.h"
#include "../XML Paresr/XmlElementParts/itokcollection.h"
#include "../XML Paresr/XmlElementParts/Tokenizer.h"


using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement > ;

//----< constructor   >--------------------------------------------------------
XmlProcessing::XmlDocument::XmlDocument() 
{
	pDocElement_ = makeDocElement();
}

//----< Move constructor operator >--------------------------------------------------------
XmlProcessing::XmlDocument::XmlDocument(XmlDocument&& xmldoc) :pDocElement_(xmldoc.pDocElement_), parsed(xmldoc.parsed), found_(xmldoc.found_), stack1(xmldoc.stack1)
{
	xmldoc.pDocElement_.reset();
	xmldoc.parsed.clear();
	xmldoc.found_.clear();
	while (xmldoc.stack1.size())xmldoc.stack1.pop();// Display::display_text("Move Constructor invoked");
}

//----< Move assignment operator >--------------------------------------------------------
XmlDocument& XmlProcessing::XmlDocument::operator=(XmlDocument && xmldoc)
{
	pDocElement_ = xmldoc.pDocElement_;
	parsed = xmldoc.parsed;
	found_ = xmldoc.found_;
	stack1 = xmldoc.stack1;
	xmldoc.pDocElement_.reset();
	xmldoc.parsed.clear();
	xmldoc.found_.clear();
	while (xmldoc.stack1.size())xmldoc.stack1.pop();// Display::display_text("Move Assignment invoked");
	return *this;
}

//----< Parses the input file or the string based on the type provided >----------------------------------------------------------
std::vector<std::string> XmlDocument::parse(std::string input, bool type)
{
	std::vector<std::string> parsed2; if (!input.size())return parsed2;
	try
	{
		Toker toker(input, type);							//input canbe filename or the string 
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);// parts.verbose(); 
		while (parts.get())
			parsed2.push_back(parts.show().c_str());
		for (auto itr = parsed2.begin(); itr != parsed2.end(); itr++);
			//std::cout << *itr << "\n";
	}
	catch (std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}
	return(parsed2);
}

//----< Takes Pareser output and builts the XML tree >----------------------------------------------------------
void  XmlDocument::BuildTree(std::vector<std::string>& parsed){
	for (auto itr = parsed.begin(); itr != parsed.end(); itr++){			//processes the input line by line
		std::string line = *itr; if ((*itr).size() == 3 && itr == parsed.begin())
			return;
		BuildElement(line);
	}
}
//----< Takes the line parser output and builts a element >----------------------------------------------------------
void XmlDocument::BuildElement(std::string& line){
	std::istringstream iss(line); bool flag = false, flagPI = false;
	
	do{
		std::string sub, sub2;
		iss >> sub;
		if (sub != "<"&&flag == false){										 //Text Element
			size_t p = line.find_first_not_of(" ");
			line.erase(0, p);
			sPtr TextE = makeTextElement(line); stackflow(TextE, false);
			break;
		}
		else{
			if (flag == false){ flag = true; continue; }
			else{
				if (sub == "?" || flagPI == true){
					if (flagPI == false){ flagPI = true;	continue; }iss >> sub2;
					if (sub2 == "version" || sub2 == "VERSION"){										//Declaration Element
						sPtr DE = makeXmlDeclarElement(); addattribute(DE, line);
						pDocElement_->addChild(DE); pDocElement_->toString(); break;
					}														//Processing instruction    
					else { sub;  sPtr PI = makeProcInstrElement(sub); addattribute(PI, line); stackflow(PI, true); }break;
				}
				else if (sub == "!"){										//Comment Element
					int i = line.find("--", 8);
					std::string dummy = line.substr(8, i - 8);
					sPtr CE = makeCommentElement(line.substr(8, i - 8)); stackflow(CE, true); break;
				}
				else if (sub == "/"){ stack1.pop(); break; }					//End Tag 
				else{															//Tagged Element
					sPtr TE = makeTaggedElement(sub);//iss >> sub;
					addattribute(TE, line);
					if (stack1.empty()){ pDocElement_->addChild(TE); stack1.push(TE); break; }
					else{ stack1.top()->addChild(TE); stack1.push(TE); break; }
				}
			}
		}
	} while (iss);

}
//----< to Handle stack underflow condition>----------------------------------------------------------
void XmlDocument::stackflow(sPtr element, bool flag)
{
	if (!stack1.empty())
		stack1.top()->addChild(element);
	else
		if (flag == true)
			pDocElement_->addChild(element);
}

//----< Returns the Tree structure into an string >----------------------------------------------------------
std::string XmlDocument::display()
{
	return pDocElement_->toString();
}

//----< Finds the attribute value pair and adds that to corresponding element >----------------------------------------------------------
void XmlDocument::addattribute(sPtr pointer, std::string& line)
{
	std::vector<std::string>split; std::string sub, sub_prev, sub_next; std::vector<std::string>::iterator itr_back;
	std::istringstream iss(line); int i = 0, position_pre = 0, position_post = 0, found = 0;
	do{
		iss >> sub;
		split.push_back(sub);
	} while (iss);
	for (auto itr_split = split.begin(); itr_split != split.end(); itr_split++)
	{
		sub = *itr_split; itr_back = itr_split;
		if (sub == "=")														//find attribute
		{
			sub_prev = *(--itr_back); itr_back += 2;
			sub_next = *itr_back; i = sub_next.find("\""); position_pre = line.find("=", found + 1, 1); position_post = line.find("\"", position_pre + 3, 1);
			found = position_post;
			int size = sub_next.size();
			if (!i)															//validate value
			{
				sub_next = line.substr(position_pre + 3, (position_post - position_pre) - 3);
			}
			else { i = sub_next.find("\'", 1); sub_next = sub_next.substr(1, i - 1); }
			pointer->addAttrib(sub_prev, sub_next);
		}
	}
}

//----< Searches for the element that matches tag and pupulates found_ vector with the search match >----------------------------------------------------------
XmlDocument& XmlDocument::elements(const std::string& tag)
{
	found_.clear();
	sPtr root = NULL;
	std::vector<sPtr> children = pDocElement_->getChildern();		
	for (auto itr : children)
		find_tag(itr, tag);														//find_tag does a recursive tag search
	if (found_.size()){
		//Display::display_text("Found the given tag \""+tag+"\"");
	}
	else{}
		//Display::display_text("Couldn't find the given tag \"" + tag + "\"");
	return(*this);
}

//----< Traverses through the XML tree for the given tag recursively and populates found_ >----------------------------------------------------------
void XmlDocument::find_tag(sPtr toFind, const std::string& tag)
{
	if (toFind->value() == tag &&
		((dynamic_cast<TaggedElement*>(toFind.get()) || dynamic_cast<ProcInstrElement*>(toFind.get()) || dynamic_cast<XmlDeclarElement*>(toFind.get()))))
	{
		found_.push_back(toFind);
	}
	std::vector<sPtr> children = toFind->getChildern();
	for (auto c_itr : children)
		if (c_itr != NULL)
			find_tag(c_itr, tag);
}

//----< Operates on found_ which is processed by elements function, generally and replaces found_ with the descendents of found_[0] >----------------------------------------------------------
XmlDocument& XmlDocument::descendents(const std::string& tag)
{
	if (found_.size()){					//requires elements to populate the found_ vector
		sPtr first_found = found_[0];
		found_.clear();
		if (tag != "")
		{
			this->find_tag(first_found, tag);								//to find the elements for its decendents 
			if (found_.size()){ first_found = found_[0]; found_.clear(); find_desc(first_found); }
			else {}//Display::display_text("Given tag\"" + tag + "\" not found");
		}
		else
			find_desc(first_found);
	}
	return(*this);
}

//----< A private function which helps descendents() for populating found_ with descendents in a recursive manner>----------------------------------------------------------
void XmlDocument::find_desc(sPtr element)
{
	
	std::vector<sPtr> children = element->getChildern();
	for (auto c_itr : children)
		if (c_itr != NULL)
		{
			found_.push_back(c_itr);									//finding its descendents
			find_desc(c_itr);
		}
}

//----< Returns the contents of found_ with a standard move so that the contents are cleared>----------------------------------------------------------
std::vector<sPtr> XmlDocument::select()
{
	return(std::move(found_));											
}

//----< Populates the found_ with address of the search match nodes>----------------------------------------------------------
sPtr XmlDocument::attrsearch(const std::string& attribute)
{
	sPtr root = NULL;
	found_.clear();
	std::vector<sPtr> children = pDocElement_->getChildern();
	for (auto itr : children)
		/*if (dynamic_cast<TaggedElement*>((itr).get()))
		root = itr;*/
		find_attr(itr, attribute);									//find the attribute match recursively
		if (found_.size()){
			//Display::display_text("Found the attribute \"" + attribute+"\"");
	}
	else
	{
		//Display::display_text("Attribute \"" + attribute + "\" not found");
	}
	if (found_.size())
		return(found_[0]); else return NULL;
}

//----< Helps attrsearch() to populate the found_ with address of the search match nodes>----------------------------------------------------------
void XmlDocument::find_attr(sPtr toFind, const std::string& attribute)
{
	if (((dynamic_cast<TaggedElement*>(toFind.get()) || dynamic_cast<ProcInstrElement*>(toFind.get()) || dynamic_cast<XmlDeclarElement*>(toFind.get()))))
	{
		std::vector<std::pair<std::string, std::string>> attributes = toFind->getAttribute();
		for (auto a_itr : attributes)
			if (a_itr.first == attribute)							//matching attribute
				found_.push_back(toFind); return;
	}
	std::vector<sPtr> children = toFind->getChildern();
	for (auto c_itr : children)										//traversiong to children for attribute
		if (c_itr != NULL)
			find_attr(c_itr, attribute);
}

//----< Adds a child to the given parent if found and can hold children>----------------------------------------------------------
bool XmlDocument::addChildDoc(std::string  parent, std::string  child)
{
	this->elements(parent); sPtr child_constr = NULL;
	if (found_.size() && dynamic_cast<TaggedElement*>(found_[0].get()))	//check for parent found
	{
		std::vector<std::string> parsed2 = parse(child, false); if (parsed2.size() == 0)return false;
		child_constr = makeChild(parsed2);
		found_[0]->addChild(child_constr);
		return true;
	}
	else
		return false;
}

//----< Helper function for addChildDoc() to make an element from the given string>----------------------------------------------------------
sPtr XmlDocument::makeChild(std::vector<std::string> parsed)
{
	for (auto itr = parsed.begin(); itr != parsed.end(); itr++){
		bool flag = false, flagPI = false; std::string line = *itr;
		std::istringstream iss(line);
		do{
			std::string sub;                          //search text is filtered if the text is only one word
			iss >> sub;
			if (sub != "<"&&flag == false)				//make text
			{
				size_t p = line.find_first_not_of(" ");
				line.erase(0, p);
				sPtr TextE = makeTextElement(line);
				return TextE;
			}
			else
			{
				if (flag == false)
				{
					flag = true; continue;
				}
				else
				{
					if (sub == "!")				//make comment
					{

						int i = line.find("--", 8);
						sPtr CE = makeCommentElement(line.substr(8, i - 8));
						return CE;
					}
					else if (sub == "/")
					{
						stack1.pop(); break;
					}
					else
					{										//make tagged
						sPtr TE = makeTaggedElement(sub);
						addattribute(TE, line);
						return TE;
					}
				}
			}
		} while (iss);
	}return NULL;
}

//----< Removes a child from the given parent if found that can hold children and taf matches its value>--------------------------------------
bool XmlDocument::removeChildDoc(std::string parent, std::string removeTag)
{
	this->elements(parent);
	if (found_.size())
	{
		TaggedElement* parent_found = dynamic_cast<TaggedElement*>(found_[0].get());
		return(parent_found->removeChild(removeTag));

	}
	else
		return false;
}
//----< Adds a root to the given DocElement if it doesn't have and the root can only be a tagged element>---------------------------------------
bool XmlDocument::addRoot(std::string new_root)
{
	bool flag = false;
	for (auto itr_c : pDocElement_->getChildern())
	{
		if ((dynamic_cast<TaggedElement*>(itr_c.get())))
		{
			flag = true; //Display::display_text("Tree already has a root!");
		}
	}
	if (flag == false)
	{
		std::vector<std::string> parsed2 = parse(new_root, false);
		sPtr child_constr = makeChild(parsed2);
		if ((dynamic_cast<TaggedElement*>(child_constr.get()))){
			pDocElement_->addChild(child_constr); return true;
		}
		else{  }//Display::display_text("Input element should be a tagged element");
	}

	return false;
}

//----< Returns all atribute value vector pair if the tag found and has attributes else an empty vector>---------------------------------------
std::vector<std::pair<std::string, std::string>> XmlDocument::getAttributes(std::string tag)
{
	elements(tag); std::vector<std::pair<std::string, std::string>> attributes;
	if (found_.size())
	{
		attributes = found_[0]->getAttribute();
	}
	return attributes;
}

//----< Returns all children pointer vector if the tag found and has children needs elements() to operate on else an empty vector>---------------------------------------
XmlDocument& XmlDocument::allChildren()
{
	std::vector<std::string> values;
	if (found_.size())
	{
		sPtr parent_element = found_[0]; found_.clear();
		for (auto itr_child : parent_element->getChildern())
			found_.push_back(itr_child);
	}
	return(*this);
}

//----< Adds an attribute, value pair if the tag found returns a bool to know the status of operation upon its completion>--------------------------
bool XmlDocument::addAttribute(const std::string& parent, const std::string& attribute, const std::string& value)
{
	elements(parent);
	if (found_.size())
	{
		sPtr parent_element = found_[0];
		parent_element->addAttrib(attribute, value); return true;
	}
	return false;
}

//----< Removes an attribute, value pair if the tag found that matches the attribute name, returns a bool to know the status of operation upon its completion>--
bool XmlDocument::removeAttribute(std::string parent, std::string attribute)
{
	elements(parent);
	if (found_.size())
	{
		sPtr parent_element = found_[0];
		return parent_element->removeAttrib(attribute);
	}
	return false;
}

//----< Write the built XML into a file>------------------------------------------------------------------------------------------------------------
bool XmlDocument::writeToFile(std::string& writeFilename)
{
	std::ofstream myfile;
	myfile.open(writeFilename);
	myfile << pDocElement_->toString();
	myfile.close();
	return true;
}

std::vector<std::string> XmlDocument::Values(std::vector<sPtr> tags)
{
	std::vector<std::string> returntag;
	if (tags.size())
	{
		for (auto itr_tag : tags)
			returntag.push_back(itr_tag->value());
	}
	return returntag;
}



//----< test stub >----------------------------------------------------------
#ifdef TEST_XMLDOC

int main(int argc, char* argv[])
{
	XmlProcessing::XmlDocument xmldoc;
	xmldoc.BuildTree(xmldoc.parse(argv[1], true));
	std::cout << xmldoc.display();
	return 1;
}

#endif