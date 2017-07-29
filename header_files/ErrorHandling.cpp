#include "ErrorHandling.h"

namespace error_handling
{
	// Error Class
	// ---------------------------------------------------------------------------------------------------------
	
	// Constructors																						
	Error::Error()
	{
		isError = false;
		Message = "";
	}
	Error::Error(std::string msg)
	{
		isError = true;
		Message = msg;
	}																									
	// Operator Overloading																				
	Error::operator bool() const { return isError; }													
	bool Error::operator !() const { return !isError;  }												
	// Public Methods																					
	void Error::ChangeMessage(std::string msg) { Message = msg; }										
	const std::string& Error::GetErrMessage() { return Message; }										
	void Error::DisplayMessage() { std::cout << Message << "\n"; }										
	// ---------------------------------------------------------------------------------------------------------
	
	// Error Derived Classes
	// ---------------------------------------------------------------------------------------------------------

	Err_Empty::Err_Empty() : Error("Entry cannot be empty!") {}

	Err_InvalidInput::Err_InvalidInput() : Error("Entry was invalid!") {}

	Err_SpecChar::Err_SpecChar(char c) : Error(std::string(1, c) + " is a special character and entries cannot contain it") {}

	Err_InvalidChar::Err_InvalidChar() : Error("Entry contained invalid characters!") {}

	Err_OutOfRange::Err_OutOfRange() : Error("Entry contains a value that is out of range!") {}	

	Err_InvalidFuncName::Err_InvalidFuncName() : Error("Entry contained invalid function name!") {}	

	Err_InvalidParans::Err_InvalidParans() : Error("Entry contained a parantheses error!") {}	

	Err_InvalidOperator::Err_InvalidOperator() : Error("Entry contained an operator error!") {}	

	Err_DivByZero::Err_DivByZero() : Error("Entry contained an attempt to divide by zero!") {}	

	Err_InvalidBase::Err_InvalidBase() : Error("Entry contained an invalid base!") {}
	
	Err_FileNotOpen::Err_FileNotOpen() : Error("File opening error!") {}

	Err_NoErr::Err_NoErr() {}
	
	// Overloaded '<<' operator
	// ---------------------------------------------------------------------------------------------------------

	std::ostream& operator<< (std::ostream &os, Error &t) { os << t.GetErrMessage(); return os; }
}