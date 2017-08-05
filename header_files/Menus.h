#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "ProgramData.h"

namespace menu
{
	class Menu
	{
		std::vector<std::string> Selections;
		std::string Heading;
		std::string Prompt;
		int CurrentSelection;
		Request* request;

	public:

		// Constructor
		Menu();

		Menu(std::vector<std::string> selections, std::string heading, Request& req,
			std::string prompt = "Enter number of desired selection: ");

		// Modifiers
		void UpdateHeading(std::string heading);
		void UpdatePrompt(std::string prompt);
		void ReloadContents(std::vector<std::string> selections);
		void AddContent(std::string selection);
		void AddContent(std::vector<std::string> selections);
		void AddContent(std::initializer_list<std::string> selections);
		void ReloadRequest(Request& req);

		// Accessors
		std::string Str();
		int NumOfSelections();
		int Curr_Selection();
		void Curr_Selection(int new_selection);
		void DisplayMenu(std::ostream& out = std::cout);
		int ReadMenuChoice(std::string* input = nullptr, std::ostream& out = std::cout, std::istream& in = std::cin);
		int RunMenu();
	};
}