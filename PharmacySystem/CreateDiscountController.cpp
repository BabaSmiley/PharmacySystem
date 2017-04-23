#pragma once
#include <iostream>
#include <string>
#include "CommandUtils.cpp"
#include "DatabaseManager.h"
#include "Discount.cpp"
using namespace std;

class CreateDiscountController {
public:

	void promptForInput(int itemId, int storeId) {
		Discount *discount = getDiscountInput(itemId, storeId);
	}

private:

	Discount* getDiscountInput(int itemId, int storeId) throw(const char*) {
		string percentOffString;
		int percentOff;
		string startDate;
		string endDate;

		cout << endl << "Enter the following fields to add a discount" << endl;
		percentOffString = getInput("Percent Off");
		try {
			percentOff = stoi(percentOffString);
		}
		catch (const char *e) {
			throw e;
		}
		startDate = getInput("Start Date");
		endDate = getInput("End Date");

		Discount *discount = new Discount(itemId, storeId, percentOff, startDate, endDate);
	}
};