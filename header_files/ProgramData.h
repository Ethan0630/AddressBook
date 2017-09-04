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
#include "BookCase.h"
#include "StringFunctions.h"

using namespace error_handling;
using namespace contact;
using namespace str_manip;
using namespace address_book;
using namespace book_case;

enum Route { NONE, MAIN, EXIT, LIST, SEARCH, CREATE, DISPLAY, ADD_CHARAS, REMOVE, EDIT, EDIT_CHARA, CLEAR, CURRENT, 
	VIP, SETTINGS, BOOK_MNGR, EDIT_BOOK, CREATE_BOOK };

const int MAX_BOOKS = 10;

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
	{ "/settings", SETTINGS},
	{ "/book", BOOK_MNGR }
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
	"/help",
	"/out_path",
	"/in_path",
	"/back",
	"/settings",
	"/book"
});

class Request {
public:
	BookCase Case;
	int edit_case;
	AddressBook Book;
	Contact CurrentContact;
	Route PrevRoute;
	Route AltPrevRoute;
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