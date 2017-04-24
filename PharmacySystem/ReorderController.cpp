#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "DatabaseManager.h"
#include "CommonUserPrompts.cpp"
using namespace std;

class ReorderController {
public:

	AddItem* placeOrder() {

		const char *endProcessMessage = "Ended reorder process.";

		Store *store = CommonUserPrompts::getStoreFromInput("Enter the store id to reorder from:");
		if (store == nullptr) {
			cout << "Store was not found for this ID." << endl;
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		Item* item = CommonUserPrompts::getItemFromInput("Enter the item id you wish to reorder:", store);
		if (item == nullptr) {
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		int quantity = CommonUserPrompts::getQuantityFromUser("Enter quantity to reorder:");
		if (quantity < 0) {
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		// Reach here if store, item, and quantity have all been entered
		AddItem *reorder = DatabaseManager::shared()->createAddItemOrder(item->getId(), store->getId(), quantity);

		return reorder;
	}


private:

	



	

};