#include "CommandControl.h"

using namespace error_handling;
using namespace contact;
using namespace str_manip;
using namespace std;
using namespace address_book;

Route RunCommand(Request& request)
{
	if (request.Input == "/clear")
	{
		str_manip::ClearScreen();
	}
	else if (request.Input == "/save")
	{
		request.Book.Save() ?
			cout << "Address Book saved successfully!\nPress [Enter] to continue " :
			cout << "Error occured while trying to save to file!\nPress [Enter] to continue ";
		string temp;
		getline(std::cin, temp);
	}
	else if (request.Input == "/read")
	{
		request.Book.Read() ?
			cout << "Address Book loaded successfully!\nPress [Enter] to continue " :
			cout << "Error occured while trying to read from file!\nPress [Enter] to continue ";
		string temp;
		getline(std::cin, temp);
	}
	else if (request.Input == "/current")
	{
		std::cout << "Current Contact: " << request.CurrentContact.GetDisplayName() << endl;
		std::cout << "Press [Enter] to continue ";
		string temp;
		getline(std::cin, temp);
		str_manip::ClearScreen();
	}
	else if (request.Input == "/list")
	{
		request.PrevRoute = LIST;
		return LIST;
	}
	else if (Commands.find(request.Input) != Commands.end()) return Commands.at(request.Input);
	else if (request.Input.find("/delete") != string::npos)
	{
		Route R = RunDelete(request);
		if (R != REMOVE) getchar();		
		return R;
	}
	else if (request.Input.find("/list") != string::npos)
	{
		Route R = RunList(request);
		if (R != LIST) getchar();
		return R;
	}
	else if (request.Input.find("/vip") != string::npos)
	{
		Route R = RunVIP(request);
		if (R != LIST) getchar();
		return R;
	}
	else if (request.Input.find("/create") != string::npos)
	{
		cout << RunCreate(request);
		getchar();
	}
	else if (request.Input.find("/addcharas") != string::npos)
	{
		cout << RunAddCharas(request);
		getchar();
	}
	return NONE;
}

string RunCreate(Request& request)
{
	request.Input.erase(0, 8);
	vector<string> InputVec = SplitInput(request.Input);

	bool vipset = false;
	bool disset = false;
	bool isvip = false;
	string vip = "";
	vector<string> Inputs = vector<string>();
	vector<pair<string, string>> Attrs = vector<pair<string, string>>();

	if (InputVec.size() < 2) return CmdError;
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
		else
		{
			vector<string> Attr = Str_SplitByChar(*colon_pos, '|');
			if (isEmpty(Attr)) return CmdError;
			if (Attr.size() != 2) return CmdError;
			if (isSpec(Attr)) return CmdError;
			Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
			InputVec.erase(colon_pos, colon_pos + 1);
		}

		colon_pos = std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
			return v.find("|") != string::npos;
		});
	}
	
	for (int i = 0; i < Attrs.size(); i++) 
	{
		string name = Attrs[i].first;
		bool dup = std::find_if(std::begin(Attrs) + i + 1, std::end(Attrs),
			[name](pair<string, string> val)
		{
			return val.first == name;
		}) != Attrs.end();
		if (dup) return "You can't repeat Attribute Names\nPress any key to continue";
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
	if (request.Book.DupName(Temp)) return "Name already taken\nPress any key to continue ";
	Inputs.push_back(Temp);

	Contact NewContact = Contact(Inputs[0], Inputs[1], Inputs[2], isvip);
	for (pair<string, string> Attr : Attrs)
	{
		NewContact.AddAttr(Attr.first, Attr.second);
	}
	request.Book.Add(NewContact);
	request.CurrContact(NewContact.GetID());

	return "Contact created successfully\nPress any key to continue ";
}

string RunAddCharas(Request& request)
{
	request.Input.erase(0, 11);
	vector<string> InputVec = SplitInput(request.Input);
	vector<pair<string, string>> Attrs = vector<pair<string, string>>();

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
		else
		{
			vector<string> Attr = Str_SplitByChar(*colon_pos, '|');
			if (isEmpty(Attr)) return CmdError;
			if (Attr.size() != 2) return CmdError;
			if (isSpec(Attr)) return CmdError;
			Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
			InputVec.erase(colon_pos, colon_pos + 1);
		}

		colon_pos = std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
			return v.find("|") != string::npos;
		});
	}
	for (int i = 0; i < Attrs.size(); i++)
	{
		string name = Attrs[i].first;
		bool dup = std::find_if(std::begin(Attrs) + i + 1, std::end(Attrs),
			[name](pair<string, string> val)
		{
			return val.first == name;
		}) != Attrs.end();
		if (dup) return "You can't repeat Attribute Names\nPress any key to continue";
	}
	if (!InputVec.empty())
	{
		if (InputVec.size() != 1) return CmdError;
		int C = request.Book.SearchDname(InputVec[0]);
		if (C == -1) return CmdContactNotFound;
		request.CurrContact(C);
	}
	vector<pair<string, string>> OldAttrs = request.CurrentContact.GetAttributes();	
	for (int i = 0; i < Attrs.size(); i++)
	{
		string name = Attrs[i].first;
		bool dup = std::find_if(std::begin(OldAttrs), std::end(OldAttrs),
			[name](pair<string, string> val)
		{
			return val.first == name;
		}) != OldAttrs.end();
		if (dup) return "You can't repeat Attribute Names\nPress any key to continue ";
	}
	for (pair<string, string> Attr : Attrs) request.CurrentContact.AddAttr(Attr.first, Attr.second);
	request.Book.Edit(request.CurrentContact.GetID(), request.CurrentContact);
	return "Characteristics Updated Successfully!\nPress any key to continue ";
}

Route RunDelete(Request& request)
{
	request.Input.erase(0, 8);
	vector<string> InputVec = SplitInput(request.Input);
	if (InputVec.empty()) { cout << CmdError; return NONE; }

		// Get Forced
	bool f = false;
	vector<string>::iterator forced = std::find_if(InputVec.begin(), InputVec.end(), [](string v) {
		return v == "-f";
	});
	if (forced != InputVec.end())
	{
		f = true;
		InputVec.erase(forced, forced + 1);
	}
	if (InputVec.empty())
	{
		request.Book.Delete(request.CurrentContact.GetID());
		request.CurrContact(request.Book.First());
		cout << "Contact Deleted Successfully!\nPress any key to continue ";
		switch (request.CurrRoute)
		{
		case EDIT:
		case DISPLAY:
		case ADD_CHARAS:
		case REMOVE:
		case EDIT_CHARA:
			return LIST;
		default:
			return NONE;
		}
	}
	if (InputVec.size() > 1) { cout << CmdError; return NONE; }
	int DeleteC = request.Book.SearchDname(InputVec[0]);
	if (DeleteC == -1) { cout << CmdContactNotFound; return NONE; }
	if (!f)
	{
		request.CurrContact(DeleteC);
		return REMOVE;
	}
	else
	{
		request.Book.Delete(DeleteC);
		cout << "Contact Deleted Successfully!\nPress any key to continue ";
		if (request.CurrentContact.GetID() == DeleteC)
		{
			request.CurrContact(request.Book.First());
			switch (request.CurrRoute)
			{
			case EDIT:
			case DISPLAY:
			case ADD_CHARAS:
			case REMOVE:
			case EDIT_CHARA:
				return LIST;
			default:
				return NONE;
			}
		}
		request.CurrContact(request.Book.First());
		return NONE;
	}
}

Route RunList(Request& request)
{
	request.Input.erase(0, 5);
	vector<string> InputVec = SplitInput(request.Input);
	if (InputVec.empty()) { cout << CmdError; return NONE; }
	str_manip::Str_RemoveChar(request.Input, ' ');
	int Temp;
	bool isNum = true;
	int ListSize = 1 + (request.Book.List().size() / 10);
	for (char c : request.Input) { if (!isdigit(c)) isNum = false; break; }
	if (!isNum) { cout << CmdError; return NONE; }
	if (!ListSize) { cout << "List is empty\nPress any key to continue "; return NONE; }
	else Temp = stoi(request.Input);
	if (!((Temp >= 1) && (Temp <= ListSize))) { cout << "Entry was out of range!\nPress any key to continue "; return NONE; }
	request.list_idx = Temp - 1;
	request.PrevRoute = LIST;
	return LIST;
}

Route RunVIP(Request& request)
{
	request.Input.erase(0, 5);
	vector<string> InputVec = SplitInput(request.Input);
	if (InputVec.empty()) { cout << CmdError; return NONE; }
	str_manip::Str_RemoveChar(request.Input, ' ');
	int Temp;
	bool isNum = true;
	int ListSize = 1 + (request.VIPRole.size() / 10);
	for (char c : request.Input) { if (!isdigit(c)) isNum = false; break; }
	if (!isNum) { cout << CmdError; return NONE; }
	if (!ListSize) { cout << "VIP Role is empty\nPress any key to continue "; return NONE; }
	else Temp = stoi(request.Input);
	if (!((Temp >= 1) && (Temp <= ListSize))) { cout << "Entry was out of range!\nPress any key to continue "; return NONE; }
	request.list_idx = Temp - 1;
	request.PrevRoute = VIP;
	return LIST;
}

vector<string> SplitInput(string input)
{
	string Input = input;
	string OrigInput = Input;

	vector<string> MergedResults = vector<string>();
	vector<string> QuotedResults = vector<string>();
	vector<string> SingleResults = vector<string>();
	vector<pair<string, string>> Charas = vector<pair<string, string>>();

	std::size_t idx = Input.find('\"');
	while (idx != std::string::npos)
	{
		std::size_t next_idx = Input.find('\"', idx + 1);
		string Temp = Input.substr(idx, (next_idx - idx + 1));
		QuotedResults.push_back(Temp);
		Input.erase(idx, Temp.length());
		idx = Input.find('\"');
	}
	SingleResults = str_manip::Str_SplitByChar(Input, ' ');

	while (!QuotedResults.empty() && !SingleResults.empty())
	{
		if (OrigInput.find(QuotedResults[0]) < OrigInput.find(SingleResults[0]))
		{
			MergedResults.push_back(QuotedResults[0]);
			QuotedResults.erase(QuotedResults.begin());
		}
		else
		{
			MergedResults.push_back(SingleResults[0]);
			SingleResults.erase(SingleResults.begin());
		}
	}

	if (QuotedResults.empty()) MergedResults.insert(MergedResults.end(), SingleResults.begin(), SingleResults.end());
	else MergedResults.insert(MergedResults.end(), QuotedResults.begin(), QuotedResults.end());

	for (int i = 0; i < MergedResults.size(); i++)
	{
		if ((MergedResults[i][0] == '\"') && (MergedResults[i][MergedResults[i].length() - 1] == '\"'))
		{
			MergedResults[i] = MergedResults[i].substr(1, MergedResults[i].length() - 2);
		}
	}
	return MergedResults;
}
