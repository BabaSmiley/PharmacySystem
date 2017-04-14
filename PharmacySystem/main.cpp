#include <iostream>
#include <string>
#include <vector>
#include "DatabaseManager.h"
#include "LoginRegistration.cpp"
using namespace std;


void testDatabaseUser(DatabaseManager *dbm) {

	User *validEmployeeUser = dbm->getUser("jon", "testpass");
	if (validEmployeeUser != nullptr) {
		cout << "jon is a user." << endl;
		cout << "id: " << validEmployeeUser->getUserID() << endl;
		cout << "isEmployee: " << validEmployeeUser->isEmployee() << endl;
	}
	else {
		cout << "jon is not a user." << endl;
	}

	User *invalidUser = dbm->getUser("jonathan", "testpass1");
	if (invalidUser != nullptr) {
		cout << "jonathan is a user." << endl;
		cout << "id: " << invalidUser->getUserID() << endl;
		cout << "isEmployee: " << invalidUser->isEmployee() << endl;
	}
	else {
		cout << "jonathan is not a user." << endl;
	}

	//Test adding user
	User *newUser = dbm->createUser("jose2", "siracha", Employee);
	if (newUser != nullptr) {
		cout << "User was created!" << endl;
		cout << "User: " << newUser->getUserID() << ". Type: " << newUser->getUserType();
	}
	else {
		cout << "New user was not created. Jose may already exist." << endl;
	}
}




bool testCreateStore(DatabaseManager *dbm) {
	//Test Create
	Store *newStore = dbm->createStore(13, "123 Alley", "NewTown", "NY", 34592, 199);
	if (newStore) {
		cout << endl << "New store created" << endl;
		cout << "ID: " << newStore->getId() << endl;
		cout << "Address: " << newStore->getAddress() << endl;
		cout << "City/State: " << newStore->getCity() << "/" << newStore->getState() << " " << newStore->getZipCode() << endl;
		cout << "Priority: " << newStore->getPriorityLevel() << endl;
		return true;
	}
	else {
		cout << "Could not create new Store." << endl;
		return false;
	}
}

bool testGetStore(DatabaseManager *dbm) {
	Store *existingStore = dbm->getStore(13);
	if (existingStore == nullptr) {
		cout << "Store which should exist for id=1 does not." << endl;
		return false;
	}

	Store *doesNotExist = dbm->getStore(-1);
	if (doesNotExist != nullptr) {
		cout << "Store which shouldnt exist, does" << endl;
		return false;
	}
	return true;
}

bool testDeleteStore(DatabaseManager *dbm) {
	Store *s = dbm->getStore(13);
	return dbm->deleteStore(s);
}


/// Will clear the windows console
void clearWindowsConsole() {
	system("cls");
}

int main() {

	DatabaseManager *dbm = DatabaseManager::shared();
	//testDatabaseUser(dbm);
	//testCreateStore(dbm);

	///STORE DB FUNCTIONALITY
	//cout << testCreateStore(dbm) << endl;
	//cout << testGetStore(dbm) << endl;
	//cout << testDeleteStore(dbm) << endl;

	
	LoginRegistration lr;
	lr.displayScreen();

	//clearWindowsConsole();

	User *user = lr.getAuthorizedUser();
	cout << "Logged in as: " << user->getUserID() << " " << user->getUsername() << endl;
	string booleanText = user->isEmployee() ? "true" : "false";
	cout << "Is an employee: " << booleanText << endl;
	
	return 0;
}