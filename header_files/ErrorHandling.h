#pragma once

#include <string>
#include <iosfwd>
#include <iostream>

namespace error_handling
{
	class Error																								// error-handling class
	{																										//
	public:																									// public stuff:
		// Constructors																						//
		Error();																							// default empty no error
		Error(std::string msg);																				// error with custom message
																											//
		// Operator Overloading																				//
		explicit operator bool() const;																		// return whether or not object is an error
		bool operator !() const;																			// return whether or not object is an error
																											//
		// Public Methods																					//
		void ChangeMessage(std::string msg);																// function to set a custom message for error
		const std::string& GetErrMessage();																	// returns Message string
		void DisplayMessage();																				// displays message with endl
																											//
																											//
	private:																								// private stuff:
																											//
		bool isError;																						// type of error
		std::string Message;																				// message for object
	};																										//

	class Err_Empty : public Error { public: Err_Empty(); };
	class Err_InvalidInput : public Error { public: Err_InvalidInput(); };
	class Err_SpecChar : public Error { public: Err_SpecChar(char c); };
	class Err_InvalidChar : public Error { public: Err_InvalidChar(); };
	class Err_OutOfRange : public Error { public: Err_OutOfRange(); };
	class Err_InvalidFuncName : public Error { public: Err_InvalidFuncName(); };
	class Err_InvalidParans : public Error { public: Err_InvalidParans(); };
	class Err_InvalidOperator : public Error { public: Err_InvalidOperator(); };
	class Err_DivByZero : public Error { public: Err_DivByZero(); };
	class Err_InvalidBase : public Error { public: Err_InvalidBase(); };
	class Err_FileNotOpen : public Error { public: Err_FileNotOpen(); };
	class Err_NoErr : public Error { public: Err_NoErr(); };

	std::ostream& operator<< (std::ostream &os, Error &t);
}