#include <string>
#include <iostream>
using namespace std;

struct Inventory {
public:
	Inventory(int storeId, int itemId, long itemLevel, long refillLevel, long refillQuantity) {
		this->storeId = storeId;
		this->itemId = itemId;
		this->itemLevel = itemLevel;
		this->refillLevel = refillLevel;
		this->refillQuantity = refillQuantity;
	}

	int getStoreId() { return storeId; }

	int getItemId() { return itemId; }

	long getItemLevel() { return itemLevel; }

	long getRefillLevel() { return refillLevel; }

	long getRefillQuantity() { return refillQuantity; }

	bool operator == (const Inventory& i) {
		if (storeId == i.storeId && itemId == i.itemId && itemLevel == i.itemLevel && refillLevel == i.refillLevel && refillQuantity == i.refillQuantity) {
			return false;
		}
		return true;
	}

	bool operator != (const Inventory& i) {
		if (*this == i) {
			return false;
		}
		return true;
	}

	void printDescription() {
		cout << "Store: " << storeId << endl;
		cout << "Item: " << itemId << endl;
		cout << "Item Level: " << itemLevel << endl;
		cout << "Refill Level: " << refillLevel << endl;
		cout << "Refill Quantity: " << refillQuantity << endl;
	}
	
private:
	int storeId;
	int itemId;
	long itemLevel;
	long refillLevel;
	long refillQuantity;
};