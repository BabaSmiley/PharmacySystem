#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "DatabaseManager.h"
using namespace std;

class PrescriptionHistoryTablePrinter {
public:

	static void printHistoryTable(DatabaseManager *dbm, int customerId) {
		vector<Prescription*> prescriptions = dbm->getPrescriptionHistory(customerId);

		cout << "Prescriptions:" << endl;
		if (prescriptions.size() == 0) {
			cout << "No prescriptions found for this user." << endl;
		}

		for (Prescription *p : prescriptions) {
			User *customer = dbm->getUser(customerId);
			if (customer == nullptr) { continue; } //Guard

			vector<Purchase*> purchases = dbm->getPurchases(p);
			if (purchases.size() == 0) { continue; } //Guard

			cout << string(40, '-') << endl;
			cout << "Prescription on " << p->getDate() << " by " << customer->getUsername() << ". " << "Store: " << p->getStoreId() << endl;

			const char separator = ' ';
			const int nameWidth = 32;
			const int numWidth = 8;

			cout << left << setw(nameWidth) << setfill(separator) << "Name";
			cout << left << setw(numWidth) << setfill(separator) << "Qty";
			cout << left << setw(numWidth) << setfill(separator) << "Sale Price" << endl << endl;

			for (Purchase* purchase : purchases) {
				Item *item = dbm->getItem(purchase->getItemId());

				string parsedName = item->getName();
				if (parsedName.size() >= nameWidth) {
					parsedName = parsedName.substr(0, nameWidth - 4) + "...";
				}

				cout << left << setw(nameWidth) << setfill(separator) << parsedName;
				cout << left << setw(numWidth) << setfill(separator) << purchase->getQuantity();
				cout << left << setw(numWidth) << setfill(separator) << "$" + to_string(purchase->getSalePrice());
				cout << endl;
			}
		}
		cout << string(40, '-') << endl << endl;
	}
};
