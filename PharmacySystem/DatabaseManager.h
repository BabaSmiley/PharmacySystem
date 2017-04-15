#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "sqlite3.h"
#include "Constants.h"

/* Import DB Models */
#include "User.cpp"
#include "Store.cpp"
#include "Item.cpp"
#include "Prescription.cpp"

class DatabaseManager
{
public:

	/* A singleton instance of the database manager
		The DatabaseManager handles all connections to the database
	*/
	static DatabaseManager *shared() {
		//if (!sharedInstance)
		DatabaseManager *sharedInstance = new DatabaseManager();
		return sharedInstance;
	}

	/* Get a user from the database if they exist.
		Returns: Returns a User object if a user exists in the database for the credentials. Or else returns `nullptr`.
	*/
	User* getUser(string username, string password);

	/* Create and add a user to the database
		Returns: returns the User object if it was successfully added to the database. 
				 Will return nullptr if a username already exists for `username` in the database.
	*/
	User* createUser(string username, string password, UserType userType);


	/* Delete a store from the database
		Parameter store: A store object representing the store to delete in the database.
		  All fields in store must match database exactly for it to be deleted.
		Returns: returns true if `store` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteStore(Store *store);

	/* Creates and stores parameters in stores database
		Returns: returns a Store object representing a DB Store model
	*/
	Store* createStore(int id, string address, string city, string state, int zipCode, int priorityLevel);

	/* Get a store from the database
		Returns: returns a filled in Store object, or nullptr if no store was found for `storeId`
	*/
	Store* getStore(int storeId);

	/* Delete an item from the database
		Parameter item id: An item id representing the item to delete in the database.
		  Do not delete from sales and item data stores.
		Returns: Returns true if `item` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteItem(int itemId);

	/* Creates and stores parameters in items database
		Returns: Returns an item object representing a DB Item model
	*/
	Item* createItem(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, long onOrderQty);

	/* Gets an item from the database
		Returns: Returns a filled in Item object, or nullptr if no item was found for `itemId`
	*/
	Item* getItem(int itemId);

	/* Delete a prescription from the database
		Parameter prescription id: A prescription id representing the prescription to delete in the database.
		Returns: Returns true if `prescription` is an exact match in the database and was able to successfully delete it
	*/
	bool deletePrescription(int prescriptionId);

	/* Creates and stores parameters in prescription database
		Returns: Returns a prescription object representing a DB Prescription model
	*/
	Prescription* createPrescription(int id, string date, int customerId, int storeId);

	/* Gets a prescription from the database
		Returns: Returns a filled in Prescription object, or nullptr if no prescription was found for `prescriptionId`
	*/
	Prescription* getPrescription(int prescriptionId);

private:
	//static DatabaseManager *sharedInstance;
	sqlite3 *db;

	/* Constructor for creating a Manager object for a SQLite3 database */
	DatabaseManager();

	~DatabaseManager();

	void closeDatabase();
	vector<vector<string>> query(const char *sql);
};

