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
		case DEV: return (DevTest(request) == "Good Job") ? MAIN : MAIN;
		}
	}

	Route Main(Request& request)
	{
		str_manip::ClearScreen();
		request.PrevRoute = MAIN;
		Menu MainMenu = Menu({ "Contact List", "VIP Role", "New Contact", "Search Contacts", "Dev Test", "Save & Exit" }, "Main Menu: ", request);
		if (!MainMenu.RunMenu()) return request.CurrRoute;
		switch (MainMenu.Curr_Selection())
		{
		case 1: return LIST;
		case 2: return VIP;
		case 3: return CREATE;
		case 4: return SEARCH;
		case 5: return DEV;
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
				if (isCommand(request.Input))
				{
					Route CmdRoute = RunCommand(request);
					if (CmdRoute == NONE) continue;
					return CmdRoute;
				}
				if ((request.Input.empty() && (i != 2)) || hasSpec({ request.Input }) || 
					isSpec({ request.Input }) || ((i == 2) && request.Book.DupName(request.Input)))
				{
					if (request.Input.empty() && (i != 2)) cout << Prompts[i] << " cannot be empty!\n";
					else if (hasSpec({ request.Input })) cout << Prompts[i] << " cannot contain \'|\'\n";
					else if (isSpec({ request.Input })) cout << request.Input << " is a command line special entry\n";
					else if ((i == 2) && request.Book.DupName(request.Input)) cout << "Display name " << request.Input << " already exists\n";
					continue;
				}
				if (request.Input.empty()) request.Input = Inputs[0] + " " + Inputs[1];
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
		request.PrevRoute = LIST;
		Menu ContactListMenu = Menu(CurrentPage, PageHeading, request);
		if (!ContactListMenu.RunMenu()) return request.CurrRoute;
		int Choice = ContactListMenu.Curr_Selection();
		if (Choice <= ContactList[page_idx].size())
		{
			request.CurrentContact = ContactList[page_idx][Choice - 1];
			page_idx = 0;
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
				if (request.Input.empty()) { cout << "Entry cannot be empty!\nTry again: "; continue; }
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
				page_idx = Temp - 1;
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

	string DevTest(Request& request)
	{
		string Input = "\"Ethan H\" Basham \"Blackberry Slave\" -t \"Phone Number\"|\"(931) 939-2391\"";
		cout << Input << endl;

		vector<string> InputVec = SplitInput(Input);


		bool vipset = false;
		bool disset = false;
		bool isvip = false;
		string vip = "";
		vector<string> Inputs = vector<string>();
		vector<pair<string, string>> Attrs = vector<pair<string, string>>();

		if (InputVec.size() < 3) return CmdError;
		if (isEmpty(InputVec)) return CmdError;

		// Get Characteristics
		vector<string>::iterator colon_pos = std::find_if(InputVec.begin(), InputVec.end(), [](string v) { 
				return v.find("|") != string::npos; 
		});
		while (colon_pos != InputVec.end())
		{
			if (*colon_pos == "|")
			{
				if ((colon_pos == InputVec.begin()) || (colon_pos == InputVec.end() - 1)) return CmdError;
				vector<string> Attr = vector<string>({ *(colon_pos - 1), *(colon_pos + 1) });
				if (isSpec(Attr)) return CmdError;
				Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
				InputVec.erase(colon_pos - 1, colon_pos + 2);
			}
			else if ((*colon_pos)[0] == '|')
			{
				if (colon_pos == InputVec.begin()) return CmdError;
				if ((*colon_pos).find('|', 1) != string::npos) return CmdError;
				vector<string> Attr = vector<string>({ *(colon_pos - 1), colon_pos->substr(1) });
				if (isSpec(Attr)) return CmdError;
				Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
				InputVec.erase(colon_pos - 1, colon_pos + 1);
			}
			else if ((*colon_pos)[colon_pos->size() - 1] == '|')
			{
				if (colon_pos == InputVec.end()) return CmdError;
				vector<string> Attr = vector<string>({ colon_pos->substr(0, colon_pos->size() - 1), *(colon_pos + 1) });
				if (isSpec(Attr)) return CmdError;
				Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
				InputVec.erase(colon_pos, colon_pos + 2);
			}
			else return CmdError;
			 
			colon_pos = std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
				return v.find("|") != string::npos;
			});
		}

		// Get Vip
		vector<string>::iterator vip_pos = std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
			return (v == "-f") || (v == "-t");
		});
		if (vip_pos != InputVec.end())
		{
			isvip = (*vip_pos == "-t");
			InputVec.erase(vip_pos, vip_pos + 1);
			if (std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
				return (v == "-f") || (v == "-t");
			}) != InputVec.end()) return CmdError;
		}

		if ((InputVec.size() > 3) || (InputVec.size() < 2)) return CmdError;

		Inputs.push_back(InputVec[0]);
		Inputs.push_back(InputVec[1]);
		
		string Temp = Inputs[0] + " " + Inputs[1];
		if (InputVec.size() == 3) if (InputVec[2] != "-d") Temp = InputVec[2];
		Inputs.push_back(Temp);

		Contact NewContact = Contact(Inputs[0], Inputs[1], Inputs[2], isvip);
		for (pair<string, string> Attr : Attrs)
		{
			NewContact.AddAttr(Attr.first, Attr.second);
		}
		
		cout << endl << NewContact.DisplayFormat() << endl;

		getchar();
		return "Good Job";
		//getline(std::cin, Input);

	}
}