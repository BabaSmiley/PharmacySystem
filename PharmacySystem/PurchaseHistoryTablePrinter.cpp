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
				Item *item = dbm->getItemRegardlessOfActiveness(purchase->getItemId());

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
	/*static void printItemPurchaseHistoryTable(DatabaseManager *dbm, Item *item) {
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
	}*/

	static void printItemPurchaseHistoryTable(DatabaseManager *dbm, Item *item) {
		cout << string(60, '-') << endl;
		cout << "Item '" << item->getName() << "' Sale History:" << endl;

		vector<Sale*> sales = dbm->getSalesByItem(item->getId());

		cout << string(60, '-') << endl;

		printSales(sales);

		cout << string(60, '-') << endl << endl;
	}

	/*static void printStorePurchaseHistoryTable(DatabaseManager *dbm, Store *store) {
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
	}*/

	static void printStorePurchaseHistoryTable(DatabaseManager *dbm, Store *store) {
		cout << string(60, '-') << endl;
		cout << "Store #" << store->getId() << " Sale History:" << endl;

		vector<Sale*> sales = dbm->getSalesByStore(store->getId());
		
		cout << string(60, '-') << endl;

		printSales(sales);

		cout << string(60, '-') << endl << endl;
	}

	private:

		static int getMaxDay(int month, int day) {
			if (month == 1 && day > 31)
				return 31;
			if (month == 2 && day > 28)
				return 28;
			if (month == 3 && day > 31)
				return 31;
			if (month == 4 && day > 30)
				return 30;
			if (month == 5 && day > 31)
				return 31;
			if (month == 6 && day > 30)
				return 30;
			if (month == 7 && day > 31)
				return 31;
			if (month == 8 && day > 31)
				return 31;
			if (month == 9 && day > 30)
				return 30;
			if (month == 10 && day > 31)
				return 31;
			if (month == 11 && day > 30)
				return 30;
			if (month == 12 && day > 31)
				return 31;

			return day;
		}


		static void printSales(vector <Sale*> sales) {
			if (sales.size() > 0) {

				string year = (sales[0]->getDate()).substr(0, 4), month = (sales[0]->getDate()).substr(5, 2), day = (sales[0]->getDate()).substr(8, 2);

				int salesD = 0, salesM = 0, salesY = 0, salesT = 0,
					qtyD = 0, qtyM = 0, qtyY = 0, qtyT = 0;


				cout << "Weekly sales <YYYY-MM-DD thru YYYY-MM-DD> <quantity> $<total revenue>" << endl;
				for (int i = 0; i < sales.size(); i++) {
					string monthtmp = (sales[i]->getDate()).substr(5, 2);
					string yeartmp = (sales[i]->getDate()).substr(0, 4);
					string daytmp = sales[i]->getDate().substr(8, 2);

					if (monthtmp != month || yeartmp != year || (stoi(daytmp) / 7) != (stoi(day) / 7)) { //If new month, then set new month, and reset month counter
						cout << year << "-" << month << "-" << (stoi(day) / 7) * 7 << " thru " <<
							year << "-" << month << "-" << getMaxDay(stoi(month), ((stoi(day) + 7) / 7) * 7) << " " << qtyD << " $" << salesD << endl;

						day = daytmp;
						month = monthtmp;
						year = yeartmp;

						salesD = 0; qtyD = 0;  //reset counters
					}

					int qty = sales[i]->getQuantity();
					int salesAmt = sales[i]->getSalePrice() * qty;

					salesD += salesAmt;
					qtyD += qty;
				}

				if (qtyD > 0 || salesD > 0)
				cout << year << "-" << month << "-" << (stoi(day) / 7) * 7 << " thru " <<
					year << "-" << month << "-" << getMaxDay(stoi(month), ((stoi(day) + 7) / 7) * 7) << " " << qtyD << " $" << salesD << endl;

				/*if (qtyD > 0 || salesD > 0)
					cout << year << "-" << month << " " << qtyM << " $" << salesM << endl;*/

				cout << endl;

				qtyD = 0; salesD = 0;

				year = (sales[0]->getDate()).substr(0, 4);
				month = (sales[0]->getDate()).substr(5, 2);
				day = sales[0]->getDate().substr(8, 2);


				cout << "daily sales <YYYY-MM-DD> <quantity> $<total revenue>" << endl;
				for (int i = 0; i < sales.size(); i++) {
					string monthtmp = (sales[i]->getDate()).substr(5, 2);
					string yeartmp = (sales[i]->getDate()).substr(0, 4);
					string daytmp = sales[i]->getDate().substr(8, 2);

					if (monthtmp != month || yeartmp != year || daytmp != day) { //If new month, then set new month, and reset month counter
						cout << year << "-" << month << "-" << day << " " << qtyD << " $" << salesD << endl;

						day = daytmp;
						month = monthtmp;
						year = yeartmp;

						salesD = 0; qtyD = 0;  //reset counters
					}

					int qty = sales[i]->getQuantity();
					int salesAmt = sales[i]->getSalePrice() * qty;

					salesD += salesAmt;
					qtyD += qty;
				}

				if (qtyD > 0 || salesD > 0)
					cout << year << "-" << month << "-" << day << " " << qtyD << " $" << salesD << endl;

				cout << endl;

				year = (sales[0]->getDate()).substr(0, 4);
				month = (sales[0]->getDate()).substr(5, 2);


				cout << "Monthly sales <YYYY-MM> <quantity> $<total revenue>" << endl;
				for (int i = 0; i < sales.size(); i++) {
					string monthtmp = (sales[i]->getDate()).substr(5, 2);
					string yeartmp = (sales[i]->getDate()).substr(0, 4);

					if (monthtmp != month || yeartmp != year) { //If new month, then set new month, and reset month counter
						cout << year << "-" << month << " " << qtyM << " $" << salesM << endl;

						month = monthtmp;
						year = yeartmp;

						salesM = 0; qtyM = 0; //reset counters
					}

					int qty = sales[i]->getQuantity();
					int salesAmt = sales[i]->getSalePrice() * qty;

					salesM += salesAmt; salesT += salesAmt;
					qtyM += qty; qtyT += qty;
				}

				if (qtyM > 0 || salesM > 0)
					cout << year << "-" << month << " " << qtyM << " $" << salesM << endl;

				cout << endl;

				year = (sales[0]->getDate()).substr(0, 4);


				cout << "Yearly sales <YYYY> <quantity> $<total revenue>" << endl;
				for (int i = 0; i < sales.size(); i++) {
					string yeartmp = (sales[i]->getDate()).substr(0, 4);

					if (yeartmp != year) { //If new year, then set new year, and roll over month, and reset counters
						cout << year << " " << qtyY << " $" << salesY << endl;

						year = yeartmp;

						salesY = 0; qtyY = 0; //reset counters
					}

					int qty = sales[i]->getQuantity();
					int salesAmt = sales[i]->getSalePrice() * qty;

					salesY += salesAmt; qtyY += qty;
				}

				if (qtyY > 0 || salesY > 0)
					cout << year << " " << qtyY << " $" << salesY << endl;

				cout << endl << "GRAND TOTAL: " << qtyT << " $" << salesT << endl << endl;
			}
		}
};
