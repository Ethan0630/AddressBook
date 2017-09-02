#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "ProgramData.h"
#include "ErrorHandling.h"
#include "Contact.h"
#include "AddressBook.h"
#include "StringFunctions.h"

using namespace error_handling;
using namespace contact;
using namespace str_manip;
using namespace std;
using namespace address_book;

Route RunCommand(Request& request);
string RunCreate(Request& request);
string RunAddCharas(Request& request);
string RunEdit(Request& request);
Route RunDelete(Request& request);
Route RunList(Request& request);
Route RunVIP(Request& request);
vector<string> SplitInput(string input);