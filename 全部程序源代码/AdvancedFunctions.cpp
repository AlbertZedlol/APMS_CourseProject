/******************************************************************************************************************
Filename		AdvancedFunctions.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Advanced service for human resource management.
******************************************************************************************************************/
#include "DepartmentAbstractClass.h"
#include <chrono>
#include <thread>
using namespace std;

//Test whether "testingTeamName" stands for a teammate. Return yes/no.
bool HRoffice::validTeamName(const string& testingTeamName)
{
	return(this->teamNames.has(testingTeamName));
}
//Test whether "testingLeaderName" stands for a leader in HRoffice. Return yes/no.
bool HRoffice::validLeaderName(const string& testingLeaderName)
{
	bool answer = this->leaderNames.has(testingLeaderName);
	return(answer);
}

//Test whether "testingId" stands for a teammate that belongs to "teamId¡°'s owner.Return yes/no.
bool HRoffice::validSubId(unsigned long teamId, unsigned long testingId)
{
	TeamBase* pTeam = this->teamAddrB(teamId);

	return(pTeam->validSubId(testingId));
}

/// Let the user input a valid leader name(that exists in database) or quit program.
/// param name="_leaderName": input leader's name.
/// param name="subordinateName": A selectable parameter to form the instruction to tell user to type in a leadername for a given subordinate.
/// param name="hasSpecified": boolean in the advancedService() method to record whether user has specified a team leader previously.
/// param name="DIYDescription"> A selectable parameter to substitute default output information with any text message.
/// returns: whether the user wants to quit this verification.
bool HRoffice::integratedLeaderVerifier(string& _leaderName, const string& subordinateName, bool& hasSpecified, const string& DIYDescription)
{
	if (hasSpecified)
		return false;
	string instruction = "\n\tPlease type in the name of " + subordinateName + "'s superiority \n\t>>>";
	if (DIYDescription.length() != 0)
		instruction = DIYDescription;
	cout << instruction;

	getline(cin, _leaderName);

	while (this->validLeaderName(_leaderName) == false)
	{
		system("cls");
		cout << "\n\tSorry, currently \" " << _leaderName << " \" is not recorded in this database as a leader." << endl;
		cout << "\n\tPlease choose another leader or press 0 to exit.";
		cout << instruction;

		getline(cin, _leaderName);

		if (_leaderName.length() == 1 && stoi(_leaderName) == 0)
			return true;
	}
	hasSpecified = true;
	return false;
}

/// Let the user input a valid ID which belongs to a given leader.
/// param name="_leaderName": input leader's name.
/// param name="tesingID": user-input subordinate's ID.
/// returns: whether the user wants to quit this verification.
bool HRoffice::integratedSubIDVerifier(string& _leaderName, unsigned long& testingID)
{
	cin >> testingID;
	string instruction = "\n\tSorry, this person is not a subordinate of leader \" " + _leaderName + " \".\nAre you looking for one of these persons? \n";

	if (this->validLeaderName(_leaderName) == false)
	{
		cout << "\n\tSorry, \"" << _leaderName << "\" is not recorded in this database as a leader so you can't find his/her subordinates.Please choose leader again :)" << endl;
		return false;
	}

	while (this->validSubId(this->Leader(_leaderName)->myTeam->teamID, testingID) == false)
	{
		system("cls");
		cout << instruction;
		this->Leader(_leaderName)->showSubordinates();
		cout << "\n\tPlease choose a person from above or press 0 to exit.\n\t>>>";
		cin >> testingID;
		if (testingID == 0)
			return true;
	}
	return false;
}

///Let the user input a valid department name is recorded in hierarchical database.
/// param name="_DepName"£ºgiven department name.
/// returns:whether the user wants to quit selection.
bool HRoffice::integratedDepNameVerifier(string& _DepName)
{
	getline(cin, _DepName);
	string instruction = "\n\tSorry, currently  \"" + _DepName + "\" is not recorded in this database as a team/department.";

	while (this->validTeamName(_DepName) == false)
	{
		system("cls");
		cout << instruction;
		cout << "\n\tPlease choose again or press 0 to exit.\n\t>>>";
		getline(cin, _DepName);
		if (_DepName.length() == 1 && stoi(_DepName) == 0)
			return true;
	}
	return false;
}

//Test whether a person type is in management series that can be appointed as a leader.
bool HRoffice::isLeader(int personType)
{
	bool isL = (personType == dictionary.PersonTypeTeamLeader || personType == dictionary.PersonTypeDepartmentLeader || personType == dictionary.PersonTypeExecutive ||
		personType == dictionary.PersonTypeCEO);
	return isL;
}

//Advanced service interface and control
///param name="mg": file manager keeping personal database.
///ret: none.
void HRoffice::advancedService(FileManager& mg)
{
	this->depFileIO(loadDep, mg);

	bool isLeaving = false;//Whether user will leave this service.
	bool hasSpecifiedLeader = false;
	string dismissalPassword = "APMS";//Password to dismiss an employee.
	string _charBuf = "", _leaderName = "";//buffers to hold user input.
	int leaderType = -1;//Person type of leader read in from file.
	const enum options { exitOffice = 0, createTeam, appoinLeader, chooseLeader, sortByIndex, proDemote, changeSalRe, changeDep, dism };
	TeamLeader* t = nullptr;
	TeamBase* tm = nullptr;//Pointers to controll person obj buffers.

	while (isLeaving == false)
	{
		system("cls");
		cout << "\n\tHello, welcome to Human Resource Management Office for Advanced Service.\n\tI'm your guide Alex. \n\tWhat can I do for you ?";
		cout << " Please select from the options below : )\n" << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Create a new team" << "press " << createTeam << endl;
		/*Currently only supports appointing team leaders. because function HRoffice::apppoint only reveices team leaders.*/
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Appoint a leader    " << "press " << appoinLeader << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Choose a leader to view subordinates" << "press " << chooseLeader << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "View subordinates' performance  " << "press " << sortByIndex << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Promote/Demote a staff  " << "press " << proDemote << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Change a staff's salary/reward  " << "press " << changeSalRe << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Change a staff's department  " << "press " << changeDep << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Dismiss a staff  " << "press " << dism << endl;
		cout << "\t" << setw(40) << setiosflags(ios::left) << "Leave this office " << "press " << exitOffice << endl;
		cout << "\n\t" << ">>>";

		int _intBuf = -1;
		int _choice = -1;
		cin >> _choice;

		unsigned long _id = 0;
		Staff* pB = nullptr;

		Team<Technician> _newTechTeam;
		Team<FinancialAnalyst> _newFinTeam;
		int numNewTeammates = 0;
		int _newTeamType = -1;
		string _newTeammateName = "", _newTeamLeader = "", _newTeamName = "";
		unsigned long _newTeamID = 0;
		int findWriteResult = -1;

		switch (_choice)
		{

		case createTeam:
			//select team composition.
			cout << "\n\tPlease select from the list below to specify the posts of your new team.\n";
			for (int i = 6; i < 6 + 2; i++)
				cout << "\n\t" << setw(20) << setiosflags(ios::left) << dictionary.postName[i] << " press " << (i - 5);
			cout << "\n\t>>>";
			cin >> _newTeamType;
			cin.ignore();//#Change

			//Input basic team info.
			cout << "\n\tPlease specify name of this new team's leader. \n\t>>>";
			getline(cin, _newTeamLeader);//#Change
			cout << "\n\tAnd the team name? \n\t>>>";
			getline(cin, _newTeamName);//cin >> _newTeamName;//#Change
			cout << "\n\tAnd the team id?\n\t>>>";
			cin >> _newTeamID;
			cin.ignore();//#Change

			//Verify leader info.
			while (this->isLeader(mg.findAndWriteTo(_newTeamLeader)) == false)
			{
				cout << "\n\tSorry, \"" << _newTeamLeader << "\" is not a manager and thus can't be appointed to be a team leader.\n";
				cout << "\n\tPlease choose again.\n\t>>>";
				getline(cin, _newTeamLeader);//#Change
			}

			//Save newly established fin/tech team to temp buffer and then append to HRoffice hierarchy(using DEEP COPY)
			switch (_newTeamType)
			{
			case 1:
				_newFinTeam.teamID = _newTeamID;
				_newFinTeam.teamName = _newTeamName;
				_newFinTeam.teamType = fin;
				this->finTeams.append(_newFinTeam);
				/*load in leader.*/
				(this->finTeams[finTeams.size() - 1].leader()) = new TeamLeader[1];
				*(this->finTeams[finTeams.size() - 1].leader()) = *(TeamLeader*)&bufTld;
				(this->finTeams[finTeams.size() - 1].leader())->myTeam = &this->finTeams[finTeams.size() - 1];
				this->finTeams[finTeams.size() - 1].leaderID = bufTld.id;//#Change!!!
				this->record(this->finTeams[finTeams.size() - 1]);
				break;
			case 2:
				_newTechTeam.teamID = _newTeamID;
				_newTechTeam.teamName = _newTeamName;
				_newTechTeam.teamType = tech;

				this->techTeams.append(_newTechTeam);
				/*load in leader.*/
				(this->techTeams[techTeams.size() - 1].leader()) = new TeamLeader[1];
				*(this->techTeams[techTeams.size() - 1].leader()) = *(TeamLeader*)&bufTld;
				(this->techTeams[techTeams.size() - 1].leader())->myTeam = &this->techTeams[techTeams.size() - 1];
				this->techTeams[techTeams.size() - 1].leaderID = bufTld.id;
				this->record(this->techTeams[techTeams.size() - 1]);
				break;
			default:
				break;
			}
			//Input newly recruited teammates to team.
			cout << "\n\tHow many people will be recruited? \n\t>>>";
			cin >> numNewTeammates;
			cin.ignore();//#change!
			for (int i = 0; i < numNewTeammates; i = i)
			{
				cout << "\n\tPlease type in the name of the newly recruited subordinate (No. " << i + 1 << ") \n\t>>>";
				getline(cin, _newTeammateName);//cin >> numNewTeammates;#Change!

				//Call the searching engine to find teammate by name.
				findWriteResult = mg.findAndWriteTo(_newTeammateName);

				//Make sure that person type matches team type.
				switch (findWriteResult)
				{
				case -1:
					cout << "\n\tSorry, this person has not been recorded in this database";
					cout << "\n\tPlease return to standard service interface to complete his/her information.\n\t";
					std::this_thread::sleep_for(std::chrono::milliseconds(3000));
					return;
				case 0:
					return;
				//If mathes , recruit.
				case dictionary.PersonTypeFinancialAnalyst:
					if (_newTeamType == 1)
					{
						this->finTeams[finTeams.size() - 1].recruit(bufFa);
						i++;
					}
					else
					{
						cout << "\n\tSorry, this new team mate doesn't match the post requirement of this new team:" << dictionary.postName[5 + 1];
						cout << "\n\tPlease select again.\n\t";
						continue;
					}
					break;

				case dictionary.PersonTypeTechnician:
					if (_newTeamType == 2)
					{
						this->techTeams[techTeams.size() - 1].recruit(bufTech);
						i++;
					}
					else
					{
						cout << "\n\tSorry, this new team mate doesn't match the post requirement of this new team:" << dictionary.postName[5 + 2];
						cout << "\n\tPlease select again.\n\t";
						continue;
					}
					break;
				default:
					cout << "\n\tSorry, this person can not be recruited to a team for now.Please select again.\n";
					continue;
					break;
				}
			}

			break;
		case appoinLeader:
			//Input a valid leader name.
			if (this->integratedLeaderVerifier(_leaderName, "", hasSpecifiedLeader, "\n\tWho do you want to appoint as a leader? Please type in his/her name.\n\t>>>") == true)
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Find leader by name using searching engine and write it to global buffer bufTld.
			leaderType = mg.findAndWriteTo(_leaderName);
			if (leaderType != dictionary.PersonTypeTeamLeader)
			{
				cout << "\n\tSorry, currently we don't support appointing staffs that have higher rank than team leader.Please choose again. " << endl;
				{
					this->depFileIO(saveDep, mg);
					break;
				}
			}
			cout << "\n\tAnd which team will \"" << _leaderName << "\" lead ? Please type the team name.\n\t>>>";
			if (this->integratedDepNameVerifier(_charBuf) == true)
				break;
			//Call appoint method to establish relationship between leader and team(Deep Copy)
			office.appoint(bufTld, office.team(_leaderName));
			cout << "\n\tNow " << _leaderName << " leads team " << _charBuf << " . This team's detailed information are as follows:\n";
			office.Leader(_leaderName)->showSubordinates();
			system("pause");

			////Save immediately after change.
			//this->save(depFile);

			break;

		case chooseLeader:
			//Change the current leader view .
			hasSpecifiedLeader = false;
			if (this->integratedLeaderVerifier(_leaderName, "", hasSpecifiedLeader, "\n\tWhich leader's profile do you want to check? Please type in his/her name.\n\t>>>") == true)
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			cout << "\n\tSuccessul! Now whenever you check subordinates' information, Alex will know that their leader is \"" << _leaderName << "\" .\n";
			cout << "\n\tLeader " << _leaderName << "'s subordinates are as follow.\n";
			office.Leader(_leaderName)->showSubordinates();
			system("pause");
			break;

		case sortByIndex:
			cin.ignore();//#Change
			if (integratedLeaderVerifier(_leaderName, "these staffs", hasSpecifiedLeader) == true)
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Show teammate info.
			cout << "\nHere is the brief information of \"" << _leaderName << "\"'s subordinates." << endl;
			t = office.Leader(_leaderName);
			tm = t->myTeam;
			tm->showMembersBrief();
			//Let user select a VALID key index for sorting. 
			cout << "\n\t Which item do you want to select as the key index? Please select from below.\n\t1. KPI\t2. Rank\t3. Salary\t\n\t>>>";
			cin >> _choice;
			while (_choice != 1 && _choice != 2 && _choice != 3)
			{
				cout << "\nSorry we haven't extended index choice to this item: " << _choice << " ." << endl;
				cout << "\nPlease choose again. You can also press 0 to exit." << endl;
				system("pause");
				system("cls");
				cout << "\n\t Which item do you want to select as the key index? Please select from below.\n\t1. KPI\t2. Rank\t3. Salary\t\n\t>>>";
				cin >> _choice;
			}
			switch (_choice)
			{
			case 0:
				break;
			case 1:
				_charBuf = "KPI";
				break;
			case 2:
				_charBuf = "Rank";
				break;
			case 3:
				_charBuf = "Salary";
				break;
			}
			//Sort team and display.
			cout << "\n<NOTION>*************Here is the information of " << _leaderName << "'s subordinates sorted by " << _charBuf << ": ***********";
			office.Leader(_leaderName)->sortSubBy(_charBuf);
			office.Leader(_leaderName)->myTeam->showMembersBrief();
			cout << "\n\tSuccessful" << endl;
			system("pause");
			break;

		case proDemote:
			cin.ignore();//#Change
			if (integratedLeaderVerifier(_leaderName, "this person", hasSpecifiedLeader) == true)
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Find employee's leader.
			office.Leader(_leaderName)->myTeam->showMembersBrief();
			cout << "\n\tWill this person be promoted or a demoted?\n\tPress 1 for promotion and 2 for demotion.\n\t>>>";
			cin >> _choice;
			while (_choice != 1 && _choice != 2)
			{
				cout << "\n\tSorry, this choice \" " << _choice << " \" indicates neither promotion nor demotion." << endl;
				cout << "\n\tPlease choose again. You can also press 0 to exit." << endl << endl;
				system("pause");
				cin.ignore();
				system("cls");
				cout << "\n\tWill this person be promoted or a demoted?\n\tPress 1 for promotion and 2 for demotion.\n\t>>>";
				cin >> _choice;
			}
			cout << "\n\tLeader " << _leaderName << "'s subordinates are as follow.\n";
			office.Leader(_leaderName)->showSubordinates();
			system("pause");
			//Set post change type.
			cout << "\n\tAnd who will be";
			switch (_choice)
			{
			case 0:

				this->depFileIO(saveDep, mg);
				break;
			case 1:
				cout << " promoted?";
				break;
			case 2:
				cout << " demoted?";
				break;
			}
			cout << "\t Please type in this person's  id.\n\t>>>";
			cin >> _id;
			cout << "\n\tAnd how many grades do you want this person to be";
			switch (_choice)
			{
			case 1:
				cout << " promoted ";
				break;
			case 2:
				cout << " demoted ";
				break;
			}
			//Set post change magnitude.
			cout << "by ?\n\t>>>";
			cin >> _intBuf;
			//Call leader to change his/her subordinates.
			switch (_choice)
			{
			case 1:
				pB = office.Leader(_leaderName)->promote(_id, _intBuf);
				break;
			case 2:
				pB = office.Leader(_leaderName)->demote(_id, _intBuf);
				break;
			}
			//Demonstrate result immediately afterwards.
			cout << "\tInformation of this person after post change is listed below: \n";
			if (pB != nullptr)
				pB->showPersonalProfile(true);
			cout << "\n\tSuccessful." << endl;
			system("pause");
			break;

		case changeSalRe:
			//Find a valid leader and show subordinates.
			cin.ignore();//#Change
			if (integratedLeaderVerifier(_leaderName, "this person", hasSpecifiedLeader))
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			cout << "\n\tLeader " << _leaderName << "'s subordinates are as follow.\n";
			office.Leader(_leaderName)->showSubordinates();
			system("pause");
			//Select a valid subordinate.
			cout << "\n\tAnd who will get a salary change ? Please type in this person's id.\n\t>>>";
			if (integratedSubIDVerifier(_leaderName, _id))
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Set salary change magnitude
			cout << "\n\tAnd how much will this person's salary change? \n\t>>>";
			cin >> _intBuf;
			//Call leader to make salary change.
			pB = office.Leader(_leaderName)->raiseSal(_id, _intBuf);
			cout << "\tThis is Staff No." << _id << " 's information after salary change.\n";
			//Show result of change immediately.
			if (pB != nullptr)
				pB->showPersonalProfile(true);
			cout << "\n\tSuccessful" << endl;
			system("pause");

			break;

		case changeDep:
			//Input former boss(with verification)
			cin.ignore();//#Change
			if (integratedLeaderVerifier(_leaderName, "", hasSpecifiedLeader, "\n\tWho is this person's former superiority? Please type in leader's name.\n\t>>>") == true)
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			cout << "\n\tLeader " << _leaderName << "'s subordinates are as follow.\n";
			office.Leader(_leaderName)->showSubordinates();
			system("pause");
			//Input a valid subordiate id.
			cout << " Please type in this person's ID.\n\t>>>";
			if (integratedSubIDVerifier(_leaderName, _id))
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Input next department's name
			cin.ignore();//#Change
			cout << "\n\tFinally, please type in the name of this person's next department.\n\t>>>";
			if (integratedDepNameVerifier(_charBuf))
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//call office to find next depart by name, then call former boss to push to next department and dismiss that person.
			office.Leader(_leaderName)->changeSubDep(_id, office.team(_charBuf));
			//Show nex department's info after change.
			cout << "\n\tInformation of \"" << _charBuf << "\" after this change is listed below :";
			office.teamAddrB(office.team(_charBuf))->showTeam();
			cout << "\n\tSuccessful" << endl;
			system("pause");

			break;

		case dism:
			//Find former boss by name(valid)
			cin.ignore();//#Change
			if (integratedLeaderVerifier(_leaderName, "this person", hasSpecifiedLeader) == true)
				break;
			cout << "\n\tTeam info of leader \"" << _leaderName << "\" is as follow." << endl;
			office.Leader(_leaderName)->myTeam->showTeam();
			//Find valid subordinate by id.
			cout << "\n\tWho will be dismissed ? Please type in this person's id.\n\t>>>";
			cin.ignore();
			if (integratedSubIDVerifier(_leaderName, _id))
			{
				this->depFileIO(saveDep, mg);
				break;
			}
			//Check dismissal permission password.
			cout << "\n\t<WARNING> DISMISSAL OF EMPLOYEES REQUIRES HUMAN RESOURCE OFFICE MANAGERS' PERMISSION.";
			cout << "\n\tENTER PASSWORD TO VERIFY YOUR PERMISSION.\n\t>>>";
			cin.ignore();
			getline(cin, _charBuf);
			if (_charBuf == dismissalPassword)
			{	//If password is correct, call former boss to dismiss.
				office.Leader(_leaderName)->dismiss(_id);
				//Show info of former boss's team immediately.
				cout << "\n\tSuccessful.This <<" << _leaderName << "'s team after dismissing staff No. " << _id << endl;

				office.Leader(_leaderName)->myTeam->showTeam();
			}
			else
			{	//If password is wrong, deny access and return .
				cout << "\n\tWRONG PASSWORD. YOU ARE NOT ALLOWED TO MAKE THIS DISSMISSAL OPERATION.\n" << endl;
			}
			system("pause");
			break;

		case exitOffice:
			system("cls");
			this->depFileIO(saveDep, mg, isLeaving = true);
			cout << "\n\n\n\n\n\tThanks for coming here and Alex appreciates your dedication to our company. \n\tWish you a good day! See you next time.\n\n\n\t" << endl;
			system("pause");
			cin.ignore();//#Change!
			return;
			break;
		default:
			//If input a wrong choice...
			cout << "\n\tSorry, \" " << _choice << "\" does not stand for any advanced management service." << endl;
			cout << "\tPlease try again : )" << endl << endl;
			system("pause");
			cin.ignore(2);//#Change!
			break;

		}

		////Save immediately after change.
		this->save(depFile);

	}
}
