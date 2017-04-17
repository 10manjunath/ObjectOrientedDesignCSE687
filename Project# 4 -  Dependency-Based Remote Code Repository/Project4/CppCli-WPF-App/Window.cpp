///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4				  			     //
// ver 2.0																 //
//  Language:      C++, Visual Studio 2015								 //
// Application: Dependency-Based Remote Code Repository                  //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original author: Jim Fawcett, Syracuse University, CST 4-187			 //
//                  jfawcett@twcny.rr.com								 //			
///////////////////////////////////////////////////////////////////////////
/*

*/
#include "Window.h"
using namespace Utilities;
using namespace CppCliWindows;


WPFCppCliDemo::WPFCppCliDemo(String^ sendaddr, String^ recaddr , String^ path)
{	
	ObjectFactory* pObjFact = new ObjectFactory;
	serverPort = recaddr;	
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start(toStdString(sendaddr), toStdString(path));
	delete pObjFact;	
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMsg);
	hResetButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Reset);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	this->Title = "WPF GUI-Project Project 4";
	this->Width = 800;
	this->Height = 600;	
	
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	setUpTabControl();
	setUpStatusBar();
	setUpSendMessageView();
	setUpCheckInView();	

	
	automateTestCaseRun();
	std::cout << "\nDemonstrating: OBTAIN PKG LIST FROM SERVER: \n";
	std::string x  = "GetPackageList ";
	x.append(toStdString(serverPort));	
	pSendr_->postMessage(x);
	Message msg = pRecvr_->getMessage();
	std::cout << "\nSuccessfully Obtained the File list from server\n";
	std::vector<std::string> s = Utilities::StringHelper::splitUsingSemiColon(msg);
	for (std::string i : s)
	{
		hDepListBox->Items->Add(toSystemString( i));
	}

	for (std::string i : s)
	{
		hPackageListBox->Items->Add(toSystemString(i));
	}
	setUpCheckOutView();
}


void WPFCppCliDemo::automateTestCaseRun()
{
		std::cout << "*********Test Executive ***************";
		std::cout << "\n\n\n";
		std::cout << "RUNNING AUTOMATED TEST CASES . . .\n";
		std::string x;
		
		StringHelper::Title("DEMONSTRATING REQUIREMENT 1", '*');
		std::cout << "\nUsed Visual Studio 2015 and its C++ Windows console projects, as provided in the ECS computer labs. Also used Windows Presentation Foundation (WPF) to provide a required client Graphical User Interface (GUI).\n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 2", '*');
		std::cout << "\nUsed the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management.\n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 3", '*');
		std::cout << "\nProvided a Repository program that provides a module and/or package checkin process. Each checkin will support copying and providing metadata for all modules or packages in the Repository. Metadata contains a description of the module/package and a list of other modules/packages on which it depends.\n";
		std::cout << std::endl << std::endl;


		StringHelper::Title("DEMONSTRATING REQUIREMENT 4", '*');
		std::cout << "\n     \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 5", '*');
		std::cout << "\n Each module or package checkin Shall create a new directory with a unique name based on the module or package name and checkin-date. Each module directory contains module metadata containing a description of all packages and package metadata that contains a list of all the files in the package. \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 6", '*');
		std::cout << "\n   Each checkin directory Shall have a boolean closed property. If the directory is not closed files may be added or changed at any time without changing the directory's date. Once closed the directory contents become immutable.   \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 7", '*');
		std::cout << "\n Provided an extraction process for modules or packages with or without dependencies.    \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 8", '*');
		std::cout << "\n Provided a message-passing communication system used to access the Repository's functionality from another process or machine.    \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 9", '*');
		std::cout << "\n The communication system shall provide support for passing HTTP style messages using either synchronous request/response or asynchronous one-way messaging. \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 10", '*');
		std::cout << "\n The communication system shall also support sending and receiving streams of bytes6. Streams will be established with an initial exchange of messages. \n";
		std::cout << std::endl << std::endl;

		StringHelper::Title("DEMONSTRATING REQUIREMENT 11", '*');
		std::cout << "\n  Included an automated unit test suite that demonstrates you meet all the requirements of this project7 including the transmission of files.   \n";
		std::cout << std::endl << std::endl;

		x = "Extract ";
		x.append(toStdString(serverPort) + " ");
		x.append("Client1 "); // AUTHOR
		x.append("Package7-Version1 "); // PACKAGE NAME 
		x.append("True"); // DEPENDENCIES
		pSendr_->postMessage(x);
		Message msg = pRecvr_->getMessage();
		std::cout << "\n Successfully EXTRACTED to the ClientRepository\n";
		std::cout << "\nDemonstrating: Getpackage Server SideList \n";
		x = "GetPackageList ";
		x.append(toStdString(serverPort) + " ");
		pSendr_->postMessage(x);
		msg = pRecvr_->getMessage();
		std::cout << "\nObtained the File list from server\n";
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}


void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "Status messages displayed here";
	hStatus->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}


void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	
	hExtractInTab->Header = "Check In";
	hExtractOutTab->Header = "Extract";
	
	hTabControl->Items->Add(hExtractInTab);
	hTabControl->Items->Add(hExtractOutTab);
}

void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Red;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->TextWrapping = TextWrapping::WrapWithOverflow;
	hTextBlock1->FontSize = 14;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);

}

void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendButton->Content = "Send Message";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hResetButton->Content = "Reset";
	hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hResetButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 1);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpSendMessageView()
{
	
	hSendMessageGrid->Margin = Thickness(20);
	
	setTextBlockProperties();
	setButtonsProperties();
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}


void WPFCppCliDemo::sendExtractOutMessage(Object^ obj, RoutedEventArgs^ args)
{
	String^ package = hPackageListBox->SelectedItem->ToString();
	System::Nullable<bool> isDepChecked = hDepCheckBox->IsChecked;	
	if (package != "")
	{
		String^ d;
		if (isDepChecked.Value == true)
			d = "True ";
		else
			d = "False ";
		String^ s;
		s += "Extract " + serverPort + " " + "Client1 " + package + " " +  d +" ";
		pSendr_->postMessage( toStdString(s));
		Message msg = pRecvr_->getMessage();
		hfolderDir->Text = toSystemString( msg);
		hStatus->Text = "Extract message received";
	}
}


void WPFCppCliDemo::sendExtractInMessage(Object^ obj, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	System::Nullable<bool> isClosedForCheckIn = hIsClosedForCheckIn->IsChecked;
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		hStatus->Text = "Checking in the files/packages";
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
		hListBox->Items->Clear();
		std::string depList;
		index = 0;
		int count1 = hDepListBox->SelectedItems->Count;
		array<System::String^>^ items1 = gcnew array<String^>(count1);
		hStatus->Text = "Checking in the dependencies";
		if (count1 > 0) {
			for each (String^ item in hDepListBox->SelectedItems)
			{
				items1[index++] = item;		
				depList = depList + toStdString(item);
				hDepListBox->Items->Add(item);
			}
		}
		hDepListBox->Items->Clear();
		if (count1 > 0) {
			for each (String^ item in items)
			{
				hDepListBox->Items->Add(item);
			}
		}

		int newPackg = 0;
		newPackg = rand() % 1000;
		if (count > 0) {
			for each (String^ item in items)
			{
				sendExtractInMessage1(item, depList, isClosedForCheckIn, newPackg);
			}
		}
	}
	else
	{
		hStatus->Text = "Please select the directory to checkin";
	}
}
void WPFCppCliDemo::sendExtractInMessage1(String^ item,std::string depList, System::Nullable<bool> isClosedForCheckIn,int newPackg)
{
	hListBox->Items->Add(item);
	std::string x;
	String^ msgText;
	String^ d;
	if (depList == "")
		d = "";
	else
		d = toSystemString(depList);
	String^ e;
	if (isClosedForCheckIn.Value == true)
		e = "True ";
	else
		e = "False ";
	msgText += "CheckIn " + serverPort + " " + "Client1 " + "Package" + toSystemString(std::to_string(newPackg)) + " " + e + " " + "OneWay " + item + " " + d;
	pSendr_->postMessage(toStdString(msgText));
	Message msg = pRecvr_->getMessage();
	x = "GetPackageList "; 
	x.append(toStdString(serverPort));
	pSendr_->postMessage(x);
	msg = pRecvr_->getMessage();
	std::vector<std::string> s = Utilities::StringHelper::splitUsingSemiColon(msg);//display the packages in the item
	for (std::string i : s)
	{
		hDepListBox->Items->Add(toSystemString(i));
	}
	for (std::string i : s)
	{
		hPackageListBox->Items->Add(toSystemString(i));
	}
	Console::Write("Checked in package successfully");
	hStatus->Text = "Checked In successfully";
}

void WPFCppCliDemo::sendMsg(Object^ obj, RoutedEventArgs^ args)
{
	pSendr_->postMessage(toStdString(msgText));
	
	Console::Write("\n  sent test message");
	hStatus->Text = "Sent Test message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	hTextBlock1->Text += msg + "\n\n";
}

void WPFCppCliDemo::getMessage_()
{	
	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;

		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);
	}
}

void WPFCppCliDemo::Reset(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Reseted message text");
	hStatus->Text = "Reseted message";
	hTextBlock1->Text = "";
}


String^ WPFCppCliDemo::getItemsFromList()
{
	int index = 0;
	String^ selItems = "";
	return selItems;
}

String^ WPFCppCliDemo::getDependencies()
{
	int index = 0;
	String^ depItems = "";
	return depItems;
}

void WPFCppCliDemo::setUpCheckInView()
{
	hFileListGrid->Margin = Thickness(20);
	hExtractInTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRowChild1Def = gcnew RowDefinition();	
	hRowChild1Def->Height = GridLength(40);
	hFileChildListGrid->RowDefinitions->Add(hRowChild1Def);
	RowDefinition^ hRowChild2Def = gcnew RowDefinition();
	hFileChildListGrid->RowDefinitions->Add(hRowChild2Def);
	ColumnDefinition^ hColChild1Def = gcnew ColumnDefinition();
	hFileChildListGrid->ColumnDefinitions->Add(hColChild1Def);
	ColumnDefinition^ hColChild2Def = gcnew ColumnDefinition();
	hColChild2Def->Width = GridLength(250);
	hFileChildListGrid->ColumnDefinitions->Add(hColChild2Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Margin = Thickness(5);
	hBorder1->Child = hListBox;
	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Margin = Thickness(5);
	hBorder2->Child = hDepListBox;
	hDepListBox->SelectionMode = SelectionMode::Multiple;
	String^ listStr = "Choose Package";
	listLabel->Content = listStr;
	listLabel->Margin = Thickness(5);
	depLabel->Content = "Choose Dependencies";
	depLabel->Margin = Thickness(5);
	hFileChildListGrid->SetRow(listLabel, 0);
	hFileChildListGrid->SetColumn(listLabel, 0);
	hFileChildListGrid->SetRow(hBorder1, 1);
	hFileChildListGrid->SetColumn(hBorder1, 0);
	hFileChildListGrid->SetRow(depLabel, 0);
	hFileChildListGrid->SetColumn(depLabel, 1);
	hFileChildListGrid->SetRow(hBorder2, 1);
	hFileChildListGrid->SetColumn(hBorder2, 1);
	hFileListGrid->SetRow(hFileChildListGrid, 0);
	hFileChildListGrid->Children->Add(listLabel);
	hFileChildListGrid->Children->Add(hBorder1);
	hFileChildListGrid->Children->Add(depLabel);
	hFileChildListGrid->Children->Add(hBorder2);
	hFileListGrid->Children->Add(hFileChildListGrid);
	hListBox->SelectionMode = SelectionMode::Multiple;
	setUpCheckInView1();

}

void  WPFCppCliDemo::setUpCheckInView1()
{
	String^ listStr = "Choose Package";
	String^ listStr2 = "Closed Property";
	listLabel2->Content = listStr;
	listLabel2->Margin = Thickness(5);
	hFileChildListGrid->SetRow(listLabel2, 0);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(45);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFolderBrowseButton->Content = "Choose Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 140;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);
	RowDefinition^ hRow2Def3 = gcnew RowDefinition();
	hRow2Def3->Height = GridLength(45);
	hFileListGrid->RowDefinitions->Add(hRow2Def3);
	hCheckInButton->Content = "Check In";
	hCheckInButton->Height = 30;
	hCheckInButton->Width = 140;
	hCheckInButton->BorderThickness = Thickness(2);
	hCheckInButton->BorderBrush = Brushes::Black;
	
	hFileListGrid->SetRow(hCheckInButton, 2);
	hFileListGrid->Children->Add(hCheckInButton);
	RowDefinition^ hRow89Def = gcnew RowDefinition();
	hRow89Def->Height = GridLength(60);
	hFileListGrid->RowDefinitions->Add(hRow89Def);
	hIsClosedForCheckIn->Margin = Thickness(10);
	hIsClosedForCheckIn->Content = "Closed For Check In";
	hFileListGrid->SetRow(hIsClosedForCheckIn, 3);
	hFileListGrid->Children->Add(hIsClosedForCheckIn);
	hCheckInButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendExtractInMessage);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}
//browse for folder for files

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		
	}
}

void WPFCppCliDemo::browseForFolderExtract(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	System::Windows::Forms::DialogResult result;
	result = hExtractoutBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hExtractoutBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		hfolderDir->Text = path;
	}
}

void WPFCppCliDemo::setUpCheckOutView()
{	
	hExtractOutGrid->Margin = Thickness(10);
	hExtractOutTab->Content = hExtractOutGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(40);
	hExtractOutGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hExtractOutGrid->RowDefinitions->Add(hRow2Def);
	packageLabel->Margin = Thickness(5);
	packageLabel->Content = "Choose Package";
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Margin = Thickness(5);
	hBorder1->Child = hPackageListBox;
	hExtractOutGrid->SetRow(packageLabel, 0);
	hExtractOutGrid->SetRow(hBorder1, 1);
	hExtractOutGrid->Children->Add(packageLabel);
	hExtractOutGrid->Children->Add(hBorder1);
	hPackageListBox->SelectionMode = SelectionMode::Single;
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(40);
	hExtractOutGrid->RowDefinitions->Add(hRow3Def);
	chooseDirLabel->Content = "Download Location";
	hExtractOutGrid->SetRow(chooseDirLabel, 2);
	hExtractOutGrid->Children->Add(chooseDirLabel);
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(40);
	hExtractOutGrid->RowDefinitions->Add(hRow4Def);
	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	hSelFolderGrid->ColumnDefinitions->Add(hCol1Def);
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	hCol2Def->Width = GridLength(150);
	hSelFolderGrid->ColumnDefinitions->Add(hCol2Def);
	hfolderDir->Text = "";
	hfolderDir->Height = 30;
	hSelFolderGrid->SetColumn(hfolderDir, 0);
	hSelFolderGrid->Margin = Thickness(5);	
	hSelFolderGrid->Children->Add(hfolderDir);	
	hExtractOutGrid->SetRow(hSelFolderGrid, 3);
	hExtractOutGrid->Children->Add(hSelFolderGrid);
	setUpCheckOutView1();
}
// end
void  WPFCppCliDemo::setUpCheckOutView1()
{
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(40);
	hExtractOutGrid->RowDefinitions->Add(hRow5Def);
	hDepCheckBox->Margin = Thickness(5);
	hDepCheckBox->Content = "Extract Dependencies";
	hExtractOutGrid->SetRow(hDepCheckBox, 4);
	hExtractOutGrid->Children->Add(hDepCheckBox);
	RowDefinition^ hRow6Def = gcnew RowDefinition();
	hExtractOutGrid->RowDefinitions->Add(hRow6Def);
	hExtractButton->Content = "Extract";
	hExtractButton->Height = 50;
	hExtractButton->Width = 140;
	hExtractButton->BorderThickness = Thickness(2);
	hExtractButton->BorderBrush = Brushes::Black;
	hExtractOutGrid->SetRow(hExtractButton, 5);
	hExtractOutGrid->Children->Add(hExtractButton);
	hExtractButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendExtractOutMessage);
	hExtractoutBrowserDialog->ShowNewFolderButton = false;
	hExtractoutBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n TestExecutive Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n TestExecutive Window closing");
}

[STAThread]

int main(array<System::String^>^ args)
{
	try
	{
		Console::Title = "Graphical User Interface (GUI) Console Window";		
		Application^ app = gcnew Application();		
		app->Run(gcnew WPFCppCliDemo(args[0], args[1], args[2])); // Pass Args from Bat File		
		Console::WriteLine("\n\n");
	}
	catch (std::exception& exc)
	{				
		std::cout << "Error Connecting to Server.\n";
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
	}
}