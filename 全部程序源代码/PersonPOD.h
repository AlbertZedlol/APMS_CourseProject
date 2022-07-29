/****************************************************************************************************************************
Filename		PersonPOD.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of the PODs of all the classes.This is a copy of real classes without virtual tables and pointers 
				and thus is friendly to file IO.
*****************************************************************************************************************************/
#pragma once
#include <string>
using namespace std;

class PODPerson
{
public:
	char deliminator = '\n';
	char personType[40] = { "    " };//int  personType=-1;
	char name[40]={"                                       "};
	char age[4] = {"   "};
	char sex[40]={"                                       "};
};

class PODStaff :public PODPerson
{
public:
	char id[16]={"               "};
	char rank[3] = { "  " };
	char KPI[9] = { "        " };
	char baseSalary[15] = { "             " };
	char bonus[15] = { "              " };
};


class PODManager :public PODStaff 
{
public:
	char complaintNum[4] ={"   "};
	char projectCompletion[4] = { "   " };
};

class PODTeamLeader:public PODManager
{
public:
	char myTeamNumber[5] ={"    "};

	char teamSize[3] ={"  "};
	char teamDuty[100]="                                                                                                   ";
};


class PODDepLeader :public PODTeamLeader
{
public:
	char myDepNumber[3] = { "  " };

	char departmentKPI[9] = {"        "};
	char depDuty[100]= "                                                                                                   ";
};


class PODExecutive :public PODTeamLeader
{
public:
	char  stockShare[15] ={"              "};
	int	  depInChargeNumber[2] = { 0 };
};

class PODCEO :public PODExecutive
{
public:
	int		executiveAtCommandNum[5]={0};
};


class PODProfessional :public PODStaff
{
public:
	char customerApproval[10] = {"         "};
};

class PODTechnician :public PODProfessional
{
public:
	char patent[10] = { "         " };
};


class PODFA :public PODProfessional
{
public:
	char FS[5] = { "    " };
};


class PODSeniorTechnician :public PODDepLeader, public PODTechnician
{
public:
	char  engineeringProject[40]= { "                                       " };
	double projectCompletion;
};

class PODSeniorFA :public PODDepLeader, public PODFA
{
public:
	char seasonalBudget[15] = { "              " };
};

class PODHRoffficer :public PODProfessional, public PODManager
{
};


class TypeConversionMachine
{
public:
	TypeConversionMachine(int i) {	};

	template <class T>
	void toPODbasic(T*);

	template <class T>
	void toPODForManagers(T* sb);

	 
	template <class T>
	void fromPODbasic(T*);

	template <class T>
	void fromPODForManagers(T* sb);

	void toCharArray(const int& dataSource, char* _destination);
	void toCharArray(const double& dataSource, char* _destination);
	void toCharArray(const unsigned long& dataSource, char* _destination);
	void toCharArray(const unsigned int& dataSource, char* _destination);
};


template <class T>
void TypeConversionMachine::toPODbasic(T* sb)
{
	toCharArray(sb->personType, sb->pPODbuf->personType);
	strncpy_s(sb->pPODbuf->name, sb->name.c_str(), strlen(sb->pPODbuf->name) - 1);
	toCharArray(sb->age, sb->pPODbuf->age);
	strncpy_s(sb->pPODbuf->sex, sb->sex.c_str(), strlen(sb->pPODbuf->sex) - 1);

	toCharArray(sb->id, sb->pPODbuf->id);

	toCharArray(sb->rank, sb->pPODbuf->rank);
	toCharArray(sb->KPI, sb->pPODbuf->KPI);
	toCharArray(sb->baseSalary, sb->pPODbuf->baseSalary);
	toCharArray(sb->bonus, sb->pPODbuf->bonus);
}

template <class T>
void TypeConversionMachine::toPODForManagers(T* sb)
{
	toPODbasic(sb);
	this->toCharArray(sb->complaintNum, sb->pPODbuf->complaintNum);
	this->toCharArray(sb->projectCompletion, sb->pPODbuf->projectCompletion);
	this->toCharArray(sb->teamSize, sb->pPODbuf->teamSize);
	strncpy_s(sb->pPODbuf->teamDuty, sb->teamDuty.c_str(), strlen(sb->pPODbuf->teamDuty) - 1);
}

template <class T>
void TypeConversionMachine::fromPODbasic(T* sb)
{
	sb->personType = stoi(sb->pPODbuf->personType);
	sb->name = sb->pPODbuf->name;
	sb->age = stoi(sb->pPODbuf->age);
	sb->sex = sb->pPODbuf->sex;
	sb->id = stoul(sb->pPODbuf->id);
	sb->rank = stoi(sb->pPODbuf->rank);
	sb->KPI = stod(sb->pPODbuf->KPI);
	sb->baseSalary = stoi(sb->pPODbuf->baseSalary);
	sb->bonus = stoi(sb->pPODbuf->bonus);
}

template <class T>
void TypeConversionMachine::fromPODForManagers(T* sb)
{
	this->fromPODbasic(sb);
	sb->complaintNum = stoi(sb->pPODbuf->complaintNum);
	sb->projectCompletion = stoi(sb->pPODbuf->projectCompletion);
	sb->teamSize = stoi(sb->pPODbuf->teamSize);
	sb->teamDuty = sb->pPODbuf->teamDuty;
}