#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "ProgramData.h"
#include "CommandControl.h"
#include "ErrorHandling.h"
#include "Menus.h"
#include "Contact.h"
#include "AddressBook.h"
#include "BookCase.h"
#include "StringFunctions.h"
#include "CommandControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace address_book;

namespace methods
{
	Route RunRoute(Request& request);

	Route Main(Request& request);
	Route Create(Request& request);
	Route AddCharas(Request& request);
	Route List(Request& request);
	Route Display(Request& request);
	Route Remove(Request& request);
	Route Edit(Request& request);
	Route EditChara(Request& request);
	Route Search(Request& request);
	Route VIPList(Request& request);
	Route Settings(Request& request);
	Route BookManager(Request& request);
	Route EditBook(Request& request);
	Route CreateBook(Request& request);
}