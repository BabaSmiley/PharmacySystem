#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Discount.cpp"
#include "Date.cpp"
#include <ctime>
using namespace std;

class DiscountController {
public:

	void promptForCreateInput(int itemId, int storeId) {
		try {
			getCreateDiscountInput(itemId, storeId);
		}
		catch(const char *e) {
			cout << "[!] Discount was not created." << endl;
			cout << "Ended discount process." << endl << endl;
		}
	}

	void deleteDiscount(int itemId, int storeId) {
		bool deleted = DatabaseManager::shared()->deleteDiscount(storeId, itemId);
		if (deleted) {
			cout << "Discount successfully deleted." << endl;
		}
		else {
			throw "Unable to delete discount.";
		}
	}
private:

	void getCreateDiscountInput(int itemId, int storeId) throw(const char*) {
		string percentOffString;
		int percentOff;
		string startDate;
		string endDate;

		cout << endl << "Enter the following fields to add a discount" << endl;
		percentOffString = getInput("Percent Off");
		try {
			percentOff = stoi(percentOffString);
			if (percentOff < 0)
			{
				cout << "Error you have entered a percent that is below 0%." << endl;
				throw "Invalid percent entered";
			}
			else if (percentOff > 100)
			{
				cout << "Error you have entered a percent that is above 100%." << endl;
				throw "Invalid percent entered";
			}
		}
		catch (const char *e) {
			throw e;
		}
		startDate = getInput("Start Date");
		endDate = getInput("End Date");


		if (!Date::validateDate(startDate) || !Date::validateDate(endDate)) {
			// Invalid date was entered
			cout << "An invalid date format was entered. Must be in the form YYYY-MM-DD." << endl;
			throw "Invalid date format entered.";
		}
		else if (endDate < startDate) {
			cout << "The end date can not come before the start date." << endl;
			throw "End date is before the start date.";
		}

		Discount *discount = DatabaseManager::shared()->createDiscount(storeId, itemId, percentOff, startDate, endDate);

		if (discount) {
			cout << "Discount successfully created." << endl;
		}
		else {
			throw "Unable to create discount.";
		}
	}
};