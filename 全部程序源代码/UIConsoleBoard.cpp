/****************************************************************************************************************************
Filename		UIConsoleBoard.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Details of  member functions of class ConsoleBoard.
*****************************************************************************************************************************/

#include "UIConsoleBoard.h"
#include<iomanip>
using namespace std;
ConsoleBoard::ConsoleBoard()
{
	_buff = "";
	directive = 0;
	dictation = { "NULL" };
	directive = 0;
	exitDatebase = false;
	strcpy_s(answer, "TtFfLlDdEeCc");
	maxChoiceNum = 6;
	//init menu.
	clear();
	phrases[0] = "";
	phrases[1] = "Albert Personnel Management System ";
	phrases[2] = "MENU   ";
	phrases[3] = "1.Enter   database   ";
	phrases[4] = "2.Record information ";
	phrases[5] = "3.Update information ";
	phrases[6] = "4.Search any item    ";


	//#Change3

	phrases[7] = "5.Advanced service   ";
	phrases[8] = "6.Leave this database";


	phrases[maxChoiceNum + 3] = "   ";
	phrases[maxChoiceNum + 4].append("Press 1 to ").append(to_string(this->maxChoiceNum).c_str()).append(" to visit the database.");
}

int ConsoleBoard::getDirective()
{
	return this->directive;
}

bool ConsoleBoard::whetherExitDateBase()
{
	return exitDatebase;
}

void ConsoleBoard::clear()
{
	for (int i = 0; i < maxChoiceNum + 3; i++)
		this->phrases[i].clear();
}

void ConsoleBoard::show()
{
	cout << "\n\n";
	const int middleX = 75;
	const int roofWidth = 100;
	const int pillarToRoofLeftCorner = 15;
	const int skyThickness = 10;
	const int roofThickness = 11;
	const int pillarHeight = this->maxChoiceNum + 5;
	const int pillarWidth =10;
	int roofWidthOnThisLayer = 1;
	//Draw sky above roof.
	for (int i = 0; i < skyThickness; i++)
		cout << endl;
	//Draw roof.
	const int leftCornerX = middleX - roofWidth / 2;
	const int rightCornerX = middleX + roofWidth / 2;
	int whiteSpaceDecreasePerLayer = (middleX - leftCornerX) / roofThickness;
	for (int whiteSpaceNum = middleX - 1; whiteSpaceNum >= leftCornerX; whiteSpaceNum -= whiteSpaceDecreasePerLayer)
	{
		//Draw tile on left ringe.
		for (int i = 0; i < whiteSpaceNum; i++)
			cout << " ";
		cout<< "&";
		//Draw roof of this layer.
		roofWidthOnThisLayer = ((middleX - whiteSpaceNum) - 1) * 2;
		for (int i = 0; i < roofWidthOnThisLayer; i++)
			cout << "&";
		//Draw next layer.
		cout << endl;
	}
	//Draw pillar and options.
	const int leftPillarLeftRinge = leftCornerX + pillarToRoofLeftCorner;
	const int leftPillarRightRinge = leftPillarLeftRinge + pillarWidth;
	const int rightPillarRightRinge = rightCornerX - pillarToRoofLeftCorner;
	const int rightPillarLeftRinge = rightPillarRightRinge - pillarWidth;
	int phraseLeftFringe = 0, phraseRightFringe = 0;
	for (int lineNum = 0; lineNum < pillarHeight; lineNum++)
	{
		//Draw left pillar.
		for (int i = 0; i < leftPillarLeftRinge - 1; i++)
			cout << " ";
		for (int i = 0; i < pillarWidth; i++)
			cout << 1;

		//Draw blank space for line No.0 and lines without options.
		if (lineNum == 0 || lineNum > maxChoiceNum + 4)
		{
			for (int i = 0; i < rightPillarLeftRinge - leftPillarRightRinge; i++)
				cout << " ";
		}
		//Draw phrases in the middle.
		else
		{
			phraseLeftFringe = middleX - phrases[lineNum].length() / 2;
			phraseRightFringe = middleX + phrases[lineNum].length() / 2;
			for (int i = leftPillarRightRinge; i < phraseLeftFringe; i++)
				cout << " ";
			cout << phrases[lineNum];
			for (int i = phraseRightFringe + 1; i < rightPillarLeftRinge; i++)
				cout << " ";
		}
		//Draw right pillar.
		for (int i = 0; i < pillarWidth; i++)
			cout << 1;
		//Next layer.
		cout << endl;
	}
	cout << "\n\n\n" << setw(middleX - 3) << setfill(' ') << ">>>";
}

bool ConsoleBoard::isValidOperationType(int choice)
{
	return(choice > 0 && choice < 7);
}

void ConsoleBoard::showMenu()
{
	system("color 70");
	this->show();
	directive = 0;
	cin.clear();
	dictation.clear();
	directive = 2;
	getline(cin, _buff);
	directive = atoi(_buff.c_str());
	while (isValidOperationType(directive)==false)
	{
		system("cls");
		std::cout << "\n\n\n\n\n\t\t\t\t\t:( Sorry, \n\t\t\t\t\t\"" << _buff << "\" \n\t\t\t\t\tis not a valid choice.Please choose again.\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3500));
		system("cls");
		this->show();
		cin.clear();
		directive = 0;
		dictation.clear();
		getline(cin, _buff);
		directive = atoi(_buff.c_str());
	}
	cout <<"succeed" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ConsoleBoard::inputNum_s(void (*operation)(), bool condition(int value), int& valueStorage)
{
	int directive = -1;
	std::string _buff;
	std::string dictation;

	operation();

	cin.clear();
	dictation.clear();
	directive = 2;
	getline(cin, _buff);
	if (_buff.length() == 1)
	{
		directive = atoi(_buff.c_str());
	}
	else
	{
		directive = -1;
	}
	while (condition(directive) == false)
	{
		system("cls");

		std::cout << "\n\n\n\n\n\t\t\t\t\t:( Sorry, \n\t\t\t\t\t\"" << _buff << "\" \n\t\t\t\t\tis not a valid choice.Please choose again.\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3500));
		system("cls");

		operation();

		cin.clear();
		directive = 0;
		dictation.clear();
		getline(cin, _buff);
		if (_buff.length() != 1)
			continue;
		directive = atoi(_buff.c_str());
	}
	valueStorage = directive;
	cout << "succeed" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ConsoleBoard::inputCh_s(const char* notion)
{
	dictation.clear();
	std::cout << "\n\n\n\n\n\t\t\t\t\t\t" << notion << "\n" << "\t\t\t\t\t\tYes: Y   " << "  NO : N   \n\t\t\t\t\t\t>>>";
	while ((dictation[0] != 'N' && dictation[0] != 'n' && dictation[0] != 'Y' && dictation[0] != 'y') || dictation.length() != 1)
	{
		system("cls");
		std::cout << "\n\n\n\n\n\t\t\t\t\t\t" << notion << "\n" << "\t\t\t\t\t\tYes: Y   " << "  NO : N   \n\t\t\t\t\t\t>>>";
		std::cin >> dictation;
	}
}

int ConsoleBoard::inputMultiChoice(char* answers)
{
	for (int i = 0; i < strlen(answers); i++)
	{
		if (dictation[0] == *(answers + i))
			return i;
	}
	return -1;
}

void ConsoleBoard::inputIdentity()
{
	dictation.clear();
	finalChoice = -1;
	std::cout << "\n\n\t\t\t\t\t\t" << "Please select this person's identity.\n";
	std::cout << "\n\n\t\t\t\t\t\t" << "Technician            press T";
	std::cout << "\n\n\t\t\t\t\t\t" << "Financial Analyist    press F";
	std::cout << "\n\n\t\t\t\t\t\t" << "Team Leader           press L";
	std::cout << "\n\n\t\t\t\t\t\t" << "Department Leader	  press D";
	std::cout << "\n\n\t\t\t\t\t\t" << "Executive			  press E";
	std::cout << "\n\n\t\t\t\t\t\t" << "Chief Executive		  press C";
	
	std::cout << "\n\n\n\n\n\t\t\t\t\t\t";

	std::cin >> dictation;
	while (inputMultiChoice(answer) == -1 || dictation.size() != 1)
	{
		system("cls");
		dictation.clear();
		
		std::cout << "\n\n\t\t\t\t\t\t" << "Sorry, \""<< dictation<<"\" is not a valid choice, please choose again.\n";
		std::cout << "\n\n\t\t\t\t\t\t" << "Please select this person's identity.\n";
		std::cout << "\n\n\t\t\t\t\t\t" << "Technician            press T";
		std::cout << "\n\n\t\t\t\t\t\t" << "Financial Analyist    press F";
		std::cout << "\n\n\t\t\t\t\t\t" << "Team Leader           press L";
		std::cout << "\n\n\t\t\t\t\t\t" << "Department Leader	  press D";
		std::cout << "\n\n\t\t\t\t\t\t" << "Executive			  press E";
		std::cout << "\n\n\t\t\t\t\t\t" << "Chief Executive		  press C";
		std::cout << "\n\n\n\n\n\t\t\t\t\t\t";
		cin.clear();
		cin >> dictation;
	}
	finalChoice = inputMultiChoice(answer) / 2;
}

void ConsoleBoard::exitDatabase()
{	
	system("cls");
	inputCh_s("Are you sure to leave the database?"); 
	cout << "\n";
	cin.ignore();
	if (dictation[0] == 'N' || dictation[0] == 'n')
	{
		return;
	}
	exitDatebase = true;
	system("cls");
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tAlbert Personnel Management System (APMS) was first created on May 13th, 2022.\n\n\n\n\n\n\n\n\n\n";
	std::cout << "\t\t\t\t\t\t\t\tThanks for using.\n\n\n" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	exit(0);
}