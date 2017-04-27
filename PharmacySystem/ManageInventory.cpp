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
		getInventoryAddInput(itemId, storeId);
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

	void getInventoryAddInput(int itemId, int storeId) {
		string quantityString;
		int quantity;
		
		quantityString = getInput("Quantity");
		try {
			quantity = stoi(quantityString);
		}
		catch (const char *e) {
			throw e;
		}

		AddItem *addItem = DatabaseManager::shared()->createAddItemOrder(itemId, storeId, quantity);

		if (addItem) {
			cout << "Item added to inventory queue successfully." << endl;
		}
		else {
			throw "Unable to add item to queue.";
		}
	}
};