#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Menus.h"

namespace contact
{
	class Contact																				// Contact
	{
	private:
		// Fields
		std::string FirstName, LastName;														// field - name
		menu::Menu ContactInfoMenu;																// field - menu 
		std::vector<std::pair<std::string, std::string>> Characteristics;						// field - characteristics

		// Private Methods
		void SortData();																		// method - sort characteristics
		void UpdateContactInfoMenu();															// method - update contact menu
	public:
		// Constructors
		Contact();																				// constructor - default, takes nothing
		Contact(std::string first_name, std::string last_name);									// constructor - simple, takes name
		Contact(std::string first_name, std::string last_name, 
			std::vector<std::pair<std::string, std::string>> Characteristics);					// constructor - complex, takes name and data (assumed to be sorted)
		
		// Update name
		void ChangeFirst(std::string first_name);												// mutable - change f_name
		void ChangeLast(std::string last_name);													// mutable - change l_name

		// Edit data
		void AddChar(std::string characteristic, std::string value);							// mutable - add characteristic (plus sort)
		void UpdateChar(int index, 
			std::string new_characteristic, std::string value);									// mutable - update characteristic (plus sort)
		void RemoveChar(int index);																// mutable - remove characteristic 

		// Output Contact
		void Display();																			// method - display contact info
		std::string FileFormat();																// method - return info in file format

		// Accessors
		std::string GetName();																	// accessor - return name
		std::vector<std::pair<std::string, std::string>> GetCharacteristics();					// accessor - return characteristics

		// Tests for searching
		bool hasFirstName(std::string first_name);												// test - f_name 
		bool hasLastName(std::string last_name);												// test - l_name
		bool hasName(std::string name);															// test - name
		bool hasCharacteristic(std::string characteristic, int index = -1);						// test - characteristic
		bool hasValue(std::string value);														// test - value
		bool hasCharacteristicValue(std::string characteristic, std::string value);				// test - characteristic + value
	};
}