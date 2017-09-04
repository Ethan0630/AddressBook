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

		std::string Name;
		void Sort();
	public:
		// Constructor
		AddressBook();															// constructor - default constructor
		AddressBook(std::string name);											// constructor - complex, take filestream objects

		void Add(const Contact& c);												// edit - add contact to book
		void Edit(int id, Contact& c);											// edit - edit existing contact
		void Delete(int id);													// edit - delete existing contact
		bool Save();															// file - save
		bool Read();															// file - read
		bool DupName(std::string input, int exception = 0);
		int First();
		vector<Contact> List();
		vector<Contact> Search(std::string keyword);
		int SearchDname(std::string dname);
		vector<Contact> VIPRole();
		Contact Find(int id);
		std::string GetName();
		void ChangeName(std::string name);
	};
}