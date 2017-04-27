#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "DatabaseManager.h"
using namespace std;

class PurchaseHistoryTablePrinter {
public:

	static void printUserPurchaseHistoryTable(DatabaseManager *dbm, int customerId) {
		vector<Prescription*> prescriptions = dbm->getPrescriptionHistory(customerId);

		cout << string(60, '-') << endl;
		cout << "Prescriptions:" << endl;
		if (prescriptions.size() == 0) {
			cout << "No prescriptions found." << endl;
		}

		for (Prescription *p : prescriptions) {
			User *customer = dbm->getUser(customerId);
			if (customer == nullptr) { continue; } //Guard

			vector<Purchase*> purchases = dbm->getPurchases(p);
			if (purchases.size() == 0) { continue; } //Guard

			cout << string(60, '-') << endl;
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
		cout << string(60, '-') << endl << endl;
	}

	//cout << "Prescription history of Store #" << store->getId() << ":" << endl;
	static void printItemPurchaseHistoryTable(DatabaseManager *dbm, Item *item) {
		cout << string(60, '-') << endl;
		cout << "Item '" << item->getName() << "' Sale History:" << endl;

		vector<Sale*> itemSales = dbm->getSalesByItem(item->getId());
		
		cout << string(60, '-') << endl;
		const char separator = ' ';
		const int nameWidth = 32;
		const int numWidth = 15;

		cout << left << setw(numWidth) << setfill(separator) << "Store Id";
		cout << left << setw(numWidth) << setfill(separator) << "Qty";
		cout << left << setw(numWidth) << setfill(separator) << "Sale Price";
		cout << left << setw(nameWidth) << setfill(separator) << "Date Sold" << endl << endl;

		for (Sale* sale : itemSales) {
			cout << left << setw(numWidth) << setfill(separator) << sale->getStoreId();
			cout << left << setw(numWidth) << setfill(separator) << sale->getQuantity();
			cout << left << setw(numWidth) << setfill(separator) << "$" + to_string(sale->getSalePrice());
			cout << left << setw(nameWidth) << setfill(separator) << sale->getDate();
			cout << endl;
		}

		cout << string(60, '-') << endl << endl;
	}


	static void printStorePurchaseHistoryTable(DatabaseManager *dbm, Store *store) {
		cout << string(60, '-') << endl;
		cout << "Store #" << store->getId() << " Sale History:" << endl;

		vector<Sale*> storeSales = dbm->getSalesByStore(store->getId());
		
		cout << string(60, '-') << endl;
		const char separator = ' ';
		const int nameWidth = 32;
		const int numWidth = 15;

		cout << left << setw(numWidth) << setfill(separator) << "Item Name";
		cout << left << setw(numWidth) << setfill(separator) << "Qty";
		cout << left << setw(numWidth) << setfill(separator) << "Sale Price";
		cout << left << setw(nameWidth) << setfill(separator) << "Date Sold" << endl << endl;

		for (Sale* sale : storeSales) {
			Item *item = dbm->getItem(sale->getItemId());

			cout << left << setw(numWidth) << setfill(separator) << item->getName();
			cout << left << setw(numWidth) << setfill(separator) << sale->getQuantity();
			cout << left << setw(numWidth) << setfill(separator) << "$" + to_string(sale->getSalePrice());
			cout << left << setw(nameWidth) << setfill(separator) << sale->getDate();
			cout << endl;
		}

		cout << string(60, '-') << endl << endl;
	}
};
