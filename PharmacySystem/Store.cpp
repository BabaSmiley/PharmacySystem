#include <string>
using namespace std;

class Store {
public:
	string getAddress() { return Address; }

private:
	int Id;
	string Address;
	string City;
	string State;
	int ZipCode;
	int PriorityLevel;
};