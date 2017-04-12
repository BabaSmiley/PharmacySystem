#include <string>
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

private:
	int id;
	string address;
	string city;
	string state;
	int zipCode;
	int priorityLevel;
};