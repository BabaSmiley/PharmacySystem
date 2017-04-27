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