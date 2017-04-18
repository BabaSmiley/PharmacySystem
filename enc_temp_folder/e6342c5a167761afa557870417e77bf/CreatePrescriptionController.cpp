#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
using namespace std;


class InputTerminationException : public exception {
public:
	InputTerminationException(string message = "") {
		this->message = message;
	}

	virtual const char* what() const throw() {
		return message.c_str();
	}
private:
	string message;
};


class CreatePrescriptionController {
public:

	Prescription* startCreatePrescription() {
		
		cout << exitAnytimeMessage << endl;
		try {
			User *customer = getUserFromInput();
			stoi("wrong");

		}
		catch (const InputTerminationException &e) {
			cout << endProcessMessage << endl;
			return nullptr;
		}

		//customer is valid
		cout << "customer is valid" << endl;


		return nullptr;
	}

private:
	//const char *terminateString = "exit"; //string to end process on

	const char *exitAnytimeMessage = "Enter 'exit' anytime in order to exit the create prescription process.";
	const char *endProcessMessage = "Ended create prescription process.";

	/// Prompts user for user input and returns a user from the database, or nullptr if none exists
	User* getUserFromInput() throw(InputTerminationException) {
		User *customer = nullptr;

		while (customer == nullptr) {
			
			cout << "Enter a customer's username:" << endl;
			string username = getInput("username");
			customer = DatabaseManager::shared()->getUser(username);

			if (customer == nullptr || customer->getUserType() != Customer) {
				cout << "[!] Invalid customer credentials." << endl;

				string input;
				cout << "Try again? (y/n) ";
				cin >> input;
				if (input == "n" || input == "N") {
					cout << "entered no..." << endl;
					//throw "error mesage";
					throw InputTerminationException();
					
					//cout << endProcessMessage << endl;
					//return;
				}
				cout << "entered yes";
				// otherwise loop and try again
			}
		}
	}



};