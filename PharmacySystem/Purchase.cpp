#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Purchase {
public:

	Purchase(int prescriptionId, int itemId, int quantity, int salePrice) {
		this->prescriptionId = prescriptionId;
		this->itemId = itemId;
		this->quantity = quantity;
		this->salePrice = salePrice;
	}

	int getPrescriptionId() { return prescriptionId; }

	int getItemId() { return itemId; }

	int getQuantity() { return quantity; }

	int getSalePrice() { return salePrice; }

private:
	int prescriptionId;
	int itemId;
	int quantity;
	int salePrice;
};