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
	list_idx = 0;
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
	list_idx = 0;
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

string CheckNameInput(vector<string> Names)
{
	return "";
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

vector<string> ListNav(int idx, int pages_num)
{
	if (pages_num == 1) return { "Back to Main Menu" };
	else if (!idx) return { "Next Page", "Jump to Page", "Back to Main Menu" };
	else if (idx == (pages_num - 1)) return { "Previous Page", "Jump to Page", "Back to Main Menu" };
	else return { "Next Page", "Previous Page", "Jump to Page", "Back to Main Menu" };
}

string ListHeading(Request request, int idx, int pages_num)
{
	switch (request.PrevRoute)
	{
	case SEARCH: return "Search Results of \"" + request.Input + "\""; break;
	case VIP: return "VIP Role: Page " + to_string(idx + 1) + " of " + to_string(pages_num); break;
	default: return "Contact List : Page " + to_string(idx + 1) + " of " + to_string(pages_num);
	}
}