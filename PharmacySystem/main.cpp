#include <iostream>
#include <string>
#include <vector>
#include "Constants.h"
#include "DatabaseManager.h"
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
	User *newUser = dbm->addUser("jose2", "siracha", Employee);
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
	Store *newStore = dbm->createStore("123 Alley", "NewTown", "NY", 34592, 199);
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




int main() {

	DatabaseManager *dbm = new DatabaseManager(DatabaseConstants::DatabaseName);	
	//testDatabaseUser(dbm);
	//testCreateStore(dbm);

	///TEST DELETE STORE


	

	return 0;
}