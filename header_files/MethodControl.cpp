#include "MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace std;
using namespace address_book;

namespace methods
{
	bool isCommand(std::string input)
	{
		for (std::string cmd : SpecialEntries)
		{
			if (!strncmp(input.c_str(), cmd.c_str(), cmd.size()))
			{
				return true;
			}
		}
		return 0;
	}

	Route RunRoute(Route route, AddressBook& book, int& current)
	{
		switch (route)
		{
		case MAIN: return Main(book, current);
		case CREATE: return Create(book, current);
		case ADD_CHARAS: return AddCharas(book, current);
		case LIST: return List(book, current);
		case DISPLAY: return Display(book, current);
		case REMOVE: return Remove(book, current);
		case EDIT: return Edit(book, current);
		}
	}

	Route RunCommand(std::string& input, AddressBook& book, int& current)
	{
		if (input == "/clear")
		{
			str_manip::ClearScreen();
		}
		if (input == "/current")
		{
			std::cout << "Command Output " << book.Find(current).GetDisplayName() << endl;
			std::cout << "Press [Enter] to continue ";
			std::cin.ignore();
			str_manip::ClearScreen();
		}
		else if (Commands.find(input) != Commands.end()) return Commands.at(input);
		return NONE;
	}


	Route Main(AddressBook& book, int& current)
	{
		str_manip::ClearScreen();
		Menu MainMenu = Menu({ "Contact List", "New Contact", "Search Contacts", "Save & Exit" }, "Main Menu: ", SpecialEntries);
		std::string Input;
		if (!MainMenu.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) return MAIN;
			return CmdRoute;
		}
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
		for (int i = 0; i < 4;)
		{
			if (i < 3)
			{
				cout << "Enter " << Prompts[i] << ": ";
				getline(cin, Input);
				if (isCommand(Input))
				{
					Route CmdRoute = RunCommand(Input, book, current);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				if (Input.empty())
				{
					if (i != 2)
					{
						cout << Prompts[i] << " cannot be empty!\n";
						continue;
					}
					else Input = Inputs[0] + " " + Inputs[1];
				}
				Inputs.push_back(Input);
			}
			else
			{
				Menu VIP_Menu = Menu({ "Yes", "No" }, "Add this contact to the VIP role?", SpecialEntries);
				if (!VIP_Menu.RunMenu(Input))
				{
					Route CmdRoute = RunCommand(Input, book, current);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				(VIP_Menu.Curr_Selection() == 1) ? Inputs.push_back("true") : Inputs.push_back("false");
			}
			i++;
		}
		NewContact = Contact(Inputs[0], Inputs[1], Inputs[2], (Inputs[3] == "true"));
		current = NewContact.GetID();
		book.Add(NewContact);
		cout << "New Contact Created!\n";
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add some characteristics?", SpecialEntries);
	Start_Menu:
		if (!AddCharas.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
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
			if (isCommand(Input))
			{
				Route CmdRoute = RunCommand(Input, book, current);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}
			if (Input.empty())
			{
				cout << "Input cannot be empty!\n";
				continue;
			}
			Inputs.push_back(Input);
		}
		CurrentContact.AddAttr(Inputs[0], Inputs[1]);
		book.Edit(current, CurrentContact);
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add another characteristic?", SpecialEntries);
		Start_Menu:
		if (!AddCharas.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
		if (AddCharas.Curr_Selection() == 1) return ADD_CHARAS;
		return MAIN;
	}

	Route List(AddressBook& book, int& current)
	{
		str_manip::ClearScreen();
		vector<vector<Contact>> ContactList = book.List();
		static int page_idx = 0;
		string Input = "";
		string PageHeading = "Contact List : Page " + to_string(page_idx + 1) + " of " + to_string(ContactList.size());
		vector<string> CurrentPage = vector<string>();
		for (Contact c : ContactList[page_idx]) CurrentPage.push_back(c.GetDisplayName());
		if (ContactList.size() == 1) CurrentPage.push_back("Back to Main Menu");
		else if (!page_idx) CurrentPage.insert(CurrentPage.end(), { "Next Page", "Jump to Page", "Back to Main Menu" });
		else if (page_idx == (ContactList.size() - 1)) CurrentPage.insert(CurrentPage.end(), { "Previous Page", "Jump to Page", "Back to Main Menu" });
		else CurrentPage.insert(CurrentPage.end(), { "Next Page", "Previous Page", "Jump to Page", "Back to Main Menu" });
		Menu ContactListMenu = Menu(CurrentPage, PageHeading, SpecialEntries);
		Start_Menu:
		if (!ContactListMenu.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
		int Choice = ContactListMenu.Curr_Selection();
		if (Choice <= ContactList[page_idx].size())
		{
			page_idx = 0;
			current = ContactList[page_idx][Choice - 1].GetID();
			return DISPLAY;
		}
		if (Choice == CurrentPage.size())
		{
			page_idx = 0;
			return MAIN;
		}
		else if (Choice == (CurrentPage.size() - 1))
		{
			int Temp = page_idx;
			while (1)
			{
				cout << "Enter Page Number (1-" << ContactList.size() << "): ";
				getline(cin, Input);
				if (isCommand(Input))
				{
					Route CmdRoute = RunCommand(Input, book, current);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				str_manip::Str_RemoveChar(Input, ' ');
				bool isNum = true;
				for (char c : Input) { if (!isdigit(c)) isNum = false; break; }
				if (!isNum) { cout << "Entry must be integral!\n"; continue; }
				else Temp = stoi(Input);
				if (!((Temp >= 1) && (Temp <= ContactList.size()))) { cout << "Entry was out of range!\n"; continue; }
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
		Contact CurrentContact = book.Find(current);
		string Input = "";
		str_manip::ClearScreen();
		cout << CurrentContact.DisplayFormat() << endl;
		vector<string> Options = { "Edit", "Delete", "Back", "Back to Main" };
		Menu OptionsMenu = Menu(Options, "Contact Options: ", SpecialEntries);
	Start_Menu:
		if (!OptionsMenu.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
		int Choice = OptionsMenu.Curr_Selection();
		if (Choice == 4) return MAIN;
		if (Choice == 3) return LIST;
		if (Choice == 2) return REMOVE;
		else return EDIT;
	}

	Route Remove(AddressBook& book, int& current)
	{
		Contact CurrentContact = book.Find(current);
		string Input = "";
		cout << "Delete Contact\n";
		string Heading = "Are you sure you wish to delete " + CurrentContact.GetDisplayName() + "?";
		Menu SureMenu = Menu({ "Yes", "No" }, Heading, SpecialEntries);
		Start_Menu:
		if (!SureMenu.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
		if (SureMenu.Curr_Selection() == 1)
		{
			book.Delete(CurrentContact.GetID());
			current = book.First();
			return LIST;
		}
		else return DISPLAY;
	}

	Route Edit(AddressBook& book, int& current)
	{
		Contact CurrentContact = book.Find(current);
		string NameAttrs[3] = { "Display Name", "First Name", "Last Name" };
		vector<pair<string, string>> Attributes = CurrentContact.GetAttributes();
		vector<string> Options = vector<string>({
			"Display Name: " + CurrentContact.GetDisplayName(),
			"First Name: " + CurrentContact.GetFirstName(),
			"Last Name: " + CurrentContact.GetLastName(),
		});
		for (pair<string, string> Attr : Attributes)
			Options.push_back(Attr.first + ": " + Attr.second);
		if (CurrentContact.isVIP()) Options.push_back("Remove from VIP Role");
		else Options.push_back("Add to VIP Role");
		Options.push_back("Add Attributes");
		Options.push_back("Back to Display");
		Options.push_back("Back to Main");
		string Input = "";
		Menu EditMenu = Menu(Options, "Edit Contact:", SpecialEntries);
		Start_Menu:
		if (!EditMenu.RunMenu(Input))
		{
			Route CmdRoute = RunCommand(Input, book, current);
			if (CmdRoute == NONE) goto Start_Menu;
			return CmdRoute;
		}
		int Choice = EditMenu.Curr_Selection();
		if (Choice == Options.size()) return MAIN;
		if (Choice == (Options.size() - 1)) return DISPLAY;
		if (Choice == (Options.size() - 2)) return ADD_CHARAS;
		if (Choice == (Options.size() - 3))
		{
			CurrentContact.SetVIP(!CurrentContact.isVIP());
			return EDIT;
		}
		else if (Choice < 4)
		{
			Start_Edit:
			cout << "Enter " << NameAttrs[Choice - 1] << ": ";
			getline(cin, Input);
			if (isCommand(Input))
			{
				Route CmdRoute = RunCommand(Input, book, current);
				if (CmdRoute == NONE) goto Start_Edit;
				return CmdRoute;
			}
			if (Input.empty())
			{
				if (Choice != 1)
				{
					cout << NameAttrs[Choice - 1] << " cannot be empty!\n";
					goto Start_Edit;
				}
				CurrentContact.SetDisplayName(CurrentContact.GetFirstName() + " " + CurrentContact.GetLastName());
			}
			else
			{
				switch (Choice)
				{
				case 1: CurrentContact.SetDisplayName(Input); break;
				case 2: CurrentContact.SetFirstName(Input); break;
				case 3:	CurrentContact.SetLastName(Input); break;
				}
			}
			return EDIT;
		}
		else
		{
			Menu AttrMenu = Menu({ "Change Name", "Change Value", "Change Both", "Delete Attribute", "Cancel" }, Options[Choice - 4], SpecialEntries);
			Start_Attr:
			if (!AttrMenu.RunMenu(Input))
			{
				Route CmdRoute = RunCommand(Input, book, current);
				if (CmdRoute == NONE) goto Start_Attr;
				return CmdRoute;
			}
			Choice = AttrMenu.Curr_Selection();
			string Attr_Name = str_manip::Str_SplitByChar(Options[Choice - 4], ':')[0];
			if (Choice == 5) return EDIT;
			if (Choice == 4)
			{
				Menu SureMenu = Menu({ "Yes", "No" }, "Are you sure?", SpecialEntries);
				Start_Sure:
				if (!SureMenu.RunMenu(Input))
				{
					Route CmdRoute = RunCommand(Input, book, current);
					if (CmdRoute == NONE) goto Start_Sure;
					return CmdRoute;
				}
				if (SureMenu.Curr_Selection() == 1)
				{
					CurrentContact.RemoveAttr(Attr_Name);
					return EDIT;
				}
				else return EDIT;
			}
			pair<string, string> Attr = CurrentContact.GetAttr(Attr_Name);
			if (Choice == 1)
			{
				Change_Name:
				while (1)
				{
					cout << "Enter Attribute Name: ";
					getline(cin, Input);
					if (isCommand(Input))
					{
						Route CmdRoute = RunCommand(Input, book, current);
						if (CmdRoute == NONE) continue;
						return CmdRoute;
					}
					if (Input.empty())
					{
						cout << "Input cannot be empty!\n";
						continue;
					}
					break;
				}
				Attr.first = Input;
			}
			else if (Choice == 2)
			{
				Change_Val:
				while (1)
				{
					cout << "Enter " + Attr.first + ": ";
					getline(cin, Input);
					if (isCommand(Input))
					{
						Route CmdRoute = RunCommand(Input, book, current);
						if (CmdRoute == NONE) continue;
						return CmdRoute;
					}
					if (Input.empty())
					{
						cout << "Input cannot be empty!\n";
						continue;
					}
					break;
				}
				Attr.second = Input;
			}
			else if (Choice == 3)
			{
				goto Change_Name;
				goto Change_Val;
			}
			CurrentContact.RemoveAttr(Attr_Name);
			CurrentContact.AddAttr(Attr.first, Attr.second);
			return DISPLAY;
		}
	}
}