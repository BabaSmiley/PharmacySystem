#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
using namespace std;

class StoresTablePrinter {
public:

	/* Will print a formatted table for stores via the database manager
		Parameter count: (Optional) the number of results to show/display
	*/
	static void printStoresTable(DatabaseManager *dbm, unsigned int count = NULL) {
		vector<Store*> stores = dbm->getStores(count);

		const char separator = ' ';
		const int addressWidth = 32;
		const int numWidth = 10;

		cout << string(40, '-') << endl;
		cout << left << setw(numWidth) << setfill(separator) << "Id";
		cout << left << setw(addressWidth) << setfill(separator) << "Address";
		cout << left << setw(addressWidth/2) << setfill(separator) << "City";
		cout << left << setw(numWidth) << setfill(separator) << "State";
		cout << left << setw(numWidth) << setfill(separator) << "ZipCode" << endl << endl;

		for (Store* store : stores) {
			//cout << "[id " << store->getId() << "] " << store->getAddress() << " " << store->getCity() << ", " << store->getState() << " " << store->getZipCode() << endl;
			cout << left << setw(numWidth) << setfill(separator) << store->getId();
			cout << left << setw(addressWidth) << setfill(separator) << store->getAddress();
			cout << left << setw(addressWidth / 2) << setfill(separator) << store->getCity();
			cout << left << setw(numWidth) << setfill(separator) << store->getState();
			cout << left << setw(numWidth) << setfill(separator) << store->getZipCode();
			cout << endl;
		}
		cout << string(40, '-') << endl << endl;
	}
};