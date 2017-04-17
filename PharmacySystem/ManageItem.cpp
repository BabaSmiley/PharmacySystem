#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Item.cpp"
using namespace std;

class ManageItem {
public:

	void promptForInput(int id) {
		getItemUpdateInput(id);
	}

private:

	void getItemUpdateInput(int id) throw(const char*) {
		string name;
		string description;
		string priceString;
		int price;
		string dosage;
		string vendorString;
		int vendorId;
		string expectedDeliveryDate;
		string whRefillLevelString;
		long whRefillLevel;
		string whRefillQtyString;
		long whRefillQty;
		string whLevelString;
		long whLevel;
		string onOrderQtyString;
		long onOrderQty;

		cout << endl << "To update an attribute of the item, enter the new value when prompted. Otherwise, just hit enter." << endl;
		name = getInput("Name");
		description = getInput("Description");
		priceString = getInput("Price");
		try {
			if (priceString.empty()) {
				price = NULL;
			}
			else {
				price = stoi(priceString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		dosage = getInput("Dosage");
		vendorString = getInput("Vendor ID");
		try {
			if (vendorString.empty()) {
				vendorId = NULL;
			}
			else {
				vendorId = stoi(vendorString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		expectedDeliveryDate = getInput("Expected Delivery Date");
		whRefillLevelString = getInput("Warehouse Refill Level");
		try {
			if (whRefillLevelString.empty()) {
				whRefillLevel = NULL;
			}
			else {
				whRefillLevel = stoi(whRefillLevelString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		whRefillQtyString = getInput("Warehouse Refill Quantity");
		try {
			if (whRefillQtyString.empty()) {
				whRefillQty = NULL;
			}
			else {
				whRefillQty = stoi(whRefillQtyString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		whLevelString = getInput("Warehouse Level");
		try {
			if (whLevelString.empty()) {
				whLevel = NULL;
			}
			else {
				whLevel = stoi(whLevelString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		onOrderQtyString = getInput("On Order Quantity");
		try {
			if (onOrderQtyString.empty()) {
				onOrderQty = NULL;
			}
			else {
				onOrderQty = stoi(onOrderQtyString);
			}
		}
		catch (const char *e) {
			throw e;
		}

		Item *item = DatabaseManager::shared()->updateItem(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, onOrderQty);

		if (item) {
			cout << "Item successfully updated." << endl;
		}
		else {
			cout << "Item not found" << endl;
		}
	}
};