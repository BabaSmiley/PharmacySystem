#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

enum UserType {
	Customer,
	Employee
};

class User {
public:

	User(int userID, string username, UserType userType) {
		this->userID = userID; 
		this->username = username;
		this->userType = userType;
	}

	/// Returns the type of user a User is
	UserType getUserType() {
		return userType;
	}

	/// Returns the ID of a user
	int getUserID() {
		return userID;
	}

	/// Returns true if the user is an employee
	bool isEmployee() {
		if (userType == Employee)
			return true;
		return false;
	}

	/// Returns the username of the user
	string getUsername() {
		return username;
	}

private:
	UserType userType;
	int userID;
	string username;
};

#endif 