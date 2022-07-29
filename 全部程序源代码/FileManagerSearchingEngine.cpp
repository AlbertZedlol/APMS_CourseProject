/****************************************************************************************************************************
Filename		FileManagerSearchingEngine.cpp
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Implementation of member functions of Class KMPsearching_engine
*****************************************************************************************************************************/

#include "FileManagerSearchingEngine.h"
//For _assert_s
#include <io.h>
#include "ExceptionList.h"


#define SHOW_OCCURRENCE_TABLE
#define SHOWCONTEXT


KMPsearching_engine::KMPsearching_engine()
{
	_filename = "";
	pKMPfile = NULL;

	initialize_before_loop();
}

void KMPsearching_engine::KMPset_file(fstream& fileWorkingOn)
{
	pKMPfile = &fileWorkingOn;
}

void KMPsearching_engine::KMPget_filename(const string& givenFilename)
{
	_filename = givenFilename;

	initialize_before_loop();

	KMPopen_file();
}

void KMPsearching_engine::KMPopen_file()
{
	pKMPfile->close();
	pKMPfile->open(_filename, ios::in | ios::ate | ios::_Nocreate | ios::binary);
	if (_access_s(_filename.c_str(), 04) != 0)
	{
		throw _operation_erro_no_read_permission_;
	}
}

void KMPsearching_engine::KMPget_pattern(const string& givenPattern)
{
	/*std::cout << "What do you want to search in this file? \n";
	getline(cin, _pattern);*/

	_pattern = givenPattern;

	_pattern_length =(int)(_pattern.length());

	initialize_with_Pattern(true);

}

void KMPsearching_engine::initialize_with_Pattern(bool show_context)
{
	if (show_context)
		_text_origin_length = _pattern_length + _context_suffix_length;

	//Initialize _text_origin
	_text_origin = new char[_text_origin_length + 1];
	for (int i = 0; i < strlen(_text_origin); i++)
	{
		_text_origin[i] = '\0';
	}

	//Initialize _buffer.
	_buffer = new char[_buffer_size];
	for (int i = 0; i < strlen(_buffer); i++)
	{
		_buffer[i] = '\0';
	}

	//Initialize _occurrence_index_table_current_page
	_occurrence_index_table_current_page = new int[_index_table_current_page_size];
	for (int i = 0; i < _index_table_current_page_size; i++)
	{
		_occurrence_index_table_current_page[i] = -1;
	}

	//Initialize _occurrence_index_table
	_occurrence_index_table = new int[_index_table_size];
	for (int i = 0; i < _index_table_size; i++)
	{
		_occurrence_index_table[i] = -1;
	}

	//Initialize _text_between_leafs
	if (_pattern_length < 2)													//If pattern is too short, we don't need to worry about this.
		return;
	_text_between_leafs = new char[(_pattern_length - 1) * 2];					// if pattern has 4 chars and it sits BETWEEN pages
	for (int i = 0; i < (_pattern_length - 1) * 2; i++)							// it can distrubute in this region:   _ _ _ | _ _ _  
	{
		_text_between_leafs[i] = '\0';
	}

	//Initialize _table_Next based on _pattern.
	create_table_Next(_pattern.c_str(),(unsigned long)_pattern.length());

}

void KMPsearching_engine::initialize_before_loop()
{
	objectStartPos = -1;
	_pattern = "";
	_pattern_length = 0;
	_context_suffix_length = 30;					//we will show 30 words after the last word of "pattern" as a context.
	_source_place = -1;
	_page_number = 0;
	_absolute_pos = -1;
	_current_pos = -1;
	_index_table_current_page_size = _buffer_size;  //we will record the first _buffer_size occurrences in each page(in the worst case...)
	_index_table_size = 300;						//we will only record the first 200 occurences  in this file.				
	_number_of_occurrences = 0;						//!!!!!!!this is VERY IMPORTANT!!! OR YOU WILL MAKE MYSTERIOUS MISTAKES!!!
}


void  KMPsearching_engine::initialize_after_pattern_before_loop()
{
	objectStartPos = -1;
	_source_place = -1;
	_page_number = 0;
	_absolute_pos = -1;
	_current_pos = -1;
	_number_of_occurrences = 0;
}


void KMPsearching_engine::KMPsearch_loop()
{
	/*restart searching.clear all memory.*/
	this->pKMPfile->close();
	this->KMPopen_file();
	pKMPfile->seekg(ios::beg);
	/*this->_number_of_occurrences = 0;
	this->_page_number = 0;*/
	this->initialize_after_pattern_before_loop();
	////Old code(went wrong).
	//	this->pKMPfile->close();
	//	this->KMPopen_file();
	//	pKMPfile->seekg(ios::beg);
	//	this->_number_of_occurrences = 0;
	////End of old codes.
	//	this->_page_number = 0;

	while ((_current_pos = pKMPfile->peek()) != EOF)
	{
#ifdef DEBUG_SEARCHING_PROBLEM
		cout << "_current_pos==" << pKMPfile->peek() << endl;
#endif // DEBUG_SEARCHING_PROBLEM

		pKMPfile->read(_buffer, _buffer_size);

		if (_page_number % 2 == 0)												//page 0 is in. read the last line, into first half.
			record_last_line();
		else																	//page 1 is in. read the first line, into second half.
		{
			record_first_line();

			KMPsearch_multi_pages(_text_between_leafs, _pattern.c_str(), true); //now that the sentence is integrate, find pattern in it.
		}

		KMPsearch_multi_pages(_buffer, _pattern.c_str());

		_page_number++;

		//clear _buffer.
		for (int i = 0; i < _buffer_size; i++) { _buffer[i] = '\0'; }
	}
}

int KMPsearching_engine::read_safe(char* _text_origin, unsigned long  _text_origin_length)
{
	unsigned long i = 0;
	for (; (i < _text_origin_length )&& pKMPfile->peek() != EOF; i++)
	{
		pKMPfile->get(_text_origin[i]);
	}
	_text_origin[i] = '\0';
	_text_origin[_text_origin_length] = '\0';
	return i;
}

void KMPsearching_engine::record_first_line()
{
	for (int i = 0; i < _pattern_length - 1; i++)												//The first line on the new page is the second half of this sentence.
	{
		*(_text_between_leafs + _pattern_length - 1 + i) = _buffer[i];
	}
}

void KMPsearching_engine::record_last_line()
{
	for (int i = _pattern_length - 1; i < (_pattern_length - 1) * 2; i++)						//The last line on the last page is the first half of this sentence.
	{
		_text_between_leafs[i - (_pattern_length - 1)] = *(_buffer + 1024 - ((_pattern_length - 1) * 2 - i));
	}
}

//*******************************************************************************************************************************************************//
//*********APIs ARE AS FOLLOWS.They define the searching engine's response to different search results.**************************************************//
bool KMPsearching_engine::KMP_response_not_found()
{
	std::system("cls");
	std::cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\tSorry,\"" << _pattern << "\"is not in  this database.";
	std::cout << "\n\t\t\t\t\tPlease select again." << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	
	return false;

}

void KMPsearching_engine::show_index_table()
{
	int elemnentsInARow = 14;
	for (int currentIndex = 0; currentIndex < _index_table_size; currentIndex += elemnentsInARow)
	{
		std::cout << "\n\n\t*time:";
		for (int i = 0; i < elemnentsInARow; i++)
		{

			std::cout << setw(6) << currentIndex + i << "  ";
		}
		std::cout << "\n\t  Pos:";
		for (int i = 0; i < elemnentsInARow; i++)
		{
			if (currentIndex + i == _index_table_size)
			{
				std::system("pause");
				std::cout << "\n\n";
				return;
			}
			std::cout << setw(6) << _occurrence_index_table[currentIndex + i] << "  ";
		}
	}
	std::system("pause");

}

void KMPsearching_engine::show_current_matching(int i)
{
	std::cout << "\n\nOccurrence No." << i + 1 << "\n\tSource Position : " << setw(6) << _current_pos << "\t Pattern Length = " << setw(6) << _pattern_length << "\t Context length= " << strlen(_text_origin); /*_text_origin_length;*/
	std::cout << "\n\tSource:||" << _text_origin << "||";
}