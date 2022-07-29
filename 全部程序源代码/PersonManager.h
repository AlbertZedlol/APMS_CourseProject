/****************************************************************************************************************************
Filename		PersonManager.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of the manager family and an enumerate personLimit which indicates maximum size of certain personnels.
				Classes included: TeamLeader, Department Leader, Executive, ChiefExecutive.
*****************************************************************************************************************************/
#pragma once

#include "A-GeneralHeader.h"
 
#include "PersonAbstractClass.h"

#include "PersonPOD.h"



class Staff;
class TeamBase;
class TypeConversionMachine;
class HRoffice;

const enum personLimit {
	maxDepInCharge = 2,
	maxExecutiveNum = 5
};
 
class TeamLeader :public Manager
{
protected:
	int teamSize;
	string teamDuty;					//What job this manager is in charge of currently.
	void updateSubKPI();
	int   myTeamNumber;					//There's a problem about storing the dynamically allocated value of pointers in a file.Best to use team member to memorize?
	TeamBase* myTeam;
public:
	TeamLeader(int i=0); 
	friend TypeConversionMachine;
	friend HRoffice;
	string getName(int passWord);
	void changeSubDep(int id, int _nextTeamId);
	void showSubordinates();
	void sortSubBy(const string& itemName);
	Staff* promote(int id, int rankChange);
	Staff* raiseSal(int id, double payRise);
	Staff* demote(int id, int rankChange);
	void dismiss(int id);
protected:
	Staff* changeInfo(int id, const string& item, double delta);
public:
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual ~TeamLeader();
protected:



	void loadFromFile(const string&, FileManager&);


	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();
private:
	PODTeamLeader* pPODbuf;
};


class DepartmentLeader :public TeamLeader
{
protected:
	double departmentKPI;
	string depDuty;
private:
	PODDepLeader* pPODbuf;
	//Department* myDepartment;
	int			  myDepNumber;
	friend TypeConversionMachine;
public:
	DepartmentLeader();
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual ~DepartmentLeader(); 
protected:

	void loadFromFile(const string&, FileManager&);


	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();
};



class Executive :public DepartmentLeader
{
	

private:
	PODExecutive* pPODbuf;
protected:
	 
	int			  depInChargeNumber[personLimit::maxDepInCharge];
	unsigned long stockShare;
public:
	Executive();
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual ~Executive();
	friend TypeConversionMachine;
protected:



	void loadFromFile(const string&, FileManager&);

	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();

};


class ChiefExecutive: public Executive
{
private:
	PODCEO* pPODbuf;
protected:



	int			executiveAtCommandNumber[personLimit::maxExecutiveNum];
public:
	ChiefExecutive();
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual ~ChiefExecutive();
	friend TypeConversionMachine;
protected:


	void loadFromFile(const string&, FileManager&);


	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();
};


