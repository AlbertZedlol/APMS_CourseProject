/****************************************************************************************************************************
Filename		PersonProfessional.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Implementation of the Professional family. Classes included:
*****************************************************************************************************************************/
#include "PersonProfessional.h"

#include "FileManager.h"
class FileManger;













/// Constructors.
Technician::Technician(int i)
{
	this->personType = dictionary.PersonTypeTechnician;
	this->customerApproval = 0;
	this->patent = 0;
	this->pPODbuf = NULL;
}
FinancialAnalyst::FinancialAnalyst(int i)
{
	this->personType = dictionary.PersonTypeFinancialAnalyst;
	this->customerApproval = 0;
	this->FS = 0;
	this->pPODbuf = NULL;
}

Technician::~Technician()
{
	;
}
FinancialAnalyst::~FinancialAnalyst()
{
	;
}


///showPersonalInfo:
void Professional::showPersonalProfile(bool is_thisClass)
{
	Staff::showPersonalProfile(false);

	cout << setw(15) << setiosflags(ios::left) << "\tCustomer Approval:" << setw(20) << setiosflags(ios::left) <<this->customerApproval<< endl;
}
void Technician::showPersonalProfile(bool is_thisClass)
{
	Professional::showPersonalProfile(false);
	cout << setw(15) << setiosflags(ios::left) << "\tPatents published:" << setw(20) << setiosflags(ios::left) << this->patent << endl;
}
void FinancialAnalyst::showPersonalProfile(bool is_thisClass)
{
	Professional::showPersonalProfile(false);
	cout << setw(15) << setiosflags(ios::left) << "\tFinancial reports finished:" << setw(20) << setiosflags(ios::left) << this->FS << endl;
}


///inputPersonalInfo
void Professional::inputPersonalInfo(bool is_thisClass)
{
	Staff::inputPersonalInfo(false);
	cout << "\tCustomer Approval:";
	cin >> this->customerApproval;
	cin.ignore();
}
void Technician::inputPersonalInfo(bool is_thisClass)
{
	Professional::inputPersonalInfo(false);
	cout << "\tPatents:";
	cin >> this->patent;
	cin.ignore();
}
void FinancialAnalyst::inputPersonalInfo(bool is_thisClass)
{
	Professional::inputPersonalInfo(false);
	cout << "\tFinancial reports finished:";
	cin >> this->FS;
	cin.ignore();
}



void Technician::loadFromFile(const string& pattern, FileManager& mg)
{
	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou erro in teammates type recording. Mismatch found when loading a " << this->personType << " pattern" << pattern << "from file." << endl;
		return;
	}
	*this = *(Technician*)&bufTech;


}

void FinancialAnalyst::loadFromFile(const string& pattern, FileManager& mg)
{

	int personTypeForLoad = mg.findAndWriteTo(pattern);
	if (personTypeForLoad != this->personType)
	{
		cerr << "Seriou error in teammates type recording. Mismatch found when loading a " << this->personType << " pattern" << pattern << "from file." << endl;
		return;
	}
	*this = *(FinancialAnalyst*)&bufFa;
}

void Technician::showSubProfileBrief()
{
	std::cout << "\n\t" << "Technician:";
	cout << "\n\tName: " << setiosflags(ios::left) << setw(15) << name << "\tID: " << setw(12) << setiosflags(ios::left) << this->id << "\tpatent == " << setw(3) << setiosflags(ios::left) << patent << "\tKPI: " << setiosflags(ios::fixed) << setprecision(3) << this->KPI <<
		setw(2) << "\tRank: " << this->rank << "\tBase salary: " << this->baseSalary << endl;
}

void FinancialAnalyst::showSubProfileBrief()
{
	std::cout << "\n\t" << "FinancialAnalyst:";
	cout << "\n\tName: " << setiosflags(ios::left) << setw(15) << name << "\tID: " << setw(12) << setiosflags(ios::left) << this->id << "\tFinancial Statement:  " << setw(3) << setiosflags(ios::left) << FS << "\tKPI: " << setiosflags(ios::fixed) << setprecision(3) << this->KPI <<
		setw(2) << "\tRank: " << this->rank << "\tBase salary: " << this->baseSalary << endl;
}