#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "ItemViewController.cpp"
using namespace std;


class CreatePrescriptionController {
public:

	Prescription* startCreatePrescription() {
		
		User *customer = getUserFromInput();
		if (customer == nullptr) {
			// Customer user was not able to be retrived
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		Store *store = getStoreFromInput();
		if (store == nullptr) {
			// Store was not able to be retrived
			cout << endProcessMessage << endl << endl;
			return nullptr;
		}

		//1. Prompt and get Items from user to include in the prescription
		vector<ItemOrder*> ordersToInclude;
		ItemOrder *currentOrder;
		do {
			currentOrder = getItemOrderFromInput();
			if (currentOrder != nullptr) {
				ordersToInclude.push_back(currentOrder);
			}
			
		} while (currentOrder != nullptr);

		if (ordersToInclude.size() == 0) { //Guard: No items added for prescription
			cout << "No items added to prescription." << endl;
			cout << endProcessMessage << endl;
			return nullptr;
		}

		//2. Create a new Prescription in the database
		string today = getDate();
		Prescription *prescription = DatabaseManager::shared()->createPrescription(today, customer->getUserID(), store->getId());
		int prescriptionId = prescription->getId();

		//3. Create Purchases for every item specified and tie it to the created prescription
		int prescriptionCost = 0;
		for (ItemOrder* itemOrder : ordersToInclude) {
			Item *item = itemOrder->item;

			int salePrice = item->getPrice(); //TODO: account for Discounts on items

			prescriptionCost += salePrice;
			DatabaseManager::shared()->createPurchase(prescriptionId, item->getId(), itemOrder->quantity, salePrice);
		}

		cout << endl << "Prescription #" << prescription->getId() << " successfully created." << endl;
		cout << "Total Cost: " << prescriptionCost << endl << endl;

		return prescription;
	}

private:

	struct ItemOrder {
		Item *item;
		int quantity;

		ItemOrder(Item *item, int quantity) {
			this->item = item;
			this->quantity = quantity;
		}
	};

	const char *exitAnytimeMessage = "Enter 'exit' anytime in order to exit the create prescription process.";
	const char *endProcessMessage = "Ended create prescription process.";
	const char *itemDoesNotExistMessage = "That item does not exist.";


	/// Prompts user for user input and returns a user from the database, or else returns nullptr
	User* getUserFromInput() {
		cout << endl << "Enter a customer's username: (Or 'exit' to cancel)" << endl;
		string input = getInput("username");
		
		if (input == "exit") {
			return nullptr;
		}

		User *user = DatabaseManager::shared()->getUser(input);
		if (user == nullptr || user->getUserType() != Customer) {
			cout << "[!] Invalid customer credentials." << endl;
			return nullptr;
		}
		return user;
	}

	/// Prompts user for user input and returns a store from the database, or else returns nullptr
	Store* getStoreFromInput() {
		cout << "Enter the store id to order from: (Or 'exit' to cancel)" << endl;
		string input = getInput("storeId");
		
		if (input == "exit") {
			return nullptr;
		}

		try {
			return DatabaseManager::shared()->getStore(stoi(input));
		}
		catch (const exception &e) {
			cout << "[!] Invalid store id." << endl;
			return nullptr;
		}
	}

	/// Prompts user for input of an item name and quantity. Will return an ItemOrder if successful, or nullptr if user types 'exit' during method
	ItemOrder* getItemOrderFromInput() {
		ItemOrder *itemOrder = nullptr;

		while (itemOrder == nullptr) {

			cout << endl << "Enter an item's name to add to the prescription:" << endl << "(Or type 'list items' to a view a list of all items) ('exit' to cancel)" << endl;
			string userInput = getInput("item");

			vector<string> input = splitString(userInput, " ");

			try {
				if ("exit" == input.at(0)) {
					return nullptr;
				}
				else if (input.size() == 2 &&  "list items" == input.at(0) + " " + input.at(1)) {
					printItemTable();
				}
				else {
					Item *resultForInput = DatabaseManager::shared()->getItem(userInput);
					if (resultForInput == nullptr) {
						throw exception(); //result nonexistant
					}

					cout << "Enter quantity to order: " << endl;
					string quantityStr = getInput("quantity");
					int quantity = stoi(quantityStr);

					if (quantity > 0) {
						itemOrder = new ItemOrder(resultForInput, quantity);
					}
					else {
						cout << "Invalid quantity entered." << endl;
					}
				}
			}
			catch (const exception &err) {
				cout << itemDoesNotExistMessage << endl;
			}
		}
		
		// Reaches here if itemOrder != nullptr
		return itemOrder;
	}


	/* HELPERS */
	void printItemTable() {
		ItemViewController::printItemTable();
	}

	/// Returns the current date in the format YYYY-MM-DD
	string getDate() {
		time_t t = time(0);
		struct tm now;
		localtime_s(&now, &t);

		char time[80];
		strftime(time, 80, "%Y-%m-%d", &now);
		return time;
	}

};