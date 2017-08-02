#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
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
	enum Route {MAIN, EXIT, LIST, SEARCH, CREATE, DISPLAY, ADD_CHARAS};

	Route RunRoute(Route route, AddressBook& book, int& current);

	Route Main();
	Route Create(AddressBook& book, int& current);
	Route AddCharas(AddressBook& book, int& current);
	Route List(AddressBook& book, int& current);
	Route Display(AddressBook& book, int& current);
}