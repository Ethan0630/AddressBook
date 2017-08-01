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
//using namespace address_book;

namespace methods
{
	enum Route {MAIN, EXIT, LIST, SEARCH, CREATE};

	vector<pair<string, Route>> Commands = 
	{
		pair<string, Route>("/mainmenu", MAIN),
		pair<string, Route>("/exit", EXIT),
		pair<string, Route>("/create", CREATE)
	};



	Route RunRoute(Route route);

	Route Main();
	Route Create();
}