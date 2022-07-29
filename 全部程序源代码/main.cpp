/****************************************************************************************************************************
Filename		main.cpp
Copyright(c)	Albert Corporation.All rights reserved.
Author			Albert Zhang
Description		Process control and exception handling.
*****************************************************************************************************************************/
#include "A-GeneralHeader.h"
#include "FileManager.h"
#include "UIConsoleBoard.h"
#include "ExceptionList.h"
#include "DepartmentAbstractClass.h"

int main()
{
	FileManager mg;
	ConsoleBoard consoleBoard;

	while (consoleBoard.whetherExitDateBase() == false)
	{
		consoleBoard.showMenu();

		try
		{
			switch (consoleBoard.getDirective())
			{
			case _createFile:		mg.createFile();						break;
			case _dataEntry:		mg.dataEntry();							break;
			case _findAndUpdate:	mg.findAndUpdate();						break;
			case _findAndDisplay:	mg.findAndDisplay();					break;
			case _exitDatabase:		consoleBoard.exitDatabase();			break;
			case _advancedService:	mg.createFile();
									office.advancedService(mg);				break;
			default:				throw _operation_erro_selection_error;	break;
			}
		}
		catch (erroType error)
		{
			switch (error)
			{
				std::cout << "main catched error!" << endl;
				std::cout << "error type==" << error << endl;

			case _operation_erro_file_creation_error: 		std::cout << "Sorry, we CANNOT CREATE this file. Please try again.\n"; 												break;

			case _operation_erro_file_open_error: 			std::cout << "Sorry, this file DOES NOT HAVE READ AND/OR WRITE PERMISSION.\n";										break;

			case _operation_erro_no_read_permission_:		std::cout << "Sorry, we CANNOT READ this file. Please try again.\n";												break;

			case _operation_erro_invalid_io_person_type:	std::cout << "Sorry,this is NOT A PROPER PERSONNEL IDENTITY. Please try again.\n";									break;

			case _operation_erro_selection_error:			std::cout << "Sorry, there's NO SUCH A CHOICE AS \"" << consoleBoard.getDirective() << "\"Please select again.\n";	break;

			case _operation_erro_invalid_occurrence_number:	std::cout << "<ERROR>System error: the searching engine returned an impossible occurrence number!";
				std::cout << "Please contact the maintainence team for help.\n";													break;

			case _security_offence_wrong_password:			std::cout << "<WARNING>\n\t\t\t\t\tYOU HAVE ENTERED PASSWORD WRONG FOR THREE TIMES.";
				std::cout << "            \t\t\t\t\tYOU ARE NOT ALLOWED TO VISIT THIS DATABASE.\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
				return -1;																											break;

			default:										std::cout << "<ERROR>System error: An unknown error detected,Please contact the maintainence team for help.\n";		break;

			}
			
			std::cout << "\n\n\t\t\t\t\tPress any ket to continue.\n\n";
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	return 0;
}

