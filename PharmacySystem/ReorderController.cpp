#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "DatabaseManager.h"
#include "CommonUserPrompts.cpp"
using namespace std;

class ReorderController {
public:

	// Returns true if reorder was able to update sucessfully
	bool promptUserToPlaceOrder() {

		const char *endProcessMessage = "Ended reorder process.";

		Store *store = CommonUserPrompts::getStoreFromInput("Enter the store id to reorder from:");
		if (store == nullptr) {
			cout << "Store was not found for this ID." << endl;
			cout << endProcessMessage << endl << endl;
			return false;
		}

		Item* item = CommonUserPrompts::getItemFromInput("Enter the item id you wish to reorder:", store);
		if (item == nullptr) {
			cout << "Item was not found for this ID." << endl;
			cout << endProcessMessage << endl << endl;
			return false;
		}

		int quantity = CommonUserPrompts::getQuantityFromUser("Enter quantity to reorder:");
		if (quantity < 0) {
			cout << "Quantity value was not valid." << endl;
			cout << endProcessMessage << endl << endl;
			return false;
		}
		
		Inventory *inventory = DatabaseManager::shared()->getInventory(store->getId(), item->getId());
		int newQuantity = inventory->getOnOrderQty() + quantity;
		bool success = DatabaseManager::shared()->updateInventoryOnOrderQty(store->getId(), item->getId(), newQuantity);

		if (success) {
			cout << "Reorder was set for store #" << store->getId() << ", item #" << item->getId() << " to quantity " << quantity << "." << endl << endl;
		}
		else {
			cout << "Reorder failed and was not placed." << endl << endl;
		}

		return success;
	}

};