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
		case EDIT_CHARA: return EditChara(request);
		case SEARCH: return Search(request);
		case VIP: return VIPList(request);	
		case SETTINGS : return Settings(request);
		case BOOK_MNGR: return BookManager(request);
		case EDIT_BOOK: return EditBook(request);
		case CREATE_BOOK: return CreateBook(request);
		case SEARCH_RESULTS: return SearchResults(request);
		}
	}

	Route Main(Request& request)
	{
		str_manip::ClearScreen();
		request.PrevRoute = MAIN;
		Menu MainMenu = Menu({ "Contact List", "VIP Role", "New Contact", "Search Contacts", "Manage Books", "Save & Exit" }, "Main Menu: ", request);
		if (!MainMenu.RunMenu()) return request.CurrRoute;
		switch (MainMenu.Curr_Selection())
		{
		case 1: return LIST;
		case 2: return VIP;
		case 3: return CREATE;
		case 4: return SEARCH;		
		case 5: return BOOK_MNGR;
		case 6: return EXIT;
		}
	}

	Route Create(Request& request)
	{
		str_manip::ClearScreen();
		vector<string> Inputs = vector<string>();
		bool vip = false;
		string Prompts[3] = { "First Name", "Last Name", "Display Name (optional)" };
		cout << "Create New Contact\n";
		for (int i = 0; i < 4;)
		{
			if (i < 3)
			{
				cout << "Enter " << Prompts[i] << ": ";
				getline(cin, request.Input);

				// Check for Command
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}

				// Check for Errors
				string err = "";
				if (request.Input.empty()) err = Prompts[i] + " cannot be empty!\n";
				else if (hasSpec({ request.Input })) err = Prompts[i] + " cannot contain \'|\'\n";
				else if (isSpec({ request.Input })) err = request.Input + " is a command line special entry\n";
				else if ((i == 2) && request.Book.DupName(request.Input)) err = "Display name " + request.Input + " already exists\n";
				if (!err.empty()) { cout << err; continue; }
				
				Inputs.push_back(request.Input);
			}
			else
			{
				BoolMenu VIP_Menu = BoolMenu("Add this contact to the VIP role?", request);
				if (!VIP_Menu.RunMenu()) return request.CurrRoute;
				vip = VIP_Menu.GetBool();
			}
			i++;
		}
		request.CurrentContact = Contact(Inputs[0], Inputs[1], Inputs[2], vip);
		request.Book.Add(request.CurrentContact);
		cout << "New Contact Created!\n";
		BoolMenu AddCharas = BoolMenu("Would you like to add some characteristics?", request);
		if (!AddCharas.RunMenu()) return request.CurrRoute;
		if (AddCharas.GetBool()) return ADD_CHARAS;
		return MAIN;
	}

	Route AddCharas(Request& request)
	{
		str_manip::ClearScreen();
		vector<string> Inputs = vector<string>();		
		cout << "Add Characteristic to " << request.CurrentContact.GetDisplayName() << "\n";
		for (int i = 0; i < 2; i++)
		{
			if (!i) cout << "Enter Attribute Name: ";
			else cout << "Enter " << request.Input << ": ";
			getline(cin, request.Input);
			
			// Check for Command
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}
			
			// Check for Errors
			string err = "";
			if (request.Input.empty()) err = "Input cannot be empty!\n";
			else if (hasSpec({ request.Input })) err = "Input cannot contain \'|\'\n";
			else if (isSpec({ request.Input })) err = request.Input + " is a command line special entry\n";
			else if ((i == 1) && request.CurrentContact.hasAttr(request.Input)) err = "Attribute name " + request.Input + " already exists\n";
			if (!err.empty()) { cout << err; continue; }

			Inputs.push_back(request.Input);
		}
		request.CurrentContact.AddAttr(Inputs[0], Inputs[1]);
		request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
		BoolMenu AddCharas = BoolMenu("Would you like to add another characteristic?", request);
		if (!AddCharas.RunMenu()) return request.CurrRoute;
		if (AddCharas.GetBool()) return ADD_CHARAS;
		if (request.PrevRoute == EDIT) return EDIT;
		return MAIN;
	}

	Route List(Request& request)
	{
		str_manip::ClearScreen();
		vector<vector<Contact>> ContactList;
		if (request.PrevRoute == SEARCH) ContactList = Paginate(request.SearchResults);
		else if (request.PrevRoute == VIP) ContactList = Paginate(request.VIPRole);
		else ContactList = Paginate(request.Book.List());

		if (ContactList.empty())
		{
			cout << "List is empty!\nPress any key to continue ";
			getchar();
			return MAIN;
		}

		// Create Options
		vector<string> CurrentPage = vector<string>();
		for (Contact c : ContactList[request.list_idx]) CurrentPage.push_back(c.GetDisplayName());
		vector<string> NavOptions = ListNav(request.list_idx, ContactList.size());
		CurrentPage.insert(CurrentPage.end(), NavOptions.begin(), NavOptions.end());
		int MaxPages = ContactList.size(), Options = CurrentPage.size(), PageContacts = ContactList[request.list_idx].size();

		// Display List
		string PageHeading = ListHeading(request, request.list_idx, MaxPages);
		Menu ContactListMenu = Menu(CurrentPage, PageHeading, request);
		if (!ContactListMenu.RunMenu()) return request.CurrRoute;
		int Choice = ContactListMenu.Curr_Selection();
		request.PrevRoute = LIST;
		
		// Process Choice
		if ((Choice <= PageContacts) || (Choice == Options)) request.list_idx = 0;
		if (Choice <= PageContacts)
		{
			request.CurrentContact = ContactList[request.list_idx][Choice - 1];
			return DISPLAY;
		}
		if (Choice == Options) return MAIN;
		if (Choice == (Options - 1))
			return GetRangedInput(1, MaxPages, request, "Enter Page Number (1-" + to_string(MaxPages) + "): ");
		if ((Choice == (PageContacts + 1)) && (request.list_idx != (MaxPages - 1))) request.list_idx++;
		else request.list_idx--;
		return LIST;
	}

	Route Display(Request& request)
	{
		str_manip::ClearScreen();
		cout << request.CurrentContact.DisplayFormat() << endl;
		vector<string> Options = { "Edit", "Delete", "Back", "Back to Main" };
		Menu OptionsMenu = Menu(Options, "Contact Options: ", request);
		if (!OptionsMenu.RunMenu()) return request.CurrRoute;
		request.PrevRoute = DISPLAY;
		int Choice = OptionsMenu.Curr_Selection();
		if (Choice == 4) return MAIN;
		if (Choice == 3) return LIST;
		if (Choice == 2) return REMOVE;
		else return EDIT;
	}

	Route Remove(Request& request)
	{
		str_manip::ClearScreen();
		cout << "Delete Contact\n";
		string Heading = "Are you sure you wish to delete " + request.CurrentContact.GetDisplayName() + "?";
		BoolMenu SureMenu = BoolMenu(Heading, request);
		if (!SureMenu.RunMenu()) return request.CurrRoute;
		if (SureMenu.GetBool())
		{
			request.Book.Delete(request.CurrentContact.GetID());
			request.CurrContact(request.Book.First());
			return LIST;
		}
		else return DISPLAY;
	}

	Route Edit(Request& request)
	{
		str_manip::ClearScreen();
		string NameAttrs[3] = { "Display Name", "First Name", "Last Name" };
		vector<string> Options = request.CurrentContact.MenuOptions();
		Menu EditMenu = Menu(Options, "Edit Contact:", request);
		if (!EditMenu.RunMenu()) return request.CurrRoute;
		int Choice = EditMenu.Curr_Selection();
		if (Choice == Options.size()) return MAIN;
		if (Choice == (Options.size() - 1)) return DISPLAY;
		if (Choice == (Options.size() - 2))
		{
			request.PrevRoute = EDIT;
			return ADD_CHARAS;
		}
		if (Choice == (Options.size() - 3))
		{
			request.CurrentContact.SetVIP(!request.CurrentContact.isVIP());
			request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
			return EDIT;
		}
		else if (Choice < 4)
		{
			while (1)
			{
				cout << "Enter " << NameAttrs[Choice - 1] << ": ";
				getline(cin, request.Input);
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				if ((request.Input.empty() && (Choice != 1)) || hasSpec({ request.Input }) ||
					isSpec({ request.Input }) || ((Choice == 1) && request.Book.DupName(request.Input, request.CurrentContact.GetID())))
				{
					if (request.Input.empty() && (Choice != 1)) cout << NameAttrs[Choice - 1] << " cannot be empty!\n";
					else if (hasSpec({ request.Input })) cout << NameAttrs[Choice - 1] << " cannot contain \'|\'\n";
					else if (isSpec({ request.Input })) cout << request.Input << " is a command line special entry\n";
					else if ((Choice == 1) && request.Book.DupName(request.Input, request.CurrentContact.GetID()))
						cout << "Display name " << request.Input << " already exists\n";
					continue;
				}
				if (request.Input.empty())
					request.CurrentContact.SetDisplayName(request.CurrentContact.GetFirstName() + " " + request.CurrentContact.GetLastName());
				else
				{
					switch (Choice)
					{
					case 1: request.CurrentContact.SetDisplayName(request.Input); break;
					case 2: request.CurrentContact.SetFirstName(request.Input); break;
					case 3:	request.CurrentContact.SetLastName(request.Input); break;
					}
				}
				request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
				return EDIT;
			}
		}
		else
		{
			request.CurrentAttr = str_manip::Str_SplitByChar(Options[Choice - 1], ':')[0];
			return EDIT_CHARA;
		}
	}

	Route EditChara(Request& request)
	{
		str_manip::ClearScreen();
		std::pair<std::string, std::string> Attr = request.CurrentContact.GetAttr(request.CurrentAttr);
		std::string Orig_Name = Attr.first;
		std::string Heading = Attr.first + ": " + Attr.second;
		Menu AttrMenu = Menu({ "Change Name", "Change Value", "Change Both", "Delete Attribute", "Cancel" }, Heading, request);
		if (!AttrMenu.RunMenu()) return request.CurrRoute;
		int Choice = AttrMenu.Curr_Selection();
		if (Choice == 5) return EDIT;
		if (Choice == 4)
		{
			BoolMenu SureMenu = BoolMenu("Are you sure?", request);		
			if (!SureMenu.RunMenu()) return request.CurrRoute;
			if (SureMenu.GetBool())
			{
				request.CurrentContact.RemoveAttr(Attr.first);
				return EDIT;
			}
			else return EDIT;
		}
		int i = (Choice == 2) ? 1 : 0;
		int end = (Choice == 1) ? 1 : 2;
		for (i; i < end; i++)
		{
			if (!i) cout << "Enter Attribute Name: ";
			else cout << "Enter " << Attr.first << ": ";
			getline(cin, request.Input);
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}
			if (request.Input.empty() || hasSpec({ request.Input }) || isSpec({ request.Input }) ||
				((i == 1) && request.CurrentContact.hasAttr(request.Input)))
			{
				if (request.Input.empty()) cout << "Input cannot be empty!\n";
				else if (hasSpec({ request.Input })) cout << "Input cannot contain \'|\'\n";
				else if (isSpec({ request.Input })) cout << request.Input << " is a command line special entry\n";
				else if ((i == 1) && request.CurrentContact.hasAttr(request.Input))
					cout << "Attribute name " << request.Input << " already exists\n";
				continue;
			}
			if (!i) Attr.first = request.Input;
			else Attr.second = request.Input;
		}
		request.CurrentContact.RemoveAttr(Orig_Name);
		request.CurrentContact.AddAttr(Attr.first, Attr.second);
		request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
		return EDIT;
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
		if (request.SearchResults.empty())
		{
			cout << "Nothing Found!\nPress any key to continue ";
			getchar();
			return SEARCH;
		}
		request.AltPrevRoute = SEARCH;
		request.PrevRoute = SEARCH;
		return SEARCH_RESULTS;
	}

	Route SearchResults(Request& request)
	{
		std::string Results = "";
		for (Contact c : request.SearchResults) Results += c.DisplayFormat() + "\n";
		
		int start = 0;
		std::string::size_type n = -1;
		while ((n = Results.find(request.Input, n + 1)) != std::string::npos)
		{
			cout << Results.substr(start, n - start);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
			cout << Results.substr(n, request.Input.size());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
			start = n + request.Input.size();
		}
		cout << Results.substr(start, Results.size() - start + 1);
		getchar();
		return MAIN;
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
		request.AltPrevRoute = request.PrevRoute;
		request.PrevRoute = VIP;
		return LIST;
	}

	Route Settings(Request& request)
	{
		return MAIN;
	}

	Route BookManager(Request& request)
	{
		str_manip::ClearScreen();
		vector<string> Page = vector<string>();
		vector<AddressBook> Books = request.Case.List();
		for (AddressBook B : Books)
		{
			std::string option = B.GetName();
			if (B.GetName() == request.Case.GetCurrent().GetName()) option += " *";
			Page.push_back(option);
		}
		if (Books.size() < MAX_BOOKS) Page.push_back("Create New Book");
		Page.push_back("Back to Main Menu");
		request.PrevRoute = BOOK_MNGR;
		Menu BookMenu = Menu(Page, "Manage Books: ", request);
		if (!BookMenu.RunMenu()) return request.CurrRoute;
		if (BookMenu.Curr_Selection() == BookMenu.NumOfSelections()) return MAIN;
		else if ((Books.size() < MAX_BOOKS) && (BookMenu.Curr_Selection() == BookMenu.NumOfSelections() - 1)) return CREATE_BOOK;
		else
		{
			request.edit_case = BookMenu.Curr_Selection() - 1;
			return EDIT_BOOK;
		}
		return MAIN;
	}

	Route EditBook(Request& request)
	{
		str_manip::ClearScreen();
		std::string Bname = request.Case.List()[request.edit_case].GetName();
		Menu EditMenu = Menu({"Edit Name", "Swap to this Book", "Delete", "Back to Book Manager"}, "Edit Book: " + Bname, request);
		if (!EditMenu.RunMenu()) return request.CurrRoute;
		if (EditMenu.Curr_Selection() == 1)
		{
			while (1)
			{
				cout << "Enter Name of Address Book: ";
				getline(cin, request.Input);

				// Check for Command
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}

				// Check for Errors
				string err = "";
				if (request.Input.empty()) err = "Name cannot be empty!\n";
				else if (request.Input.find_first_of("\\/?\":<>*|") != string::npos)
					err = "Name can't contain characters that are not permitted in file names (\\ / ? \" : < > * |)";
				else if (isSpec({ request.Input })) err = request.Input + " is a command line special entry\n";
				else if (request.Case.DupName(request.Input, Bname)) err = "Name " + request.Input + " already exists\n";
				if (!err.empty()) { cout << err; continue; }
				break;
			}
			AddressBook NewBook = AddressBook(request.Input);
			request.Case.Edit(Bname, NewBook);
			return EDIT_BOOK;
		}
		else if (EditMenu.Curr_Selection() == 2)
		{
			if (!request.Book.Save())
			{
				"Error while writing file output\nPress any key to exit: ";
				getchar();
				return EXIT;
			}
			request.Case.ChangeCurrent(Bname);
			request.Book = request.Case.GetCurrent();
			if (!request.Book.Read())
			{
				"Error while reading file input\nPress any key to exit: ";
				getchar();
				return EXIT;
			}
			request.CurrentContact = request.Book.Find(request.Book.First());
		}
		else if (EditMenu.Curr_Selection() == 3)
		{
			BoolMenu SureMenu = BoolMenu("Are you sure you want to delete " + Bname + "?", request);
			if (!SureMenu.RunMenu()) return request.CurrRoute;
			if (SureMenu.Curr_Selection() == 2) return EDIT_BOOK;
			string Path = Bname + ".book";
			if (remove(Path.c_str()))
			{
				"Error while writing file output\nPress any key to exit: ";
				getchar();
				return EXIT;
			}
			else
			{
				request.Case.Delete(Bname);
				if (request.Book.GetName() == Bname)
				{
					if (request.Case.List().empty())
					{
						AddressBook NewBook = AddressBook("Default");
						if (!NewBook.Save())
						{
							"Error while creating file\nPress any key to exit: ";
							getchar();
							return EXIT;
						}
						request.Case.ChangeCurrent(NewBook.GetName());
						request.Book = request.Case.GetCurrent();
						request.CurrentContact = Contact();
					}
					else
					{
						AddressBook FirstBook = request.Case.List()[0];
						request.Case.ChangeCurrent(FirstBook.GetName());
						request.Book = request.Case.GetCurrent();
						if (!request.Book.Read())
						{
							"Error while reading file input\nPress any key to exit: ";
							getchar();
							return EXIT;
						}
						request.CurrentContact = request.Book.Find(request.Book.First());
					}
				}
			}
		}
		return BOOK_MNGR;
	}

	Route CreateBook(Request& request)
	{
		str_manip::ClearScreen();
		while (1)
		{
			cout << "Enter Name of Address Book: ";
			getline(cin, request.Input);

			// Check for Command
			if (isCommand(request.Input))
			{
				Route CmdRoute = RunCommand(request);
				if (CmdRoute == NONE) continue;
				return CmdRoute;
			}

			// Check for Errors
			string err = "";
			if (request.Input.empty()) err = "Name cannot be empty!\n";
			else if (request.Input.find_first_of("\\/?\":<>*|") != string::npos)
				err = "Name can't contain characters that are not permitted in file names (\\ / ? \" : < > * |)";
			else if (isSpec({ request.Input })) err = request.Input + " is a command line special entry\n";
			else if (request.Case.DupName(request.Input)) err = "Name " + request.Input + " already exists\n";
			if (!err.empty()) { cout << err; continue; }
			break;
		}
		AddressBook NewBook = AddressBook(request.Input);
		request.Case.Add(NewBook);
		if (!NewBook.Save())
		{
			"Error while creating file\nPress any key to exit: ";
			getchar();
			return EXIT;
		}
		BoolMenu SwitchMenu = BoolMenu("Would you like to switch to this book?", request);
		if (!SwitchMenu.RunMenu()) return request.CurrRoute;
		if (SwitchMenu.GetBool()) 
		{
			if (!request.Book.Save())
			{
				"Error while writing file output\nPress any key to exit: ";
				getchar();
				return EXIT;
			}
			request.Case.ChangeCurrent(NewBook.GetName());
			request.Book = request.Case.GetCurrent();
			if (!request.Book.Read())
			{
				"Error while reading file input\nPress any key to exit: ";
				getchar();
				return EXIT;
			}
			request.CurrentContact = Contact();
		}
		return BOOK_MNGR;
	}
}