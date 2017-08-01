#include "Contact.h"

using namespace std;

namespace contact
{
	int Contact::ID_Num = 1;

	Contact::Contact()
	{
		FirstName = "";
		LastName = "";
		DisplayName = "";
		VIP = false;
		ID = ID_Num++;
	}
	Contact::Contact(string fname, string lname, string dname, bool vip)
	{
		FirstName = fname;
		LastName = lname;
		DisplayName = dname;
		VIP = vip;
		ID = ID_Num++;
	}


	Contact& Contact::operator= (Contact& c)
	{
		if (this == &c)
			return *this;
		FirstName = c.GetFirstName();
		LastName = c.GetLastName();
		DisplayName = c.GetDisplayName();
		VIP = c.isVIP();
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

	string Contact::GetFirstName()
	{
		return FirstName;
	}
	string Contact::GetLastName()
	{
		return LastName;
	}
	string Contact::GetDisplayName()
	{
		return DisplayName;
	}
	bool Contact::isVIP()
	{
		return VIP;
	}
	int Contact::GetID()
	{
		return ID;
	}


	string Contact::FileFormat()
	{
		std::string Result =  FirstName + "|" + LastName + "|" + DisplayName + "|";
		Result += VIP ? "true" : "false";																													
		return Result;																
	}


	void Contact::Save(ofstream& outfile)
	{
		outfile << this->FileFormat();
		outfile.close();
	}

	void Contact::Read(ifstream& infile)
	{
		std::string InputLine;
		while (std::getline(infile, InputLine))
		{
			std::vector<std::string> InputVec = str_manip::Str_SplitByChar(InputLine, '|');			
			this->F_Name(InputVec[0]);
			this->L_Name(InputVec[1]);
			this->D_Name(InputVec[2]);
			this->VIP_Status(InputVec[3] == "true");
		}
	}
}


/*#include "Contact.h"

namespace contact
{
	// Constructors
	// *********************************************************************************************
	Contact::Contact()																				// default constructor
	{																								//
		FirstName = "";																				// assign FirstName to empty string
		LastName = "";																				// assign LastName to empty string
		DisplayName = "";																			// assign DisplayName to empty string
		VIP = false;																				// assign VIP to false
		Characteristics = std::vector<std::pair<std::string, std::string>>();						// assign characteristics to empty vector of string pairs
	}																								//
																									
	Contact::Contact(std::string first_name, std::string last_name, 
		std::string display_name, bool vip)															// simple constructor - takes name
	{																								//
		FirstName = first_name;																		// assign FirstName to first_name parameter
		LastName = last_name;																		// assign LastName to last_name parameter
		DisplayName = display_name;																	// assign DisplayName to display_name
		VIP = vip;																					// assign VIP to vip
		Characteristics = std::vector<std::pair<std::string, std::string>>();						// assign characteristics to empty vector of string pairs
	}																								//

	Contact::Contact(std::string first_name, std::string last_name, std::string display_name, 
		bool vip, std::vector<std::pair<std::string, std::string>> characteristics)					// complex constructor - takes name and characteristics
	{																								//
		FirstName = first_name;																		// assign FirstName to first_name parameter
		LastName = last_name;																		// assign LastName to last_name parameter
		DisplayName = display_name;																	// assign DisplayName to display_name
		VIP = vip;																					// assign VIP to vip
		Characteristics = characteristics;															// assign characteristics to characteristics parameter
	}																								//

	// Update Name
	// *********************************************************************************************
	void Contact::ChangeFirst(std::string first_name)												// mutable - change FirstName
	{																								//
		FirstName = first_name;																		// assign FirstName to first_name parameter
	}																								//

	void Contact::ChangeLast(std::string last_name)													// mutable - change LastName
	{																								//
		LastName = last_name;																		// assign LastName to last_name parameter
	}																								//

	void Contact::ChangeDisplay(std::string display_name)
	{
		DisplayName = display_name.empty() ? FirstName + " " + LastName : display_name;				// assign DisplayName to display_name
	}

	// Edit Characteristics
	// *********************************************************************************************
	void Contact::AddChar(std::string characteristic, std::string value)							// mutable - add characteristic (plus sort)
	{																								//				 
		Characteristics.push_back(std::pair<std::string, std::string>(characteristic, value));		// add new characteristic to characteristics vector
		SortData();																					// sort characteristics vector				 
	}																								//				 
																									
	void Contact::UpdateChar(int index, std::string new_characteristic, std::string value)			// mutable - update characteristic (plus sort)
	{																								//
		Characteristics[index] = std::pair<std::string, std::string>(new_characteristic, value);	// change contents of indexed characteristic pair
		SortData();																					// sort characteristics vector
	}																								//
																									
	void Contact::RemoveChar(int index)																// mutable - remove characteristic
	{																								//
		Characteristics.erase(Characteristics.begin() + index);										// remove indexed characteristic pair
	}																								//

	// Output 
	// *********************************************************************************************
	void Contact::Display()																			// method - display contact info
	{																								// 
		std::cout << DisplayName;																	// output name	
		if (DisplayName != (FirstName + " " + LastName))											// if the display name differs from first and last name
			std::cout << "\n\tFirst Name: " << FirstName << "\n\tLast Name: " << LastName;			// display first and last name
		for (const std::pair<std::string, std::string> &Val : Characteristics)						// loop through characteristics
		{																							// 
			std::cout << "\n\t" << Val.first << ": " << Val.second;									// display current characteristic
		}																							// 
	}																								// 
																									 
	std::string Contact::FileFormat()																// method - return info in file format
	{																								// 
		std::string Result = DisplayName + "|" + FirstName + "|" + LastName + "|";					// separate first and last name with delimiter
		Result += VIP ? "true" : "false";															// add vip data to result													
		for (const std::pair<std::string, std::string> &Val : Characteristics)						// loop through characteristics
		{																							// 
			Result += "|" + Val.first + "|" + Val.second;											// separate current characteristic with delimiter
		}																							// 
		return Result;																				// return formatted contact file line string
	}																								// 

	// Accessors
	// *********************************************************************************************
	std::string Contact::GetName()																	// accessor - return name
	{																								// 
		return DisplayName;																			// return display name
	}																								// 
																									
	std::vector<std::pair<std::string, std::string>> Contact::GetCharacteristics()					// accessor - return characteristics
	{																								// 
		return Characteristics;																		// return characteristics vector
	}																								// 			  

	bool Contact::isVip()																			// accessor - return VIP
	{																								// 
		return VIP;																					// return VIP
	}																								// 

	bool isDuplicateName(std::string display_name)													// accessor - determine whether parameter is duplicate
	{																								//
		return DisplayName == display_name;															// return comparison
	}																								//

	// Tests
	// *********************************************************************************************
	bool Contact::hasFirstName(std::string first_name)												// test - FirstName
	{																								// 
		return FirstName == first_name;																// return if FirstName is same as first name parameter
	}																								// 
																									 
	bool Contact::hasLastName(std::string last_name)												// test - LastName
	{																								// 
		return LastName == last_name;																// return if LastName is same as last name parameter
	}																								// 
																									 
	bool Contact::hasValue(std::string value)														// test - characteristic 
	{																								// 
		if (Characteristics.empty()) return false;													// if contact has no characteristics, return false
		return std::find_if(Characteristics.begin(), Characteristics.end(),							// return find_if results searching through characteristics
			[value](std::pair<std::string, std::string> val) 										// 
			{ 																						// 
				return val.second == value; 														// determine if characteristic's data is equal to value parameter
			}) != Characteristics.end();															// 
	}																								// 
																									 
	bool Contact::hasCharacteristic(std::string characteristic, int index)							// test - value
	{																								// 
		if (Characteristics.empty()) return false;													// if contact has no characteristics, return false
		auto it = std::find_if(Characteristics.begin(), Characteristics.end(), 						// find_if results searching through characteristics
			[characteristic](std::pair<std::string, std::string> val) 								// 
		{ 																							// 
			return val.first == characteristic; 													// determine if characteristic's name is equal to characteristic parameter
		});																							// 
		if (it == Characteristics.end()) return false;												// if no matching characteristic was found, return false
		if ((index != -1) && (it == Characteristics.begin() + index))								// if matching characteristic is current characteristic, return false
		return true;																				// match found, return true
	}																								// 
																									 
	bool Contact::hasCharacteristicValue(std::string characteristic, std::string value)				// test - characteristic + value
	{																								// 
		if (Characteristics.empty()) return false;													// if contact has no characteristics, return false
		auto It = std::find_if(Characteristics.begin(), Characteristics.end(), 						// return find_if results searching through characteristics
			[characteristic](std::pair<std::string, std::string> val) 								// 
			{ 																						// 
				return val.first == characteristic; 												// determine if characteristic's name is equal to characteristic parameter
			});																						// 
		if (It != Characteristics.end()) return It->second == value;								// match found characteristic's data against value parameter
		return false;																				// if characteristic not found, return false
	}																								// 
																									 
	// Private Methods
	// *********************************************************************************************
	void Contact::SortData()																		// method - sort characteristics
	{																								// 
		std::sort(Characteristics.begin(), Characteristics.end(),									// sort using characterisitics vector's iterators
			[](std::pair<std::string, std::string> l_val, 											// lambda to determine order of sort
				std::pair<std::string, std::string> r_val)											// 
			{																						// 
				return l_val.first < r_val.first;													// 
			});																						// sort characteristics vector according to characteristic's name
	}																								// 
}*/