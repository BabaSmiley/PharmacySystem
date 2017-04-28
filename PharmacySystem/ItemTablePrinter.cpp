#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
using namespace std;

class ItemTablePrinter {
public:

	/* Will print a formatted table for items via the database manager
		Parameter store: The store in which to get a list of item inventory from
		Parameter count: (Optional) the number of results to show/display
	*/
	static void printItemTable(DatabaseManager *dbm, Store *store, unsigned int count = NULL) {
		vector<Inventory*> storeInventory = dbm->getStoreInventory(store->getId(), count);
		vector<Item*> items;
		for (Inventory *i : storeInventory) {
			Item *item = dbm->getItem(i->getItemId());
			if(item != nullptr)
				items.push_back(item);
		}
		
		printTable(items, store);
	}

	/* Will print a formatted table for all items accessible via the database manager.
		Parameter count: (Optional) the number of results to show/display
		Paramater onlyActiveItems: (Optional) whether or not active items should be printed or not. Default is to only print active items
	*/
	static void printCompanyItemTable(DatabaseManager *dbm, unsigned int count = NULL, bool onlyActiveItems = true) {
		vector<Item*> items = dbm->getItems(count, onlyActiveItems);
		printTable(items);
	}


private:

	/* Will print out a table for an array of Items
		Paramater store: (Optional) can specify the store in which the items array is located in. Do not specify if at an entire database level
	*/
	static void printTable(vector<Item*> items, Store *store = nullptr) {
		const char separator = ' ';
		const int idWidth = 8;
		const int nameWidth = 20;
		const int dosageWidth = 30;
		const int priceWidth = 20;

		cout << string(50, '-') << endl;
		cout << left << setw(idWidth) << setfill(separator) << "Id";
		if(store)
			cout << left << setw(idWidth) << setfill(separator) << "StoreId";
		cout << left << setw(nameWidth) << setfill(separator) << "Name";
		cout << left << setw(priceWidth) << setfill(separator) << "Wholesale Price";
		cout << left << setw(dosageWidth) << setfill(separator) << "Dosage" << endl << endl;

		for (Item* item : items) {

			string itemName = truncatedString(item->getName(), nameWidth);
			string itemDosage = truncatedString(item->getDosage(), dosageWidth);

			cout << left << setw(idWidth) << setfill(separator) << item->getId();
			if (store)
				cout << left << setw(idWidth) << setfill(separator) << store->getId();
			cout << left << setw(nameWidth) << setfill(separator) << itemName;
			cout << left << setw(priceWidth) << setfill(separator) << "$" + to_string(item->getPrice());
			cout << left << setw(dosageWidth) << setfill(separator) << itemDosage << endl;
		}
		cout << string(50, '-') << endl << endl;
	}


	// Returns a truncated string with '...' which fits inside 'clipLength' size
	static string truncatedString(string s, int clipLength) {
		if (s.size() >= clipLength) {
			return s.substr(0, clipLength - 4) + "...";
		}
		return s;
	}

};