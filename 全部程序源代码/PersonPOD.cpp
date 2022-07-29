/****************************************************************************************************************************
Filename		PersonPOD.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Implementation of embeded member functions to convert class obj to and from PODs.
*****************************************************************************************************************************/

#include "PersonPOD.h"
#include "PersonManager.h"
#include "PersonProfessional.h"
TypeConversionMachine tcm(1);

/// Convert T to string.
/// The reason we don't use template is that std::to_string can't be added to all T types.
void TypeConversionMachine::toCharArray(const int& dataSource,char* _destination)
{
	if (_destination == NULL)
		return;
	string conversionBuffer = "";
	conversionBuffer = std::to_string(dataSource);
	strncpy(_destination, conversionBuffer.c_str(), strlen(_destination) - 1);
}
void TypeConversionMachine::toCharArray(const double& dataSource,char* _destination)
{
	if (_destination == NULL)
		return;
	string conversionBuffer = "";
	conversionBuffer = std::to_string(dataSource);
	strncpy(_destination, conversionBuffer.c_str(), strlen(_destination) - 1);
}
void TypeConversionMachine::toCharArray(const unsigned long& dataSource, char* _destination)
{
	if (_destination == NULL)
		return;
	string conversionBuffer = "";
	conversionBuffer = std::to_string(dataSource);
	strncpy(_destination, conversionBuffer.c_str(), strlen(_destination) - 1);
}
void TypeConversionMachine::toCharArray(const unsigned int& dataSource, char* _destination)
{
	if (_destination == NULL)
		return;
	string conversionBuffer = "";
	conversionBuffer = std::to_string(dataSource);
	strncpy(_destination, conversionBuffer.c_str(), strlen(_destination) - 1);
}


/// Convert  classObj   to    POD
/// 
void Technician::toPOD(bool isThisClass)
{
		
	this->pPODbuf = new PODTechnician[1];

	tcm.toPODbasic(this); 

	tcm.toCharArray(this->customerApproval, this->pPODbuf->customerApproval);
	tcm.toCharArray(this->patent,this->pPODbuf->patent);
}
void FinancialAnalyst::toPOD(bool isThisClass)
{
	this->pPODbuf = new PODFA[1];
	tcm.toPODbasic(this); 
	tcm.toCharArray(this->customerApproval, this->pPODbuf->customerApproval);
	tcm.toCharArray(this->FS, this->pPODbuf->FS); 
}

void TeamLeader::toPOD(bool isThisClass)
{
	this->pPODbuf = new PODTeamLeader[1];

	tcm.toPODForManagers(this);

	tcm.toCharArray(this->myTeamNumber, this->pPODbuf->myTeamNumber);
}

void DepartmentLeader::toPOD(bool isThisClass)
{
	this->pPODbuf = new PODDepLeader[1];

	tcm.toPODForManagers(this);

	tcm.toCharArray(this->departmentKPI, this->pPODbuf->departmentKPI);
	strncpy_s(this->pPODbuf->depDuty, this->depDuty.c_str(), strlen(this->pPODbuf->depDuty) - 1);

	tcm.toCharArray(this->myTeamNumber, this->pPODbuf->myTeamNumber);
}

void Executive::toPOD(bool isThisClass) 
{
	this->pPODbuf = new PODExecutive[1];

	tcm.toPODForManagers(this);

	for (int i = 0; i < maxDepInCharge; i++)
	{
		this->pPODbuf->depInChargeNumber[i] = this->depInChargeNumber[i];
	}
	tcm.toCharArray(this->stockShare, this->pPODbuf->stockShare);
}


void ChiefExecutive::toPOD(bool isThisClass)
{
	this->pPODbuf = new PODCEO[1];


	tcm.toPODForManagers(this);


	for (int i = 0; i < maxDepInCharge; i++)
	{
		this->pPODbuf->depInChargeNumber[i] = this->depInChargeNumber[i];
	}
	tcm.toCharArray(this->stockShare, this->pPODbuf->stockShare);

	for (int i = 0; i < maxExecutiveNum; i++)
	{
		this->pPODbuf->executiveAtCommandNum[i]=this->executiveAtCommandNumber[i];
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
}



/// Data type conversion:  POD¡úclassObj
void Technician::fromPOD()
{
	tcm.fromPODbasic(this);

	this->customerApproval = stoi(this->pPODbuf->customerApproval);
	this->patent = stoi(this->pPODbuf->patent);
}

void FinancialAnalyst::fromPOD()
{
	tcm.fromPODbasic(this);

	this->customerApproval = stoi(this->pPODbuf->customerApproval);
	this->FS = stoi(this->pPODbuf->FS);
}

void TeamLeader::fromPOD()
{
	tcm.fromPODForManagers(this);

	this->myTeamNumber = stoi(this->pPODbuf->myTeamNumber);
}

void DepartmentLeader::fromPOD()
{
	tcm.fromPODForManagers(this);

	this->departmentKPI =stod(this->pPODbuf->departmentKPI);
	this->depDuty = this->pPODbuf->depDuty;

	this->myDepNumber = stoi(this->pPODbuf->myDepNumber);

}

void Executive::fromPOD()
{
	tcm.fromPODForManagers(this);

	for (int i = 0; i < maxDepInCharge; i++)
	{
		this->depInChargeNumber[i]=this->pPODbuf->depInChargeNumber[i];
	}

	this->stockShare =stoi(this->pPODbuf->stockShare);
}

void ChiefExecutive::fromPOD()
{
	tcm.fromPODForManagers(this);

	for (int i = 0; i < maxDepInCharge; i++)
	{
		this->depInChargeNumber[i] = this->pPODbuf->depInChargeNumber[i];
	}

	this->stockShare = stoi(this->pPODbuf->stockShare);

	for (int i = 0; i < maxExecutiveNum; i++)
	{
		this->executiveAtCommandNumber[i]=this->pPODbuf->executiveAtCommandNum[i];
	}
}


/// Write POD to file

void TeamLeader::write2file(fstream& mg)
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void DepartmentLeader::write2file(fstream& mg)
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void Executive::write2file(fstream& mg) 
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
void ChiefExecutive::write2file(fstream& mg)
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void Technician::write2file(fstream& mg)
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void FinancialAnalyst::write2file(fstream& mg)
{
	mg.write((char*)this->pPODbuf, sizeof(*pPODbuf));
}


/// Read POD from file.
void TeamLeader::readFromFile(fstream& mg)
{
	/* u have to initilaize buffer before writing into it.Or a seiour error shall occur.*/
	this->pPODbuf = new PODTeamLeader[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void DepartmentLeader::readFromFile(fstream& mg)
{
	/* u have to initilaize buffer before writing into it.Or a seiour error shall occur.*/
	this->pPODbuf = new PODDepLeader[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void Executive::readFromFile(fstream& mg)
{
	/* u have to initilaize buffer before writing into it.Or a seiour error shall occur.*/
	this->pPODbuf = new PODExecutive[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void ChiefExecutive::readFromFile(fstream& mg)
{
	/* u have to initilaize buffer before writing into it.Or a seiour error shall occur.*/
	this->pPODbuf = new PODCEO[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void Technician::readFromFile(fstream& mg)
{
	this->pPODbuf = new PODTechnician[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}
void FinancialAnalyst::readFromFile(fstream& mg)
{
	this->pPODbuf = new PODFA[1];
	mg.read((char*)this->pPODbuf, sizeof(*pPODbuf));
}