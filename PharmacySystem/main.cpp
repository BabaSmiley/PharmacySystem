#include <iostream>
#include <string>
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
	// General all-user commands
	

	// User specific commands
	if (type == Employee) {

	}
	else if (type == Customer) {

	}
}


int main() {

	DatabaseManager *dbm = DatabaseManager::shared();

	runTests(dbm);

	
	LoginRegistration lr;
	lr.displayScreen();

	User *user = lr.getAuthorizedUser();
	if (user == nullptr) { //Guard
		cout << "[!] An error occured in logging in. Please close the program and try again." << endl;
		return 1; //1 is standardly returned for entire program errors 
	}

	//clearWindowsConsole();
	cout << string(8, '*') << " Logged in as: " << user->getUsername() << " " << string(8, '*') << endl;
	
	/* Verbose log for user info */
	//cout << "Logged in as: " << user->getUserID() << " " << user->getUsername() << endl;
	//string booleanText = user->isEmployee() ? "true" : "false";
	//cout << "Is an employee: " << booleanText << endl;
	
	cout << "Enter a command to begin. Or type 'help' to get a list of available commands." << endl;



	return 0;
}