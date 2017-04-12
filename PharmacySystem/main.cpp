#include <iostream>
#include <string>
#include <vector>
#include "Constants.h"
#include "DatabaseManager.h"
using namespace std;


int main() {

	DatabaseManager *dbm = new DatabaseManager(DatabaseConstants::DatabaseName);
	
	/*
	const char * sql_query = "select * from stores where id=1";
	vector<vector<string>> some = dbm->query(sql_query);

	for (int i = 0; i < some.size(); i++) {
		for (int j = 0; j < some[i].size(); j++) {
			cout << some[i][j] << " ";
		}
		cout << endl;
	}
	*/

	//Test users
	User *validEmployeeUser = dbm->getUser("jon", "testpass");
	if (validEmployeeUser != nullptr) {
		cout << "jon is a user." << endl;
		cout << "id: " << validEmployeeUser->getUserID() << endl;
		cout << "isEmployee: " << validEmployeeUser->isEmployee() << endl;
	} else {
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
	User *newUser = dbm->addUser("jose2", "siracha", false);
	if (newUser != nullptr) {
		cout << "User was created!" << endl;
		cout << "User: " << newUser->getUserID() << ". Type: " << newUser->getUserType();
	} else {
		cout << "New user was not created. Jose may already exist." << endl;
	}



	return 0;
}