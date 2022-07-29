/******************************************************************************************************************
Filename		PersonAbstractClass.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declaration of abstract classes. Inclusion list: Person, Staff, Investor, Professional, Manager.
******************************************************************************************************************/
#pragma once

#include "PersonPOD.h"
class FileManager;
class Person;
class TypeConversionMachine;

class TeamLeader;
class DepartmentLeader;
class Executive;
class ChiefExecutive;
class FinancialAnalyst;
class Technician;

extern TeamLeader bufTld;
extern DepartmentLeader bufDld;
extern Executive bufExe;
extern ChiefExecutive bufCeo;
extern FinancialAnalyst bufFa;
extern Technician bufTech;


class Date {
protected:
	int day, month, year;
};

class PersonDictionary
{
public:
	int numberOfPosts = 6;
	const enum personType {
		NoneType = -2, AbsBase = -1,
		PersonTypeTeamLeader = 1, PersonTypeDepartmentLeader = 2, PersonTypeExecutive = 3, PersonTypeCEO = 4,
		PersonTypeFinancialAnalyst = 5, PersonTypeTechnician = 6,
	};
	const string postName[9] = {
		"NoneType","AbstractBase","Team Leader","Department Leader","Executive","Chief Executive",
		"Financial Analyst","Technician"," "
	};
};
extern PersonDictionary dictionary;

class Person
{
protected:
	/*data members for file IO*/
	char deliminator = '\n';
	int  personType = -1;

protected:
	/*personal info*/
	string 	name;								//No more than 40 words.
	int		age;
	string 	sex;								//Male: Female: Sexual Minority
public:

	virtual void showPersonalProfile(bool isThisClass = true) = 0;

	virtual void inputPersonalInfo(bool is_thisClass = true) = 0;

	friend FileManager;
	friend TypeConversionMachine;
protected:
	/*intrusive funtions about file IO*/


	virtual void loadFromFile(const string&, FileManager& )=0;

	virtual void toPOD(bool isThisClass = true) = 0;
	virtual void write2file(fstream& mg) = 0;
	virtual void readFromFile(fstream& mg) = 0;
	virtual void fromPOD() = 0;
};

class Staff :public Person
{
protected:
	unsigned long id;
	//Hierarchical info
	int rank;
	//Performance.
	double 	KPI;
	//Reward.
	unsigned int baseSalary;
	unsigned int bonus;
	const enum rewardType { BasicSalary = 0, Bonus = 1 };
	const enum changeMode { changeTo = 0, changeBy = 1 };
public:
	Staff();
#ifdef DEBUG_CONSTRUCTOR
	Staff(string nm, double kpi = 0, int r = 0, int _id = 0, double bs = 0, int pT = 0) :KPI(kpi), rank(r), id(_id), baseSalary(bs), Person(name, pT) {};
#endif // DEBUG_CONSTRUCTOR
#ifdef DEBUG_CONSTRUCTOR
	Staff(int _id, const string& myName = "null") :id(_id), Person(myName) { };
#endif // DEBUG_CONSTRUCTOR
	friend TypeConversionMachine;
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual void showPersonalProfile(bool isThisClass = true);
	virtual void promote(int);
	virtual void demote(int);
	virtual void changeReward(int rewardType, int rewardChangeMode, unsigned long deltaValue);
protected:
	virtual void changeSalary(int rewardChangeMode, unsigned long Value);
	virtual void changeBonus(int rewardChangeMode, unsigned long Value);
	//virtual void changeDepartment() = 0;
};


class Manager :public Staff
{
protected:
	friend TypeConversionMachine;
	int complaintNum;
	int projectCompletion;
	virtual void updateSubKPI() = 0;				//Update KPI for subordinates(optional)
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);

#ifdef DEBUG_CONSTRUCTOR
	Manager(int _id = 0, const string& myName = "null") :Staff(_id, myName) { };
#endif // DEBUG_CONSTRUCTOR
};
