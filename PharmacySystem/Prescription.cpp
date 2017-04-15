#include <string>
#include <iostream>
using namespace std;

struct Prescription {
public:
	Prescription(int id, string date, int customerId, int storeId) {
		this->id = id;
		this->date = date;
		this->customerId = customerId;
		this->storeId = storeId;
	}

	int getId() { return id; }

	string getDate() { return date; }

	int getCustomerId() { return customerId; }

	int getStoreId() { return storeId; }

	bool operator == (const Prescription& p) {
		if (id == p.id && date == p.date && date == p.date && customerId == p.customerId && storeId == p.storeId) {
			return true;
		}
		return false;
	}

	bool operator != (const Prescription& p) {
		if (*this == p) {
			return false;
		}
		return true;
	}

	void printDescription() {
		cout << "Id: " << id << endl;
		cout << "Date: " << date << endl;
		cout << "Customer: " << customerId << endl;
		cout << "Store: " << storeId << endl;
	}

private:
	int id;
	string date;
	int customerId;
	int storeId;
};