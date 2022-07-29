/******************************************************************************************************************
Filename		FileManager.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Implementation of class FileManager's member funcitons.
******************************************************************************************************************/
#include "A-GeneralHeader.h"

#include "ExceptionList.h"

/*this file needs all the declarations of person classes in this project.*/

#include "PersonManager.h"

#include "PersonProfessional.h"

/*this file needs all the declarations of person classes in this project.*/

#include "FileManager.h"

#include "io.h"

#include <fstream>

#include "PersonAbstractClass.h"

#include "UIConsoleBoard.h"

FileManager::FileManager()
{
	se.KMPset_file(_file);
	_filename = "";
	password[modeRead] = "APMS";//"i luv albert";
	password[modeWrite] = "APMS";//"i luv albert";
	password[modeReadAndWrite] = "APMS";//"i luv albert";
	inputBuffer = "";
	hasAccessPermission[0] = hasAccessPermission[1] = hasAccessPermission[2] = false;
	defaultFolderPath="D:\\AlbertPersonelManagmentSystem";
}

void FileManager::createFile()
{
	std::cout << "\n[Open File]\n";
	_filename.clear();
	try
	{
		openFile();
	}
	catch(erroType err)
	{
		if (err != _operation_erro_file_open_error)
		{
			cerr << "An unexpected error occurred at void FileManager::createFile()" << endl;
		}
		throw err;
	}
	
}

void FileManager::getFilename()
{
	_file.close();
	std::system("cls");
	std::cout << "\n\n\n\n\n\n\tDatabases created before are listed below:\n\n";
	this->selectDataBase();
	cout<<"\n\tYou can open one of them above or create a new one.";
	cout<<"\n\tPlease type in the name of your database to open or create.\n\t>>>";	
	_filename = this->defaultFolderPath;
	_databaseName = "";
	getline(cin, _databaseName);
	_filename.append("\\personal\\").append(_databaseName.c_str());
	_filename.append(".dat");
	if(hasCreatedFile)
		this->se.KMPget_filename(_filename);
}

void FileManager::openFile()
{
	_file.close();
	/*Make folders in disk D.*/
	this->makeAPMSfolders();
	depFileName = "";
	if (_filename.length() == 0)														//If haven't created any file...
	{			
		int failureCount = 0;															//Create and open.
		
		getFilename();
		_file.open(_filename.c_str(), ios::out | ios::app | ios::binary);

		depFileName = this->defaultFolderPath + "\\hierarchy\\" + this->_databaseName + ".dat";

		while(_access_s(_filename.c_str(),06)!=0)
		{
			failureCount=(!failureCount?1:2);
			system("cls");
			_file.open(_filename.c_str(), ios::out | ios::app | ios::binary);
			std::cout << "\n\n\n\t\t\t\t\tSorry,we can't open this file:\n\t\t\t\t\t\"" << _filename << "\". \n\t\t\t\t\tPlease check your spelling and file access permission and try again.\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(failureCount==1?7500:2500));
			system("cls");
			std::cout << "\n\n\n\nPlease type in your filename.\n>>>";
			getFilename();
			_file.open(_filename.c_str(), ios::out | ios::app | ios::binary);

			depFileName = this->defaultFolderPath + "\\hierarchy\\" + this->_databaseName + ".dat";
		}
		hasCreatedFile = true;
		cout << "\n\nSuccessful." << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		system("cls");
		this->se.KMPget_filename(_filename);
	}
	else
	{
		_file.open(_filename.c_str(), ios::out | ios::app | ios::binary | ios::_Nocreate);
	}//If a file has been opened...

	if (_access_s(_filename.c_str(), 06))														//Don't create, just open. 
	{
		cout << "\nWe can't open this file\"" << _filename << "\"" << "it has no read & write persmission." << endl;
		throw   _operation_erro_file_open_error;												//1==fileOpenErr;
	}
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}



	//Open a new personal database and update department database name


	office.changeDataBase(*this);



	office.depFileIO(0, *this);






	/*NOTION: method above only tests the existence of a file and creates new file.
	But the use of ios::app glues the file pointer to EOF.To use pointer swiftly, we might use se.KMPopen_file()before  searching:*/
}

void FileManager::makeFolder(string folderPath)
{
	std::string command = "mkdir -p " + folderPath;
	system(command.c_str());
	system("cls");
}

void FileManager::makeAPMSfolders(bool useDefaultPath, const string& givenPath)
{
	string folderPath = givenPath;
	if (_access_s(folderPath.c_str(), 06) != 0)
	{
		makeFolder(folderPath);
		makeFolder(folderPath + "\\hierarchy");
		makeFolder(folderPath + "\\personal");
	}
	string filePath = folderPath;
	filePath.append("\\personal\\database001.dat");
	fstream fileCreator;
	fileCreator.open(filePath.c_str(), ios::out | ios::app | ios::binary);
	fileCreator.close();
	filePath = folderPath;
	filePath.append("\\hierarchy\\hierarchical_info.dat");
	fileCreator.open(filePath.c_str(), ios::out | ios::app | ios::binary);
	fileCreator.close();
}

void FileManager::findFilesUnderDirectory(string directoryPath, List<string>& fileNameContainer)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(directoryPath).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					findFilesUnderDirectory(p.assign(directoryPath).append("\\").append(fileinfo.name), fileNameContainer);
			}
			else
			{
				fileNameContainer.append(p.assign(directoryPath).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void FileManager::selectDataBase()
{
	List<string> filesUnderFolder;
	const string folderPathOriginal = "D:\\AlbertPersonelManagmentSystem\\personal";
	string folderPath = folderPathOriginal;
	findFilesUnderDirectory(folderPath, filesUnderFolder);
	for (int i = 0; i < filesUnderFolder.size(); i++)
	{
		filesUnderFolder[i] = filesUnderFolder[i].substr(folderPathOriginal.length() + 1, filesUnderFolder[i].length() - (folderPathOriginal.length() + 1) - 4);
		cout << "\t" << filesUnderFolder[i] << "\n\n";
	}
}

bool FileManager::checkPassword(int accessPermission)
{//Wrong input.
	if ((accessPermission != 0) && (accessPermission != 1) && (accessPermission != 2))
		return false;
	if (hasAccessPermission[accessPermission] == false)
	{
		string permissionType[3] = { "read","write","read and write" };
		int timeOfErr = 0;
		std::system("cls");
		inputBuffer.clear();
		cin.clear();
		fflush(stdin);
		std::cout << "\n\n\n\n\n\n\t\t\t\t\tPlease enter password for " << permissionType[accessPermission] << " permission:\n\t\t\t\t\t>>>";
		getline(cin, inputBuffer);
		while (!(inputBuffer == password[accessPermission]) && timeOfErr < 2)
		{
			std::system("cls");
			std::cout << "\n\n\n\n\n\n\t\t\t\t\tWrong password. You have " << 2 - timeOfErr << " chance(s) left.\n";
			_sleep(1500);
			std::system("cls");
			inputBuffer.clear();
			cin.clear();
			fflush(stdin);
			std::cout << "\n\n\n\n\n\n\t\t\t\t\tPlease enter password for " << permissionType[accessPermission] << " permission:\n\t\t\t\t\t>>>";
			getline(cin, inputBuffer);
			++timeOfErr;
		}
		if (timeOfErr < 2)/*unlock*/
		{
			hasAccessPermission[accessPermission] = true;
			std::cout << "\n\t\t\t\t\tAccess Allowed. " << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			system("cls"); return true;
		}
		try
		{
			if (timeOfErr == 2)
				throw (_security_offence_wrong_password);
		}
		catch (erroType)
		{
			throw;
			return false;
		}
	};
}

void FileManager::convertIOPersonType()
{
	AbsPtr = nullptr;

	switch (this->_io_person_type)
	{
	case dictionary.PersonTypeTeamLeader:
		AbsPtr = new TeamLeader[1];
		break;
	case dictionary.PersonTypeDepartmentLeader:
		AbsPtr = new DepartmentLeader[1];
		break;
	case dictionary.PersonTypeExecutive:
		AbsPtr = new Executive[1];
		break;
	case dictionary.PersonTypeCEO:
		AbsPtr = new ChiefExecutive[1];
		break;
	case dictionary.PersonTypeFinancialAnalyst:
		AbsPtr = new FinancialAnalyst[1];
		break;
	case dictionary.PersonTypeTechnician:
		AbsPtr = new Technician[1];
		break;
	case dictionary.NoneType:
		return;
		break;
	case dictionary.AbsBase:
		cerr << "\n\tpersontype is \"AbsBase\". Possible reason: didn't initialize personType in constructor function.\n";
		break;
	default:
		break;
	}
	if (AbsPtr == NULL)
	{
		cerr << "Something went really wrong: Input/Output an invalid person type makes AbsPtr a nullPtr!" << endl;
		throw _operation_erro_invalid_io_person_type;
	}
}

void FileManager::uniformWrite()
{
	_file.close();
	this->openFile();
	try 
	{
		this->convertIOPersonType();
	}
	catch(erroType error)
	{
		if (error == _operation_erro_invalid_io_person_type)
			throw error;
		else
		{
			cerr << "Unexpected erro found in void FileManager::uniformWrite().Error: " << error << " ." << endl;
			throw _programming_error_unexpected_error;
		}
	}

	AbsPtr->inputPersonalInfo();

	AbsPtr->toPOD();

	AbsPtr->write2file(_file);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

void FileManager::uniformRead(int startPos, int updated_io_person_type)
{
	this->_io_person_type = updated_io_person_type;
	_file.close();
	this->openFile();

	_file.seekg(startPos);

	try
	{
		this->convertIOPersonType();

		AbsPtr->readFromFile(_file);

		AbsPtr->fromPOD();

		AbsPtr->showPersonalProfile(true);
	}
	catch(erroType err)
	{
		throw err;
	}
}

void  FileManager::replace(const char* newContent, unsigned long startPos)
{
	try
	{
		checkPassword(modeReadAndWrite);
	}
	catch(erroType er)
	{
		throw er;
	}

	//***Nocreate and ate saves content and enables rewinding.
	_file.close();
	_file.open(_filename.c_str(), ios::out | ios::ate | ios::binary | ios::_Nocreate);
	if (_access_s(_filename.c_str(), 06))
	{
		throw _operation_erro_file_open_error;
	}
	//Rewrite
	_file.seekp(startPos);
	_file.write(newContent, strlen(newContent));
	_file.close();

	cout << "\n\t\t\t\t\t\tData updated successfully.";
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

/*************************************************************************************************************************************************************************************/
/*Interfaces are as follows:***************************************************************************************************************************************************************/

void FileManager::inputPersonType()
{
	/* The best method is to create a dictionary in C++ and use it to output or update.*/
	system("cls");
	cout << "Please enter this person's identity.\n";
	for (int i = 2; i < dictionary.numberOfPosts+2; i++)
	{
		cout << setw(25) << setiosflags(ios::left) << dictionary.postName[i]; cout << "press ";
		cout << i-1 << endl;
	}
}

bool FileManager::validPersonType(int type) { return (type >= 1 && type <= dictionary.numberOfPosts); }

void FileManager::dataEntry(int startPos)
{
	if (this->hasCreatedFile == false)
		this->openFile();
	try
	{
		checkPassword(modeWrite);
	}
	catch(erroType)
	{
		throw ((_security_offence_wrong_password));
		return;
	}

	std::cout << "\n[Recording information...]\n";
	if (startPos == -1)/*then goto end of file.*/
	{
		_file.close();
		_file.open(_filename, ios::in | ios::ate | ios::binary | ios::_Nocreate);
	}
	_file.seekp(startPos);

	ConsoleBoard cb;

	cb.inputNum_s(FileManager::inputPersonType, FileManager::validPersonType, _io_person_type);
	
	this->uniformWrite();
}

void FileManager::findAndDisplay()
{
	try
	{
		checkPassword(modeRead);
	}
	catch(erroType er)
	{
		throw er;

		return;
	}

	if (this->hasCreatedFile == false)
		this->openFile();
	std::cout << "\n[Search database]\n";
	/*reset to DNE value so as to make it convenient to debug*/
	_io_person_type = -2;

	cout << "Please enter the item to be found in this database\n>>>";
	string whatToFind = "";
	getline(cin, whatToFind);

	se.KMPget_pattern(whatToFind);
	se.KMPsearch_loop();
	int numOccur = se.KMPrespond(-2, &PersonBuffer);

	if (numOccur == 0)
		se.KMPrespond(0, &PersonBuffer);
	else if (numOccur == 1)
	{
		/*get to the only source.*/
		_file.seekg(se.KMPrespond(-1, &PersonBuffer));
		/*use a base class obj to test person type.*/
		_file.read((char*)&PersonBuffer, sizeof(&PersonBuffer));
		/*get back to object start position*/
		_file.seekg((int)(_file.tellg()) - (int)sizeof(&PersonBuffer));

		try
		{
			this->_io_person_type = stoi(PersonBuffer.personType);
			this->uniformRead((unsigned long)_file.tellg(), _io_person_type);

		}
		catch(erroType error)
		{
			if (error== _operation_erro_invalid_io_person_type)
			{
				cerr << "\n\tOperation error :There's no such a person type!";
				throw(error);
			}
		}
	}
	else if (numOccur > 1)
	{
		se.KMPrespond(2, &PersonBuffer);
	}

	else
	{
		throw _operation_erro_invalid_occurrence_number;		// error.:Impossible occurrence number
		cerr << "\n\tOpertaion error: impossible occurrence number " << numOccur << endl;;
	}
	cout << "\nPress any key to continue.\n";
	system("pause");
	system("cls");
}

void  FileManager::findAndUpdate()
{
	try
	{
		checkPassword(modeReadAndWrite);
	}
	catch(erroType er)
	{
		throw er;
	}

	if (this->hasCreatedFile == false)
		this->openFile();
	std::cout << "\n[Update information]\n";

	cout << "\n\n\n\n\n\n\n\t\t\t\t\tPlease input the item to be changed in this database\n\n>>>";
	string whatToFind = "";
	getline(cin, whatToFind);

	se.KMPget_pattern(whatToFind);
	se.KMPsearch_loop();
	int numOccur = se.KMPrespond(-2, &PersonBuffer);

	if (numOccur == 0)
		se.KMPrespond(0, &PersonBuffer);
	else if (numOccur == 1)
	{
		/*SINGLE ITEM REPLACEMENT*/
		/*find the first and only occurrence place of the given item.*/
		int itemPlace = this->se.KMPrespond(-3, &PersonBuffer);

		/*rewrite the entire object.*/
		/////*FIND the PROPER class that SUITS this object.And pass it to searching engine.*/
		/////*get to the only source.*/
		//////_file.seekg(se.KMPrespond(-1, &PersonBuffer));
		///////*use a base class obj to test person type.*/
		//////_file.read((char*)&PersonBuffer, sizeof(&PersonBuffer));
		///////*get back to where u started.*/
		//////_file.seekg((int)(_file.tellg()) - (int)sizeof(&PersonBuffer));
#ifdef DEBUG_SEARCHING_PROBLEM
		se.show_index_table();
		se.show_current_matching(0);
#endif // DEBUG_SEARCHING_PROBLEM
		try
		{
			/*std::cout << "\npersontype==" << PersonBuffer.personType << endl;*/
			cout << "\n\n\n\n\n\n\n\t\t\t\t\tPlease input the item to be changed in this database\n\n>>>";

			/*skip the enter pressed just then? it looks like we don't need it.*/
			string newContent = "";
			getline(cin, newContent);
#ifdef DEBUG_SEARCHING_PROBLEM
			cout << "\byou want to change to " << newContent.c_str() << endl;
#endif // DEBUG_SEARCHING_PROBLEM

			this->replace(newContent.c_str(), itemPlace);
		}
		catch(erroType error)
		{
			throw(error);
		}
	}
	else if (numOccur > 1)
	{
		se.KMPrespond(2, &PersonBuffer);
	}
	else
	{
		throw _operation_erro_invalid_occurrence_number;		
		cerr << "ERROR: Impossible occurrence number.";
	}
}

string FileManager::getDatabaseName()
{
	depFileName = this->defaultFolderPath + "\\hierarchy\\" + this->_databaseName + ".dat";
	return  depFileName;
}


int FileManager::findAndWriteTo(string pattern)
{
	string whatToFind = pattern;
	se.KMPget_pattern(whatToFind);
	se.KMPsearch_loop();
	int numOccur = se.KMPrespond(-2, &PersonBuffer);
	if (numOccur == 0)
		se.KMPrespond(0, &PersonBuffer);
	else if (numOccur == 1)
	{
		_file.seekg(se.KMPrespond(-1, &PersonBuffer));
		_file.read((char*)&PersonBuffer, sizeof(&PersonBuffer));
		_file.seekg((int)(_file.tellg()) - (int)sizeof(&PersonBuffer));
		try
		{
			this->_io_person_type = stoi(PersonBuffer.personType);

			switch (this->_io_person_type)
			{
			case dictionary.PersonTypeTeamLeader:
				AbsPtr = &bufTld;
				break;
			case dictionary.PersonTypeDepartmentLeader:
				AbsPtr = &bufDld;
				break;
			case dictionary.PersonTypeExecutive:
				AbsPtr = &bufExe;
				break;
			case dictionary.PersonTypeCEO:
				AbsPtr = &bufCeo;
				break;
			case dictionary.PersonTypeFinancialAnalyst:
				AbsPtr = &bufFa;
				break;
			case dictionary.PersonTypeTechnician:
				AbsPtr = &bufTech;
				break;
			case dictionary.NoneType:
				return -1;
				break;
			case dictionary.AbsBase:
				cerr << "\n\tpersontype is \"AbsBase\". Possible reason: didn't initialize personType in constructor function.\n";
				break;
			default:
				return -1;
				break;
			}
			if (AbsPtr == NULL)
			{
				cerr << "Something went really wrong: Input/Output an invalid person type makes AbsPtr a nullPtr!" << endl;
				throw _operation_erro_invalid_io_person_type;
			}

			AbsPtr->readFromFile(_file);
			AbsPtr->fromPOD(); 
		}
		catch (erroType error)
		{
			if (error == _operation_erro_invalid_io_person_type)
			{
				cerr << "\n\tOperation error :There's no such a person type!";
				throw(error);
			}
		}
	}
	else if (numOccur > 1)
	{
		se.KMPrespond(2, &PersonBuffer);
	}
	else
	{
		throw _operation_erro_invalid_occurrence_number;		// error.:Impossible occurrence number
		cerr << "\n\tOpertaion error: impossible occurrence number " << numOccur << endl;;
	}

	return _io_person_type;
}

