#include "AddressBook.h"

using namespace contact;
using namespace error_handling;

namespace address_book
{
	void AddressBook::Sort()
	{
		std::sort(Contacts.begin(), Contacts.end(),								
			[](Contact l_val, Contact r_val)										
		{																						
			return str_manip::Str_ToLower(l_val.GetDisplayName()) < str_manip::Str_ToLower(r_val.GetDisplayName());
		});
	}

	AddressBook::AddressBook()
	{
		Contacts = std::vector<Contact>();
		Name = "";
	}

	AddressBook::AddressBook(std::string name)
	{
		Contacts = std::vector<Contact>();
		Name = name;
	}

	void AddressBook::Add(const Contact& c)
	{
		Contacts.push_back(c);
		Sort();
	}

	void AddressBook::Edit(int id, Contact& c)
	{
		auto val = std::find_if(std::begin(Contacts), std::end(Contacts), 
			[id](Contact C)
			{
				return C.GetID() == id;
		});
		val->Edit(c);
		Sort();
	}

	void AddressBook::Delete(int id)
	{
		Contacts.erase(std::remove_if(Contacts.begin(), Contacts.end(), [id](Contact C)
			{
				return C.GetID() == id;
		}), Contacts.end());
	}

	bool AddressBook::Save()
	{
		std::string path = Name + ".book";
		std::ofstream Out = ofstream(path);
		if (!Out) { return false; }
		for (Contact c : Contacts)
		{
			Out << c.FileFormat() << "\n";
		}
		Out.close();
		return true;
	}

	bool AddressBook::Read()
	{
		std::string path = Name + ".book";
		std::ifstream In = ifstream(path);
		if (!In) { return false; }
		Contacts.clear();		
		std::string InputLine;
		while (std::getline(In, InputLine))
		{
			std::vector<std::string> InputVec = str_manip::Str_SplitByChar(InputLine, '|');
			bool vip = (InputVec[3] == "true") ? true : false;
			Contact CurrentContact = Contact(InputVec[0], InputVec[1], InputVec[2], vip);
			std::string Temp;
			for (int i = 4; i < InputVec.size(); i++)
			{
				if (i % 2) CurrentContact.AddAttr(Temp, InputVec[i]);
				else Temp = InputVec[i];				
			}
			Contacts.push_back(CurrentContact);
		}
		Sort();
	}

	bool AddressBook::DupName(std::string input, int exception)
	{
		auto it = std::find_if(Contacts.begin(), Contacts.end(), [input](Contact c) {
			return c.GetDisplayName() == input;
		});
		if (it == Contacts.end()) return false;
		else if (!exception) return true;
		else if (it->GetID() == exception)
		{
			return std::find_if(it + 1, Contacts.end(), [input](Contact c) {
				return c.GetDisplayName() == input;
			}) != Contacts.end();
		}
		else return true;
	}

	int AddressBook::First()
	{
		if (Contacts.empty()) return 0;
		return Contacts[0].GetID();
	}

	std::vector<Contact> AddressBook::List()
	{
		return Contacts;
	}

	std::vector<Contact> AddressBook::Search(std::string input)
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
	}

	int AddressBook::SearchDname(std::string dname)
	{
		for (Contact c : Contacts) if (c.GetDisplayName() == dname) return c.GetID();
		return -1;
	}

	std::vector<Contact> AddressBook::VIPRole()
	{
		std::vector<Contact> Results = std::vector<Contact>();
		for (Contact c : Contacts)
		{
			if (c.isVIP()) Results.push_back(c);
		}
		return Results;
	}

	Contact AddressBook::Find(int id)
	{
		vector<Contact>::iterator Results = std::find_if(std::begin(Contacts), std::end(Contacts),
			[id](Contact C)
		{
			return C.GetID() == id;
		});
		return (Results == Contacts.end()) ? Contact() : *Results;
	}

	std::string AddressBook::GetName() { return Name; }

	void AddressBook::ChangeName(std::string name) { Name = name; }
}