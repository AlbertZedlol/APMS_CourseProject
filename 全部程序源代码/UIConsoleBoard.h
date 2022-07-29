/****************************************************************************************************************************
Filename		UIConsoleBoard.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of Class ConsoleBoard which is a pivot of programme-user-interaction.
*****************************************************************************************************************************/#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
using namespace std;

/*extendable interface*/
const enum dataBaseOperationType {
	_createFile = 1, _dataEntry, _findAndUpdate, _findAndDisplay, _advancedService, _exitDatabase=6   //#Change!
};


class ConsoleBoard
{
public:
	ConsoleBoard();
	void showMenu();
	int getDirective();
	bool whetherExitDateBase();
	void exitDatabase();
	void inputNum_s(void (* operation)(), bool condition(int value), int& valueStorage);
	bool isValidOperationType(int choice);
private:
	/*extendable interface*/
	int maxChoiceNum;
	/*extendable interface*/
	string phrases[10];   //10==maxChoiceNum+4

	void show();
	string _buff;
	void clear();
	void inputCh_s(const char* notion);
	int  inputMultiChoice(char* answers);
	void inputIdentity();
	std::string dictation;
	char answer[13];
	int finalChoice;
	bool exitDatebase;
	int  directive;
	
};
