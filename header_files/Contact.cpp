#include "Contact.h"

using namespace std;

namespace contact
{
	int Contact::ID_Num = 1;

	void Contact::Sort()
	{
		std::sort(Attributes.begin(), Attributes.end(),
			[](std::pair<std::string, std::string> l_val, std::pair<std::string, std::string> r_val)
		{
			return l_val.first < r_val.first;
		});
	}

	Contact::Contact()
	{
		FirstName = "";
		LastName = "";
		DisplayName = "";
		VIP = false;
		Attributes = vector<pair<string, string>>();
		ID = ID_Num++;
	}
	Contact::Contact(string fname, string lname, string dname, bool vip)
	{
		FirstName = fname;
		LastName = lname;
		DisplayName = dname;
		VIP = vip;
		Attributes = vector<pair<string, string>>();
		ID = ID_Num++;
	}

	Contact Contact::Edit(Contact& c) 
	{
		FirstName = c.GetFirstName();
		LastName = c.GetLastName();
		DisplayName = c.GetDisplayName();
		VIP = c.isVIP();
		Attributes = c.GetAttributes();
		Sort();
		return *this;
	}


	string Contact::SetFirstName(string fname)
	{
		FirstName = fname;
		return FirstName;
	}
	string Contact::SetLastName(string lname)
	{
		LastName = lname;
		return LastName;
	}
	string Contact::SetDisplayName(string dname)
	{
		DisplayName = dname;
		return DisplayName;
	}
	bool Contact::SetVIP(bool vip)
	{
		VIP = vip;
		return VIP;
	}
	pair<string, string> Contact::AddAttr(string attr_name, string attr_val)
	{
		pair<string, string> Attr = pair<string, string>(attr_name, attr_val);
		Attributes.push_back(Attr);
		Sort();
		return Attr;
	}
	void Contact::RemoveAttr(string attr_name)
	{
		Attributes.erase(std::remove_if(Attributes.begin(), Attributes.end(), [attr_name](pair<string, string> attr)
		{
			return attr.first == attr_name;
		}), Attributes.end());
	}

	string Contact::GetFirstName() { return FirstName; }
	string Contact::GetLastName() { return LastName; }
	string Contact::GetDisplayName() { return DisplayName; }
	bool Contact::isVIP() { return VIP; }
	int Contact::GetID() { return ID; }
	vector<pair<string, string>> Contact::GetAttributes() { return Attributes; }
	pair<string, string> Contact::GetAttr(string name) 
	{
		if (!this->hasAttr(name)) return pair<string, string>("", "");
		return *(std::find_if(std::begin(Attributes), std::end(Attributes),
			[name](pair<string, string> val)
		{
			return val.first == name;
		}));
	}
	string Contact::GetAttrVal(string name)
	{
		if (!this->hasAttr(name)) return "";
		return std::find_if(std::begin(Attributes), std::end(Attributes),
			[name](pair<string, string> val)
		{
			return val.first == name;
		})->first;
	}


	string Contact::FileFormat()
	{
		std::string Result =  FirstName + "|" + LastName + "|" + DisplayName + "|";
		Result += VIP ? "true" : "false";
		for (pair<string, string> Attr : Attributes)
			Result += "|" + Attr.first + "|" + Attr.second;
		return Result;																
	}

	vector<string> Contact::MenuOptions()
	{
		vector<string> Options = vector<string>({
			"Display Name: " + DisplayName,
			"First Name: " + FirstName
		});
		if (Attributes.empty()) Options.push_back("Last Name: " + LastName + "\n");
		else Options.push_back("Last Name: " + LastName);
		for (int i = 0; i < Attributes.size(); i++)
		{
			if (i == (Attributes.size() - 1)) 
				Options.push_back(Attributes[i].first + ": " + Attributes[i].second + "\n");
			else
				Options.push_back(Attributes[i].first + ": " + Attributes[i].second);
		}
		if (VIP) Options.push_back("Remove from VIP Role");
		else Options.push_back("Add to VIP Role");
		Options.push_back("Add Attributes");
		Options.push_back("Back to Display");
		Options.push_back("Back to Main");
		return Options;
	}

	string Contact::DisplayFormat()
	{
		string Result = DisplayName;
		if (DisplayName != FirstName + " " + LastName) 
			Result += "\n\tFirst Name: " + FirstName + "\n\tLast Name: " + LastName;
		for (pair<string, string> Attr : Attributes) 
			Result += "\n\t" + Attr.first + ": " + Attr.second;
		if (Result == DisplayName)
			Result += "\n\tContact has no Info";
		return Result;
	}

	bool Contact::hasAttr(string name)
	{
		return std::find_if(std::begin(Attributes), std::end(Attributes),
			[name](pair<string, string> val)
		{
			return val.first == name;
		}) != Attributes.end();
	}
}