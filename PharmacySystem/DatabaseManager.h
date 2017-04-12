#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "User.cpp"
#include "StringUtils.cpp"

class DatabaseManager
{
public:

	/*
	Constructor for creating a Manager object for a SQLite3 database
		Paramater filename: a string for the name of the database file relative to this class
	*/
	DatabaseManager(const char *filename);
	~DatabaseManager();

	/*
	Returns a user from the database if they exist.
		Returns: Returns a User object if a user exists in the database for the credentials. Or else returns `nullptr`.
	*/
	User* getUser(string username, string password);

	/* Create and add a user to the database
		Returns: returns the User object if it was successfully added to the database.
	*/
	User* addUser(string username, string password, bool isEmployee);

private:
	sqlite3 *db;

	void closeDatabase();
	vector<vector<string>> query(const char *sql);
};

