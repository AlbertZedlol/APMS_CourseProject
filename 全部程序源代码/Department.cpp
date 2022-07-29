#include "DepartmentAbstractClass.h"
#include "Department.h"
#include <fstream>
using namespace std;

HRoffice office(0);

void HRoffice::depFileIO(int mode, FileManager& mg, bool willExit)
{
	pDepFile->open(this->depFileName, ios::in | ios::out | ios::_Nocreate | ios::ate | ios::binary);
	pDepFile->seekg(ios::beg);
	pDepFile->seekp(ios::beg);
	int answer = 1;
	switch (mode)
	{
	case loadDep:
		this->loadT(depFile,mg);
		this->hasLoaded = true;
		break;
	case saveDep:
		if (willExit)
		{
			system("cls");
			cout << "\n\n\n\n\n\tDo you want to save previous changes to company's management structure? Press 1 for yes and 0 for no." << endl;
			cin >> answer;
			if (answer)
			{
				this->save(depFile);
			}
		}
		else
			this->save(depFile);
		break;
	default:
		cerr << "Sorry, \""<< mode<<"\" is a wrong depFileIOMode!" << endl;
		exit(-8);
		break;
	}
}

HRoffice::HRoffice(int i)
{
	this->teamNum = 0;
	this->pDepFile= &depFile;
	this->depFileName = "D:\\AlbertPersonelManagmentSystem\\hierarchy";
	this->hasLoaded = false;
}

void HRoffice::changeDataBase(FileManager& mg)
{
	this->depFileName = mg.getDatabaseName();

	if (_access(depFileName.c_str(), 06) != 0)
	{
		this->depFile.open(depFileName, ios::out | ios::app | ios::binary);
	}

	if (_access(depFileName.c_str(), 06) != 0)
	{
		cout << "can't open dep file!" << endl;
		abort();
	}

	this->depFile.close();

	this->depFile.open(depFileName, ios::in | ios::out | ios::ate | ios::binary | ios::_Nocreate);
	depFile.seekg(ios::beg);
	depFile.seekp(ios::beg);
}


HRoffice& HRoffice::addTeam(Team<FinancialAnalyst> _newTeam)
{
	finTeams.append(_newTeam);
	record(_newTeam);
	return (*this);
}

HRoffice& HRoffice::addTeam(Team<Technician> _newTeam)
{
	techTeams.append(_newTeam);
	record(_newTeam);
	return (*this);
}

int HRoffice::team(const string& _teamName)
{
	Node<std::string>* p = this->teamNames.first();
	for (int i = 0; p != this->teamNames.last()->_successor; i++, p = p->_successor)
	{
		if (_teamName == p->_node_data)
			return this->teamIds[i];
	}
	return -1;
}

TeamLeader* HRoffice::Leader(const string& leaderName)
{
	if (this->validLeaderName(leaderName) == false)
		return nullptr;
	string _nmBuf = "";
	for (int i = 0; i < this->techTeams.size(); i++)
	{
		_nmBuf = techTeams[i].leader()->name;

		if (leaderName == _nmBuf)
			return (techTeams[i].leader());
	}
	for (int i = 0; i < this->finTeams.size(); i++)
	{
		_nmBuf = finTeams[i].leader()->name;
		if (leaderName == _nmBuf)
			return (finTeams[i].leader());
	}
	
}

TeamBase* HRoffice::teamAddrB(int _id)
{
	TeamBase* pB = nullptr;
	pB = this->finAddr(_id);

	if (pB == nullptr)
		pB = this->techAddr(_id);
	return pB;
}

Team<FinancialAnalyst>* HRoffice::finAddr(int _id)
{
	Node<Team<FinancialAnalyst>>* pf = nullptr;
	for (pf = this->finTeams.first(); pf != this->finTeams.last()->_successor; pf = pf->_successor)
	{
		if (pf->_node_data.id() == _id)
			return &(pf->_node_data);
	}
	return nullptr;
}

Team<Technician>* HRoffice::techAddr(int _id)
{
	Node<Team<Technician>>* pt = nullptr;
	for (pt = this->techTeams.first(); pt != this->techTeams.last()->_successor; pt = pt->_successor)
	{
		if (pt->_node_data.id() == _id)
			return &(pt->_node_data);
	}
	return nullptr;
}

void HRoffice::clearAll()
{
	teamNum = 0;
	techTeams.clear();
	finTeams.clear();
	teamIds.clear();
	teamTypes.clear();
	teamNames.clear();
}

#ifdef DEBUG_FILE_IO
void HRoffice::showBuffer()
{
	cout << "\n\n\nTable:\t Team Number == " << teamNum << endl;
	cout << setw(10) << "ID:" << "\t";
	Node<int>* p = nullptr;
	for (p = teamIds.first(); p != teamIds.last()->_successor; p = p->_successor)
	{
		cout << p->_node_data << "\t";
	}
	cout << endl;
	cout << setw(10) << "teamType:" << "\t";
	for (p = teamTypes.first(); p != teamTypes.last()->_successor; p = p->_successor)
	{
		cout << p->_node_data << "\t";
	}
}
#endif // DEBUG_FIL_IO

void HRoffice::save(fstream& f)
{
	f.seekp(ios::beg);
	//Save team number.
	f.write((char*)&this->teamNum, sizeof(this->teamNum));
	//Save team ids.
	for (Node<int>* p = teamIds.first(); p != teamIds.last()->_successor; p = p->_successor)
	{
		f.write((char*)&p->_node_data, sizeof(p->_node_data));
	}
	//Save team types.
	for (Node<int>* p = teamTypes.first(); p != teamTypes.last()->_successor; p = p->_successor)
	{
		f.write((char*)&p->_node_data, sizeof(p->_node_data));
	}
	//Write techTeams num
	int _sizeBuf = this->techTeams.size();
	f.write((char*)&(_sizeBuf), sizeof(int));
	//Write techTeams.
	for (Node<Team<Technician>>* q = this->techTeams.first(); q != this->techTeams.last()->_successor; q = q->_successor)
	{
		q->_node_data.saveT(f);
	}
	//Write finTeams num
	_sizeBuf = this->finTeams.size();
	f.write((char*)&(_sizeBuf), sizeof(int));
	//Write finteams.
	for (Node<Team<FinancialAnalyst>>* r = this->finTeams.first(); r != this->finTeams.last()->_successor; r = r->_successor)
	{
		r->_node_data.saveT(f);
	}
	//Close file.
	this->depFile.close();
}

void HRoffice::loadT(fstream&f,FileManager&mg)
{
	this->clearAll();
	f.seekg(ios::beg);
	//Load team num table.
	f.read((char*)&this->teamNum, sizeof(this->teamNum));
	//Read team ids.
	int _intBuf = -1;
	for (int i = 0; i < this->teamNum; i++)
	{
		f.read((char*)&_intBuf, sizeof(_intBuf));
		this->teamIds.append(_intBuf);
	}
	//Read team types.
	for (int i = 0; i < this->teamNum; i++)
	{
		f.read((char*)&_intBuf, sizeof(_intBuf));
		this->teamTypes.append(_intBuf);
	}

	//Read techTeam num.
	f.read((char*)&_intBuf, sizeof(_intBuf));
	//Create techTeams buffers.
	Team<Technician> techStuffing;
	for (int i = 0; i < _intBuf; i++)
	{
		this->techTeams.append(techStuffing);
	}
	//Load all techTeams.
	for (Node<Team<Technician>>* p = techTeams.first(); p != techTeams.last()->_successor; p = p->_successor)
	{
		p->_node_data.loadT(f,mg);
	}
	//Load all techTeamNames and leaderNames.
	for (Node<Team<Technician>>* p = techTeams.first(); p != techTeams.last()->_successor; p = p->_successor)
	{
		this->teamNames.append(p->_node_data.teamName);
		this->leaderNames.append(p->_node_data.myLeader->name);
	}

	//Load finTeams num.
	f.read((char*)&_intBuf, sizeof(_intBuf));
	//Create finTeams buffers.
	Team<FinancialAnalyst> finStuffing;
	for (int i = 0; i < _intBuf; i++)
	{
		this->finTeams.append(finStuffing);
	}
	//Load all finTeams.
	for (Node<Team<FinancialAnalyst>>* p = finTeams.first(); p != finTeams.last()->_successor; p = p->_successor)
	{
		p->_node_data.loadT(f,mg);
	}
	//Load all finTeamNames and leaderNames.
	for (Node<Team<FinancialAnalyst>>* p = finTeams.first(); p != finTeams.last()->_successor; p = p->_successor)
	{
		this->teamNames.append(p->_node_data.teamName);
		this->leaderNames.append(p->_node_data.myLeader->name);
	}
	//Close file.
	this->depFile.close();
}
