#include <string>
using namespace std;

class Store {
public:
	string getAddress() { return address; }

private:
	int id;
	string address;
	string city;
	string state;
	int zipCode;
	int priorityLevel;
};