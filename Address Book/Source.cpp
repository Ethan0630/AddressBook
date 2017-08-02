#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "../header_files/ErrorHandling.h"
#include "../header_files/Menus.h"
#include "../header_files/Contact.h"
#include "../header_files//AddressBook.h"
#include "../header_files/StringFunctions.h"
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
		cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		cin.ignore();
		return 0;
	}
	int CurrentContactID = Book.First();
	Route CurrentRoute = MAIN;
	while (CurrentRoute != EXIT)
	{
		CurrentRoute = RunRoute(CurrentRoute, Book, CurrentContactID);
	}
	if (!Book.Save())
	{
		cout << "Error while reading initial file input!\n"
			<< "Press any key to exit: ";
		cin.ignore();		
	}
}


/*
Contact Test Code

Contact cTest;
cTest = Contact("Ethan", "Basham");
cTest.AddChar("phone", "...");
cTest.Display(); std::cout << "\n";
cTest.UpdateChar(0, "email", "ethan_basham@outlook.com");
cTest.Display(); std::cout << "\n";
cTest.ChangeFirst("Herbert");
cTest.Display(); std::cout << "\n";
cTest.ChangeLast("Smith");
cTest.Display(); std::cout << "\n";
std::cout << cTest.FileFormat() << "\n";
std::cout << cTest.GetName() << "\n";
for (std::pair<std::string, std::string> Val : cTest.GetCharacteristics()) std::cout << Val.first;
cTest.RemoveChar(0);
cTest.AddChar("phone", "...");
if (cTest.hasCharacteristic("phone")) std::cout << "true"; else std::cout << "false";			  std::cout << "\n";
if(cTest.hasCharacteristic("pho")) std::cout << "true"; else std::cout << "false";				  std::cout << "\n";
if(cTest.hasCharacteristicValue("phone", "...")) std::cout << "true"; else std::cout << "false";  std::cout << "\n";
if(cTest.hasCharacteristicValue("phone", "..")) std::cout << "true"; else std::cout << "false";	  std::cout << "\n";
if(cTest.hasValue("...")) std::cout << "true"; else std::cout << "false";						  std::cout << "\n";
if(cTest.hasValue("..")) std::cout << "true"; else std::cout << "false";						  std::cout << "\n";
if(cTest.hasFirstName("Herbert")) std::cout << "true"; else std::cout << "false";				  std::cout << "\n";
if(cTest.hasFirstName("Herb")) std::cout << "true"; else std::cout << "false";					  std::cout << "\n";
if(cTest.hasLastName("Smith")) std::cout << "true"; else std::cout << "false";					  std::cout << "\n";
if(cTest.hasLastName("Smit")) std::cout << "true"; else std::cout << "false";					  std::cout << "\n";

std::cin.ignore();

*/

/*
AddressBook test code

std::ifstream InFile;
InFile.open("data.txt");
AddressBook Book = AddressBook();
Book.Read(InFile);
Book.NewContact();
Book.Display();
std::ofstream OutFile;
OutFile.open("data.txt");
Book.Save(OutFile);
std::cin.ignore();
*/