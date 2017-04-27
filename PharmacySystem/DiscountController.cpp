#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Discount.cpp"
#include <ctime>
using namespace std;

class DiscountController {
public:

	void promptForCreateInput(int itemId, int storeId) {
		getCreateDiscountInput(itemId, storeId);
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
				cout << "Error you have entered a percent that is below 0%" << endl;
				cout << "Ended discount process" << endl;
				return;
			}
			else if (percentOff > 100)
			{
				cout << "Error you have entered a percent that is above 100%" << endl;
				cout << "Ended discount process" << endl;
				return;
			}
		}
		catch (const char *e) {
			throw e;
		}
		startDate = getInput("Start Date");
		endDate = getInput("End Date");

		Discount *discount = DatabaseManager::shared()->createDiscount(storeId, itemId, percentOff, startDate, endDate);

		if (discount) {
			cout << "Discount successfully created." << endl;
		}
		else {
			throw "Unable to create discount.";
		}
	}
};