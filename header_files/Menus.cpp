#include "Menus.h"

namespace menu
{
		// Constructor
		Menu::Menu()
		{
			Selections = std::vector<std::string>();
			SpecialEntries = std::vector<std::string>();
			Heading = "";
			Prompt = "";
			CurrentSelection = -1;
		}

		Menu::Menu(std::vector<std::string> selections, std::string heading, std::string prompt, std::vector<std::string> special_entries)
		{
			Selections = selections;
			Heading = heading;
			Prompt = prompt;
			SpecialEntries = special_entries;
			CurrentSelection = -1;
		}

		// Modifiers
		void Menu::UpdateHeading(std::string heading) { Heading = heading; }
		void Menu::UpdatePrompt(std::string prompt) { Prompt = prompt; }
		void Menu::ReloadContents(std::vector<std::string> selections) { Selections = selections; }
		void Menu::AddContent(std::string selection) { Selections.push_back(selection); }
		void Menu::AddContent(std::vector<std::string> selections) 
		{
			Selections.reserve(Selections.size() + selections.size());
			Selections.insert(Selections.end(), selections.begin(), selections.end());
		}
		void Menu::AddContent(std::initializer_list<std::string> selections)
		{
			Selections.reserve(Selections.size() + selections.size());
			Selections.insert(Selections.end(), selections.begin(), selections.end());
		}
		void Menu::ReloadSpecialEntries(std::vector<std::string> special_entries) { SpecialEntries = special_entries; }
		void Menu::AddSpecialEntry(std::string special_entry) { SpecialEntries.push_back(special_entry); }

		// Accessors
		std::string Menu::Str()
		{
			std::string Result = "";
			if (!Heading.empty()) Result += Heading;
			int i = 1;
			for (std::string s : Selections) Result += "\n\t" + std::to_string(i++) + ") " + s;
			if (!Prompt.empty()) Result += "\n" + Prompt;
			return Result;
		}
		int Menu::NumOfSelections()
		{
			return Selections.size();
		}
		int Menu::Curr_Selection()
		{
			return CurrentSelection;
		}
		void Menu::Curr_Selection(int new_selection)
		{
			CurrentSelection = new_selection;
		}
		void Menu::DisplayMenu(std::ostream& out)
		{
			out << Str();
		}
		int Menu::ReadMenuChoice(std::string* input, std::ostream& out, std::istream& in)
		{
			CurrentSelection = -1;
			while (1)
			{
				std::string Input;
				getline(in, Input);
				int num = Input.length();
				for (char c : Input) { if (c != ' ') Input += c; }
				Input = Input.substr(num);
				if (std::find(SpecialEntries.begin(), SpecialEntries.end(), Input) != SpecialEntries.end()) { if (input != nullptr) *input = Input; return -1; }
				bool isNum = true;
				for (char c : Input) { if (!isdigit(c)) isNum = false; break; }
				if (!isNum) { out << "Entry must be integral!\nTry again: "; continue; }
				else num = stoi(Input);
				if (!((num >= 1) && (num <= Selections.size()))) { out << "Entry was not a selection!\nTry again: "; continue; }
				CurrentSelection = num;
				return num;
			}
		}
		int Menu::RunMenu(std::string* input, std::ostream& out, std::istream& in)
		{
			DisplayMenu(out);
			return ReadMenuChoice(input, out, in);
		}
}