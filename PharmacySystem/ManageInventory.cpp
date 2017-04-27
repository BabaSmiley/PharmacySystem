#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Inventory.cpp"
using namespace std;

class ManageInventory {
public:

	void promptForInventoryInput(int itemId, int storeId) {
		try {
			getInventoryAddInput(itemId, storeId);
		}
		catch (const char *e) {
			cout << e << endl;
		}
	}

	void deleteInventory(int itemId, int storeId, long quantityToAdd) {
		Item *item = DatabaseManager::shared()->getItem(itemId);
		long newLevel = item->getWhLevel() + quantityToAdd;
		bool inventoryDeleted = DatabaseManager::shared()->deleteInventory(storeId, itemId);
		if (inventoryDeleted) {
			Item *updatedItem = DatabaseManager::shared()->updateItem(itemId, "", "", -1, "", -1, "", -1, -1, newLevel, -1);
			if (updatedItem != nullptr) {
				cout << "Inventory deleted successfully." << endl;
			}
			else {
				cout << "Warehouse level not updated." << endl;
			}
		}
		else {
			cout << "Unsuccessful in deleting inventory" << endl;
		}
	}

private:

	void getInventoryAddInput(int itemId, int storeId) throw (const char *) {
		string input;
		int quantity, refillLevel, refillQuantity;
		
		try {
			input = getInput("Quantity");
			quantity = stoi(input);

			input = getInput("Refill Level");
			refillLevel = stoi(input);

			input = getInput("Refill Quantity");
			refillQuantity = stoi(input);

			if (quantity < 0 || refillQuantity < 0 || refillLevel < 0)
				throw "Negative value inputted.";
		}
		catch (const char *e) {
			throw "Unable to add item to queue.";
		}

		AddItem *addItem = DatabaseManager::shared()->createAddItemOrder(itemId, storeId, quantity, refillLevel, refillQuantity);

		if (addItem != nullptr) {
			cout << "Item added to inventory queue successfully." << endl;
		}
		else {
			throw "Unable to add item to queue.";
		}
	}
};