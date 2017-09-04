#include "BookCase.h"

using namespace contact;
using namespace book_case;
using namespace error_handling;

namespace book_case
{
	void BookCase::Sort()
	{
		std::sort(Books.begin(), Books.end(),
			[](AddressBook l_val, AddressBook r_val)
		{
			return str_manip::Str_ToLower(l_val.GetName()) < str_manip::Str_ToLower(r_val.GetName());
		});
	}

	BookCase::BookCase()
	{
		Books = std::vector<AddressBook>();
		CurrentBook = 0;
		Path = "";
	}

	BookCase::BookCase(std::string path)
	{
		Books = std::vector<AddressBook>();
		CurrentBook = 0;
		Path = path;
	}

	void BookCase::Add(const AddressBook& b)
	{
		Books.push_back(b);
		Sort();
	}

	void BookCase::Edit(string dname, AddressBook& b)
	{
		auto val = std::find_if(std::begin(Books), std::end(Books),
			[dname](AddressBook B)
		{
			return B.GetName() == dname;
		});
		val->ChangeName(b.GetName());
		Sort();
	}

	void BookCase::Delete(string dname)
	{
		Books.erase(std::remove_if(Books.begin(), Books.end(), [dname](AddressBook B)
		{
			return B.GetName() == dname;
		}), Books.end());
	}

	void BookCase::ChangeCurrent(string dname)
	{
		CurrentBook = std::distance(Books.begin(), std::find_if(std::begin(Books), std::end(Books),
			[dname](AddressBook B)
		{
			return B.GetName() == dname;
		}));
	}

	bool BookCase::Save()
	{
		std::ofstream Out = ofstream(Path);
		if (!Out) { return false; }
		for (int i = 0; i < Books.size(); i++)
		{
			string isCurrent = (i == CurrentBook) ? "-t" : "-f";
			Out << Books[i].GetName() << '|' << isCurrent << "\n";
		}
		Out.close();
		return true;
	}

	bool BookCase::Read()
	{
		std::ifstream In = ifstream(Path);
		if (!In) { return false; }
		Books.clear();
		std::string InputLine;
		int idx = 0;
		while (std::getline(In, InputLine))
		{
			std::vector<std::string> InputVec = str_manip::Str_SplitByChar(InputLine, '|');
			if (InputVec[1] == "-t") CurrentBook = idx;
			Books.push_back(AddressBook(InputVec[0]));
			idx++;
		}
		Sort();
	}

	bool BookCase::DupName(std::string input, std::string exception)
	{
		if (input == exception) return false;
		return std::find_if(Books.begin(), Books.end(), [input](AddressBook b) {
			return b.GetName() == input;
		}) != Books.end();
	}

	std::vector<AddressBook> BookCase::List()
	{
		return Books;
	}

	/*std::vector<Contact> BookCase::Search(std::string input)
	{
		std::vector<Contact> Results = std::vector<Contact>();
		for (Contact c : Contacts)
		{
			if (c.GetDisplayName().find(input) != std::string::npos) Results.push_back(c);
			else if (c.GetFirstName().find(input) != std::string::npos) Results.push_back(c);
			else if (c.GetLastName().find(input) != std::string::npos) Results.push_back(c);
			else if (c.AttrContainsNeedle(input) != -1) Results.push_back(c);
		}
		return Results;
	}*/

	AddressBook BookCase::GetCurrent()
	{
		return Books[CurrentBook];
	}
}