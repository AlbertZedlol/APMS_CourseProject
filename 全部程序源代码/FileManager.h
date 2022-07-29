/******************************************************************************************************************
Filename		FileManager.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of Class FileManager.
******************************************************************************************************************/
#pragma once
//For _assert_s();
#include "io.h"
//For file IO
#include <fstream>
//For HRmanagement programme.
#include "ExceptionList.h"

#include "FileManagerSearchingEngine.h"

#include "PersonPOD.h"

#include "AlbertSTL_List.h"
/* You need to pre-declare the abstract class to let the compiler know this is a class.But you don't need to linke the entire header file.*/
class Person;


class FileManager
{
public:

	FileManager();

	/*what u need to do at the first.*/
	void createFile();

	/*write data from a person array pointer by "personWrittenFrom" to file. */
	void dataEntry(int startPos = -1);

	/*[Function  findAndDisplay]
	find a person by given item,Store him/her info into a proper buffer according to its class type.
	Display it.If there're more than one item matching,...	still needs improvement.
	It's also possible to extend this method to searching a team...*/
	void findAndDisplay();

	/*[Function findAndUpdate]
	find a person by given item,Store him/her info into a proper buffer according to its class type.
	Change the given item of objBuf, and write buf back to replace old version.
	This algorithm is not effective but it free us from worrying about where to find that item in the file.*/
	void findAndUpdate();

	/*find the owner of pattern and write it to global obj buffers. Return person type.If not found, return -1;*/
	int findAndWriteTo(string pattern);

	string getDatabaseName();



public://For UI.
	static void inputPersonType();
	static bool validPersonType(int type);

private:
	void makeFolder(string folderPath);
	void makeAPMSfolders(bool useDefaultPath = true, const string& givenPath="");
	void findFilesUnderDirectory(string directoryPath, List<string>& fileNameContainer);
	void selectDataBase();
	string defaultFolderPath;

private:
	/*[Data for basic file operations.]*/
	fstream _file;
	KMPsearching_engine se;
	string _filename;
	string inputBuffer;
	bool hasCreatedFile = false;
	string _databaseName;
	string depFileName;
	/*[Data to maintain database security.]*/
	/*a long pass word that limits read and/or write permission into the database.*/
	string password[3];
	/*value at [0][1][2] each represents read, write r&w permission */
	bool hasAccessPermission[3];
	const enum accessPermissionMode { modeRead = 0, modeWrite = 1, modeReadAndWrite = 2 };
	const enum readWriteMode { absolute = 0, relative = 1 };

	/*[Data For fileIO General Programming]*/
	Person* AbsPtr;

	int _io_person_type;
	/*use base class obj to check a person's identity.*/
	PODTeamLeader	 PersonBuffer;

	/*[Function openFile]
	openFile()function needs to operate with exception handling.
	Open an existing file (if it doesn't exist, create one).*/
	void openFile();

	/*[Function getFilename]
	get filename from console input and updates searching engine's filename.*/
	void getFilename();

	/*[Function checkPassword]
	check read and write permission. read = 0,write = 1 ,modeReadAndWrite=2.
	Return whether the user has specific permission.*/
	bool checkPassword(int accessPermission);

	/*[Function convertIOPersonType]
	alter AbsPtr according to input persontype commanded by user
	or the personType detected while reading from file.This function is embeded in uniformWrite/Read.*/
	void convertIOPersonType();
	/*[Function uniform read/write]
	use pointer to abstract class to read/write info regardless of class type. ]:*/
	void uniformWrite();
	/*WARNING:YOU MUST UPDATE _io_person_type BEFORE CALLING uniformRead!
	OR YOU CAN PUT THE NEW _io_person_type INTO THIS FUNCTION'S PARAMETER LIST AND UPDATE BY IT.*/
	void uniformRead(int starPos, int updated_io_person_type);

	/*[Function writeToFile]
	write specific number of persons to the file.Writing starts at startPos in the file.
	if mode==absolute, starPos means absolute pos in file.(first letter=0)
	if mode==relative, startPos means i* sizeof(Person*), where i is the array index.*/
	template<class T>
	void writeToFile(T* PersonModelArray_Source, int startPos = 0, int numberOfItems = 1, int mode = FileManager::absolute);

	/*[Function readFromFile]
	read specific number of persons to a person array.Reading starts at startPos in the file.
	if mode==absolute, starPos means absolute pos in file.(first letter=0)
	if mode==relative, startPos means i* sizeof(Person*), where i is the array index.*/
	template<class T>
	void readFromFile(T* PersonModelArray_Destination, int starPos = 0, int numberOfItems = 1, int mode = FileManager::absolute);

	/*[Function replace]
	use a const char[] to replace text at a specific position in the file.This is for quick replacement of single items.*/
	void replace(const char* newContent, unsigned long startPos); 

};

template<class T>
void FileManager::writeToFile(T* PersonModelArray_Source, int startPos, int numberOfItems, int mode)
{
	openFile();

	if (mode == absolute)
		_file.seekp(startPos);
	else if (mode == relative)
		//_file.seekp(startPos * sizeof(TeamLeader));

		for (int i = 0; i < numberOfItems; i++)
			_file.write((char*)&PersonModelArray_Source[i], sizeof(PersonModelArray_Source[i]));
	_file.close();
}

template<class T>
void FileManager::readFromFile(T* PersonModelArray_Destination, int startPos, int numberOfItems, int mode)
{
	openFile();
	if (mode == absolute)
		_file.seekg(startPos);
	else if (mode == relative)
		//_file.seekg(startPos * sizeof(TeamLeader));

		for (int i = 0; i < numberOfItems; i++)
			_file.read((char*)&PersonModelArray_Destination[i], sizeof(PersonModelArray_Destination[i]));
	_file.close();
}
