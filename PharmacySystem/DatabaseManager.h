#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "sqlite3.h"

/* Import DB Models */
#include "User.cpp"
#include "Store.cpp"

class DatabaseManager
{
public:

	/* Constructor for creating a Manager object for a SQLite3 database
		Paramater filename: a string for the name of the database file relative to this class
	*/
	DatabaseManager(const char *filename);

	~DatabaseManager();

	/* Get a user from the database if they exist.
		Returns: Returns a User object if a user exists in the database for the credentials. Or else returns `nullptr`.
	*/
	User* getUser(string username, string password);

	/* Create and add a user to the database
		Returns: returns the User object if it was successfully added to the database. 
				 Will return nullptr if a username already exists for `username` in the database.
	*/
	User* addUser(string username, string password, UserType userType);


	/* Delete a store from the database
		Parameter storeId: the id of the store to delete from the database
	*/
	void deleteStore(int storeId);

	/* Creates and stores parameters in stores database
		Returns: returns a Store object representing a DB Store model
	*/
	Store* createStore(string address, string city, string state, int zipCode, int priorityLevel);

	/* Get a store from the database
		Returns: returns a filled in Store object, or nullptr if no store was found for `storeId`
	*/
	Store* getStore(int storeId);



private:
	sqlite3 *db;

	void closeDatabase();
	vector<vector<string>> query(const char *sql);
};

