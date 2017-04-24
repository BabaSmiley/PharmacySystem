#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "ItemTablePrinter.cpp"

class CommonUserPrompts {
public:

	/// Prompts user for user input and returns a user from the database, or else returns nullptr
	static User* getUserFromInput() {
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
	static Store* getStoreFromInput(string prompt) {
		cout << prompt << " (Or 'exit' to cancel)" << endl;
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

	/* Prompts user for input of an item name and quantity. Will return an ItemOrder if successful, or nullptr if user types 'exit' during method
		Parameter prompt: A string of what to display to the user, asking input for an item
		Parameter store: A existing store object to be used for printing items from the store (must not be null)
	*/
	static Item* getItemFromInput(string prompt, Store *store) {
		Item *item = nullptr;

		while (item == nullptr) {

			cout << endl << prompt << endl;
			cout << "(Or type 'list items' to a view a list of all items in the store) ('exit' to cancel)" << endl;
			string userInput = getInput("item id");

			vector<string> input = splitString(userInput, " ");

			try {
				if ("exit" == input.at(0)) {
					return nullptr;
				}
				else if (input.size() == 2 && "list items" == input.at(0) + " " + input.at(1)) {
					printItemTable(store);
				}
				else {
					int itemId = stoi(userInput);

					Item *resultForInput = DatabaseManager::shared()->getItem(itemId);
					if (resultForInput == nullptr) {
						throw exception("Result nonexistant.");
					}
					else if (DatabaseManager::shared()->getInventory(store->getId(), resultForInput->getId()) == nullptr) {
						throw exception("Item is not carried by this store.");
					}

					item = resultForInput;
				}
			}
			catch (const exception &err) {
				cout << "That item does not exist for the given store." << endl;
			}
		}

		return item;
	}

	/* Will repeatedly prompt the user until a quantity is entered
	Parameter prompt: A string to display to the user asking for an quantity input
	Returns: A positive integer if a quantity was entered or -1 if the user chose to exit
	*/
	static int getQuantityFromUser(string prompt) {
		int quantity = -1;

		while (quantity == -1) {
			try {
				cout << prompt << " ('exit' to cancel)" << endl;
				string quantityStr = getInput("quantity");

				if ("exit" == quantityStr) {
					return -1;
				}
				else if (stoi(quantityStr) > 0) {
					return stoi(quantityStr);
				}
				else {
					throw exception("Negative quantity entered.");
				}
			}
			catch (const exception &e) {
				cout << "Invalid quantity entered." << endl;
			}
		}

		return quantity;
	}

private:

	/* HELPERS */
	static void printItemTable(Store *store) {
		DatabaseManager *dbm = DatabaseManager::shared();
		ItemTablePrinter::printItemTable(dbm, store);
	}
};