#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace menu
{
	class Menu
	{
		std::vector<std::string> Selections;
		std::vector<std::string> SpecialEntries;
		std::string Heading;
		std::string Prompt;
		int CurrentSelection;

	public:

		// Constructor
		Menu();

		Menu(std::vector<std::string> selections, std::string heading = "",
			std::vector<std::string> special_entries = std::vector<std::string>(), std::string prompt = "Enter number of desired selection: ");

		// Modifiers
		void UpdateHeading(std::string heading);
		void UpdatePrompt(std::string prompt);
		void ReloadContents(std::vector<std::string> selections);
		void AddContent(std::string selection);
		void AddContent(std::vector<std::string> selections);
		void AddContent(std::initializer_list<std::string> selections);
		void ReloadSpecialEntries(std::vector<std::string> special_entries);
		void AddSpecialEntry(std::string special_entry);

		// Accessors
		std::string Str();
		int NumOfSelections();
		int Curr_Selection();
		void Curr_Selection(int new_selection);
		void DisplayMenu(std::ostream& out = std::cout);
		int ReadMenuChoice(std::string* input = nullptr, std::ostream& out = std::cout, std::istream& in = std::cin);
		int RunMenu(std::string* input = nullptr, std::ostream& out = std::cout, std::istream& in = std::cin);
		int RunMenu(std::string& input);
	};

	class YesNoMenu : public Menu 
	{
		public: YesNoMenu(std::string heading); 
		bool GetBoolean();
	};
}