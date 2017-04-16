//
//  LoginRegistration.cpp
//  Some
//
//  Created by Cardasis, Jonathan (J.) on 4/13/17.
//  Copyright © 2017 Jonathan Cardasis. All rights reserved.
//

#pragma once
#include <iostream>
#include <string>
#include "Utils/ASCIIBlockM.cpp"
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "User.cpp"
using namespace std;

class LoginRegistration {
public:
        
    /*Displays login/registration screen and begins taking user input
    */
	void displayScreen() {
        printBlockM();
        promptForInput();
    }
    
    /*
     Returns: returns an authorized user or nullptr if the user is not yet authorized
              Will return a user once the user logins in with valid credentials or registers correctly
     */
    User* getAuthorizedUser() {
        return authorizedUser;
    }

private:
    User *authorizedUser;
    
    void printBlockM() {
        cout << ASCIIBlockM(5) << endl << endl;
    }
    
    void printInitialText() {
        cout << "Type \"login\" to login as an existing user, or \"register\" to register as a new user." << endl;
        cout << string(40, '-') << endl;
    }

	/* Prompts user for login or registation command
		Will continue to prompt until either a valid existing user is set or
		a new user is created.
		Post: `authorizedUser` will point to a valid user in the database
	*/
	void promptForInput() {
		bool shouldRepeat = false;

		do {
			printInitialText();
			string input = getInput();

			try {
				if (input == "login") {
					displayLoginScreen();
					shouldRepeat = false; //end if successful
				}
				else if (input == "register") {
					displayRegisterScreen();
					shouldRepeat = false; //end if successful
				}
				else {
					cout << "[!] Invalid command." << endl << endl;

					promptForInput(); //re-prompt for input
				}
			}
			catch (const char* msg) {
				cout << "[!] " << msg << endl << endl;
				shouldRepeat = true;
			}
		} while (shouldRepeat);
	}

	/* Displays login screen and prompts user to enter username and password.
		Throws if no user exists in the current database for those credentials
	*/
	void displayLoginScreen() throw(const char*) {
		string username;
		string password;

		cout << endl << "To login, type the username for the account:" << endl;
		username = getInput("username");

		cout << "Please type the password for the account:" << endl;
		password = getInput("password");
		cout << endl;

		//Check database for user credentials
		User *dbUser = DatabaseManager::shared()->getUser(username, password);

		if (dbUser) {
			cout << "Successfully authenticated." << endl;
			authorizedUser = dbUser;
		}
		else {
			throw "A user with this username and password combination does not exist.";
		}
	}

	void displayRegisterScreen() throw(const char*) {
		string userType;
		string username;
		string password;

		cout << endl << "Type \"employee\" to register as an employee, or \"customer\" to register as a customer" << endl;
		userType = getInput();

		if (userType != "customer" && userType != "employee") {
			//Guard protecttion in case user enters invalid type of user
			throw "Invalid command.";
		}

		cout << "To create an account please enter a username:" << endl;
		username = getInput("username");
		cout << "Please choose a password for your account:" << endl;
		password = getInput("password");

		if (userType == "customer") {
			User *newUser = DatabaseManager::shared()->createUser(username, password, Customer);
			
			if (!newUser) {
				throw "A user may already exist for this username. Try again.";
			}
			else {
				cout << "Customer account has been created." << endl;
				authorizedUser = newUser;
			}
		}
		else if (userType == "employee") {
			string employeeUsername;
			string employeePassword;

			cout << "Enter an existing employee's credentials to create the account:" << endl;
			employeeUsername = getInput("employee username");
			employeePassword = getInput("employee password");

			User *employee = DatabaseManager::shared()->getUser(employeeUsername, employeePassword);
			if (employee == nullptr || !employee->isEmployee()) {
				throw "Employee Credentials Incorrect";
			}

			// Employee credentials are valid
			User *newEmployeeUser = DatabaseManager::shared()->createUser(username, password, Employee);
			cout << "Employee account has been created." << endl;
			authorizedUser = newEmployeeUser;
		}
	}
    
};
