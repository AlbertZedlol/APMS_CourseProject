///Methods that helps you manipulate a filestream object to
///open BINARY files safely and move pointers swiftly.
///Copyright: Albert Zhang

#include <string>
#include <fstream>
using namespace std;

///Function: manipulate a filestream object to open BINARY files when they exist and create them when they're not.
///and then place file pointer to beginning of file(you can move file pointer swiftly after that.)
///Note: you don't need to open this file before.this 
///TricK:fstream objs can't be assigned directly so we have to use references or pointers to control them.
///Para: a filestream obj working currently and the name of the file you want to open. 
void open_s(fstream& _myFile, const string& _fileName)
{
	_myFile.open(_fileName.c_str(), ios::out | ios::app | ios::binary);
	_myFile.close();
	_myFile.open(_fileName.c_str(), ios::in | ios::out | ios::binary | ios::ate | ios::_Nocreate);
	_myFile.seekg(ios::beg);
	_myFile.seekp(ios::beg);
}

//move file pointer n-steps forward(n can be negative) to PUT data.
void move_p(fstream& _myFile, long long steps)
{
	_myFile.seekp((int)_myFile.tellp() + steps);
}
//move file pointer n-steps forward(n can be negative) to GET data.
void move_g(fstream& _myFile, long long n)
{
	_myFile.seekg((int)_myFile.tellg() + n);
}


int main()
{
	fstream someFile;
	string fileName = "C:\\Users\\tongh\\Desktop\\helloWorld.dat";
	open_s(someFile, fileName);
	someFile.write("11111111111111111111111111", 27);

	fstream* pf = &someFile;
	pf->seekp(2);
	pf->write("hello?", 6 * sizeof(char));
	move_p(*pf, 10);
	pf->write("hello?", 6 * sizeof(char));
	pf->close();
}


///Other notes on moving file pointers and controling fstream objs.
/*
	controling fstream objs: 1.use referenc like above.
							 2.use pointers:
							 fstream* pf= &someFile;
							 pf->write(...);
							 pf->open(....);

	move file pointer by n positions to write/read.
							pf->seekp((int)pf->tellp()+n);
							pf->seekg((int)pf->tellg()+n);


*/