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
#include "Purchase.cpp"
#include "Inventory.cpp"
#include "Review.cpp"
#include "Sale.cpp"
#include "Discount.cpp"
#include "AddItem.cpp"
#include "SideEffect.cpp"

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
	User* getUser(string username);
	User* getUser(int userID);

	/* Create and add a user to the database
		Returns: returns the User object if it was successfully added to the database. 
				 Will return nullptr if a username already exists for `username` in the database.
	*/
	User* createUser(string username, string password, string address, UserType userType);


	/* Delete a store from the database
		Parameter store: A store object representing the store to delete in the database.
		  All fields in store must match database exactly for it to be deleted.
		Returns: returns true if `store` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteStore(Store *store);

	/* Creates and stores parameters in stores database
		Returns: returns a Store object representing a DB Store model
	*/
	Store* createStore(int id, string address, string city, string state, int zipCode, int priorityLevel, int isActive);

	/* Updates and stores parameters in stores database
		Returns: Returns an updated store object representing a DB Store model
	*/
	Store* updateStore(int id, string address, string city, string state, int zipCode, int priorityLevel);

	/* Get a store from the database
		Returns: returns a filled in Store object, or nullptr if no store was found for `storeId`
	*/
	Store* getStore(int storeId);

	/* Get a store from the database
	Returns: returns a filled in Store object, or nullptr if no store was found for `storeId`
	*/
	Store* getStoreRegardlessOfActiveness(int storeId);

	/* Gets all stores stored in the database
		Paramater count: the number of stores to return. Returns historically, oldest first.
		Returns: A vector of Store objects representing all stores in the database
	*/
	vector<Store*> getStores(unsigned int count = NULL);
	
	/* Delete an item from the database (set item isActive to false, delete all store inventory that has item)
	Parameter itemId: An item id representing the item to delete in the database.
	Returns: Returns true if `item` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteItem(int itemId);

	/* Delete all of an item's inventory from the database
		Parameter itemId: An item id representing the item to delete in the database.
		  Do not delete from sales and item data stores.
		Returns: Returns true if `item` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteItemInventory(int itemId);

	/* Creates and stores parameters in items database
		Returns: Returns an item object representing a DB Item model
	*/
	Item* createItem(string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, int isActive);

	/* Creates and stores parameters in items database with manual item id
		Returns: Returns an item object representing a DB Item model
	*/
	Item* createItem(int id, string name, string description, int price, string dosage, int vendorId, string expectedDeliveryDate, long whRefillLevel, long whRefillQty, long whLevel, int isActive);

	/* Updates and stores parameters in items database
		Must specific an id, all other fields optional
		Returns: Returns an item object representing a DB Item model
	*/
	Item* updateItem(int id, string name = NULL, string description = NULL, int price = -1, string dosage = NULL, int vendorId = -1, string expectedDeliveryDate = NULL, long whRefillLevel = -1, long whRefillQty = -1, long whLevel = -1, int isActive = -1);

	/* Gets an item from the database
		Parameter onlyActiveItem: Whether or not to only return active items from the database. Default is to only return active items.
		Returns: Returns a filled in Item object, or nullptr if no item was found
	*/
	Item* getItem(int itemId, bool onlyActiveItem = true);
	Item* getItem(string itemName, bool onlyActiveItem = true);


	/* Gets an item from the database
	Returns: Returns a filled in Item object, or nullptr if no item was found
	*/
	Item* getItemRegardlessOfActiveness(int itemId);
	/* Gets a list of all items in the database
		Parameter onlyActiveItems: Will return only the items marked as IsActive in the database
		Returns: a vector of all items in the database. If count is specified it will return items from oldest to newest.
	*/
	vector<Item*> getItems(unsigned int count = NULL, bool onlyActiveItems = true);


	/* Delete a prescription from the database
		Parameter prescription id: A prescription id representing the prescription to delete in the database.
		Returns: Returns true if `prescription` is an exact match in the database and was able to successfully delete it
	*/
	bool deletePrescription(int prescriptionId);

	/* Creates and stores parameters in prescription database
		Returns: Returns a prescription object representing a DB Prescription model
	*/
	Prescription* createPrescription(string date, int customerId, int storeId);

	/* Gets a prescription from the database
		Returns: Returns a filled in Prescription object, or nullptr if no prescription was found for `prescriptionId`
	*/
	Prescription* getPrescription(int prescriptionId);

	/* Gets an array of prescriptions for a given customers prescription history
		Returns: All prescriptions a customer has ever purchased
	*/
	vector<Prescription*> getPrescriptionHistory(int customerId);
	
	/* Created a Purchase object in the database
		Returns: a Purchase object representing the one created in the database or nullptr if it wasnt sucessfully created
	*/
	Purchase* createPurchase(int prescriptionId, int itemId, int quantity, int salePrice);

	/* Gets an array of purchases contained in a prescription
		Returns: All purchases inside of a given prescription
	*/
	vector<Purchase*> getPurchases(Prescription *prescription);

	/* Delete an inventory entry from the database
		Parameter item id, store id: An item id and a store id representing the inventory entry to delete in the database
		Returns: Returns true if `inventory` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteInventory(int storeId, int itemId);

	/* Creates and stores parameters in inventory database
		Returns: Returns an inventory object representing a DB Inventory model
	*/
	Inventory* createInventory(int storeId, int itemId, long itemLevel, long refillLevel, long refillQuantity, long onOrderQty);

	/* Gets an inventory entry from the database
		Returns: Returns a filled in Inventory object, or nullptr if no inventory was found for `storeId` and `itemId`
	*/
	Inventory* getInventory(int storeId, int itemId);

	/* Gets a list of all inventory at a store
		Returns: Returns a vector of inventory objects associated with a `storeId`
	*/
	vector<Inventory*> getStoreInventory(int storeId, unsigned int count = NULL);

	/* Updates a specified inventorys item level in the store
		Returns: Returns a filled inventory object representing the DB Inventory model
	*/
	Inventory* updateInventory(int storeId, int itemId, long quantity);

	/*
		Zeros an onorderqty for a store-item
		Returns: a boolean on whether it was done or not
	*/
	bool updateInventoryOnOrderQty(int storeId, int itemId, int newOnOrderQty);

	/* Deletes all of the inventory for a store
		Returns: Returns true if all inventory for store was successfully deleted
	*/
	bool deleteStoreInventory(int storeId);

	/* Gets a list of all inventory company wide
		Returns: Returns a vector of inventory objects across the whole company
	*/
	vector<Inventory*> getAllInventory();

	/* Creates a customer review for a store
		Returns: A Review object for the review created
	*/
	Review* createReview(int accountId, int storeId, int rating, string text, string date);

	/* Gets a stores reviews from the database
		Returns: Returns vector of review objects
	*/
	vector<Review*> getReviews(int storeId);

	/* Gets an items sales from the database
		Returns: Returns a vector of sales objects
	*/
	vector<Sale*> getSalesByItem(int itemId);

	/* Gets an stores sales from the database
		Returns: Returns a vector of sales objects
	*/
	vector<Sale*> getSalesByStore(int storeId);

	/* Creates and stores parameters in discount database
		Returns: Returns a discount object representing a DB Discount model
	*/
	Discount* createDiscount(int storeId, int itemId, int percentOff, string startDate, string endDate);

	/* Delete a discount from the database
		Parameter store id, item id: A store id and an item id represending the discount to delete in the database
		Returns: Returns true if `discount` is an exact match in the database and was able to successfully delete it
	*/
	bool deleteDiscount(int storeId, int itemId);

	/* Gets a discount from the database
		Returns: Returns a discount object
	*/
	Discount* getDiscount(int storeId, int itemId);

	/* Adds item to AddItem database
		Returns: Returns an add item object representing a DB AddItem model
	*/
	AddItem* createAddItemOrder(int itemId, int storeId, long quantity, long refillLevel, long refillQuantity);

	/* Gets list of all items requested in AddItem DB
		Returns: Returns list of all items in AddItem DB
	*/
	vector<AddItem*> getAllAddItems();

	/* Get an AddItem for a given store and item
		Returns: An AddItem which has the store and item id passed. Or nullptr if none is found.
	*/
	AddItem* getAddItem(int storeId, int itemId);

	/* Set the reorder quantity for a given item at a given store
	*/
	void setAddItemQuantity(int storeId, int itemId, int newQuantity);

	/* Clears AddItems table after retrieving them
		Returns returns true if table was cleared successfully
	*/
	bool clearAddItems();

	/* Gets a list of side effects associated with an item
		Returns: a vector of SideEffects which coorespond to an item
	*/
	vector<SideEffect*> getSideEffects(int itemId);



private:
	//static DatabaseManager *sharedInstance;
	sqlite3 *db;

	/* Constructor for creating a Manager object for a SQLite3 database */
	DatabaseManager();

	~DatabaseManager();

	void closeDatabase();

	Item* itemFromSQL(sqlite3_stmt *stmt);

	vector<vector<string>> query(const char *sql);
};

