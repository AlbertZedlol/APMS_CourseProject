/******************************************************************************************************************
Filename		DepartmentAbstractClass.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of class Team and its derived classes.
				Classes included: Team, Department,HRoffice.
******************************************************************************************************************/
#pragma once
//header dependencies and pre-declares of classes.
#include "A-GeneralHeader.h"
#include "AlbertSTL_List.h"

#include "PersonManager.h"
class TeamLeader;

#include "PersonProfessional.h" 
#include "FileManager.h"
class FileManager;
class SortByItemCondition;
class FindByItemCondition;
class HRoffice;
class Technician;
class FinancialAnalyst;

const enum teamType { fin = 1, tech = 2 };
//Base class of all teams, departments and companies.
class TeamBase
{
public:
	friend HRoffice;
	friend TeamLeader;
	friend FileManager;
protected:
	int teamID;
	TeamBase(int i = 0) :teamID(i) {};
	virtual void showTeam() = 0;				//Display team info and all team mates 
	virtual void showMembersBrief() = 0;		//Only shows key indexes for sorting.
	virtual void sort(const string& item) = 0;  //sort teammates by a given item(in the form of a directive)
	virtual Staff* changeMemberInfo(int id, const string& item, double delta) = 0;//find a member by id and change info(item) by delta, return its base class address.
	virtual void dismiss(int id) = 0;			//dismiss a member with given id.
	virtual void changeDep(int id, int _nextDepNum) = 0; //Change the team of a member(whose id is _id) to another department whose id is _nexeDepNum
	virtual TeamLeader*& leader() = 0;			//Get team leader of this team.
	virtual void getLeaderId(int _id) = 0;		//Get team leader id of given team whose id is _id
	virtual bool validSubId(unsigned long testingId) = 0;//test whether the owner of testingId belongs to this team.
	virtual void loadT(fstream&f,FileManager&mg) = 0;//Load team info from hierarchical file f and personal file manager mg.
	virtual void saveT(fstream&) = 0;			//save hierarchical info to hieararchical file .
};

//Teamplate class team for all member types.Para: temmate person type.
template<class T>
class Team :public TeamBase
{
protected:
	List<T> myTeam;								//a list to store all teammates.
	TeamLeader* myLeader;						//points to leader of this team.
	friend FileManager;
public:
	int teamType;								//means person type of the teammates.  fin==1,tech==2
	std::string teamName;						//
	int leaderID;									
	List<int> teamMateIds;						//integer list that stores teammates' ids.	
	int id() { return this->teamID; };			//get team id.
	virtual TeamLeader*& leader();				//get team leader's address	WITH WRITE PERMISSION 
	virtual void getLeaderId(int _id);			//get leader's id.
	friend HRoffice;
public:
	Team(std::string name = "null", int i = 1110, int teamtp = 0, int lid = 0) :TeamBase(i), teamName(name), teamType(teamtp), leaderID(lid) { myLeader = nullptr; };
	virtual void showTeam();
	virtual void showMembersBrief();
	void sort(const string& item);
	virtual Staff* changeMemberInfo(int id, const string& item, double delta);
	Team<T>& recruit(const T& _newMember);		//Recruit new members to this team.
	void dismiss(int id);
	void changeDep(int id, int _nextDepNum);
	bool validSubId(unsigned long testingId);
	virtual void loadT(fstream&f,FileManager&mg);
	virtual void saveT(fstream&);
};

template<class T>
void Team<T>::showMembersBrief()
{
	cout << "\n\n[Team Profile] " << "\nTeam Name: " << setw(10) << this->teamName << "\tTeam Leader: " << this->myLeader->getName(this->teamID) << endl;
	Node<T>* p = myTeam.first();
	for (int i = 0; i < myTeam.size(); i++)
	{
		cout << "\n\t";
		p->_node_data.showSubProfileBrief();
		p = p->_successor;
	}
}

template <class T>
bool Team<T>::validSubId(unsigned long testingId)
{
	return(teamMateIds.has(testingId));
}

template <class T>
void Team<T>::getLeaderId(int _id)
{
	this->leaderID = _id;
}

template<class T>
void Team<T>::saveT(fstream& f)
{
	//Save team size.
	int teamSize = this->myTeam.size();
	f.write((char*)&(teamSize), sizeof(int));

	//Save team type.
	f.write((char*)&(this->teamType), sizeof(this->teamType));

	//Save team name.
	char _charBuf[41] = { "" };
	for (int i = 0; i < 40; i++)
		_charBuf[i] = ' ';
	strncpy_s(_charBuf, teamName.c_str(), 40);
	_charBuf[40] = '\0';
	f.write(_charBuf, sizeof(_charBuf));			//Only store 40 characters.

	//Save team ID
	f.write((char*)&(this->teamID), sizeof(int));

	//Save leader ID.
	f.write((char*)&this->leaderID, sizeof(this->leaderID));

	//Save members' IDs.
	Node<int>* p = this->teamMateIds.first();
	for (int i = 0; i < teamMateIds.size(); i++, p = p->_successor)
	{
		f.write((char*)&(p->_node_data), sizeof(int));
	}
}

class HRoffice
{
public:
	List<Team<Technician>>		  techTeams;	//Buffers.
	List<Team<FinancialAnalyst>>  finTeams;
protected:
	string depFileName;
	fstream depFile;
	fstream* pDepFile;
	bool hasLoaded;
	const enum depFileIOMode { loadDep = 0, saveDep = 1 };

	int teamNum;								//	[0]		[1]		[2]   .....................[Num-1]    
	List<int> teamIds;							//	Id		Id		Id			Id				Id
	List<int> teamTypes;						//	Type	Type	Type		Type			Type			
	List<string> teamNames;						//	TName	TName	TName		TName			TName	
	List<string> leaderNames;					//  LName	LName	LName		LName			LName	

	template<class T>							//Record basic info of teams loaded to form this ^ table above.
	void record(T _newTeam);
	void loadT(fstream& f, FileManager& mg);    //Load existing team/department info from file. 
	void save(fstream&);						//Save existing team/department info to   file. 

	HRoffice& addTeam(Team<FinancialAnalyst> _newTeam);
	HRoffice& addTeam(Team<Technician> _newTeam);	
protected:
	bool validTeamName(const string& testingTeamName);
	bool validLeaderName(const string& testingLeaderNam);
	/*Check whether this sub belongs to this team.*/
	bool validSubId(unsigned long teamId, unsigned long testingId);
	/*Check whether subordinateName's superiority is in the hierarchy and return whther the user want's to exit.*/
	bool integratedLeaderVerifier(string& _leaderName, const string& subordinateName, bool& hasVerified, const string& DIYDescription = "");
	/*check whether this sub is a sub of this leader.*/
	bool integratedSubIDVerifier(string& _leaderName, unsigned long& testingID);
	/*Check whether this dep name/team name is recorded in HR office table.*/
	bool integratedDepNameVerifier(string& _DepName);
	/*check whether person type is one of the four leader types.*/
	bool isLeader(int personType);
	/*clear all hierarchical info recorded in table.*/
	void clearAll();
public:
	friend TeamLeader;
	HRoffice(int i);

	void changeDataBase(FileManager&);

	void depFileIO(int mode,FileManager&,bool isExiting=false);		/*load /save HRoffice hierarchical table from/to depFile. mode==0, load; mode==1, save.*/

	template <class T,class Team>
	void loadInLeader(T*&, unsigned long, FileManager&, Team*);

	void advancedService(FileManager&);			/*advancedService with user to give advanced human management service.We need a FileManager to find and store new leader into buff so there's the para.*/
	
	template<class T>
	void appoint(T&, int teamNum);				//Appoint somebody as a team/department leader.	

	template<class T>
	HRoffice& recruit(T _newCommer, int depId);	//Recruit a person to a specific team/department.

	template<class T, class Old>				//Change a staff's department.
	void changeDep(Node<T>& person, Team<Old> _oldDep, int _nextDepNum);

public:
	TeamLeader* Leader(const string& leaderName);/*leaderNam-->leader's address. Return nullptr if not found.*/
	int team(const string& _teamName);			/*team name -->team id .Return -1 if not found.*/
	TeamBase* teamAddrB(int id);				/*team id   -->team's BASE CLASS address. Return nullptr if not found.*/
	Team<FinancialAnalyst>* finAddr(int id);	/*team id	-->fin team's addr|| tech team's addr.*/
	Team<Technician>* techAddr(int id);
	template <class T>							//Convert an anyType person to Technician.(WILL CLEAR CHARATERISTIC DATA).
	Technician toTech(const T& bd);
	template <class T>							//Convert an anyType person to FinancialAnalyst.(WILL CLEAR CHARATERISTIC DATA).
	FinancialAnalyst toFin(const T& bd);
};
extern HRoffice office;

/// WARNING: DO NOT CHANGE THE SEQUENCE OF TEMPLATE CLASS/FUNCTION' DECLARATIONS.
/// YOU MUST DECLARE THE BODY OF CLASS HR OFFICE BEFORE DECLARING Team<T>::loadT
template<class T>
void Team<T>::loadT(fstream& f, FileManager& mg)
{
	myTeam.clear();

	//Load team size and make buffer.
	int _teamSizeBuf = 0;
	f.read((char*)&_teamSizeBuf, sizeof(int));
	Node<T> _teamMateBuf;
	for (int i = 0; i < _teamSizeBuf; i++)
	{
		myTeam.append(_teamMateBuf._node_data);
	}

	//Load team type.
	f.read((char*)&(this->teamType), sizeof(this->teamType));

	//Load team name.
	char _charBuf[41] = { "" };
	for (int i = 0; i < 40; i++)
		_charBuf[i] = ' ';
	f.read(_charBuf, sizeof(_charBuf));			//Only store 40 characters.
	_charBuf[40] = '\0';
	this->teamName = _charBuf;

	//Load team id.
	f.read((char*)&(this->teamID), sizeof(this->teamID));

	//Load leader id.
	f.read((char*)&this->leaderID, sizeof(this->leaderID));

	//Load teamMateIds.
	int _idBUf = 0;
	Node<int>* p = this->teamMateIds.first();
	for (int i = 0; i < _teamSizeBuf; i++)
	{
		f.read((char*)&(_idBUf), sizeof(int));

		teamMateIds.append(_idBUf);
	}

	//Load team leader's data and establish leadership relation.
	(&office)->loadInLeader(this->leader(), this->leaderID, mg,this);
	
	//Load all teammates' data.
	Node<T>* q = this->myTeam.first();
	p = this->teamMateIds.first();
	for (; q != myTeam.last()->_successor; q = q->_successor, p = p->_successor)
	{
		q->_node_data.loadFromFile(to_string(p->_node_data), mg);
	}
}

template<class T>
HRoffice& HRoffice::recruit(T _newCommer, int depId)
{
	switch (_newCommer.personType)
	{
	case 1:
		this->techAddr(depId)->recruit(office.toTech(_newCommer));
		break;
	case 2:
		this->finAddr(depId)->recruit(office.toFin(_newCommer));
		break;
	default:
		cerr << "\n\tSorry,\"" << _newCommer.personType << " \" is not a valid person type. " << endl;
		exit(-5);
		break;
	}
	return (*this);
}

template<class T>
void HRoffice::record(T _newTeam)
{
	this->teamIds.append(_newTeam.teamID);
	this->teamNames.append(_newTeam.teamName);
	this->teamTypes.append(_newTeam.teamType);
	this->leaderNames.append(_newTeam.myLeader->name);
	this->teamNum++;
}

template<class T, class Old>
void HRoffice::changeDep(Node<T>& person, Team<Old> _oldDep, int _nextDepNum)
{
	for (int i = 0; i < office.teamNum; i++)
	{
		if (_nextDepNum == office.teamIds[i])
		{
			switch (office.teamTypes[i])
			{
			case teamType::fin:
				office.techAddr(_nextDepNum)->recruit(office.toTech(person._node_data));
				break;
			case teamType::tech:
				office.finAddr(_nextDepNum)->recruit(office.toFin(person._node_data));
				break;
			default:
				cerr << "\nSorry, \" " << _nextDepNum << "\" is an invalid department number!\n";
				exit(-3);
				break;
			}
		}
	}
	_oldDep.dismiss(person._node_data.id);
}


//convert anyone to a new technician. Para: person to be converted .Ret: conversion result
template <class T>
Technician HRoffice::toTech(const T& bd)
{
	Technician tech;
	tech.name = bd.name;
	tech.id = bd.id;
	tech.baseSalary = bd.baseSalary;
	tech.KPI = bd.KPI;
	tech.rank = bd.rank;
	//tech.patent = bd.patent;			/*This is not safe when you convert a financial analyst to a technician.*/
	return tech;
}

//convert anyone to a new financial analyst. Para: person to be converted .Ret: conversion result
template <class T>
FinancialAnalyst HRoffice::toFin(const T& bd)
{
	FinancialAnalyst fa;
	fa.name = bd.name;
	fa.id = bd.id;
	fa.baseSalary = bd.baseSalary;
	fa.KPI = bd.KPI;
	fa.rank = bd.rank;
	//fa.FS = bd.FS;						/*This is not safe when you convert a technician to a financial analyst.*/
	return fa;
}

template<class T>
TeamLeader*& Team<T>::leader()
{
	return myLeader;
}

template<class T>
void Team<T>::showTeam()
{
	cout << "\n*************************************************************************************\n";
	cout<<"*************************************************************************************\n[[Team Profile]]";
	cout << setw(15) << "\nTeam Name  :" << setw(20) << setiosflags(ios::left) << this->teamName << setw(15) << "Team ID:     " << setiosflags(ios::left) << setw(10) << this->teamID;
	cout << setw(15) << "\nTeam Leader:" << setw(20) <<setiosflags(ios::left)<< this->myLeader->getName(this->teamID) << setw(15) << "Leader's ID: " << setw(15) << setiosflags(ios::left) << this->leaderID<< endl;
	cout << "\nTeam Members' IDs: ";
	Node<T>* p = this->myTeam.first();
	Node<int>* q = this->teamMateIds.first();
	for (; p != this->myTeam.last()->_successor && q != this->teamMateIds.last()->_successor; p = p->_successor, q = q->_successor)
	{
		cout << (q->_node_data) << "   ";
	}
	cout << "\nDo you want to display detailed information of this team? Press 1 for yes and 0 for no.\n>>>" << endl;
	int choice = -1;
	cin >> choice;
	p = nullptr;
	switch (choice)
	{
	case 1 :																						
		cout << "\n[[Leader's information]]:------------------------------------------------------------\n";
		this->myLeader->showPersonalProfile();
		
		cout << "\n[[Members' information]]:------------------------------------------------------------";
		p = myTeam.first();
		for (int i = 0; i < myTeam.size(); i++)
		{
			cout << "\n\t";
			p->_node_data.showPersonalProfile();
			p = p->_successor;
		}
		break;
	default:
		return;
		break;
	}
}

template<class T>
void Team<T>::sort(const string& item)
{
	SortByItemCondition cd;
	myTeam.sortBy(cd, item);

	myTeam.reverse();
}

template<class T>
Staff* Team<T>::changeMemberInfo(int id, const string& item, double delta)
{
	FindByItemCondition eqCon;
	Node<T>* pthatPerson = myTeam.searchBy(eqCon, id, "Id");

	if (pthatPerson == nullptr)
	{
		cerr << "\nERROR: pthatPerson Not found!!!!\n";
	}
	else if (item == "Rank")
	{
		(*pthatPerson)._node_data.rank += (int)delta;
		if ((*pthatPerson)._node_data.rank > 9)
			(*pthatPerson)._node_data.rank = 9;
		else if ((*pthatPerson)._node_data.rank < 0)
			(*pthatPerson)._node_data.rank = 0;
	}
	else if (item == "Salary")
	{
		(*pthatPerson)._node_data.baseSalary +=(long long)delta;
		if ((*pthatPerson)._node_data.baseSalary < 0)
			(*pthatPerson)._node_data.baseSalary = 0;
	}
	else
	{
		cerr << "<ERROR>Item mismatch in changeMemberInfo!!" << endl;
	}

	return(Staff*)pthatPerson;
}

template<class T>
Team<T>& Team<T>::recruit(const T& _newMember)
{
	if (this == nullptr)
	{
		cerr << "\n\tSorry, there's no such a team!" << endl;
		exit(-6);
	}
	myTeam.append(_newMember);
	teamMateIds.append(_newMember.id);
	return *this;
}

template<class T>
void Team<T>::dismiss(int id)
{
	FindByItemCondition cd;
	Node<T>* pthatGuy = this->myTeam.searchBy(cd, id, "Id");
	if (this->myTeam.valid(pthatGuy) == false)
	{
		cout << "\n\t<NOTION>Sorry, we can't find this person.\n";
		return;
	}
	this->myTeam.pop(pthatGuy);
}

template<class T>
void Team<T>::changeDep(int id, int _nextDepNum)
{
	/*find that guy that wanted to change dep.*/
	FindByItemCondition cd;
	Node<T>* pthatGuyNode = this->myTeam.searchBy(cd, id, "Id");
	if (this->myTeam.valid(pthatGuyNode) == false)
	{
		cout << "\n\t<NOTION>Sorry, we can't find this person.\n";
		return;
	}
	/*Submit this issue to HR office.*/
	office.changeDep(*pthatGuyNode, *this, _nextDepNum);
}

template<class T>
void HRoffice::appoint(T& _newLeader, int teamNum)
{
	for (int i = 0; i < this->teamNum; ++i)
	{
		if (teamNum == this->teamIds[i])
		{
			_newLeader.myTeam = this->teamAddrB(teamNum);

			if (_newLeader.myTeam != nullptr)
			{
				_newLeader.myTeam->leader() = new TeamLeader[1];
				*(_newLeader.myTeam->leader()) = _newLeader;
				_newLeader.myTeam->getLeaderId(_newLeader.id);
				return;
			}
			else
			{
				cerr << "\nSorry, there is such a team number: \" " << teamNum << "\" But it belongs to no valid team.\n";
				exit(-4);
			}
		}
	}
	cerr << "\nSorry, \" " << teamNum << "\" is an invalid department number!\n";
	exit(-3);
}

template <class T, class Team>
void HRoffice::loadInLeader(T*&pL, unsigned long _id, FileManager& mg, Team* pMyteam)
{
	pL = new T[1];
	pL->loadFromFile(to_string(_id), mg);
	pL->myTeam = pMyteam;
}

