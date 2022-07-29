/******************************************************************************************************************
Filename		FileManagerSearchingEngine.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of Class KMPsearching_engine.
******************************************************************************************************************/
#pragma once
#include "A-GeneralHeader.h"
#include <fstream>

class KMPsearching_engine
{
public:
	KMPsearching_engine();
	void KMPget_filename(const string& givenFilename);
	void KMPset_file(fstream& fileWorkingOn);							//Interface: searching engine can be embeded to other file management classes by passing a fstream object's reference to its internal pointer.
	void KMPget_pattern(const string& givenPattern);
	void KMPsearch_loop();												/*KMPsearch_loop(): The pivot of searching engine. It reloads pages and search each one.*/

	/*mode==-2, return _number_of_occurences.
	  mode===-1, move the pKMPfile pointer to the place where the object that owns this item starts record its data, and return that objectStartPos.
	  mode ==-3, return _occurrence_index_table[0]
	  mode is not those special values: test the number of occurrences. If numb==0, goto "result_not_found. If num>1, deal with multiple results.*/
	template <class T>
	int KMPrespond(int mode, T* objectBuffer = NULL);

private:
	fstream* pKMPfile;
	unsigned long objectStartPos;													//This data is specially designed for interacting with FileManager,it rememebers obj start pos and helps communication between two classes	
	string _filename;																	

	string _pattern;
	int _pattern_length;
	int* Next;															//Next: a table (an array) that records "the identical zones" in pattern.(KMP algorithm)

	char* _buffer;														//_buffer:In case the file is too long, we will only manage a limited amount of data(we call them "a page", which is stored in _buffer) everytime searching function is called.
	const int _buffer_size = 1024;										//_bufferSize: How much data will we handle each time.

	char* _text_between_leafs;											//_text_between_leafs:  A character string that records the line starting from the end of last page and runs to the next page.They will make positioning typically tricky
																		//						since it is possible that a pattern might hide between pages, escaping our searching if we only check characters on a single page.
	char* _text_origin;													//_text_origin: it stores the origin(source) of pattern found in text. It's possible that we will record a bit more information in it(e.g. pattern+context).
	int _text_origin_length;
	int _context_suffix_length;											//If we want to show the context of pattern sources, we can specify _context_suffix_length to let _text_origin record more words afterwards.

	int _source_place, _page_number, _absolute_pos, _current_pos;		//

	int* _occurrence_index_table_current_page;							//_occurrence_index_table_current_page: an array that records occurrences found on the page being read which contains absolute positions of each occurrence.[0],[1],...					
	int _index_table_current_page_size;									//_occurrence_index_table_current_page[i] stores the absolute position of occurrence No. i.
	int* _occurrence_index_table;										//_occurrence_index_table: It stores all the occurrences found in the whole file whose structure is identical to that of _occurrence_index_table_current_page, but has a bigger size.
	int _index_table_size;
	int _number_of_occurrences;

	void KMPopen_file();												//KMPopen_file(): opens file named "_filename".
	void initialize_before_loop();
	void initialize_after_pattern_before_loop();
	void initialize_with_Pattern(bool show_context);					//initialize _text_origin, _buffer, _occurrence_index_table_current_page, _occurrence_index_table, _text_between_leafs, _table_Next based on _pattern according to Pattern's structure.
	int read_safe(char* destination, unsigned long length);				//A function that replaces std::filestream::read() which isn't reliable when reading the last few characters on a page. ReturnVal: number of characters read in successfully.
	void record_last_line();											//These two functions are called when turning pages. They record those lines that "sits" between pages.
	void record_first_line();

	template <class T>
	void create_table_Next(const T* Pattern, int _pattern_length);		//We will draw a Next table for each pattern searched.

	template <class T>													//Core searching funtion which searches Pattern in Text that could have multiple pages. This function will one page at a time 
	void KMPsearch_multi_pages(const T* Text, const T* Pattern, bool is_between_leafs = false);		//and record the absolute position of each pattern in the entire file.

	bool KMP_response_not_found();										//Return Value	true: select again; false: quit this.
	
	template <class T>
	void KMP_response_is_found(T* objectBuffer);						//***They are APIs that can change with the software design accordingly

public:
	void show_current_matching(int _occurrence_index);					//These functions shows matching results and they could be used to debug.
	void show_index_table();											//***They are APIs that can change with the software design accordingly
};

template <class T>
void KMPsearching_engine::create_table_Next(const T* Pattern, int _pattern_length)
{
	const int m = _pattern_length;
	const int _DNE = -1;
	int i = 0, j = 0;

	Next = new int[m];													//Constructing table Next from Pattern(standard method in KMP)
	int t = Next[0] = -1;
	/*Note: if _pattern_length==1, table Next will not be constructed.*/
	while (j < m - 1)
	{
		if (t == _DNE || Pattern[j] == Pattern[t])
		{
			j++;
			t++;
			Next[j] = (Pattern[t] != Pattern[j] ? t : Next[t]);
		}
		else
			t = Next[t];
	}

}

//Functions  :  1.Find all the "Pattern[]" s that occur in "Text[]" ;
//			    2.For each of these patterns, record the indexes of their first elements in "IndexStorage";
//				3.Calculate number of occurrences of Pattern[] in Text[] detected in this round of searching.							
//Parameters :  const T* Text	 		place you find the patterns.		
//				const T* Pattern  		pattern to be found in text.
//				bool  is_between_leafs	whether this pattern in on different pages. 
//										if it is, then we will need much delibration to record its position in text properly.
//ReturnValue:  None.
template <class T>
void KMPsearching_engine::KMPsearch_multi_pages(const T* Text, const T* Pattern, bool is_between_leafs)
{
	const int n = _buffer_size;
	const int m = _pattern_length;
	const int _DNE = -1;
	int i = 0, j = 0;

	while (i < n)														//Standard KMP algorithm.  (i-j) is the RELATIVE position of pattern (if first character of Text is 0).
	{
		if (_pattern_length == 1&& Text[i]!= Pattern[j])				//If pattern length==1,table Next will not be constructed. We will brutally traverse this text to find that
		{																//single character.
			i++;
			continue;
		}
		else if (j == _DNE || Text[i] == Pattern[j])
		{
			i++;
			j++;
		}
		else
		{
			j = Next[j];
		}

		if (j == m)														//Special API designed for multi-page searching comes now...
		{																//If a complete matching is found...
			if (((i - j) >= 0 && (i - j) < n))							//If the index is valid...
			{															//		use the ABSOLUTE position of pattern in this FILE to record one occurrence.
				if (is_between_leafs)									//If is_between_leafs...If_pattern_length==14, length of half sentence==13 (_buffer_size - 1)
				{														//If this pattern is ON THE LAST PAGE, coordination (i-j) (with respect to the first letter of _text_between_leafs) 
																		//			will have value in [0, 12](_buffer_size - 2), which means (i-j)<13. Or it will be on THIS PAGE.
					if ((i - j) < (_pattern_length - 1))				//case 1: Pattern is on the last page. Number [12] should be number [_buffer_size-1](last word )on last page. 	


						_occurrence_index_table[_number_of_occurrences] = (_page_number - 1) * _buffer_size + (_buffer_size - 1) - ((_pattern_length - 2) - (i - j));

					else												//case 2: Pattern is on this     page. Number [13] should be number [0] (first word)on this page.


						_occurrence_index_table[_number_of_occurrences] = (_page_number)*_buffer_size + ((i - j) - (_pattern_length - 1));
				}

				else													//If pattern is found in a safe zone(not on the edge!), we will use ordinary method to record its position.
				{
					_occurrence_index_table[_number_of_occurrences] = _page_number * _buffer_size + (i - j);
				}														//		calculation: (i-j) is the RELATIVE position in _buffer(with respect to the first letter of _buffer handled in this round). 
																		//		and there are  (_page_number * _buffer_size) words ahead of this buffer. 
				_number_of_occurrences++;								//As long as position is valid(((i - j) >= 0 && (i - j) < n)==true), this would be count as a new occurrence.
			}															//If the index is not valid, we will not record anything. 

			j = 0;														//Either way, j should be reset to 0 to find "head pairs" again.

		}
	}
}

/*	  mode==-2, return _number_of_occurences.
	  mode===-1, move the pKMPfile pointer to the place where the object that owns this item starts record its data, and return that objectStartPos.
	  mode ==-3, return _occurrence_index_table[0]
	  mode is not those special values: test the number of occurrences. If numb==0, goto "result_not_found. If num>1, deal with multiple results.*/
template <class T>
int KMPsearching_engine::KMPrespond(int mode, T* objectBuffer)
{
	pKMPfile->close();								//Re-open file.//close file();
	KMPopen_file();

	if (mode == -2)
		return _number_of_occurrences;

	else if (mode == -1)
	{
		/*FIND the only source and return its position.*/
		int startPosition = _occurrence_index_table[0];

		/*get back to the nearest deliminator'\n' upstream*/
		pKMPfile->seekg(startPosition);
		char _bufferSingleCh = '*';
		pKMPfile->get(_bufferSingleCh);
		while (_bufferSingleCh != '\n')
		{
			pKMPfile->seekg((int)(pKMPfile->tellg()) - 2);
			pKMPfile->get(_bufferSingleCh);
		}
		pKMPfile->seekg((int)(pKMPfile->tellg()) - 1);

		objectStartPos =(int)(pKMPfile->tellg());

		return objectStartPos;
	}

	else if (mode == -3)
	{
		return this->_occurrence_index_table[0];
	}

	if (_number_of_occurrences == 0)
	{
		this->KMP_response_not_found();
		return -1;
	}
	else
	{
		//pKMPfile->close();								//Re-open file.//close file();
		//KMPopen_file();	
		this->KMP_response_is_found(objectBuffer);
	}
	pKMPfile->close();
}

template <class T>
void KMPsearching_engine::KMP_response_is_found(T* objectBuffer)
{
	
		std::cout << "\n\n\n\n\n\n\t\t\t\t\tWe found different results for \"" << this->_pattern << "\".Number of occurrences = " << _number_of_occurrences << endl;
		_sleep(1300);
		std::cout << "\n\t\t\t\t\tWhich of the following is the item you ask for? \n";

		int i = 0;
		_current_pos = _occurrence_index_table[0];

		while (_current_pos != -1 && i < _index_table_size && i < _number_of_occurrences)
		{
			//clear _text_origin.
			for (int i = 0; i < strlen(_text_origin); i++) { _text_origin[i] = '\0'; }

			//read in pattern in text.
			pKMPfile->seekg(_current_pos);
			unsigned long tt =(unsigned long) pKMPfile->tellg();

			if (read_safe(_text_origin, _text_origin_length) == 0)				//If read in 0 bit...return.
				return;

			show_current_matching(i);

			i++;

			_current_pos = _occurrence_index_table[i];

		}
		//show_index_table();

		system("pause");
}