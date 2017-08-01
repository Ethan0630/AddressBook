#include "MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace std;
//using namespace address_book;

namespace methods
{
	Route RunRoute(Route route)
	{
		switch(route)
		{
		case MAIN: return Main();
		case CREATE: return Create();
		case EXIT: return EXIT;
		}
	}

	Route Main()
	{
		Menu MainMenu = Menu({ "Contact List", "New Contact", "Search Contacts", "Save & Exit" }, "Main Menu: ");
		MainMenu.RunMenu();
		switch (MainMenu.Curr_Selection())
		{
		case 1: return LIST;
		case 2: return CREATE;
		case 3: return SEARCH;
		case 4: return EXIT;
		}
	}

	Route Create()
	{

		return EXIT;
	}

	Route Search()
	{
		return EXIT;
	}

	/*Route CreateContact()
	{
		str_manip::ClearScreen();
		Error* Err;
		std::cout << "New Contact\n";
		bool isRepeating = true;
		std::vector<std::string> Results;
		std::string Inputs[3] = { "First Name", "Last Name", "Display Name (optional)" };
		for(int idx = 1; idx < 4;)
		{
			if (idx < 4)
			{
				std::string Temp;
				std::cout << "Enter " + Inputs[idx] + ": ";
				std::getline(std::cin, Temp);
				str_manip::Str_RemoveChar(Temp, ' ');
				if (idx == 3)
				{
					if (book.CheckDup(Temp))
					{
						cout << "Display name already exists!\n";
						continue;
					}
					if (Temp.empty())
					{
						Temp = Results[0] + " " + Results[1];
					}
				}
				if (Temp.empty())
				{
					Err_Empty()::DisplayMessage();
					continue;
				}
				Results.push_back(Temp);
			}
			else
			{
				YesNoMenu("Add contact to VIP role?")::GetBoolean() ? 
					Results.push_back("true") : Results.push_back("false");				
			}
		}		
		book.AddContact(Contact(Results[0], Results[1], Results[2], (Results[3] == "true")));
		if (YesNoMenu("Would you like to add some characteristics?")::GetBoolean())
		{
			
		}
	}*/
}