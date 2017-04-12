#include "DatabaseManager.h"
using namespace std;

/* Helper Functions*/
template<typename T>
string quotesql(const T& sql) {
	stringstream ss;
	ss << sql;
	return string("'") + ss.str() + string("'");
}


/* DatabaseManager */
DatabaseManager::DatabaseManager(const char *filename) {

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

DatabaseManager::~DatabaseManager() {
	closeDatabase();
}

void DatabaseManager::closeDatabase() {
	sqlite3_close(db);
}

/// Users ///

/*
Returns a User object if a user exists in the database for the credentials. Or else returns `nullptr`.
*/
User* DatabaseManager::getUser(string username, string password) {
	sqlite3_stmt *stmt;
	User *result = nullptr;

	string sql = "select Id, IsEmployee from Account where Username=" + quotesql(username) + " and Password=" + quotesql(password);
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
User* DatabaseManager::addUser(string username, string password, UserType userType) {
	//...
	sqlite3_stmt *stmt;
	User *newUser = nullptr;

	//TODO: check if username is already taken -> return nullptr

	int isEmployee = (userType==Employee) ? 1 : 0;
	string sql = "insert into Account (Username, Password, IsEmployee) values (" + quotesql(username) + "," + quotesql(password) + "," + quotesql(isEmployee) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			int id = (int)sqlite3_last_insert_rowid(db);
			newUser = new User(id, username, userType);
		}
	}
	sqlite3_finalize(stmt);

	return newUser;
}

/// Stores ///

void DatabaseManager::deleteStore(int storeId) {
	return; //TODO
}

Store* DatabaseManager::createStore(string address, string city, string state, int zipCode, int priorityLevel) {
	sqlite3_stmt *stmt;
	Store *newStore = nullptr;
	
	string sql = "insert into Store (Address, City, State, ZipCode, PriorityLevel) values (" + quotesql(address) + "," + quotesql(city) + ","+ quotesql(state) + "," + quotesql(zipCode) + "," + quotesql(priorityLevel) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			int id = (int)sqlite3_last_insert_rowid(db);
			newStore = new Store(id, address, city, state, zipCode, priorityLevel);
		}
	}
	sqlite3_finalize(stmt);

	return newStore;
}

Store* DatabaseManager::getStore(int storeId) {
	return nullptr; //TODO
}



vector<vector<string>> DatabaseManager::query(const char *sql) {
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

