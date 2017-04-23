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

	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

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
			int isEmployee = sqlite3_column_int(stmt, 1);
			UserType type = (isEmployee == 1) ? Employee : Customer;

			result = new User(id, username, type);
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

User* DatabaseManager::getUser(string username) {
	sqlite3_stmt *stmt;
	User *result = nullptr;

	string sql = "select Id, IsEmployee from Account where Username=" + quotesql(username);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			int isEmployee = sqlite3_column_int(stmt, 1);
			UserType type = (isEmployee == 1) ? Employee : Customer;

			result = new User(id, username, type);
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

User* DatabaseManager::getUser(int userID) {
	sqlite3_stmt *stmt;
	User *result = nullptr;

	string sql = "select Id, Username, IsEmployee from Account where Id=" + quotesql(userID);
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			string username = sqlToString(sqlite3_column_text(stmt, 1));
			int isEmployee = sqlite3_column_int(stmt, 2);
			UserType type = (isEmployee == 1) ? Employee : Customer;

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

	int isEmployee = (userType == Employee) ? 1 : 0;
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

Store* DatabaseManager::createStore(int id, string address, string city, string state, int zipCode, int priorityLevel, int isActive) {
	sqlite3_stmt *stmt;
	Store *newStore = nullptr;
	
	string sql = "insert into Store (Id, Address, City, State, ZipCode, PriorityLevel, IsActive) values (" + quotesql(id) + "," + quotesql(address) + "," + quotesql(city) + ","+ quotesql(state) + "," + quotesql(zipCode) + "," + quotesql(priorityLevel) + "," + quotesql(isActive) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newStore = new Store(id, address, city, state, zipCode, priorityLevel, isActive);
		}
	}
	sqlite3_finalize(stmt);

	return newStore;
}

Store* DatabaseManager::updateStore(int id, string address = NULL, string city = NULL, string state = NULL, int zipCode = NULL, int priorityLevel = NULL, int isActive = NULL) {
	sqlite3_stmt *stmt;
	Store *updatingStore = nullptr;

	string baseSql = "UPDATE Store SET Id = " + quotesql(id);
	if (!address.empty()) {
		baseSql += ", Address = " + quotesql(address);
	}
	if (!city.empty()) {
		baseSql += ", City = " + quotesql(city);
	}
	if (!state.empty()) {
		baseSql += ", State = " + quotesql(state);
	}
	if (zipCode != NULL) {
		baseSql += ", ZipCode = " + quotesql(zipCode);
	}
	if (priorityLevel != NULL) {
		baseSql += ", PriorityLevel = " + quotesql(priorityLevel);
	}
	if (isActive != NULL) {
		baseSql += ", IsActive = " + quotesql(isActive);
	}
	baseSql += " WHERE Id = " + quotesql(id);

	if (sqlite3_prepare_v2(db, baseSql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			updatingStore = getStore(id);
		}
	}

	sqlite3_finalize(stmt);

	return updatingStore;
}

Store* DatabaseManager::getStore(int storeId) {
	sqlite3_stmt *stmt;
	Store *result = nullptr;

	string sql = "select Id, Address, City, State, ZipCode, PriorityLevel, IsActive from Store where Id=" + quotesql(storeId);
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			string address = sqlToString(sqlite3_column_text(stmt, 1));
			string city = sqlToString(sqlite3_column_text(stmt, 2));
			string state = sqlToString(sqlite3_column_text(stmt, 3));
			int zipCode = sqlite3_column_int(stmt, 4);
			int priorityLevel = sqlite3_column_int(stmt, 5);
			int isActive	  = sqlite3_column_int(stmt, 6);

			result = new Store(id, address, city, state, zipCode, priorityLevel, isActive);
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

vector<Store*> DatabaseManager::getStores(unsigned int count) {
	sqlite3_stmt *stmt;
	vector<Store*> stores;

	string limitingSQL = "";
	if (count != NULL && count > 0) {
		limitingSQL = " limit " + to_string(count);
	}

	string sql = "select Id, Address, City, State, ZipCode, PriorityLevel, IsActive from Store" + limitingSQL;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Store *store;

			int id = sqlite3_column_int(stmt, 0);
			string address = sqlToString(sqlite3_column_text(stmt, 1));
			string city = sqlToString(sqlite3_column_text(stmt, 2));
			string state = sqlToString(sqlite3_column_text(stmt, 3));
			int zipCode = sqlite3_column_int(stmt, 4);
			int priorityLevel = sqlite3_column_int(stmt, 5);
			int isActive = sqlite3_column_int(stmt, 6);

			store = new Store(id, address, city, state, zipCode, priorityLevel, isActive);
			stores.push_back(store);
		}
	}
	sqlite3_finalize(stmt);

	return stores;
}


/// Items ///

bool DatabaseManager::deleteItem(int itemId) {
	Item *dbItem = getItem(itemId);
	if (dbItem == nullptr || itemId != dbItem->getId()) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Inventory WHERE ItemId = " + quotesql(itemId) + "; " + "UPDATE Item Set IsActive = '0' WHERE Id  " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Item* DatabaseManager::createItem(string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, int isActive) {
	sqlite3_stmt *stmt;
	Item *newItem = nullptr;

	string sql = "INSERT INTO Item (Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, IsActive) VALUES (" + quotesql(name) + "," + quotesql(description) + "," + quotesql(price) + "," + quotesql(dosage) + "," + quotesql(vendorId) + "," + quotesql(expectedDeliveryDate) + "," + quotesql(whRefillLevel) + "," + quotesql(whRefillQty) + "," + quotesql(whLevel) + "," + quotesql(isActive) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			int id = (int)sqlite3_last_insert_rowid(db);
			newItem = new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, isActive);
		}
	}

	sqlite3_finalize(stmt);

	return newItem;
}

Item* DatabaseManager::createItem(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, int isActive) {
	sqlite3_stmt *stmt;
	Item *newItem = nullptr;

	string sql = "INSERT INTO Item (Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, IsActive) VALUES (" + quotesql(id) + "," + quotesql(name) + "," + quotesql(description) + "," + quotesql(price) + "," + quotesql(dosage) + "," + quotesql(vendorId) + "," + quotesql(expectedDeliveryDate) + "," + quotesql(whRefillLevel) + "," + quotesql(whRefillQty) + "," + quotesql(whLevel) + "," + quotesql(isActive) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newItem = new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, isActive);
		}
	}

	sqlite3_finalize(stmt);

	return newItem;
}

bool AllSpaces(string str) {
	for (int i = 0; i < str.size(); i++)
		if (str[i] != ' ')
			return false;

	return true;
}

Item* DatabaseManager::updateItem(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, int isActive) {
	sqlite3_stmt *stmt;
	Item* updatingItem = nullptr;

	string baseSql = "UPDATE Item SET Id = " + quotesql(id);
	if (!name.empty() && !AllSpaces(name)) {
		baseSql += ", Name = " + quotesql(name);
	}
	if (!description.empty() && !AllSpaces(description)) {
		baseSql += ", Description = " + quotesql(description);
	}
	if (price != NULL && price >= 0) {
		baseSql += ", Price = " + quotesql(price);
	}
	if (!dosage.empty() && !AllSpaces(dosage)) {
		baseSql += ", Dosage = " + quotesql(dosage);
	}
	if (vendorId != NULL && vendorId >= 0) {
		baseSql += ", VendorId = " + quotesql(vendorId);
	}
	if (!expectedDeliveryDate.empty() && !AllSpaces(expectedDeliveryDate)) {
		baseSql += ", ExpectedDeliveryDate = " + quotesql(expectedDeliveryDate);
	}
	if (whRefillLevel != NULL && whRefillLevel >= 0) {
		baseSql += ", WhRefillLevel = " + quotesql(whRefillLevel);
	}
	if (whRefillQty != NULL & whRefillQty >= 0) {
		baseSql += ", WhRefillQty = " + quotesql(whRefillQty);
	}
	if (whLevel != NULL && whLevel >= 0) {
		baseSql += ", WhLevel = " + quotesql(whLevel);
	}
	if (isActive != NULL) {
		baseSql += ", IsActive = " + quotesql(isActive);
	}
	baseSql += " WHERE Id = " + quotesql(id);

	if (sqlite3_prepare_v2(db, baseSql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			updatingItem = getItem(id);
		}
	}

	sqlite3_finalize(stmt);

	return updatingItem;
}

Item* DatabaseManager::getItem(int itemId) {
	sqlite3_stmt *stmt;
	Item *result = nullptr;

	string sql = "SELECT Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, IsActive FROM Item WHERE Id = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		result = itemFromSQL(stmt);
	}
	sqlite3_finalize(stmt);

	return result;
}

Item* DatabaseManager::getItem(string itemName) {
	sqlite3_stmt *stmt;
	Item *result = nullptr;

	string sql = "SELECT Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, IsActive FROM Item WHERE Name = " + quotesql(itemName);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		result = itemFromSQL(stmt);
	}
	sqlite3_finalize(stmt);

	return result;
}

vector<Item*> DatabaseManager::getItems(unsigned int count) {
	sqlite3_stmt *stmt;
	vector<Item*> items;

	string limitingSQL = "";
	if (count != NULL && count > 0) {
		limitingSQL = " limit " + to_string(count);
	}

	string sql = "SELECT Id, Name, Description, Price, Dosage, VendorId, ExpectedDeliveryDate, WhRefillLevel, WhRefillQty, WhLevel, IsActive FROM Item" + limitingSQL;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Item *item;

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
			bool isActive = (sqlite3_column_int(stmt, 10) == 1) ? true : false;

			item = new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, isActive);
			items.push_back(item);
		}
	}
	sqlite3_finalize(stmt);

	return items;
}

/// Prescriptions ///

bool DatabaseManager::deletePrescription(int prescriptionId) {
	Prescription *dbPrescription = getPrescription(prescriptionId);
	if (dbPrescription == nullptr || prescriptionId != dbPrescription->getId()) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Prescription WHERE Id = " + quotesql(prescriptionId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Prescription* DatabaseManager::createPrescription(string date, int customerId, int storeId) {
	sqlite3_stmt *stmt;
	Prescription *newPrescription = nullptr;

	string sql = "INSERT INTO Prescription (Date, CustomerId, StoreId) VALUES (" + quotesql(date) + "," + quotesql(customerId) + "," + quotesql(storeId) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			int id = (int)sqlite3_last_insert_rowid(db);
			newPrescription = new Prescription(id, date, customerId, storeId);
		}
	}

	sqlite3_finalize(stmt);

	return newPrescription;
}

Prescription* DatabaseManager::getPrescription(int prescriptionId) {
	sqlite3_stmt *stmt;
	Prescription *result = nullptr;

	string sql = "SELECT Id, Date, CustomerId, StoreId FROM Prescription WHERE Id = " + quotesql(prescriptionId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			string date = sqlToString(sqlite3_column_text(stmt, 1));
			int customerId = sqlite3_column_int(stmt, 2);
			int storeId = sqlite3_column_int(stmt, 3);

			result = new Prescription(id, date, customerId, storeId);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

vector<Prescription*> DatabaseManager::getPrescriptionHistory(int customerId) {
	sqlite3_stmt *stmt;
	vector<Prescription*> prescriptions;

	string sql = "select Id, Date, StoreId from Prescription where CustomerId = " + quotesql(customerId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			string date = sqlToString(sqlite3_column_text(stmt, 1));
			int storeId = sqlite3_column_int(stmt, 2);

			Prescription *prescription = new Prescription(id, date, customerId, storeId);
			prescriptions.push_back(prescription);
		}
	}
	sqlite3_finalize(stmt);

	return prescriptions;
}

/// Purchase ///

Purchase* DatabaseManager::createPurchase(int prescriptionId, int itemId, int quantity, int salePrice) {
	sqlite3_stmt *stmt;
	Purchase *newPurchase = nullptr;

	string sql = "INSERT INTO Purchase (PrescriptionId, ItemId, Quantity, SalePrice) VALUES (" + quotesql(prescriptionId) + "," + quotesql(itemId) + "," + quotesql(quantity) + "," + quotesql(salePrice) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newPurchase = new Purchase(prescriptionId, itemId, quantity, salePrice);
		}
	}

	sqlite3_finalize(stmt);

	return newPurchase;
}

vector<Purchase*> DatabaseManager::getPurchases(Prescription *prescription) {
	sqlite3_stmt *stmt;
	vector<Purchase*> purchases;
	int prescriptionId = prescription->getId();

	string sql = "select ItemId, Quantity, SalePrice from Purchase where PrescriptionId = " + quotesql(prescriptionId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int itemId = sqlite3_column_int(stmt, 0);
			int quantity = sqlite3_column_int(stmt, 1);
			int salePrice = sqlite3_column_int(stmt, 2);

			Purchase *purchase = new Purchase(prescriptionId, itemId, quantity, salePrice);
			purchases.push_back(purchase);
		}
	}

	sqlite3_finalize(stmt);

	return purchases;
}

/// Inventory ///

bool DatabaseManager::deleteInventory(int storeId, int itemId) {
	Inventory *dbInventory = getInventory(storeId, itemId);
	if (dbInventory == nullptr || storeId != dbInventory->getStoreId() || itemId != dbInventory->getItemId()) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Inventory WHERE StoreId = " + quotesql(storeId) + " AND ItemId = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Inventory* DatabaseManager::createInventory(int storeId, int itemId, long itemLevel, long refillLevel, long refillQuantity, long onOrderQty) {
	sqlite3_stmt *stmt;
	Inventory *newInventory = nullptr;

	string sql = "INSERT INTO Inventory (StoreId, ItemId, ItemLevel, RefillLevel, RefillQuantity) VALUES (" + quotesql(storeId) + "," + quotesql(itemId) + "," + quotesql(itemLevel) + "," + quotesql(refillLevel) + "," + quotesql(refillQuantity) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newInventory = new Inventory(storeId, itemId, itemLevel, refillLevel, refillQuantity, onOrderQty);
		}
	}

	sqlite3_finalize(stmt);

	return newInventory;
}

Inventory* DatabaseManager::getInventory(int storeId, int itemId) {
	sqlite3_stmt *stmt;
	Inventory *result = nullptr;

	string sql = "SELECT StoreId, ItemId, ItemLevel, RefillLevel, RefillQuantity, onOrderQty FROM Inventory WHERE StoreId = " + quotesql(storeId) + " AND ItemId = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int storeId = sqlite3_column_int(stmt, 0);
			int itemId = sqlite3_column_int(stmt, 1);
			long itemLevel = sqlite3_column_int(stmt, 2);
			long refillLevel = sqlite3_column_int(stmt, 3);
			long refillQuantity = sqlite3_column_int(stmt, 4);
			long onOrderQty = sqlite3_column_int(stmt, 5);

			result = new Inventory(storeId, itemId, itemLevel, refillLevel, refillQuantity, onOrderQty);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

vector<Inventory*> DatabaseManager::getStoreInventory(int storeId) {
	sqlite3_stmt *stmt;
	vector<Inventory*> result;

	string sql = "SELECT StoreId, ItemId, ItemLevel, RefillLevel, RefillQuantity, onOrderQty FROM Inventory WHERE StoreId = " + quotesql(storeId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Inventory *inventory;

			int storeId = sqlite3_column_int(stmt, 0);
			int itemId = sqlite3_column_int(stmt, 1);
			long itemLevel = sqlite3_column_int(stmt, 2);
			long refillLevel = sqlite3_column_int(stmt, 3);
			long refillQuantity = sqlite3_column_int(stmt, 4);
			long onOrderQty = sqlite3_column_int(stmt, 5);

			inventory = new Inventory(storeId, itemId, itemLevel, refillLevel, refillQuantity, onOrderQty);

			result.push_back(inventory);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

bool DatabaseManager::deleteStoreInventory(int storeId) {
	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Inventory WHERE StoreId = " + quotesql(storeId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

vector<Inventory*> DatabaseManager::getAllInventory() {
	sqlite3_stmt *stmt;
	vector<Inventory*> result;

	string sql = "SELECT StoreId, ItemId, ItemLevel, RefillLevel, RefillQuantity, onOrderQty FROM Inventory";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_OK) {
			Inventory *inventory;

			int storeId = sqlite3_column_int(stmt, 0);
			int itemId = sqlite3_column_int(stmt, 1);
			long itemLevel = sqlite3_column_int(stmt, 2);
			long refillLevel = sqlite3_column_int(stmt, 3);
			long refillQuantity = sqlite3_column_int(stmt, 4);
			long onOrderQty = sqlite3_column_int(stmt, 5);

			inventory = new Inventory(storeId, itemId, itemLevel, refillLevel, refillQuantity, onOrderQty);

			result.push_back(inventory);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

/// Reviews ///

Review* DatabaseManager::createReview(int accountId, int storeId, int rating, string text, string date) {
	sqlite3_stmt *stmt;
	Review* result = nullptr;

	string sql = "INSERT INTO Review (AccountId, StoreId, Rating, Text, Date) VALUES (" + quotesql(accountId) + "," + quotesql(storeId) + "," + quotesql(rating) + "," + quotesql(text) + "," + quotesql(date) + ")";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = new Review(accountId, storeId, rating, text, date);
		}
	}

	sqlite3_finalize(stmt);

	return result;

}

vector<Review*> DatabaseManager::getReviews(int storeId) {
	sqlite3_stmt *stmt;
	vector<Review*> result;

	string sql = "SELECT AccountId, StoreId, Rating, Text, Date FROM Review WHERE StoreId = " + quotesql(storeId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Review *review;

			int accountId = sqlite3_column_int(stmt, 0);
			int storeId = sqlite3_column_int(stmt, 1);
			int rating = sqlite3_column_int(stmt, 2);
			string text = sqlToString(sqlite3_column_text(stmt, 3));
			string date = sqlToString(sqlite3_column_text(stmt, 4));

			review = new Review(accountId, storeId, rating, text, date);

			result.push_back(review);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}


/// Discount ///

Discount* DatabaseManager::createDiscount(int storeId, int itemId, int percentOff, string startDate, string endDate) {
	sqlite3_stmt *stmt;
	Discount *newDiscount = nullptr;

	string sql = "INSERT INTO Discount (StoreId, ItemId, PercentOff, StartDate, EndDate) VALUES (" + quotesql(storeId) + "," + quotesql(itemId) + "," + quotesql(percentOff) + "," + quotesql(startDate) + "," + quotesql(endDate) + ")";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			newDiscount = new Discount(storeId, itemId, percentOff, startDate, endDate);
		}
	}
	
	sqlite3_finalize(stmt);

	return newDiscount;
}

bool DatabaseManager::deleteDiscount(int storeId, int itemId) {
	Discount *dbDiscount = getDiscount(storeId, itemId);
	if (dbDiscount == nullptr || storeId != dbDiscount->getStoreId() || itemId != dbDiscount->getItemId()) {
		return false;
	}

	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM Discount WHERE StoreId = " + quotesql(storeId) + " AND ItemId = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
		}
	}
	sqlite3_finalize(stmt);

	return result;
}

Discount* DatabaseManager::getDiscount(int storeId, int itemId) {
	sqlite3_stmt *stmt;
	Discount *result = nullptr;

	string sql = "SELECT StoreId, ItemId, PercentOff, StartDate, EndDate FROM Discount WHERE StoreId = " + quotesql(storeId) + " AND ItemId = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			int storeId = sqlite3_column_int(stmt, 0);
			int itemId = sqlite3_column_int(stmt, 1);
			int percentOff = sqlite3_column_int(stmt, 2);
			string startDate = sqlToString(sqlite3_column_text(stmt, 3));
			string endDate = sqlToString(sqlite3_column_text(stmt, 4));

			result = new Discount(storeId, itemId, percentOff, startDate, endDate);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}


/// Sales ///

vector<Sale*> DatabaseManager::getSalesByItem(int itemId) {
	sqlite3_stmt *stmt;
	vector<Sale*> result;

	string sql = "SELECT Purchase.PrescriptionId, Purchase.Quantity, Purchase.SalePrice, Prescription.Date FROM Purchase LEFT JOIN Prescription ON Purchase.PrescriptionId = Prescription.Id WHERE Purchase.ItemId = " + quotesql(itemId);

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			Sale *sale;

			int prescriptionId = sqlite3_column_int(stmt, 0);
			int quantity = sqlite3_column_int(stmt, 1);
			int salePrice = sqlite3_column_int(stmt, 2);
			string date = sqlToString(sqlite3_column_text(stmt, 3));

			sale = new Sale(prescriptionId, quantity, salePrice, date);

			result.push_back(sale);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

/// Add Item For Reordering ///

AddItem* DatabaseManager::createAddItemOrder(int itemId, int storeId, long quantity) {
	sqlite3_stmt *stmt;
	AddItem* result = nullptr;

	string sql = "INSERT INTO AddItem (ItemId, StoreId, Quantity) VALUES (" + quotesql(itemId) + "," + quotesql(storeId) + "," + quotesql(quantity) + ")";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = new AddItem(itemId, storeId, quantity);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

vector<AddItem*> DatabaseManager::getAllAddItems() {
	sqlite3_stmt *stmt;
	vector<AddItem*> result;

	string sql = "SELECT ItemId, StoreId, Quantity FROM AddItem";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_OK) {
			AddItem *addItem;

			int itemId = sqlite3_column_int(stmt, 0);
			int storeId = sqlite3_column_int(stmt, 1);
			long quantity = sqlite3_column_int(stmt, 2);

			addItem = new AddItem(itemId, storeId, quantity);

			result.push_back(addItem);
		}
	}

	sqlite3_finalize(stmt);

	return result;
}

bool DatabaseManager::clearAddItems() {
	sqlite3_stmt *stmt;
	bool result = false;
	string sql = "DELETE FROM AddItem";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_DONE) {
			result = true;
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




/* ----------------- HELPERS ----------------- */

Item* DatabaseManager::itemFromSQL(sqlite3_stmt *stmt) {
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
		bool isActive = (sqlite3_column_int(stmt, 10) == 1) ? true : false;

		return new Item(id, name, description, price, dosage, vendorId, expectedDeliveryDate, whRefillLevel, whRefillQty, whLevel, isActive);
	}
	return nullptr;
}