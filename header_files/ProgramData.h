#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>
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

enum Route { NONE, MAIN, EXIT, LIST, SEARCH, CREATE, DISPLAY, ADD_CHARAS, REMOVE, EDIT, EDIT_CHARA, CLEAR, CURRENT, VIP, ERR };

const string CmdError = "Command Format Error!\nPress any key to continue ";
const string CmdContactNotFound = "Contact Not Found!\nPress any key to continue ";

const std::unordered_map<std::string, Route> Commands = std::unordered_map<std::string, Route>({
	{ "/main", MAIN },
	{ "/exit", EXIT },
	{ "/create", CREATE },
	{ "/list", LIST },
	{ "/clear", CLEAR },
	{ "/current", CURRENT },
	{ "/search", SEARCH },
	{ "/display", DISPLAY },
	{ "/addcharas", ADD_CHARAS },
	{ "/delete", REMOVE },
	{ "/vip", VIP },
	{ "/edit", EDIT },
});

const std::vector<string> SpecialEntries = std::vector<string>({
	"/main",
	"/exit",
	"/create",
	"/list",
	"/clear",
	"/current",
	"/search",
	"/display",
	"/addcharas",
	"/delete",
	"/vip",
	"/edit",
	"/save",
	"/read",
	"/out_path",
	"/in_path"
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
	std::string CurrentAttr;
	int list_idx;

	Request();
	Request(AddressBook& book);
	Contact CurrContact(int id);
	Route PrevRouteSet(Route route);
	Route CurrRouteSet(Route route);
	std::string InputSet(std::string);
};

bool isCommand(std::string input);
bool isSpec(std::vector<std::string> inputs);
bool hasSpec(std::vector<std::string> inputs);
bool isEmpty(std::vector<std::string> inputs);

string CheckNameInput(vector<string> Names);

vector<vector<Contact>> Paginate(vector<Contact> query);
vector<string> ListNav(int idx, int pages_num);
string ListHeading(Request request, int idx, int pages_num);