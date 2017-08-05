#include "StringFunctions.h"

using namespace error_handling;

namespace str_manip
{
	void Str_RemoveChar(std::string& str, char ch)																// string function to remove each instance of a certain character from a string
	{																											//
		int size = str.size();
		for (int i = 0; i < size; i++) { if (str[i] != ch) str += str[i]; }										// check each character and add to Temp if not equal to ch
		str = str.substr(size);																					// assign str to trailing substring
	}																											//

	int StrToInt(std::string str, Error* err, int base)															// function to try to convert a string to an integer
	{																											// 
		int Result;																								// integer to hold converted value
		int Negs = 0; 																							// integer to hold number of negative signs in string
		err = new Err_NoErr;																					// initialize contents of err
		if (base < 0 || base > 36) { err = new Err_InvalidBase; return 0; }										// if base is out of range, assign error and return 0										
		Str_RemoveChar(str, ' '); 																				// clear str of any whitespace
		if (str.empty()) { err = new Err_Empty; return 0; }														// if str is empty, assign error and return 0
		for (int i = 0; i < str.length(); i++) 																	// ================================== loop through str ==============================
		{																										// 
			if (str[i] == '-') Negs++; 																			// if current character is a '-', increment Negs
			else break; 																						// if current character is not a '-', break the loop
		}																										// ==================================================================================
		str = str.substr(Negs); 																				// removes negative signs from str
		if (Negs % 2) str = "-" + str;																			// if there is an odd number of negative signs, add '-' to str																				
		size_t idx = 0;																							// index for if stoi runs into value other than excepted values
		try { Result = std::stoi(str, &idx, base); }															// attempt to convert
		catch (std::invalid_argument) { err = new Err_InvalidInput; return 0; }									// if invalid_argument exception is thrown, assign error, and return 0
		catch (std::out_of_range) { err = new Err_OutOfRange; return 0; }										// if out_of_range exception is thrown, assign error, and return 0
		if (idx != str.length()) { err = new Err_InvalidInput; return 0; }										// if converter ran into aphabetic character, assign error, and return 0
		return Result; 																							// return Result
	}

	std::vector<std::string> Str_SplitByChar(std::string str, char c)
	{
		std::string Temp = "";
		std::vector<std::string> SplitVec = std::vector<std::string>();
		for (char ch : str)
		{
			if (ch != c) Temp += ch;
			else
			{
				if (!Temp.empty())
				{
					SplitVec.push_back(Temp);
					Temp = "";
				}
			}
		}
		if (!Temp.empty()) SplitVec.push_back(Temp);
		SplitVec.shrink_to_fit();
		return SplitVec;
	}

	void ClearScreen()
	{
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = csbi.dwSize.X *csbi.dwSize.Y;

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR) ' ',
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}

	Error* Str_CheckIntInRange(const std::string& str, int min, int max)
	{
		Error* Result = new Err_NoErr();
		int Num = StrToInt(str, Result);
		if (*(Result)) return Result;
		if (!((Num >= min) && (Num <= max))) return new Err_OutOfRange();
		return Result;
	}

	std::string Divider(char c)
	{
		return std::string(65, c);
	}
}