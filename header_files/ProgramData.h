#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "ErrorHandling.h"
#include "Contact.h"
#include "AddressBook.h"
#include "StringFunctions.h"

using namespace error_handling;
using namespace contact;
using namespace str_manip;
using namespace address_book;

enum Route { NONE, MAIN, EXIT, LIST, SEARCH, CREATE, DISPLAY, ADD_CHARAS, REMOVE, EDIT, CLEAR, CURRENT, VIP };

const std::unordered_map<std::string, Route> Commands = std::unordered_map<std::string, Route>({
	{ "/main", MAIN },
	{ "/exit", EXIT },
	{ "/create", CREATE },
	{ "/list", LIST },
	{ "/clear", CLEAR },
	{ "/current", CURRENT }
});

const std::vector<string> SpecialEntries = std::vector<string>({
	"/main",
	"/exit",
	"/create",
	"/list",
	"/clear",
	"/current"
});

class Request {
public:
	AddressBook Book;
	Contact CurrentContact;
	Route PrevRoute;
	Route CurrRoute;
	std::string Input;
	std::vector<Contact> SearchResults;
	std::vector<Contact> VIPRole;

	Request();
	Request(AddressBook& book);
	Contact CurrContact(int id);
	Route PrevRouteSet(Route route);
	Route CurrRouteSet(Route route);
	std::string InputSet(std::string);
};

bool isCommand(std::string input);

Route RunCommand(Request& request);

vector<vector<Contact>> Paginate(vector<Contact> query);