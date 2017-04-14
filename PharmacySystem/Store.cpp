#include <string>
#include <iostream>
using namespace std;

struct Store {
public:

	Store(int id, string address, string city, string state, int zipCode, int priorityLevel) {
		this->id = id;
		this->address = address;
		this->city = city;
		this->state = state;
		this->zipCode = zipCode;
		this->priorityLevel = priorityLevel;
	}

	int getId() { return id; }

	string getAddress() { return address; }

	string getCity() { return city; }

	string getState() { return state; }
	
	int getZipCode() { return zipCode; }

	int getPriorityLevel() { return priorityLevel; }

	bool operator == (const Store& s) {
		if (id == s.id && address == s.address && city == s.city &&
			state == s.state && zipCode == s.zipCode && priorityLevel == s.priorityLevel)
			return true;
		return false;
	}

	bool operator != (const Store& s) {
		if (*this == s)
			return false;
		return true;
	}

	void printDescription() {
		cout << "Id: " << id << endl;
		cout << "Address: " << address << endl;
		cout << "City: " << city << endl;
		cout << "State: " << state << endl;
		cout << "ZipCode: " << zipCode << endl;
		cout << "PriorityLevel: " << priorityLevel << endl;
	}

private:
	int id;
	string address;
	string city;
	string state;
	int zipCode;
	int priorityLevel;
};