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

Route RunCommand(Request& request)
{
	if (request.Input == "/clear")
	{
		str_manip::ClearScreen();
	}
	if (request.Input == "/current")
	{
		std::cout << "Command Output " << request.CurrentContact.GetDisplayName() << endl;
		std::cout << "Press [Enter] to continue ";
		std::cin.ignore();
		str_manip::ClearScreen();
	}
	else if (Commands.find(request.Input) != Commands.end()) return Commands.at(request.Input);
	return NONE;
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