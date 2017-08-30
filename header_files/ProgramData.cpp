#include "ProgramData.h"

using namespace std;

Request::Request()
{
	Book = AddressBook();
	CurrentContact = Contact();
	PrevRoute = MAIN;
	CurrRoute = MAIN;
	Input = "";
	SearchResults = std::vector<Contact>();
	VIPRole = std::vector<Contact>();
	CurrentAttr = "";
}

Request::Request(AddressBook& book)
{
	Book = book;
	int curr = Book.First();
	if (curr) CurrentContact = Book.Find(curr);
	else CurrentContact = Contact();
	PrevRoute = MAIN;
	CurrRoute = MAIN;
	Input = "";
	SearchResults = std::vector<Contact>();
	VIPRole = std::vector<Contact>();
	CurrentAttr = "";
}

Contact Request::CurrContact(int id)
{
	if (!id)
		int id = Book.First();
	if (id) CurrentContact = Book.Find(id);
	else CurrentContact = Contact();
	return CurrentContact;
}

Route Request::PrevRouteSet(Route route)
{
	PrevRoute = route;
	return PrevRoute;
}

Route Request::CurrRouteSet(Route route)
{
	CurrRoute = route;
	return CurrRoute;
}

std::string Request::InputSet(std::string input)
{
	Input = input;
	return Input;
}

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

bool isSpec(vector<string> inputs)
{
	vector<string> attrs = { "-d", "-t", "-f" };
	return std::find_if(inputs.begin(), inputs.end(), [attrs](string v) {
		return std::find(attrs.begin(), attrs.end(), v) != attrs.end();
	}) != inputs.end();
}

bool hasSpec(vector<string> inputs)
{
	return std::find_if(inputs.begin(), inputs.end(), [](string v) {
		return v.find("|") != string::npos;
	}) != inputs.end();
}

bool isEmpty(vector<string> inputs)
{
	return std::find_if(inputs.begin(), inputs.end(), [](string v) {
		return v.empty();
	}) != inputs.end();
}

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
	else if (Commands.find(request.Input) != Commands.end()) return Commands.at(request.Input);
	else if (request.Input.find("/create") != string::npos)
	{
		cout << RunCreate(request);
		string temp;
		getline(std::cin, temp);
	}	
	return NONE;
}

string RunCreate(Request& request)
{
	bool vipset = false;
	bool disset = false;
	string vip = "";
	vector<string> Inputs = vector<string>();
	vector<pair<string, string>> Attrs = vector<pair<string, string>>();
	vector<string> Cmd = str_manip::Str_SplitByChar(request.Input, ' ');
	if (Cmd.size() < 3) return CmdError;
	if (isEmpty(Cmd)) return CmdError;
	if (hasSpec({ Cmd[1], Cmd[2] })) return CmdError;
	if (isSpec({ Cmd[1], Cmd[2] })) return CmdError;
	Inputs.push_back(Cmd[1]);
	Inputs.push_back(Cmd[2]);
	Cmd.erase(Cmd.begin(), Cmd.begin() + 3);

	if (!Cmd.empty())
	{
		while (1)
		{
			if (find(Cmd.begin(), Cmd.end(), "-f") != Cmd.end())
			{
				if (vipset) return CmdError;
				vip = "false";
				vipset = true;
				Cmd.erase(find(Cmd.begin(), Cmd.end(), "-f"));
			}
			else if (find(Cmd.begin(), Cmd.end(), "-t") != Cmd.end())
			{
				if (vipset) return CmdError;
				vip = "true";
				vipset = true;
				Cmd.erase(find(Cmd.begin(), Cmd.end(), "-t"));
			}
			else break;
		}
	}

	if (!Cmd.empty())
	{
		while (1)
		{
			auto it = std::find_if(Cmd.begin(), Cmd.end(), [](string v) {
				return v.find("|") == string::npos; });
			if (find(Cmd.begin(), Cmd.end(), "-d") != Cmd.end())
			{
				if (disset) return CmdError;
				Inputs.push_back(Inputs[0] + " " + Inputs[1]);
				disset = true;
				Cmd.erase(find(Cmd.begin(), Cmd.end(), "-d"));
			}
			else if (it != Cmd.end())
			{
				if (disset) return CmdError;
				Inputs.push_back(*it);
				disset = true;
				Cmd.erase(it);
			}
			else break;
		}
	}

	for (int i = 0; i < Cmd.size(); i++)
	{
		vector<string> Attr = str_manip::Str_SplitByChar(Cmd[i], '|');
		if (Attr.size() > 2) return CmdError;
		Attrs.push_back(pair<string, string>(Attr[0], Attr[1]));
	}

	if (Inputs.size() == 2)
	{
		Inputs.push_back(Inputs[0] + " " + Inputs[1]);
		vip = "false";
	}
	else if (Inputs.size() == 3)
	{
		if (vip != "")
			Inputs.push_back(Inputs[0] + " " + Inputs[1]);
		else vip = "false";
	}
	bool isvip = (vip == "true");
	Contact NewContact = Contact(Inputs[0], Inputs[1], Inputs[2], isvip);
	for (pair<string, string> Attr : Attrs)
	{
		NewContact.AddAttr(Attr.first, Attr.second);
	}
	request.Book.Add(NewContact);
	return "Contact created successfully!\nPress [Enter] to continue ";
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

vector<vector<Contact>> Paginate(vector<Contact> query)
{
	vector<vector<Contact>> Results = vector<vector<Contact>>();
	for (int i = 0; i < query.size(); i += 10)
	{
		auto last = (query.size() < (i + 10)) ? query.size() : (i + 10);
		Results.emplace_back(query.begin() + i, query.begin() + last);
	}
	return Results;
}