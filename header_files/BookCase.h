#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Contact.h"
#include "AddressBook.h"
#include "StringFunctions.h"

using namespace contact;
using namespace address_book;

namespace book_case
{
	class BookCase																// Book Case
	{
	private:
		// Fields
		std::vector<AddressBook> Books;											// field - contact_list

		int CurrentBook;
		std::string Path;
		void Sort();
	public:
		// Constructor
		BookCase();																// constructor - default constructor
		BookCase(std::string path);												// constructor - complex, take file path

		void Add(const AddressBook& b);											// edit - add book to case
		void Edit(string dname, AddressBook& b);								// edit - edit existing book
		void Delete(string dname);												// edit - delete existing book
		void ChangeCurrent(string dname);
		bool Save();															// file - save
		bool Read();															// file - read
		bool DupName(std::string input, std::string exception = "");
		vector<AddressBook> List();
		vector<Contact> Search(std::string keyword);
		AddressBook GetCurrent();
	};
}