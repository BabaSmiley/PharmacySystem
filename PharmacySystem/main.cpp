#include <iostream>
#include <string>
#include <vector>
#include "Constants.hpp"
using namespace std;


#include "sqlite3.h"
#include "User.cpp"
#include "StringUtils.cpp"

class DatabaseManager {
public:
	DatabaseManager(const char *filename) {

		/* Attempt to open connection to file */
		int returnCode = sqlite3_open(filename, &db);

		if (returnCode) {
			cout << "Failed to open database: " << sqlite3_errmsg(db);
			//TODO: throw error here
		}
		else {
			cout << "Successfully connected to database." << endl;
		}
	}

	~DatabaseManager() {
		closeDatabase();
	}

	void closeDatabase() {
		sqlite3_close(db);
	}

	/*
	 Returns a user from the database if they exist.

	 Returns: Returns a User object if a user exists in the database for the credentials. Or else returns `nullptr`.
	*/
	User* getUser(string username, string password) {
		sqlite3_stmt *stmt;
		User *result = nullptr;

		string sql = stringbuilder() << "select Id, IsEmployee from Account where Username='" << username << "' and Password='" << password << "'";
		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
			
			if (sqlite3_step(stmt) == SQLITE_ROW) {
				int id = sqlite3_column_int(stmt, 0);
				int isEmployee = sqlite3_column_int(stmt, 0);
				UserType type = isEmployee ? Employee : Customer;

				result = new User(id, username, type);
			}
		}
		sqlite3_finalize(stmt);

		return result;
	}

	/* Create and add a user to the database
		Returns: returns the User object if it was successfully added to the database.
	*/
	User* addUser(string username, string password, bool isEmployee) {
		//...
	}


	vector<vector<string>> query(const char *sql) {
		sqlite3_stmt *stmt;
		vector<vector<string>> results;

		int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
		if (rc != SQLITE_OK) {
			printf("Error with SQL command: %s", sqlite3_errmsg(db));
			return results;
		}
		
		int cols = sqlite3_column_count(stmt);
		while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
			vector<string> rowValues;

			for (int col = 0; col < cols; col++) {
				rowValues.push_back((char *)sqlite3_column_text(stmt, col));
			}
			
			results.push_back(rowValues);
		}
		if (rc != SQLITE_DONE) {
			printf("Error with SQL command: %s", sqlite3_errmsg(db));
		}
		sqlite3_finalize(stmt);
		return results;
	}

private:
	sqlite3 *db;
};


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





	return 0;
}