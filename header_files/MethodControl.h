#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "../header_files/ErrorHandling.h"
#include "../header_files/Menus.h"
#include "../header_files/Contact.h"
#include "../header_files//AddressBook.h"
#include "../header_files/StringFunctions.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace address_book;

namespace methods
{
	enum Route {NONE, MAIN, EXIT, LIST, SEARCH, CREATE, DISPLAY, ADD_CHARAS, REMOVE, EDIT, CLEAR, CURRENT};

	const std::unordered_map<std::string, Route> Commands = std::unordered_map<std::string, Route>({
		{"/main", MAIN},
		{"/exit", EXIT},
		{"/create", CREATE},
		{"/list", LIST},
		{"/clear", CLEAR},
		{"/current", CURRENT}
	});

	const std::vector<string> SpecialEntries = std::vector<string>({
		"/main",
		"/exit",
		"/create",
		"/list",
		"/clear",
		"/current"
	});

	bool isCommand(std::string input);

	Route RunRoute(Route route, AddressBook& book, int& current);
	Route RunCommand(std::string& Input, AddressBook& book, int& current);

	Route Main(AddressBook& book, int& current);
	Route Create(AddressBook& book, int& current);
	Route AddCharas(AddressBook& book, int& current);
	Route List(AddressBook& book, int& current);
	Route Display(AddressBook& book, int& current);
	Route Remove(AddressBook& book, int& current);
	Route Edit(AddressBook& book, int& current);
}