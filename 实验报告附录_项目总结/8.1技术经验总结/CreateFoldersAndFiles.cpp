///Methods to help you create folders and files under specific directory only when they don't exist
///and browse all files under given directory.
///Copyright : Albert Zhang.


#include <fstream>
#include <io.h>
#include <vector>
#include <iostream>
using namespace std;

/// Function	:Make folder under given path.
/// Note		:Create folders under the mother folder in the same way: create only when it doesn't exist.
/// Environment :Win.
/// Para		:absolute path of the folder to be searched; Read/write permission type: 1==read; 2==write; 3==read and write.
/// Retun		:none.
void makeFolder(string folderPath, int permission = 3)
{
	switch (permission)
	{
	case 1:
		permission = 04;
		break;
	case 2:
		permission = 02;
		break;
	case 3:
		permission = 06;
		break;
	default:
		cerr << "Sorry wrong permission type : " << permission << endl;
		exit(-1);
		break;
	}
	if (_access_s(folderPath.c_str(), permission) != 0)
		system(("mkdir " + folderPath).c_str());
}

/// Function	:find all the files under a given directory path and store them into a vector .
/// Note		:this method needs to verify whether this file exists or not.Or shell will give you some notice.
/// Environment :Win.
/// Para		:absolute path of the folder to be searched; a vector string array to contain filenames.
/// Retun		:none.
void findFilesUnderDirectory(string directoryPath, vector<string>& fileNameContainer)
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
				fileNameContainer.push_back(p.assign(directoryPath).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


/// Function	:demonstrate all the files under a given folder path.
/// Note		:this method displays filenames without prefix.
/// Environment :Win.
/// Para		:absolute path of the folder to be found.
/// Retun		:none.
void showFilesUnderFolder(const string& folderPath)
{
	vector<string> filesUnderFolder;

	findFilesUnderDirectory(folderPath, filesUnderFolder);

	for (int i = 0; i < filesUnderFolder.size(); i++)
	{												//cut prefix							 
		filesUnderFolder[i] = filesUnderFolder[i].substr(folderPath.length() + 1, filesUnderFolder[i].length() - (folderPath.length() + 1));
		//show .
		cout << filesUnderFolder[i] << endl;
	}
}

/// Function	:make a folder under a given path and make two more sub-folders under it. Then add two files under each of the sub-folders.
/// Note		:creates files/folders only when they don't exist.
/// Environment :Win.
/// Para		:absolute path of the new folder.
/// Retun		:none.
void makeFolderAndFilesUnderPath(const string& givenPath)
{
	string folderPath = givenPath;
	makeFolder(folderPath);
	makeFolder(folderPath + "\\folder1", 3);
	makeFolder(folderPath + "\\folder2", 3);

	string filePath = "";
	fstream fileCreator;
	//Create files under sub-folder "folder1"
	filePath = folderPath;
	filePath.append("\\folder2\\file2.dat");
	fileCreator.open(filePath.c_str(), ios::out | ios::app | ios::binary);
	fileCreator.close();
	//Create files under sub-folder "folder2"
	filePath = folderPath;
	filePath.append("\\folder1\\file1.dat");
	fileCreator.open(filePath.c_str(), ios::out | ios::app | ios::binary);
	fileCreator.close();
}

int main()
{

	makeFolderAndFilesUnderPath("C:\\Users\\tongh\\Desktop\\demo");
	showFilesUnderFolder("C:\\Users\\tongh\\Desktop\\demo");

}