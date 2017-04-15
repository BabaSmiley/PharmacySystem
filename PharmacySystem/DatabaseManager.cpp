#include "DatabaseManager.h"
using namespace std;

/* Helper Functions*/
template<typename T>
string quotesql(const T& sql) {
	stringstream ss;
	ss << sql;
	return string("'") + ss.str() + string("'");
}

string sqlToString(const unsigned char* str) {
	return string(reinterpret_cast<const char*>(str));
}

/* DatabaseManager */
DatabaseManager::DatabaseManager() {
	
	/* Attempt to open connection to file */
	int returnCode = sqlite3_open(DatabaseConstants::DatabaseName, &db);
	
	/* MARK - disabled for now
	if (returnCode) {
		cout << "Failed to open database: " << sqlite3_errmsg(db);
		//TODO: throw error here
	}
	else {
		cout << "Successfully connected to database." << endl;
	}
	*/
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
User* DatabaseManager::createUser(string username, string password, UserType userType) {
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

bool DatabaseManager::deleteStore(Store *store) {
	
	// Preliminary check if the store exists
	Store *dbStore = getStore(store->getId());
	if (dbStore == nullptr || *store != *dbStore) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "delete from Store where Id=" + quotesql(store->getId());

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Store* DatabaseManager::createStore(int id, string address, string city, string state, int zipCode, int priorityLevel) {
	sqlite3_stmt *stmt;
	Store *newStore = nullptr;
	
	string sql = "insert into Store (Id, Address, City, State, ZipCode, PriorityLevel) values (" + quotesql(id) + "," + quotesql(address) + "," + quotesql(city) + ","+ quotesql(state) + "," + quotesql(zipCode) + "," + quotesql(priorityLevel) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newStore = new Store(id, address, city, state, zipCode, priorityLevel);
		}
	}
	sqlite3_finalize(stmt);

	return newStore;
}

Store* DatabaseManager::getStore(int storeId) {
	sqlite3_stmt *stmt;
	Store *result = nullptr;

	string sql = "select Id, Address, City, State, ZipCode, PriorityLevel from Store where Id=" + quotesql(storeId);
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id            = sqlite3_column_int(stmt, 0);
			string address    = sqlToString(sqlite3_column_text(stmt, 1));
			string city       = sqlToString(sqlite3_column_text(stmt, 2));
			string state      = sqlToString(sqlite3_column_text(stmt, 3));
			int zipCode       = sqlite3_column_int(stmt, 4);
			int priorityLevel = sqlite3_column_int(stmt, 5);

			result = new Store(id, address, city, state, zipCode, priorityLevel);
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

/// Items ///

bool DatabaseManager::deleteItem(int itemId) {
	Item *dbItem = getItem(itemId);
	if (dbItem == nullptr || itemId != dbItem->getId()) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Item WHERE Id = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Item* DatabaseManager::createItem(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, long onOrderQty) {
	sqlite3_stmt *stmt;
	Item *newItem = nullptr;

	string sql = "INSERT INTO Item (Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, onOrderQty) VALUES (" + quotesql(id) + "," + quotesql(name) + "," + quotesql(description) + "," + quotesql(price) + "," + quotesql(dosage) + "," + quotesql(vendorId) + "," + quotesql(expectedDeliveryDate) + "," + quotesql(whRefillLevel) + "," + quotesql(whRefillQty) + "," + quotesql(whLevel) + "," + quotesql(onOrderQty) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newItem = new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, onOrderQty);
		}
	}

	sqlite3_finalize(stmt);

	return newItem;
}

Item* DatabaseManager::getItem(int itemId) {
	sqlite3_stmt *stmt;
	Item *result = nullptr;

	string sql = "SELECT Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, onOrderQty FROM Item WHERE Id = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			string name = sqlToString(sqlite3_column_text(stmt, 1));
			string description = sqlToString(sqlite3_column_text(stmt, 2));
			int price = sqlite3_column_int(stmt, 3);
			string dosage = sqlToString(sqlite3_column_text(stmt, 4));
			int vendorId = sqlite3_column_int(stmt, 5);
			string expectedDeliveryDate = sqlToString(sqlite3_column_text(stmt, 6));
			long whRefillLevel = sqlite3_column_int(stmt, 7);
			long whRefillQty = sqlite3_column_int(stmt, 8);
			long whLevel = sqlite3_column_int(stmt, 9);
			long onOrderQty = sqlite3_column_int(stmt, 10);

			result = new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, onOrderQty);
		}
	}

	sqlite3_finalize(stmt);

	return result;
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

