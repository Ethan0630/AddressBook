#include "AddressBook.h"

using namespace contact;
using namespace error_handling;
using namespace menu;

namespace address_book
{
	std::vector< std::vector<std::string> > AddressBook::BuildSplitList()
	{
		std::vector< std::vector<std::string> > SplitContacts = std::vector<std::vector<std::string>>();
		std::vector<std::string> Page = std::vector<std::string>();
		Page.reserve(10);

		for (int i = 0; i < Contacts.size(); i++)
		{
			Page.push_back(Contacts[i].GetName());
			if (!((i + 1) % 10))
			{
				SplitContacts.push_back(Page);
				Page.clear();
			}
		}
		if (!(Page.empty())) SplitContacts.push_back(Page);
		return SplitContacts;
	}

	int AddressBook::GetJumpNum(int num_pgs)
	{
		while (1)
		{
			std::string Input;
			Error* Err = new Err_NoErr();
			std::cout << "Enter page number (1-" << num_pgs << "): ";
			std::getline(std::cin, Input);
			str_manip::Str_RemoveChar(Input, ' ');
			int Result = str_manip::StrToInt(Input, Err);
			if (*Err) { Err->DisplayMessage(); continue; }
			if (!((Result >= 1) && (Result <= num_pgs))) {
				std::cout << "Entry must be between 1 and " << num_pgs << "!"; continue; }
			return Result;
		}
	}

	void AddressBook::UpdateCharacteristic(Contact& c, int index, std::vector<std::string> commands)
	{
		str_manip::ClearScreen();
		std::cout << "Update Characteristic\n";
		std::cout << c.GetCharacteristics()[index].first 
					<< ": " << c.GetCharacteristics()[index].second << "\n";
		Menu UpdateMenu = Menu({ "Change Name", "Change Value", "Change Both", "Remove", "Cancel" }, "Options:");
		UpdateMenu.RunMenu();
		if (UpdateMenu.Curr_Selection() == 4) { c.RemoveChar(index); return; }
		if (UpdateMenu.Curr_Selection() == 5) { return; }
		std::pair<std::string, std::string> InputPair;
		if (UpdateMenu.Curr_Selection() == 1) InputPair.second = c.GetCharacteristics()[index].second;
		if (UpdateMenu.Curr_Selection() == 2) InputPair.first = c.GetCharacteristics()[index].first;
		if ((UpdateMenu.Curr_Selection() == 1) || (UpdateMenu.Curr_Selection() == 3))
		{
			InputPair.first = RunCharacteristicInput(c, index, commands);
		}
		if ((UpdateMenu.Curr_Selection() == 2) || (UpdateMenu.Curr_Selection() == 3))
		{
			InputPair.second = RunDataInput(c, InputPair.first, commands);
		}
		c.UpdateChar(index, InputPair.first, InputPair.second);
	}

	void AddressBook::AddCharacteristic(Contact& c, std::vector<std::string> commands)
	{
		str_manip::ClearScreen();
		std::pair<std::string, std::string> InputPair;
		std::string Input;

		std::cout << "Add Characteristic:\n";
		InputPair.first = RunCharacteristicInput(c, -1, commands);
		InputPair.second = RunDataInput(c, InputPair.first, commands);
		c.AddChar(InputPair.first, InputPair.second);
	}

	std::string AddressBook::RunNameInput(std::vector<std::string> commands)
	{
		std::string Rounds[2] = { "first", "second" };
		std::string Result = "";
		for (int i = 0; i < 2; i++)
		{
			while (1)
			{
				std::string Input;
				Error* Err = new Err_NoErr();
				std::cout << "Enter " << Rounds[i] << " name: ";
				std::getline(std::cin, Input);
				if (std::find(commands.begin(), commands.end(), Input) != commands.end()) return Input;
				if (Input.empty()) Err = new Err_Empty();
				if (Input.find('|') != std::string::npos) Err = new Err_SpecChar('|');
				if (*Err) { Err->DisplayMessage(); continue; }
				Result += Input;
				if (!i) Result += "|";
				break;
			}
		}
		return Result;
	}

	std::string AddressBook::RunCharacteristicInput(Contact& c, int index,
		std::vector<std::string> commands)
	{
		while (1)
		{
			std::string Input;
			Error* Err = new Err_NoErr();
			std::cout << "Name Characteristic (Cell, Work Email): ";
			std::getline(std::cin, Input);
			if (std::find(commands.begin(), commands.end(), Input) != commands.end()) return Input;
			if (Input.empty()) Err = new Err_Empty();
			else if (c.hasCharacteristic(Input, index))
				Err = new Error("Cannot repeat a Characteristic name!");
			else if (Input.find('|') != std::string::npos) Err = new Err_SpecChar('|');
			if (*Err) { Err->DisplayMessage(); continue; }
			return Input;
		}
	}

	std::string AddressBook::RunDataInput(Contact& c, std::string characteristic, 
		std::vector<std::string> commands)
	{
		while (1)
		{
			Error* Err = new Err_NoErr();
			std::string Input;
			std::cout << characteristic << ": ";
			std::getline(std::cin, Input);
			if (std::find(commands.begin(), commands.end(), Input) != commands.end()) return Input;
			if (Input.empty()) Err = new Err_Empty();
			else if (Input.find('|') != std::string::npos) Err = new Err_SpecChar('|');
			if (*Err) { Err->DisplayMessage(); continue; }
			return Input;
		}
	}

	AddressBook::AddressBook()
	{
		Contacts = std::vector<Contact>();
	}

	AddressBook::AddressBook(std::ifstream& in_file)
	{
		Contacts = std::vector<Contact>();
		Read(in_file);
	}
	
	int AddressBook::ViewContact(int index)
	{
		str_manip::ClearScreen();
		Contacts[index].Display();
		std::cout << "\n------------------------------------------------------------\n";
		Menu ViewMenu = Menu({ "Edit", "Delete", "Back to list", "Back to Main Menu" }, "Options");
		ViewMenu.RunMenu();
		switch (ViewMenu.Curr_Selection())
		{
		case 1: return EditContact(index);
		case 2: Contacts.erase(Contacts.begin() + index); return 1;
		case 3: return 1;
		case 4: return 2;
		}
	}

	int AddressBook::EditContact(int index)
	{
		str_manip::ClearScreen();
		Menu EditMenu = Menu({ Contacts[index].GetName() }, "Edit Contact:", 
			"Enter number of info you want to change or action you want to take: ");
		for (std::pair<std::string, std::string> Val : Contacts[index].GetCharacteristics())
		{
			EditMenu.AddContent(Val.first + ": " + Val.second);
		}
		EditMenu.AddContent({ "Add Data", "Delete Contact", "Go To List", "Back to Main Menu" });
		EditMenu.RunMenu();
		if (EditMenu.Curr_Selection() == 1)
		{
			std::vector<std::string> NameVec = str_manip::Str_SplitByChar(RunNameInput(), '|');
			Contacts[index].ChangeFirst(NameVec[0]);
			Contacts[index].ChangeLast(NameVec[1]);
			
		}
		if (EditMenu.Curr_Selection() <= (EditMenu.NumOfSelections() - 4))
		{
			UpdateCharacteristic(Contacts[index], (EditMenu.Curr_Selection() - 2));
		}
		else if (EditMenu.Curr_Selection() == (EditMenu.NumOfSelections() - 3))
		{
			AddCharacteristic(Contacts[index]);
		}
		else if (EditMenu.Curr_Selection() == (EditMenu.NumOfSelections() - 2))
		{
			Contacts.erase(Contacts.begin() + index); return 1;
		}
		else if (EditMenu.Curr_Selection() == (EditMenu.NumOfSelections() - 1))
		{
			return 1;
		}
		else if (EditMenu.Curr_Selection() == EditMenu.NumOfSelections())
		{
			return 2;
		}
	}

	int AddressBook::NewContact()
	{
		str_manip::ClearScreen();
		Error* Err;
		std::cout << "Create new contact\n";
		std::cout << "Note: All inputs are stored \"as-is\", spaces included\n";
		bool isRepeating= true;
		std::vector<std::string> NameVec;
		while (isRepeating)
		{
			NameVec = str_manip::Str_SplitByChar(RunNameInput(), '|');
			int idx = 0;
			for (; idx < Contacts.size(); idx++)
			{
				 if (Contacts[idx].hasFirstName(NameVec[0]) && Contacts[idx].hasLastName(NameVec[1]))
				 {
				 	
				 }
			}
		}
		Contact C = Contact(NameVec[0], NameVec[1]);
		Menu M = Menu({ "Yes", "No" }, "Would you like to add some characteristics?");
		M.RunMenu();
		M.UpdateHeading("Again?");
		while (M.Curr_Selection() == 1)
		{
			AddCharacteristic(C);
			M.RunMenu();
		}
		Contacts.push_back(C); 
		return 1;
	}

	void AddressBook::Save(std::ofstream& out_file)
	{
		for (Contact c : Contacts)
		{
			out_file << c.FileFormat() << "\n";
		}
		out_file.close();
	}
	
	void AddressBook::Read(std::ifstream& in_file)
	{
		std::string InputLine;
		while (std::getline(in_file, InputLine))
		{
			std::vector<std::string> InputVec = str_manip::Str_SplitByChar(InputLine, '|');
			bool vip = (InputVec[3] == "true") ? true : false;
			Contact CurrentContact = Contact(InputVec[1], InputVec[2], InputVec[0], vip);
			if (InputVec.size() == 2) continue;
			std::string Temp;
			for (int i = 2; i < InputVec.size(); i++)
			{
				(!(i % 2)) ? (Temp = InputVec[i]) : CurrentContact.AddChar(Temp, InputVec[i]);
			}
			Contacts.push_back(CurrentContact);
		}
	}

	int AddressBook::Display()
	{
		std::vector< std::vector<std::string> > SplitContacts;
		Menu PageMenu = Menu();
		int PageNum = 1;
		while (1)
		{
			str_manip::ClearScreen();
			if (Contacts.empty()) return 1;
			SplitContacts = BuildSplitList();
			PageMenu = Menu(SplitContacts[PageNum - 1], "Contacts: Page " + std::to_string(PageNum) 
				+ " of " + std::to_string(SplitContacts.size()));
			if (PageNum != SplitContacts.size()) PageMenu.AddContent("Next page");
			if (PageNum != 1) PageMenu.AddContent("Previous page");
			PageMenu.AddContent("Jump to...");
			PageMenu.AddContent("Back to Main Menu");
			PageMenu.RunMenu();
			if (PageMenu.Curr_Selection() <= SplitContacts[PageNum - 1].size())
			{
				int ReturnType = ViewContact(((PageNum - 1) * 10) + (PageMenu.Curr_Selection() - 1));
				if (ReturnType == 1) continue;
				else if (ReturnType == 2) return 1;
			}
			else if (PageMenu.Curr_Selection() == PageMenu.NumOfSelections())
			{
				return 1;
			}
			else if (PageMenu.Curr_Selection() == (PageMenu.NumOfSelections() - 1))
			{
				PageNum = GetJumpNum(SplitContacts.size());
				continue;
			}
			else if ((PageMenu.Curr_Selection() == (PageMenu.NumOfSelections() - 2)) && (PageNum != 1))
			{
				PageNum--;
				continue;
			}
			else
			{
				PageNum++;
				continue;
			}
		}
	}

	int AddressBook::Search()
	{
		str_manip::ClearScreen();
		std::string Input;
		std::cout << "Search (1 whole word only, spaces will be removed): ";
		std::getline(std::cin, Input);
		str_manip::Str_RemoveChar(Input, ' ');
		std::vector<Contact> SearchResults;
		for (Contact c : Contacts)
		{
			if (c.hasFirstName(Input) || c.hasLastName(Input) || c.hasCharacteristic(Input) || c.hasValue(Input))
				SearchResults.push_back(c);
		}
		int i = 0;
		for (i; i < SearchResults.size(); i++) 
		{ 
			std::cout << (i + 1) << ". ";
			SearchResults[i].Display(); 
			std::cout << "\n";
		}
		std::cout << (i + 1) << ". Back to Main";
		int choice;
		while (1)
		{
			std::string Input;
			std::getline(std::cin, Input);
			str_manip::Str_RemoveChar(Input, ' ');
			bool isNum = true;
			for (char c : Input) { if (!isdigit(c)) isNum = false; break; }
			if (!isNum) { std::cout << "Entry must be integral!\nTry again: "; continue; }
			else choice = stoi(Input);
			if (!((choice >= 1) && (choice <= (i + 1)))) { std::cout << "Entry was not a selection!\nTry again: "; continue; }
			break;
		}
		return 1;
	}

	bool CheckDup(std::string input)
	{
		for (Contact c : Contacts)
			if (c.isDuplicateName(input)) return true;
		return false;
	}

	void AddContact(const Contact& c)
	{
		Contacts.push_back(c);
	}
}

