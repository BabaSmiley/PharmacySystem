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



		}
		catch (const InputTerminationException &e) {
			cout << endProcessMessage << endl;
			return nullptr;
		}
		catch (const exception &e) {
			//pass
		}


		return nullptr;
	}

private:
	//const char *terminateString = "exit"; //string to end process on

	const char *exitAnytimeMessage = "Enter 'exit' anytime in order to exit the create prescription process.";
	const char *endProcessMessage = "Ended create prescription process.";

	/// Prompts user for user input and returns a user from the database, or else throws exception if user stops early
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
					throw InputTerminationException();
				}
				cout << "entered yes";
				// otherwise loop and try again
			}
		}
	}

	Store* getStoreFromInput() {
		cout << "Enter the store id to order from:" << endl;
		string storeId = getInput("storeId");

	}


};