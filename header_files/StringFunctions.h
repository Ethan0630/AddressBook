#pragma once
#include <string>
#include <windows.h>
#include <ctype.h>
#include <vector>
#include "ErrorHandling.h"

using namespace error_handling;

namespace str_manip
{
	void ClearScreen();
	void Str_RemoveChar(std::string& str, char ch);												// string function to remove each instance of a certain character from a string
	int StrToInt(std::string str, error_handling::Error* err, int base = 10);					// function to convert a string to a double value
	std::vector<std::string> Str_SplitByChar(std::string str, char c);
	//Error* Str_CheckIntInRange(const std::string& str, int min, int max);
	std::string Divider(char c);
	std::string Str_ToLower(std::string val);
}