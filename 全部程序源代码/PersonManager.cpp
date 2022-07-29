/******************************************************************************************************************
Filename		PersonManager.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Implementation of the methods of certain base class and derived classes of class Manager.
				Classes included: Staff, TeamLeader, Manager, DepartmentLeader, Executive, CEO.
******************************************************************************************************************/
#include "PersonManager.h"
#include "DepartmentAbstractClass.h"
#include "FileManager.h"

PersonDictionary dictionary;
using namespace std;

/*Constructors*/
Staff::Staff()
{
	name.clear();
	sex.clear();
	age = 0;
	id = 0;
	rank = 0;
	KPI = 0.0;
	baseSalary = 0;
	bonus = 0;
}

TeamLeader::TeamLeader(int i)
{
	this->personType = dictionary.PersonTypeTeamLeader;
	pPODbuf = NULL;
	//myTeam = NULL;
	this->complaintNum = 0;
	this->projectCompletion = 0;
	this->myTeamNumber = 0;
	teamSize = 0;
	teamDuty ="Lead team to complete assigned tasks";
}

DepartmentLeader::DepartmentLeader()
{
	personType = dictionary.PersonTypeDepartmentLeader;

	
	pPODbuf = NULL;
	//myDepartment = NULL;
	myDepNumber = 0;
	
	depDuty= "Lead department to complete assigned tasks";
}

Executive::Executive()
{
	personType = dictionary.PersonTypeExecutive;

	pPODbuf = NULL;

	//depInCharge = NULL;
	for(int i=0; i < personLimit::maxDepInCharge; i++)   /*10 departments is the maximum number. wile depInCharge is a linked list and can be appended freely.*/
	{
		depInChargeNumber[i] = -1;
	}
	stockShare = 0;
}

ChiefExecutive::ChiefExecutive()
{
	personType = dictionary.PersonTypeCEO;

	pPODbuf = NULL;
	for(int i=0;i<5;i++)
	this->executiveAtCommandNumber[i]=0;
 
}

/*Destructos*/
TeamLeader::~TeamLeader()
{
	delete this->pPODbuf;
	//delete this->myTeam;
}

DepartmentLeader::~DepartmentLeader()
{
	delete this->pPODbuf;
	//delete this->myDepartment;
}

Executive::~Executive()
{
	delete this->pPODbuf;
	//delete this->depInCharge;
}

ChiefExecutive::~ChiefExecutive()
{
	delete this->pPODbuf;
	//delete this->executiveAtCommand;
}

/*Show personal profile*/
void Staff::showPersonalProfile(bool isThisClass)
{
	cout << setw(15) << "\n[Employee profile]\n";
	cout << setw(15) << "Personal   information:\n";
	cout << setw(15) <<setiosflags(ios::left) << "\tName: " << setw(20) << this->name << "\tAge  :" << setiosflags(ios::left) << setw(4) << this->age << "\t\tSex:" << setiosflags(ios::left) << setw(16) << this->sex << endl;
	cout << setw(15) << "Vocational information:\n";
	cout << setw(15) << "\tID: " << setw(20) << setiosflags(ios::left) << this->id << endl;
	cout << setw(15) << setiosflags(ios::left) << "\tPost: " << setw(20) << setiosflags(ios::left) << dictionary.postName[this->personType+1];
	cout << setw(6) << setiosflags(ios::left) << "\tRank :" << setiosflags(ios::left) << setw(4) << this->rank << "\t\tKPI:" << setiosflags(ios::left) << this->KPI << endl;
	cout << setw(15) << setiosflags(ios::left) << "\tBase salary:  US$" << setiosflags(ios::left) << setw(20) << this->baseSalary  << "\tBonus:US$" << setiosflags(ios::left) << setw(20)<< this->bonus << endl;
}
void Manager::showPersonalProfile(bool is_thisClass)
{
	Staff::showPersonalProfile(false);

	cout << setw(15) << setiosflags(ios::left) << "\tComplaints:" << setw(20) << setiosflags(ios::left) << this->complaintNum << "\tProjects Completed:" << this->projectCompletion << endl;
}

void TeamLeader::showPersonalProfile(bool is_thisClass)
{
	if (is_thisClass)
		cout << "\nThis is team leader "<<this->name<<"'s profile.";

	Manager::showPersonalProfile(false);
	if (is_thisClass)
	{
		cout << setw(15) << "\tDuty:     " << setiosflags(ios::left) << teamDuty << endl;
		cout << setw(15) << "\tTeam size:" << setiosflags(ios::left) << setw(4) << teamSize;
		cout << setw(15) << "\tTeam Duty: " << this->teamDuty << endl;
	}
	/*Switch: whether to show team member's info.*/;

}
void DepartmentLeader::showPersonalProfile(bool is_thisClass)
{
	if (is_thisClass)
		cout << "\nThis is department leader " << this->name << "'s profile.\n";

	TeamLeader::showPersonalProfile(false);

	if (is_thisClass)
	{
		//cout << setw(15) << "\n\tDepartment in charge with: "/* << myDept.getName()*/ << endl;
		cout << setw(15) << "\tDuty:     " << setiosflags(ios::left) << depDuty << endl;
		cout << setw(15) << "\tDepartment performance" << departmentKPI;
	}

	/*Switch: whether to show department member's info. goto depart.show members.*/;
}
void Executive::showPersonalProfile(bool is_thisClass)
{
	if (is_thisClass)
		cout << "\nThis is executive "<< this->name << "'s profile.\n";

	DepartmentLeader::showPersonalProfile(false);

	if (is_thisClass)
		cout << setw(15) << "\tDepartments in charge with: ";
	cout << this->depInChargeNumber[0] << "  " << this->depInChargeNumber[1] << endl;
	cout << setw(15) << "\tStock share: " << this->stockShare << endl;
}
void ChiefExecutive::showPersonalProfile(bool is_thisClass)
{
	cout << "\nThis is chief executive " << this->name << "'s profile.\n";

	Executive::showPersonalProfile(false);

	cout << setiosflags(ios::left)<< "\tExecutives at command:\t\t      ";
	for (int i = 0; i < maxExecutiveNum; i++)
	{
		cout <<"  "<<this->executiveAtCommandNumber[i];
	}
	cout << endl;

	/*it's better to use linked list and iterator  to arrange.*/
	/*
	{
		for(int i=0;i<executiveAtCommand.getSize();i++)
		{
				executiveAtCommand[i]->name<<endl;
		}
	}*/
}

/*Input personal information.*/
void Staff::inputPersonalInfo(bool is_thisClass)
{
	cout << setw(15) << "\n[Employee Profile]\n";
	cout << setw(15) << "Personal   information:\n";
	cout << "\tName:";
	getline(cin, this->name);
	
	cout << "\tAge:";
	cin>>this->age;

	cin.ignore(1);
	cout << "\tSex:";
	getline(cin, this->sex);//Getline helps you eat the ENTER pressed after the last typing.
							//but cin>> don't. So you always have to add cin.ignore(1) after cin>>
							//buf can safely use getlin(cin,somestring) before codes.
	cout << setw(15) << "Vocational information:\n";
	
	cout << "\tID:";
	string _idBuf;
	getline(cin, _idBuf);
	this->id=stoul(_idBuf);

	cout << "\tRank:";
	cin>>this->rank;
	cin.ignore(1);

	cout << "\tKPI:";
	cin>>this->KPI;
	cin.ignore(1);

	cout << "\tBase salary:US$  ";
	cin>>this->baseSalary;
	cin.ignore(1);

	cout << "\tBonus: US$  ";
	cin>>this->bonus;
	cin.ignore(1);
}


void Manager::inputPersonalInfo(bool is_thisClass)
{
	Staff::inputPersonalInfo(false);
	cout << "\tComplaint received:";
	cin >> this->complaintNum;
	cin.ignore();
	cout << "\tProject Completed:";
	cin >> this->projectCompletion;
	cin.ignore();
}

void TeamLeader::inputPersonalInfo(bool is_thisClass)
{
	Manager::inputPersonalInfo(false);
	//cout << this << endl;
	if (is_thisClass == false)
		return;
	cout << "Size of the team:";
	cin >> this->teamSize;
	cout << "Duty:";
	cin.ignore(1);
	getline(cin, this->teamDuty);

	if (is_thisClass)
		cout << "Input complete!" << endl;
}

void DepartmentLeader::inputPersonalInfo(bool is_thisClass)
{
	TeamLeader::inputPersonalInfo(false);
	if (is_thisClass == false)
		return;
	cout << "\nDepartment KPI:\n";
	cin >> this->departmentKPI;
	cout << "\nDuty:\n";
	cin.ignore(1);/*skip the enter.*/
	getline(cin, this->depDuty);
	if(is_thisClass)
		cout << "Input complete!" << endl;
}
void Executive::inputPersonalInfo(bool is_thisClass)
{
	DepartmentLeader::inputPersonalInfo(false);
	if (is_thisClass == false)
	{
		cout << "\tStock Share: ";
		cin >> this->stockShare;
		cin.ignore(1);

		return;
	}

	cout << "\tDepartment in charge:" << endl;
	for (int i = 0; i < personLimit::maxDepInCharge;i++)
	{
		cout << "\tDepNo.";
		cin >> this->depInChargeNumber[i];
		cin.ignore();
	}
	if (is_thisClass)
		cout << "Input complete!" << endl;
}


void ChiefExecutive::inputPersonalInfo(bool is_thisClass)
{

	Executive::inputPersonalInfo(false);


	cin.clear();
	cout << "\nExecutives at commmand" << endl;
	for (int i = 0; i < personLimit::maxExecutiveNum; i++)
	{
		cout << "\tID:";
		cin >> this->executiveAtCommandNumber[i];
		cin.ignore(1);
	}
	cout << "Input complete!" << endl;
}


/*Special methods for HR management.*/
void  Staff::promote(int deltaRank)
{
	if (this->rank == 9)
		return;
	this->rank += deltaRank;
}
void Staff::demote(int deltaRank)
{
	if (this->rank == 0)
		return;
	this->rank -= deltaRank;
}

void  Staff::changeReward(int rewardType, int rewardChangeMode, unsigned long deltaValue)
{
	if (rewardType == this->rewardType::BasicSalary)
	{
		this->changeSalary(rewardChangeMode, deltaValue);
	}
	else if (rewardType == this->rewardType::Bonus)
	{
		this->changeBonus(rewardChangeMode, deltaValue);
	}
	cout << "\nchange reward complete!" << endl;

}

void Staff::changeBonus(int rewardChangeMode, unsigned long Value)
{
	if (rewardChangeMode == this->changeMode::changeTo)
		this->bonus = Value;
	else
		this->bonus += Value;
}

void  Staff::changeSalary(int rewardChangeMode, unsigned long Value)
{
	if (rewardChangeMode == this->changeMode::changeTo)
		this->baseSalary = Value;
	else
		this->baseSalary += Value;
}


void TeamLeader::updateSubKPI()
{
	double buf = 0;
}


string TeamLeader::getName(int passWord)
{
	if (passWord == this->myTeam->teamID)
		return this->name;
	return "";
}
void TeamLeader::showSubordinates()
{
	this->myTeam->showTeam();
}
void TeamLeader::sortSubBy(const string& item)
{
	this->myTeam->sort(item);
}
Staff* TeamLeader::promote(int id, int rankChange)
{
	return(this->changeInfo(id, "Rank", rankChange));
}
Staff* TeamLeader::demote(int id, int rankChange)
{
	return(this->changeInfo(id, "Rank", (-1) * rankChange));
}
Staff* TeamLeader::raiseSal(int id, double payRise)
{
	return(this->changeInfo(id, "Salary", payRise));
}
void TeamLeader::dismiss(int id)
{
	this->myTeam->dismiss(id);
}

Staff* TeamLeader::changeInfo(int id, const string& item, double delta)
{
	return (this->myTeam->changeMemberInfo(id, item, delta));
}
void TeamLeader::changeSubDep(int id, int _nexeTeamId)
{
	this->myTeam->changeDep(id, _nexeTeamId);
}

void TeamLeader::loadFromFile(const string&pattern, FileManager&mg)
{
	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou erro in teammates type recording. Mismatch found when loading a teamLeader with pattern"<<pattern<<"from file." << endl;
		return;
	} 
	*this = *(TeamLeader*)&bufTld;
}

void DepartmentLeader::loadFromFile(const string& pattern, FileManager& mg)
{

	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou erro in teammates type recording. Mismatch found when loading a " << this->personType << " pattern" << pattern << "from file." << endl;
		return;
	}
	*this = *(DepartmentLeader*)&bufDld;
}

void Executive::loadFromFile(const string& pattern, FileManager& mg)
{

	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou erro in teammates type recording. Mismatch found when loading a " << this->personType << " pattern" << pattern << "from file." << endl;
		return;
	}
	*this = *(Executive*)&bufExe;
}

void ChiefExecutive::loadFromFile(const string& pattern, FileManager& mg)
{

	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou erro in teammates type recording. Mismatch found when loading a " << this->personType << " pattern" << pattern << "from file." << endl;
		return;
	}
	*this = *(ChiefExecutive*)&bufCeo;
}

