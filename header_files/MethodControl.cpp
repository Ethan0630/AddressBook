#include "MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace std;
using namespace address_book;

namespace methods
{
	Route RunRoute(Route route, AddressBook& book, int& current)
	{
		switch(route)
		{
		case MAIN: return Main();
		case CREATE: return Create(book, current);
		case ADD_CHARAS: return AddCharas(book, current);
		case LIST: return List(book, current);
		case DISPLAY: return Display(book, current);
		}
	}

	Route Main()
	{
		str_manip::ClearScreen();
		Menu MainMenu = Menu({ "Contact List", "New Contact", "Search Contacts", "Save & Exit" }, "Main Menu: ");
		MainMenu.RunMenu();
		switch (MainMenu.Curr_Selection())
		{
		case 1: return LIST;
		case 2: return CREATE;
		case 3: return SEARCH;
		case 4: return EXIT;
		}
	}

	Route Create(AddressBook& book, int& current)
	{
		Contact NewContact = Contact();
		vector<string> Inputs = vector<string>();
		string Input = "";
		string Prompts[3] = { "First Name", "Last Name", "Display Name (optional)" };
		str_manip::ClearScreen();
		cout << "Create New Contact\n";
		for(int i = 0; i < 4; i++)
		{
			if (i < 4)
			{
				cout << "Enter " << Prompts[i] << ": ";
				getline(cin, Input);
				str_manip::Str_RemoveChar(Input, ' ');
				if (Input.empty())
				{
					cout << Prompts[i] << " cannot be empty!\n";
					continue;
				}
				Inputs.push_back(Input);
			}
			else
			{
				Menu VIP_Menu = Menu({"Yes", "No"}, "Add this contact to the VIP role?");
				VIP_Menu.RunMenu();
				Inputs.push_back((VIP_Menu.Curr_Selection() - 1) ? "false": "true" );
			}
		}
		NewContact = Contact(Inputs[0], Inputs[1], Inputs[2], (Inputs[3] == "true"));
		current = NewContact.GetID();
		book.Add(NewContact);
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add some characteristics?");
		AddCharas.RunMenu();
		if (AddCharas.Curr_Selection() == 1) return ADD_CHARAS;
		return MAIN;
	}

	Route AddCharas(AddressBook& book, int& current)
	{
		Contact CurrentContact = book.Find(current);
		vector<string> Inputs = vector<string>();
		string Input = "";	
		str_manip::ClearScreen();
		cout << "Add Characteristic to " << CurrentContact.GetDisplayName() << "\n";
		for (int i = 0; i < 2; i++)
		{
			if (!i) cout << "Enter Attribute Name: ";
			else cout << "Enter " << Input << ": ";
			getline(cin, Input);
			str_manip::Str_RemoveChar(Input, ' ');
			if (Input.empty())
			{
				cout << "Input cannot be empty!\n";
				continue;
			}
			Inputs.push_back(Input);
		}
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add another characteristic?");
		AddCharas.RunMenu();
		if (AddCharas.Curr_Selection() == 1) return ADD_CHARAS;
		return MAIN;
	}

	Route List(AddressBook& book, int& current)
	{
		str_manip::ClearScreen();
		vector<vector<Contact>> ContactList = book.List();
		static int page_idx = 0;
		string PageHeading = "Contact List : Page " + to_string(page_idx + 1) + " of " + to_string(ContactList.size());
		vector<string> CurrentPage = vector<string>();
		for (Contact c : ContactList[page_idx]) CurrentPage.push_back(c.GetDisplayName());
		if (ContactList.size() == 1) CurrentPage.push_back("Back to Main Menu");
		if (!page_idx) CurrentPage.insert(CurrentPage.end(), { "Next Page", "Jump to Page", "Back to Main Menu" });
		else if (page_idx == (ContactList.size() - 1)) CurrentPage.insert(CurrentPage.end(), { "Previous Page", "Jump to Page", "Back to Main Menu" });
		else CurrentPage.insert(CurrentPage.end(), { "Next Page", "Previous Page", "Jump to Page", "Back to Main Menu" });
		Menu ContactListMenu = Menu(CurrentPage, PageHeading);
		int Choice = ContactListMenu.RunMenu();
		if (Choice < ContactList[page_idx].size())
		{
			current = ContactList[page_idx][Choice - 1].GetID();
			return DISPLAY;
		}
		if (Choice == CurrentPage.size()) return MAIN;
		else if (Choice == (CurrentPage.size() - 1))
		{
			int Temp = page_idx;
			cout << "Enter Page Number (1-" << ContactList.size() << "): ";
			while (1)
			{
				std::string Input;
				getline(cin, Input);
				str_manip::Str_RemoveChar(Input, ' ');
				bool isNum = true;
				for (char c : Input) { if (!isdigit(c)) isNum = false; break; }
				if (!isNum) { cout << "Entry must be integral!\nTry again: "; continue; }
				else Temp = stoi(Input);
				if (!((Temp >= 1) && (Temp <= ContactList.size()))) { cout << "Entry was out of range!\nTry again: "; continue; }
				page_idx = Temp;
				return LIST;
			}
		}
		else if ((Choice == (ContactList[page_idx].size() + 1)) && (page_idx != (ContactList.size() - 1)))
		{
			page_idx++;
			return LIST;
		}
		else
		{
			page_idx--;
			return LIST;
		}
	}

	Route Display(AddressBook& book, int& current)
	{
		return MAIN;
	}
}