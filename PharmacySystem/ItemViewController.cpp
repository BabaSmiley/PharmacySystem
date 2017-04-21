#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
using namespace std;


class ItemViewController {
public:

	/* Will print a formatted table for items via the database manager
		Parameter count: (Optional) the number of results to show/display
	*/
	static void printItemTable(unsigned int count = NULL) {
		vector<Item*> items = DatabaseManager::shared()->getItems(count);

		const char separator = ' ';
		const int idWidth = 8;
		const int nameWidth = 20;
		const int dosageWidth = 30;
		const int priceWidth = 20;

		cout << string(40, '-') << endl;
		cout << left << setw(idWidth) << setfill(separator) << "Id";
		cout << left << setw(nameWidth) << setfill(separator) << "Name";
		cout << left << setw(priceWidth) << setfill(separator) << "Wholesale Price";
		cout << left << setw(dosageWidth) << setfill(separator) << "Dosage" << endl << endl;

		for (Item* item : items) {

			string itemName = truncatedString(item->getName(), nameWidth);
			string itemDosage = truncatedString(item->getDosage(), dosageWidth);

			cout << left << setw(idWidth) << setfill(separator) << item->getId();
			cout << left << setw(nameWidth) << setfill(separator) << itemName;
			cout << left << setw(priceWidth) << setfill(separator) << "$" + to_string(item->getPrice());
			cout << left << setw(dosageWidth) << setfill(separator) << itemDosage << endl;
		}
		cout << string(40, '-') << endl << endl;
	}

private:

	// Returns a truncated string with '...' which fits inside 'clipLength' size
	static string truncatedString(string s, int clipLength) {
		if (s.size() >= clipLength) {
			return s.substr(0, clipLength - 4) + "...";
		}
		return s;
	}

};