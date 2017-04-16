#include <iostream>
#include <string>
#include <algorithm>
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
	string input;
	bool shouldEndProgram = false;
	
	while (!shouldEndProgram){
		input = getInput();
		transform(input.begin(), input.end(), input.begin(), ::tolower); //Convert input to all lowercase

		if (input == "logout") {
			cout << "Logged out of system." << endl;
			shouldEndProgram = true;
		}
		else if (input == "help") {
			printHelp(user->getUserType());
		}
		else if (input == "view history") {
			if (user->isEmployee()) {

			}
			else { //is customer

			}
		}
		else {
			cout << "Invalid command. Type 'help' for a list of available commands." << endl;
		}
	}

	// Do cleanup before end of program...

	return 0;
}