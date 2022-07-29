/***********************************************************************************
Filename		PersonProfessional.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declaration of class Professional and its derived classes.
***********************************************************************************/
#pragma once

#include "DepartmentAbstractClass.h"
#include "PersonAbstractClass.h"
#include <string>

class Staff;
class PODTechnician;
class PODFA;

class TypeConversionMachine;

template <class T>
class Team;

class FinancialAnalyst;
class Technician;
class HRoffice;

/*When you want to sort by a certain item...*/
class SortByItemCondition
{
public:
	template<class T>
	bool operator() (const T& curItem, const T& targetIwant, const string& description)
	{
		if (description == "KPI")
			return curItem.KPI <= targetIwant.KPI;
		else if (description == "Rank")
			return (curItem.rank <= targetIwant.rank);
		else if (description == "Salary")
			return (curItem.baseSalary <= targetIwant.baseSalary);
		else
		{
			cerr << "\n\tWrong description at SortByItemCondition" << endl;
			exit(-2);
		}
	};
};

/*When you want to find by a certain item...*/
class FindByItemCondition
{
public:
	template<class T, class D>
	bool operator() (const T& curItem, D  target, const string& description)
	{
		if (description == "KPI")
			return (curItem.KPI == target);
		else if (description == "Id")
			return (curItem.id== target);
		else if (description == "Rank")
			return(curItem.rank == target);
		else if (description == "Salary")
			return(curItem.rank == target);
		else
		{
			cerr << "\n\tWrong description at FindByItemCondition" << endl;
			exit(-2);
		}
	};
};


class Professional :public Staff
{
public:
	friend HRoffice;
	friend SortByItemCondition;
	friend FindByItemCondition;
	friend HRoffice;
	friend Team<Technician>;
	friend Team<FinancialAnalyst>;
	friend TypeConversionMachine;
protected:
	Professional(int cust=0) :customerApproval(cust) {}; 
#ifdef DEBUG_CONSTRUCTOR
	Professional(string nm, double kpi = 0, int r = 0, int _id = 0, double bs = 0, int pT = 0):Staff(nm,kpi,r,_id,bs,pT) {};
#endif // DEBUG_CONSTRUCTOR
	int customerApproval;
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
};
 

class Technician :public Professional
{
protected:
	int patent;
public:
	Technician(int i = 0);
	friend TypeConversionMachine;
	friend HRoffice;
	friend Team<Technician>;
public:
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	virtual ~Technician();
	void showSubProfileBrief();
protected:

	void loadFromFile(const string&, FileManager&);

	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();
private:
	PODTechnician* pPODbuf;
};


class FinancialAnalyst :public Professional
{
protected:
	int FS;															//Financial Statement.
public:
	FinancialAnalyst(int i = 0);
	friend TypeConversionMachine;
	friend HRoffice;
	friend Team<FinancialAnalyst>;
public:
	virtual void showPersonalProfile(bool is_thisClass = true);
	virtual void inputPersonalInfo(bool is_thisClass = true);
	void showSubProfileBrief();
	virtual ~FinancialAnalyst();
protected:

	void loadFromFile(const string&, FileManager&);

	virtual void toPOD(bool isThisClass = true);
	virtual void write2file(fstream& mg);
	virtual void readFromFile(fstream& mg);
	virtual void fromPOD();
private:
	PODFA* pPODbuf;
};