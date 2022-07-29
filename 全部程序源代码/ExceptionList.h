/******************************************************************************************************************
Filename		ExceptionList.h
Copyright(c)	Albert Corporation.All rights reserved..
Author			Albert Zhang
Description		Declare of an enumerate which tags all the errors with an explicit name. 
				This file will be included by all files related with exception handling.
******************************************************************************************************************/
#pragma once
const enum erroType
{
	_operation_erro_file_creation_error = -6,

	_operation_erro_selection_error = 0,

	_operation_erro_file_open_error = 1,

	_operation_erro_no_read_permission_ = -3,

	_operation_erro_invalid_io_person_type = -4,

	_operation_erro_invalid_occurrence_number = -5,

	_security_offence_wrong_password = -2,

	_programming_error_unexpected_error = -114514
};