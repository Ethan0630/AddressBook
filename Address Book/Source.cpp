#include "../header_files/MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace address_book;
using namespace methods;

int main()
{
	AddressBook Book = AddressBook("data.txt");
	if (!Book.Read())
	{
		std::cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		std::getchar();
		return 0;
	}
	Request CurrRequest = Request(Book);
	while (CurrRequest.CurrRoute != EXIT)
	{
		CurrRequest.CurrRoute = RunRoute(CurrRequest);
	}
	if (!CurrRequest.Book.Save())
	{
		std::cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		std::getchar();
	}
}