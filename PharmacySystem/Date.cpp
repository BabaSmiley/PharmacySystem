#pragma once
#include <string>
using namespace std;

struct Date {
public:
	int day;
	int month;
	int year;

	// Formatted String in form YYYY-MM-DD
	// All values will be set to zero if fails to parse
	Date(string formattedString) {
		try {
			year = stoi(formattedString.substr(0, 4));
			month = stoi(formattedString.substr(5, 2));
			day = stoi(formattedString.substr(8, 2));
		}
		catch (const exception &e) {
			//cerr << "[ERROR] Could not convert string to date format: " << e.what() << endl;
			year = 0;
			month = 0;
			day = 0;
		}
	}

	// Formatted String in form YYYY-MM-DD
	// Will return true if the string is in the correct format.
	static bool validateDate(string formattedString) {
		try {
			if (formattedString.substr(4, 1) != "-" || formattedString.substr(7, 1) != "-") {
				return false;
			}

			// try conversion
			stoi(formattedString.substr(0, 4));
			stoi(formattedString.substr(5, 2));
			stoi(formattedString.substr(8, 2));
		}
		catch (const exception &e) {
			return false;
		}

		return true;
	}


};