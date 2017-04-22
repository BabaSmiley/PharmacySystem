#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"

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
	static Store* getStoreFromInput() {
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
};