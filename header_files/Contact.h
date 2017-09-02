#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "StringFunctions.h"

using namespace std;

namespace contact {
	class Contact
	{
	private:
		string FirstName, LastName, DisplayName;
		int ID;
		bool VIP;
		vector<pair<string, string>> Attributes;
		static int ID_Num;

		void Sort();
	public:
		Contact();
		Contact(string FirstName, string LastName, string DisplayName, bool vip);

		Contact Edit(Contact& c);

		string SetFirstName(string fname);
		string SetLastName(string lname);
		string SetDisplayName(string dname);
		bool SetVIP(bool vip);
		pair<string, string> AddAttr(string attr_name, string attr_val);
		void RemoveAttr(string attr_name);

		string GetFirstName();
		string GetLastName();
		string GetDisplayName();
		bool isVIP();
		int GetID();
		vector<pair<string, string>> GetAttributes();
		pair<string, string> GetAttr(string name);
		string GetAttrVal(string name);
		vector<string> MenuOptions();

		string FileFormat();
		string DisplayFormat();

		bool hasAttr(string name);
		int AttrContainsNeedle(std::string needle);
	};
}