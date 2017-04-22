#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Store.cpp"
using namespace std;

class ManageStore {
public:

	void promptForInput(int id) {
		getStoreUpdateInput(id);
	}

private:

	void getStoreUpdateInput(int id) throw(const char*) {
		string address;
		string city;
		string state;
		string zipCodeString;
		int zipCode;
		string priorityLevelString;
		int priorityLevel;
		string isActiveString;
		int isActive;

		cout << endl << "To update an attribute of the store, enter the new value when prompted. Otherwise, just hit enter." << endl;
		address = getInput("Address");
		city = getInput("City");
		state = getInput("State");
		zipCodeString = getInput("Zip Code");
		try {
			if (zipCodeString.empty()) {
				zipCode = NULL;
			}
			else {
				zipCode = stoi(zipCodeString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		priorityLevelString = getInput("Priority Level");
		try {
			if (priorityLevelString.empty()) {
				priorityLevel = NULL;
			}
			else {
				priorityLevel = stoi(priorityLevelString);
			}
		}
		catch (const char *e) {
			throw e;
		}
		isActiveString = getInput("Is Active");
		try {
			if (isActiveString.empty()) {
				isActive = NULL;
			}
			else {
				isActive = stoi(isActiveString);
			}
		}
		catch (const char *e) {
			throw e;
		}

		Store *store = DatabaseManager::shared()->updateStore(id, address, city, state, zipCode, priorityLevel, isActive);

		if (store) {
			cout << "Store successfully updated." << endl;
		}
		else {
			throw "A store with this ID does not exist.";
		}
	}
};