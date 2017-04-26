#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Store.cpp"
using namespace std;

class ManageStore {
public:

	// Pre: storeId is a valid, existing storeId
	void promptForInput(int storeId) {
		try {
			bool success = getStoreUpdateInput(storeId);
			if (success) {
				cout << "The store's values were updated." << endl << endl;;
			}
			else {
				cout << "The store was unable to update. The store was not modified." << endl << endl;;
			}
		}
		catch (const char* e) {
			cout << "[!] An invalid input was entered. The store was not modified." << endl << endl;
		}
	}

private:

	// returns a bool if the process was successful or not
	bool getStoreUpdateInput(int id) throw(const char*) {
		string address;
		string city;
		string state;
		int zipCode;
		int priorityLevel;

		string zipCodeString;
		string priorityLevelString;

		cout << endl << "To update an attribute of the store, enter the new value when prompted. Otherwise, just hit enter." << endl;
		
		try {
			// Take input for each parameter and truncate if needed to the specified integer length
			address = getInput("Address (Max 20 characters)");
			if (address.size() > 20) {
				address = address.substr(0, 20); //truncate
				cout << "Address truncated to: " << address << endl;
			}

			city = getInput("City (Max 20 characters)");
			if (city.size() > 20) {
				city = city.substr(0, 20);
				cout << "City truncated to: " << city << endl;
			}

			state = getInput("State (Max 2 characters)");
			if (state.size() > 2) {
				state = state.substr(0, 2);
				cout << "State truncated to: " << state << endl;
			}

			zipCodeString = getInput("Zip Code (Max 9 characters)");
			if (zipCodeString.size() > 9) {
				zipCodeString = zipCodeString.substr(0, 2);
				cout << "ZipCode truncated to: " << zipCodeString << endl;
			}

			if (zipCodeString.empty()) {
				zipCode = NULL;
			}
			else {
				zipCode = stoi(zipCodeString);
			}

			priorityLevelString = getInput("Priority Level (Max 2 characters and between 0-15)");
			if (priorityLevelString.size() > 2) {
				priorityLevelString = priorityLevelString.substr(0, 2);
				cout << "Priority Level truncated to: " << priorityLevelString << endl;
			}

			if (priorityLevelString.empty()) {
				priorityLevel = NULL;
			}
			else {
				priorityLevel = stoi(priorityLevelString);

				if (priorityLevel < 1 || priorityLevel > 15) {
					cout << "The priority level must be between 0 and 15." << endl;
					throw "The input was not within the specified values.";
				}
			}
		}
		catch (const char *e) {
			throw e;
		}

		// Update the Store in the database
		Store *store = DatabaseManager::shared()->updateStore(id, address, city, state, zipCode, priorityLevel);

		if (store) {
			return true;
		}
		return false;
	}
};