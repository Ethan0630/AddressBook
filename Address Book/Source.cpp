#include "../header_files/MethodControl.h"

using namespace error_handling;
using namespace menu;
using namespace contact;
using namespace str_manip;
using namespace address_book;
using namespace book_case;
using namespace methods;

int main()
{
	BookCase Case = BookCase("book_cases.info");
	if (!Case.Read())
	{
		std::cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		std::getchar();
		return 0;
	}
	string Path = Case.GetCurrent().GetName() + ".book";
	AddressBook Book = AddressBook("Default");
	if (!Book.Read())
	{
		std::cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		std::getchar();
		return 0;
	}
	Request CurrRequest = Request(Book);
	CurrRequest.Case = Case;
	while (CurrRequest.CurrRoute != EXIT)
	{
		CurrRequest.CurrRoute = RunRoute(CurrRequest);
	}
	if (!CurrRequest.Book.Save())
	{
		std::cout << "Error while writing file output!\n"
			<< "Press any key to exit: ";
		std::getchar();
	}
	if (!CurrRequest.Case.Save())
	{
		std::cout << "Error while writing file output\n"
			<< "Press any key to exit: ";
		std::getchar();
	}
}