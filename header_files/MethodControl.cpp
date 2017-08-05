#include "MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace std;
using namespace address_book;

namespace methods
{
	Route RunRoute(Request& request)
	{
		switch (request.CurrRoute)
		{
		case MAIN: return Main(request);
		case CREATE: return Create(request);
		case ADD_CHARAS: return AddCharas(request);
		case LIST: return List(request);
		case DISPLAY: return Display(request);
		case REMOVE: return Remove(request);
		case EDIT: return Edit(request);
		case SEARCH: return Search(request);
		case VIP: return VIPList(request);
		}
	}

	Route Main(Request& request)
	{
		str_manip::ClearScreen();
		Menu MainMenu = Menu({ "Contact List", "VIP Role", "New Contact", "Search Contacts", "Save & Exit" }, "Main Menu: ", request);
		if (!MainMenu.RunMenu()) return request.CurrRoute;
		switch (MainMenu.Curr_Selection())
		{
		case 1: return LIST;
		case 2: return VIP;
		case 3: return CREATE;
		case 4: return SEARCH;
		case 5: return EXIT;
		}
	}

	Route Create(Request& request)
	{
		vector<string> Inputs = vector<string>();
		string Prompts[3] = { "First Name", "Last Name", "Display Name (optional)" };
		str_manip::ClearScreen();
		cout << "Create New Contact\n";
		for (int i = 0; i < 4;)
		{
			if (i < 3)
			{
				cout << "Enter " << Prompts[i] << ": ";
				getline(cin, request.Input);
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				if (request.Input.empty())
				{
					if (i != 2)
					{
						cout << Prompts[i] << " cannot be empty!\n";
						continue;
					}
					else request.Input = Inputs[0] + " " + Inputs[1];
				}
				Inputs.push_back(request.Input);
			}
			else
			{
				Menu VIP_Menu = Menu({ "Yes", "No" }, "Add this contact to the VIP role?", request);
				if (!VIP_Menu.RunMenu()) return request.CurrRoute;
				(VIP_Menu.Curr_Selection() == 1) ? Inputs.push_back("true") : Inputs.push_back("false");
			}
			i++;
		}
		request.CurrentContact = Contact(Inputs[0], Inputs[1], Inputs[2], (Inputs[3] == "true"));
		request.Book.Add(request.CurrentContact);
		cout << "New Contact Created!\n";
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add some characteristics?", request);
		if (!AddCharas.RunMenu()) return request.CurrRoute;
		if (AddCharas.Curr_Selection() == 1) return ADD_CHARAS;
		return MAIN;
	}

	Route AddCharas(Request& request)
	{
		vector<string> Inputs = vector<string>();		
		str_manip::ClearScreen();
		cout << "Add Characteristic to " << request.CurrentContact.GetDisplayName() << "\n";
		for (int i = 0; i < 2; i++)
		{
			if (!i) cout << "Enter Attribute Name: ";
			else cout << "Enter " << request.Input << ": ";
			getline(cin, request.Input);
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}
			if (request.Input.empty()) cout << "Input cannot be empty!\n";
			else Inputs.push_back(request.Input);
		}
		request.CurrentContact.AddAttr(Inputs[0], Inputs[1]);
		request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
		Menu AddCharas = Menu({ "Yes", "No" }, "Would you like to add another characteristic?", request);
		if (!AddCharas.RunMenu()) return request.CurrRoute;
		if (AddCharas.Curr_Selection() == 1) return ADD_CHARAS;
		return MAIN;
	}

	Route List(Request& request)
	{
		str_manip::ClearScreen();
		vector<vector<Contact>> ContactList;
		if (request.PrevRoute == SEARCH) ContactList = Paginate(request.SearchResults);
		else if (request.PrevRoute == VIP) ContactList = Paginate(request.VIPRole);
		else ContactList = Paginate(request.Book.List());
		static int page_idx = 0;
		string PageHeading;
		if (request.PrevRoute == SEARCH) PageHeading = "Search Results of \"" + request.Input + "\"";
		else if (request.PrevRoute == VIP) PageHeading = "VIP Role";
		else PageHeading = "Contact List : Page " + to_string(page_idx + 1) + " of " + to_string(ContactList.size());
		vector<string> CurrentPage = vector<string>();
		for (Contact c : ContactList[page_idx]) CurrentPage.push_back(c.GetDisplayName());
		if (ContactList.size() == 1) CurrentPage.push_back("Back to Main Menu");
		else if (!page_idx) CurrentPage.insert(CurrentPage.end(), { "Next Page", "Jump to Page", "Back to Main Menu" });
		else if (page_idx == (ContactList.size() - 1)) CurrentPage.insert(CurrentPage.end(), { "Previous Page", "Jump to Page", "Back to Main Menu" });
		else CurrentPage.insert(CurrentPage.end(), { "Next Page", "Previous Page", "Jump to Page", "Back to Main Menu" });
		Menu ContactListMenu = Menu(CurrentPage, PageHeading, request);
		if (!ContactListMenu.RunMenu()) return request.CurrRoute;
		int Choice = ContactListMenu.Curr_Selection();
		if (Choice <= ContactList[page_idx].size())
		{
			page_idx = 0;
			request.CurrentContact = ContactList[page_idx][Choice - 1];
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
				getline(cin, request.Input);
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				str_manip::Str_RemoveChar(request.Input, ' ');
				bool isNum = true;
				for (char c : request.Input) { if (!isdigit(c)) isNum = false; break; }
				if (!isNum) { cout << "Entry must be integral!\n"; continue; }
				else Temp = stoi(request.Input);
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

	Route Display(Request& request)
	{
		str_manip::ClearScreen();
		cout << request.CurrentContact.DisplayFormat() << endl;
		vector<string> Options = { "Edit", "Delete", "Back", "Back to Main" };
		Menu OptionsMenu = Menu(Options, "Contact Options: ", request);
		if (!OptionsMenu.RunMenu()) return request.CurrRoute;
		int Choice = OptionsMenu.Curr_Selection();
		if (Choice == 4) return MAIN;
		if (Choice == 3) return LIST;
		if (Choice == 2) return REMOVE;
		else return EDIT;
	}

	Route Remove(Request& request)
	{
		cout << "Delete Contact\n";
		string Heading = "Are you sure you wish to delete " + request.CurrentContact.GetDisplayName() + "?";
		Menu SureMenu = Menu({ "Yes", "No" }, Heading, request);
		if (!SureMenu.RunMenu()) return request.CurrRoute;
		if (SureMenu.Curr_Selection() == 1)
		{
			request.Book.Delete(request.CurrentContact.GetID());
			request.CurrContact(request.Book.First());
			return LIST;
		}
		else return DISPLAY;
	}

	Route Edit(Request& request)
	{
		string NameAttrs[3] = { "Display Name", "First Name", "Last Name" };
		vector<pair<string, string>> Attributes = request.CurrentContact.GetAttributes();
		vector<string> Options = vector<string>({
			"Display Name: " + request.CurrentContact.GetDisplayName(),
			"First Name: " + request.CurrentContact.GetFirstName(),
			"Last Name: " + request.CurrentContact.GetLastName(),
		});
		for (pair<string, string> Attr : Attributes)
			Options.push_back(Attr.first + ": " + Attr.second);
		if (request.CurrentContact.isVIP()) Options.push_back("Remove from VIP Role");
		else Options.push_back("Add to VIP Role");
		Options.push_back("Add Attributes");
		Options.push_back("Back to Display");
		Options.push_back("Back to Main");
		Menu EditMenu = Menu(Options, "Edit Contact:", request);
		if (!EditMenu.RunMenu()) return request.CurrRoute;
		int Choice = EditMenu.Curr_Selection();
		if (Choice == Options.size()) return MAIN;
		if (Choice == (Options.size() - 1)) return DISPLAY;
		if (Choice == (Options.size() - 2)) return ADD_CHARAS;
		if (Choice == (Options.size() - 3))
		{
			request.CurrentContact.SetVIP(!request.CurrentContact.isVIP());
			request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
			return EDIT;
		}
		else if (Choice < 4)
		{
			Start_Edit:
			cout << "Enter " << NameAttrs[Choice - 1] << ": ";
			getline(cin, request.Input);
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) goto Start_Edit;
				return CmdRoute;
			}
			if (request.Input.empty())
			{
				if (Choice != 1)
				{
					cout << NameAttrs[Choice - 1] << " cannot be empty!\n";
					goto Start_Edit;
				}
				request.CurrentContact.SetDisplayName(request.CurrentContact.GetFirstName() + " " + request.CurrentContact.GetLastName());
			}
			else
			{
				switch (Choice)
				{
				case 1: request.CurrentContact.SetDisplayName(request.Input); break;
				case 2: request.CurrentContact.SetFirstName(request.Input); break;
				case 3:	request.CurrentContact.SetLastName(request.Input); break;
				}
			}
			return EDIT;
		}
		else
		{
			Menu AttrMenu = Menu({ "Change Name", "Change Value", "Change Both", "Delete Attribute", "Cancel" }, Options[Choice - 4], request);
			if (!AttrMenu.RunMenu()) return request.CurrRoute;
			Choice = AttrMenu.Curr_Selection();
			string Attr_Name = str_manip::Str_SplitByChar(Options[Choice - 4], ':')[0];
			if (Choice == 5) return EDIT;
			if (Choice == 4)
			{
				Menu SureMenu = Menu({ "Yes", "No" }, "Are you sure?", request);
				Start_Sure:
				if (!SureMenu.RunMenu()) return request.CurrRoute;
				if (SureMenu.Curr_Selection() == 1)
				{
					request.CurrentContact.RemoveAttr(Attr_Name);
					return EDIT;
				}
				else return EDIT;
			}
			pair<string, string> Attr = request.CurrentContact.GetAttr(Attr_Name);
			if (Choice == 1)
			{
				Change_Name:
				while (1)
				{
					cout << "Enter Attribute Name: ";
					getline(cin, request.Input);
					if (isCommand(request.Input))
					{
						Route CmdRoute = RunCommand(request);
						if (CmdRoute == NONE) continue;
						return CmdRoute;
					}
					if (request.Input.empty())
					{
						cout << "Input cannot be empty!\n";
						continue;
					}
					break;
				}
				Attr.first = request.Input;
			}
			else if (Choice == 2)
			{
				Change_Val:
				while (1)
				{
					cout << "Enter " + Attr.first + ": ";
					getline(cin, request.Input);
					if (isCommand(request.Input))
					{
						Route CmdRoute = RunCommand(request);
						if (CmdRoute == NONE) continue;
						return CmdRoute;
					}
					if (request.Input.empty())
					{
						cout << "Input cannot be empty!\n";
						continue;
					}
					break;
				}
				Attr.second = request.Input;
			}
			else if (Choice == 3)
			{
				goto Change_Name;
				goto Change_Val;
			}
			request.CurrentContact.RemoveAttr(Attr_Name);
			request.CurrentContact.AddAttr(Attr.first, Attr.second);
			return DISPLAY;
		}
	}

	Route Search(Request& request)
	{
		str_manip::ClearScreen();
		cout << "Search\n";
		while (1)
		{
			cout << "Enter keyword: ";
			std::getline(cin, request.Input);
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}
			if (request.Input.empty())
			{
				cout << "Input cannot be empty!\n";
				continue;
			}
			break;
		}
		request.SearchResults = request.Book.Search(request.Input);
		request.PrevRoute = SEARCH;
		return LIST;
	}

	Route VIPList(Request& request)
	{
		request.VIPRole = request.Book.VIPRole();
		if (request.VIPRole.empty())
		{
			cout << "VIP Role is Empty\nPress any key to continue ";
			cin.ignore();
			return MAIN;
		}
		request.PrevRoute = VIP;
		return LIST;
	}
}