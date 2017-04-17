#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Demo for CSE 687 Project #4                                //
// ver 2.0																 //
//  Language:          C++, Visual Studio 2015							 //
// Application:        Dependency-Based Remote Code Repository,          //
//     Platform:       MacBook Pro, Core-i5, Windows 10 on Bootcamp      //
//       Author:       Manjunath Babu, MS in Computer Engineering        //
//                     (315) 450-1435, mababu@syr.edu                    //
// Original author:    Jim Fawcett, Syracuse University, CST 4-187		 //
//                     jfawcett@twcny.rr.com							 //			
///////////////////////////////////////////////////////////////////////////
/*

*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include "../Utilities/Utilities.h"
#include <iostream>
namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references

		ISenderClass* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		
		TabItem^ hExtractInTab = gcnew TabItem();
		TabItem^ hExtractOutTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// Controls for SendMessage View

		Grid^ hSendMessageGrid = gcnew Grid();
		Button^ hSendButton = gcnew Button();
		Button^ hResetButton = gcnew Button();
		TextBlock^ hTextBlock1 = gcnew TextBlock();
		TextBlock^ hTextBlock2 = gcnew TextBlock();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		//StackPanel^ hStackPanel1 = gcnew StackPanel();
		DockPanel^ hStackPanel1 = gcnew DockPanel();
		String^ serverPort;
		String^ folderPath;
		String^ msgText
			= "TestMessage "   // command
			+ "Hello World! ";  // message body
			
		Grid^ hExtractOutGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hExtractoutBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hPackageListBox = gcnew ListBox();
		Grid^ hSelFolderGrid = gcnew Grid();
		Label^ chooseDirLabel = gcnew Label();
		TextBox^ hfolderDir = gcnew TextBox();
		//Button^ hFldrBrowseButton = gcnew Button();
		Button^ hExtractButton = gcnew Button();
		Label^ packageLabel = gcnew Label();
		CheckBox^ hDepCheckBox = gcnew CheckBox();
		CheckBox^ hIsClosedForCheckIn = gcnew CheckBox();

		// Controls for FileListView View
		Grid^ hFileListGrid = gcnew Grid();
		Grid^ hFileChildListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		ListBox^ hDepListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Button^ hShowItemsButton = gcnew Button();
		Button^ hCheckInButton = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();
		Label^ listLabel = gcnew Label();
		Label^ depLabel = gcnew Label();
		Label^ listLabel2 = gcnew Label();
		// receive thread

		Thread^ recvThread;

	public:		
		WPFCppCliDemo(String ^ sendaddr, String ^ recaddr, String ^ path);
		void automateTestCaseRun();
		~WPFCppCliDemo();

		void setUpStatusBar();
		void setUpTabControl();
		void setUpSendMessageView();
		void setUpCheckInView();
		void setUpCheckInView1();
		void setUpCheckOutView();

		void sendMsg(Object^ obj, RoutedEventArgs^ args);
		void sendExtractInMessage(Object^ obj, RoutedEventArgs^ args);				
		void WPFCppCliDemo::sendExtractInMessage1(String^ item, std::string depList, System::Nullable<bool> isClosedForCheckIn, int newPackg);
		void sendExtractOutMessage(Object^ obj, RoutedEventArgs^ args);
		void addText(String^ msg);
		void getMessage_();
		void Reset(Object^ sender, RoutedEventArgs^ args);

		String^ getItemsFromList();
		String^ getDependencies();
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void browseForFolderExtract(Object^ sender, RoutedEventArgs^ args);
		void setUpCheckOutView1();
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		void setTextBlockProperties();
		void setButtonsProperties();
	};
}


#endif