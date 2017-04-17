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

		cout << endl << "To update an attribute of the store, enter the new value when prompted. Otherwise, just hit enter." << endl;
		cout << "Address: ";
		address = getInput("address");
		cout << endl << "City: ";
		city = getInput("city");
		cout << endl << "State: ";
		state = getInput("state");
		cout << endl << "Zip Code: ";
		zipCodeString = getInput("zipCodeString");
		try {
			zipCode = stoi(zipCodeString);
		}
		catch (const char *e) {
			throw e;
		}
		cout << endl << "Priority Level: ";
		priorityLevelString = getInput("priorityLevelString");
		try {
			priorityLevel = stoi(priorityLevelString);
		}
		catch (const char *e) {
			throw e;
		}

		Store *store = DatabaseManager::shared()->updateStore(id, address, city, state, zipCode, priorityLevel);
	}
};