#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Discount {
public:

	Discount(int storeId, int itemId, int percentOff, string startDate, string endDate) {
		this->storeId = storeId;
		this->itemId = itemId;
		this->percentOff = percentOff;
		this->startDate = startDate;
		this->endDate = endDate;
	}

	int getStoreId() { return storeId; }

	int getItemId() { return itemId; }

	int getPercentOff() { return percentOff; }

	string getStartDate() { return startDate; }

	string getEndDate() { return endDate; }

	void printDescription() {
		cout << "Store ID: " << storeId << endl;
		cout << "Item ID: " << itemId << endl;
		cout << "Percent Off: " << percentOff << endl;
		cout << "Start Date: " << startDate << endl;
		cout << "End Date: " << endDate << endl;
	}

private:
	int storeId;
	int itemId;
	int percentOff;
	string startDate;
	string endDate;
};