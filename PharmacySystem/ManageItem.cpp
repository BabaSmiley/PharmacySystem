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

	void promptForAddInput() {
		getItemAddInput();
	}

	void deleteItem(int itemId) {
		Item *item = DatabaseManager::shared()->getItem(itemId);
		if (item) {
			bool isDeleted = DatabaseManager::shared()->deleteItem(itemId);
			cout << "Item deleted successfully" << endl;
		}
		else {
			cout << "Item not found." << endl;
		}
	}

private:

	void getItemAddInput() throw(const char*) {
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

		cout << endl << "Please fill in all of the attributes to create an item." << endl;
		try {
			name = getInput("Name (Max 20 characters)");
			if (name.size() > 20)
			{
				name = name.substr(0, 20); //truncate
				cout << "Name truncated to: " << name << endl;
			}

			description = getInput("Description (Max 100 characters)");
			if (description.size() > 100)
			{
				description = description.substr(0, 100); //truncate
				cout << "Description truncated to: " << description << endl;
			}

			priceString = getInput("Price (Max 9 characters)");
			if (priceString.size() > 9) {
				priceString = priceString.substr(0, 9);
				cout << "Price truncated to: " << priceString << endl;
			}

			if (priceString.empty()) {
				price = NULL;
			}
			else {
				price = stoi(priceString);
			}

			dosage = getInput("Dosage (Max 20 characters)");
			if (dosage.size() > 20)
			{
				dosage = dosage.substr(0, 20); //truncate
				cout << "dosage truncated to: " << dosage << endl;
			}

			vendorString = getInput("Vendor ID (Max 4 characters)");
			if (vendorString.size() > 4) {
				vendorString = vendorString.substr(0, 4);
				cout << "Vendor truncated to: " << vendorString << endl;
			}
			if (vendorString.empty()) {
				vendorId = NULL;
			}
			else {
				vendorId = stoi(vendorString);
			}

			expectedDeliveryDate = getInput("Expected Delivery Date (Max 20 characters)");
			if (expectedDeliveryDate.size() > 20)
			{
				expectedDeliveryDate = expectedDeliveryDate.substr(0, 20); //truncate
				cout << "Expected Delivery Date truncated to: " << expectedDeliveryDate << endl;
			}

			whRefillLevelString = getInput("Warehouse Refill Level (Max 10 characters)");
			if (whRefillLevelString.size() > 10) {
				whRefillLevelString = whRefillLevelString.substr(0, 10);
				cout << "Warehouse Refill Level truncated to: " << whRefillLevelString << endl;
			}
			if (whRefillLevelString.empty()) {
				whRefillLevel = NULL;
			}
			else {
				whRefillLevel = stoi(whRefillLevelString);
			}

			whRefillQtyString = getInput("Warehouse Refill Quantity (Max 10 characters)");
			if (whRefillQtyString.size() > 10) {
				whRefillQtyString = whRefillQtyString.substr(0, 10);
				cout << "Warehouse Quantity Level truncated to: " << whRefillQtyString << endl;
			}
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

		Item *item = DatabaseManager::shared()->createItem(name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, 0, 1);

		if (item) {
			cout << "Item successfully created." << endl;
		}
		else {
			cout << "Item not found" << endl;
		}
	}

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

		cout << endl << "To update an attribute of the item, enter the new value when prompted. Otherwise, just hit enter." << endl;
		try {
			name = getInput("Name (Max 20 characters)");
			if (name.size() > 20)
			{
				name = name.substr(0, 20); //truncate
				cout << "Name truncated to: " << name << endl;
			}

			description = getInput("Description (Max 100 characters)");
			if (description.size() > 100)
			{
				description = description.substr(0, 100); //truncate
				cout << "Description truncated to: " << description << endl;
			}

			priceString= getInput("Price (Max 9 characters)");
			if (priceString.size() > 9) {
				priceString = priceString.substr(0, 9);
				cout << "Price truncated to: " << priceString << endl;
			}

			if (priceString.empty()) {
				price = NULL;
			}
			else {
				price = stoi(priceString);
			}

			dosage = getInput("Dosage (Max 20 characters)");
			if (dosage.size() > 20)
			{
				dosage = dosage.substr(0, 20); //truncate
				cout << "dosage truncated to: " << dosage << endl;
			}

			vendorString = getInput("Vendor ID (Max 4 characters)");
			if (vendorString.size() > 4) {
				vendorString = vendorString.substr(0, 4);
				cout << "Vendor truncated to: " << vendorString << endl;
			}
			if (vendorString.empty()) {
				vendorId = NULL;
			}
			else {
				vendorId = stoi(vendorString);
			}

			expectedDeliveryDate = getInput("Expected Delivery Date (Max 20 characters)");
			if (expectedDeliveryDate.size() > 20)
			{
				expectedDeliveryDate = expectedDeliveryDate.substr(0, 20); //truncate
				cout << "Expected Delivery Date truncated to: " << expectedDeliveryDate << endl;
			}

			whRefillLevelString = getInput("Warehouse Refill Level (Max 10 characters)");
			if (whRefillLevelString.size() > 10) {
				whRefillLevelString = whRefillLevelString.substr(0, 10);
				cout << "Warehouse Refill Level truncated to: " << whRefillLevelString << endl;
			}
			if (whRefillLevelString.empty()) {
				whRefillLevel = NULL;
			}
			else {
				whRefillLevel = stoi(whRefillLevelString);
			}

			whRefillQtyString = getInput("Warehouse Refill Quantity (Max 10 characters)");
			if (whRefillQtyString.size() > 10) {
				whRefillQtyString = whRefillQtyString.substr(0, 10);
				cout << "Warehouse Quantity Level truncated to: " << whRefillQtyString << endl;
			}
			if (whRefillQtyString.empty()) {
				whRefillQty = NULL;
			}
			else {
				whRefillQty = stoi(whRefillQtyString);
			}

			whLevelString = getInput("Warehouse Level (Max 10 characters)");
			if (whLevelString.size() > 10) {
				whLevelString = whLevelString.substr(0, 10);
				cout << "Warehouse Level truncated to: " << whLevelString << endl;
			}
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

		Item *item = DatabaseManager::shared()->updateItem(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel);

		if (item) {
			cout << "Item successfully updated." << endl;
		}
		else {
			cout << "Item not found" << endl;
		}
	}
};