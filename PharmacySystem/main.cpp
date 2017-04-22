#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept> // Used for out_of_range error
#include <iomanip>
#include "DatabaseManager.h"
#include "User.cpp"
#include "DatabaseManagerTests.h"

// Controllers
#include "LoginRegistration.cpp"
#include "ManageStore.cpp"
#include "ManageItem.cpp"
#include "CreatePrescriptionController.cpp"
#include "ItemViewController.cpp"
#include "CreateReviewController.cpp";
using namespace std;

/// Will clear the windows console
void clearWindowsConsole() {
	system("cls");
}

void printHelp(UserType type) {
	cout << "#########################" << endl;
	cout << "Available Commands:" << endl;
	// General all-user commands
	cout << "'list stores {number of stores}' : Will list all available stores. Optionally can state the number of stores to show." << endl;
	cout << "'view review {store ID}' : Will list store reviews for a store ID. Optionally can state a number of reviews to print." << endl;

	// User specific commands
	if (type == Employee) {
		cout << "'create prescription' : Will begin process to create a customer prescription." << endl;
		cout << "'view history {customer ID}' : View the history of a customer. Specify the customer ID to view the history of." << endl;
		cout << "'list items {number of items}': Will list all items in the system. Optionally can state the number of items to show." << endl;
		cout << "'manage item {item ID}' : Update the attributes of a item. Specify the item ID to make modifications." << endl;
		cout << "'manage store {store ID}' : Update the attributes of a store. Specify the store ID to make modifications." << endl;
		cout << "'create prescription': Will begin process to create a new prescription" << endl;
	}
	else if (type == Customer) {
		cout << "'create review': Will begin process to leave a review for a store." << endl;
		cout << "'view history' : View your purchase history." << endl;
	}

	cout << "'clear': Will clear the console window to avoid clutter." << endl;
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

void printStoreReviews(DatabaseManager *dbm, Store *store) {
	vector<Review*> reviews = dbm->getReviews(store->getId());

	cout << "Store Reviews:" << endl;
	if (reviews.size() == 0) {
		cout << "No reviews available for this store." << endl;
	}
	for (Review *review : reviews) {
		User *reviewer = dbm->getUser(review->getAccountId());

		cout << string(40, '-') << endl;
		cout << "By " << reviewer->getUsername() << " on " << review->getDate() << "\t" << "Rating: " << review->getRating() << "/5" << endl << endl;
		cout << review->getText() << endl;
	}

	cout << endl;
}

void printHistory(DatabaseManager *dbm, int customerId) {
	vector<Prescription*> prescriptions = dbm->getPrescriptionHistory(customerId);

	cout << "Prescriptions:" << endl;
	if (prescriptions.size() == 0) {
		cout << "No prescriptions found for this user." << endl;
	}

	for (Prescription *p : prescriptions) {
		User *customer = dbm->getUser(customerId);
		if (customer == nullptr) { continue; } //Guard

		vector<Purchase*> purchases = dbm->getPurchases(p);
		if (purchases.size() == 0) { continue; } //Guard

		cout << string(40, '-') << endl;
		cout << "Prescription on " << p->getDate() << " by " << customer->getUsername() << ". " << "Store: " << p->getStoreId() << endl;
		
		const char separator = ' ';
		const int nameWidth = 32;
		const int numWidth = 8;

		cout << left << setw(nameWidth) << setfill(separator) << "Name";
		cout << left << setw(numWidth) << setfill(separator) << "Qty";
		cout << left << setw(numWidth) << setfill(separator) << "Sale Price" << endl << endl;

		for (Purchase* purchase : purchases) {
			Item *item = dbm->getItem(purchase->getItemId());

			string parsedName = item->getName();
			if (parsedName.size() >= nameWidth) {
				parsedName = parsedName.substr(0, nameWidth - 4) + "...";
			}

			cout << left << setw(nameWidth) << setfill(separator) << parsedName;
			cout << left << setw(numWidth) << setfill(separator) << purchase->getQuantity();
			cout << left << setw(numWidth) << setfill(separator) << "$" + to_string(purchase->getSalePrice());
			cout << endl;
		}
	}
	cout << string(40, '-') << endl << endl;
}

void printItemTable(unsigned int count = NULL) {
	ItemViewController::printItemTable(count);
}

//DEBUG
User* DebugGetEmployeeUser() {
	return DatabaseManager::shared()->getUser("jon", "testpass");
}
//END DEBUG


int main() {
	DatabaseManager *dbm = DatabaseManager::shared();
	runTests(dbm);
	
	/* Start Login & Registration Process */
	// DEBUG - commented out so dont have to repeatadly sign in. Uncomment to reactivate the login feature
	LoginRegistration lr;
	lr.displayScreen();

	User *user = lr.getAuthorizedUser();
	if (user == nullptr) { //Guard
		cout << "[!] An error occured in logging in. Please close the program and try again." << endl;
		return 1; //1 is standardly returned for entire program errors 
	}
	
	//User *user = DebugGetEmployeeUser();

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
			else if ("clear" == input.at(0)) { clearWindowsConsole();  }
			else if ("list stores" == input.at(0) + " " + input.at(1)) {
				if (input.size() > 2) {
					int countInput = stoi(input.at(2));
					if (countInput <= 0) { //is signed
						throw exception("Input is a signed int. Expected unsigned.");
					}
					unsigned int count = (unsigned int)countInput;
					printStores(dbm, count);
				} else {
					printStores(dbm);
				}
			}
			else if ("view history" == input.at(0) + " " + input.at(1)) {
				int customerID;
				if (user->isEmployee()) {
					int inputID = stoi(input.at(2));
					if (dbm->getUser(inputID) == nullptr) {
						throw exception("No user available for input id.");
					}
					customerID = inputID;
				}
				else { //is customer user
					customerID = user->getUserID();
				}
				printHistory(dbm, customerID);
			}
			else if ("view review" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				Store *store = dbm->getStore(stoi(input.at(2)));
				if (!store) { //gaurd
					cout << "No store available for that store ID." << endl;
				} else {
					printStoreReviews(dbm, store);
				}
			}
			// Customer Specific Commands
			else if (user->getUserType() == Customer && "create review" == input.at(0) + " " + input.at(1)) {
				CreateReviewController reviewController;
				reviewController.createReview(user);
			}

			// Employee Specific Commands
			else if (user->isEmployee() && "list items" == input.at(0) + " " + input.at(1)) {
				if (input.size() > 2) {
					int countInput = stoi(input.at(2));
					if (countInput <= 0) { //is signed
						throw exception("Input is a signed int. Expected unsigned.");
					}
					unsigned int count = (unsigned int)countInput;
					printItemTable(count);
				}
				else {
					printItemTable();
				}
			}
			else if (user->isEmployee() && "manage store" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				ManageStore ms;
				ms.promptForInput(stoi(input.at(2)));
			}
			else if (user->isEmployee() && "manage item" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				ManageItem mi;
				mi.promptForInput(stoi(input.at(2)));
			}
			else if (user->isEmployee() && "create prescription" == input.at(0) + " " + input.at(1)) {
				CreatePrescriptionController prescriptionController;
				prescriptionController.startCreatePrescription();
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