#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Sale {
public:

	Sale(int prescriptionId, int quantity, int salePrice, string date) {
		this->prescriptionId = prescriptionId;
		this->quantity = quantity;
		this->salePrice = salePrice;
		this->date = date;
	}

	int getPrescriptionId() { return prescriptionId; }

	int getQuantity() { return quantity; }

	int getSalePrice() { return salePrice; }

	string getDate() { return date; }

	void printDescription() {
		cout << "Item ID: " << prescriptionId << endl;
		cout << "Quantity: " << quantity << endl;
		cout << "Sale Price: " << salePrice << endl;
		cout << "Date: " << date << endl;
	}

private:

	int prescriptionId;
	int quantity;
	int salePrice;
	string date;
};