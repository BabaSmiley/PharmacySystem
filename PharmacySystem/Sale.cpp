#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Sale {
public:

	Sale(int itemId, int storeId, int prescriptionId, int quantity, int salePrice, string date) {
		this->itemId = itemId;
		this->storeId = storeId;
		this->prescriptionId = prescriptionId;
		this->quantity = quantity;
		this->salePrice = salePrice;
		this->date = date;
	}

	int getItemId() { return itemId; }

	int getStoreId() { return storeId; }

	int getPrescriptionId() { return prescriptionId; }

	int getQuantity() { return quantity; }

	int getSalePrice() { return salePrice; }

	string getDate() { return date; }

	void printDescription() {
		cout << "Item ID: " << itemId << endl;
		cout << "Store ID: " << storeId << endl;
		cout << "Prescription ID: " << prescriptionId << endl;
		cout << "Quantity: " << quantity << endl;
		cout << "Sale Price: " << salePrice << endl;
		cout << "Date: " << date << endl;
	}

private:
	int itemId;
	int storeId;
	int prescriptionId;
	int quantity;
	int salePrice;
	string date;
};