#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Contact.h"
#include "ErrorHandling.h"
#include "StringFunctions.h"

using namespace contact;

namespace address_book
{
	class AddressBook															// Address Book
	{
	private:
		// Fields
		std::vector<Contact> Contacts;											// field - contact_list

		std::string Path;
		
		/*std::vector< std::vector<std::string> > BuildSplitList();				// method - split contact_list into groups of ten
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
			std::vector<std::string> commands = std::vector<std::string>());*/
	public:
		// Constructor
		AddressBook();															// constructor - default constructor
		AddressBook(std::string path);											// constructor - complex, take filestream objects

		void Add(const Contact& c);												// edit - add contact to book
		void Edit(int id, Contact& c);											// edit - edit existing contact
		void Delete(int id);													// edit - delete existing contact
		bool Save();															// file - save
		bool Read();															// file - read
		int First();
		vector<Contact> List();
		vector<Contact> Search(std::string keyword);
		vector<Contact> VIPRole();
		Contact Find(int id);													

		/*
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
		*/
	};
}