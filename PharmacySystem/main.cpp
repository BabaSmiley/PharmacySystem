#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept> // Used for out_of_range error
#include "DatabaseManager.h"
#include "LoginRegistration.cpp"
#include "User.cpp"
#include "DatabaseManagerTests.h"
using namespace std;

/// Will clear the windows console
void clearWindowsConsole() {
	system("cls");
}

void printHelp(UserType type) {
	cout << "#########################" << endl;
	cout << "Available Commands:" << endl;
	// General all-user commands
	cout << "'list stores {number of stores}' : Will list all available stores. Optionally can state the number of stores to show" << endl;

	// User specific commands
	if (type == Employee) {
		cout << "'create prescription' : Will begin process to create a customer prescription." << endl;
		cout << "'create item' : Will begin process to create a new item." << endl;
		cout << "'delete item' : Will begin process to delete an item." << endl;
		cout << "'view history' : View the history of a customer." << endl;
	}
	else if (type == Customer) {
		cout << "'view history' : View your purchase history." << endl;
	}

	cout << endl << "'help' : Will print this usage statement." << endl;
	cout << "'logout' : Will log out of the system and close the program." << endl;
	cout << "#########################" << endl;
}

void printStores(DatabaseManager *dbm, unsigned int count = NULL) {
	vector<Store*> stores = dbm->getStores(count);
	cout << "Stores:" << endl;
	for (Store* store : stores) {
		cout << "[id " << store->getId() << "] " << store->getAddress() << " " << store->getCity() << ", " << store->getState() << " " << store->getZipCode() << endl;
	}
	cout << endl;
}


int main() {
	DatabaseManager *dbm = DatabaseManager::shared();
	//runTests(dbm);
	
	/* Start Login & Registration Process */
	LoginRegistration lr;
	lr.displayScreen();

	User *user = lr.getAuthorizedUser();
	if (user == nullptr) { //Guard
		cout << "[!] An error occured in logging in. Please close the program and try again." << endl;
		return 1; //1 is standardly returned for entire program errors 
	}

	//clearWindowsConsole();
	cout << string(8, '*') << " Logged in as: " << user->getUsername() << " " << string(8, '*') << endl;
	
	/* Take User Online Input For Commands */
	cout << "Enter a command to begin. Or type 'help' to get a list of available commands." << endl << endl;
	string userInput;
	bool shouldEndProgram = false;
	
	while (!shouldEndProgram) {
		userInput = getInput();
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower); //Convert input to all lowercase
		vector<string> input = splitString(userInput, " ");
		
		try { //try for out-of-range input exceptions
			if ("logout" == input.at(0)) {
				cout << "Logged out of system." << endl;
				shouldEndProgram = true;
			}
			else if ("help" == input.at(0)) {
				printHelp(user->getUserType());
			}
			else if ("list stores" == input.at(0) + " " + input.at(1)) {
				if (input.size() > 2) {
					unsigned int count = (unsigned int)stoi(input.at(2));
					cout << "count: " << count << endl;
					printStores(dbm, count);
				} else {
					printStores(dbm);
				}
			}
			else if ("view history" == input.at(0) + " " + input.at(1)) {
				if (user->isEmployee()) {

				}
				else { //is customer

				}
			}
			else {
				throw exception("User command not recognized in main.");
			}
		}
		catch (const exception& err) {
			// Catch any out_of_range errors, etc.
			cerr << "Caught exception: " << err.what() << endl; //DEBUG
			cout << "Invalid command. Type 'help' for a list of available commands." << endl;
		}
	}

	// Do cleanup before end of program...

	return 0;
}