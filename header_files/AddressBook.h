#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Contact.h"
#include "ErrorHandling.h"
#include "StringFunctions.h"
#include "Menus.h"

using namespace contact;

namespace address_book
{
	class AddressBook															// Address Book
	{
	private:
		// Fields
		std::vector<Contact> Contacts;											// field - contact_list

		std::vector< std::vector<std::string> > BuildSplitList();				// method - split contact_list into groups of ten
		int GetJumpNum(int num_pgs);											// menu method - get a jump to page number
		void UpdateCharacteristic(Contact& c, int index,
			std::vector<std::string> commands = std::vector<std::string>());	// method - update contact characteristic
		void AddCharacteristic(Contact& chosen_contact,
			std::vector<std::string> commands = std::vector<std::string>());
		std::string AddressBook::RunNameInput(
			std::vector<std::string> commands = std::vector<std::string>());
		std::string RunCharacteristicInput(Contact& c, int index, 
			std::vector<std::string> commands = std::vector<std::string>());
		std::string RunDataInput(Contact& c, std::string characteristic,
			std::vector<std::string> commands = std::vector<std::string>());
	public:
		// Constructor
		AddressBook();															// constructor - default, takes nothing
		AddressBook(std::ifstream& in_file);									// constructor - simple, take input filestream object to read in contacts

		// Edit Contacts
		int ViewContact(int index);												// view/edit - get to contact info with editing options
		int NewContact();														// edit - new contact (plus sort)
		int EditContact(int index);												// edit - edit contact (plus sort if name change)

		// File Operations
		void Save(std::ofstream& out_file);										// file - save
		void Read(std::ifstream& in_file);										// file - read

		// Output List
		int Display();															// method - display contact_list

		// Searching
		int Search();															// search - direct searching

		bool CheckDup(std::string input);

		void AddContact(const Contact& c);
	};
}