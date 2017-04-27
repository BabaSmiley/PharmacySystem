#include <string>
#include <iostream>
using namespace std;

struct AddItem {
public:

	AddItem(int itemId, int storeId, long quantity, long refillLevel, long refillQuantity) {
		this->itemId = itemId;
		this->storeId = storeId;
		this->quantity = quantity;
		this->refillLevel = refillLevel;
		this->refillQuantity = refillQuantity;
	}

	int getItemId() { return itemId; }

	int getStoreId() { return storeId; }

	long getQuantity() { return quantity; }

	long getRefillLevel() { return refillLevel; }

	long getRefillQuantity() { return refillQuantity; }

private:
	int itemId;
	int storeId;
	long quantity;
	long refillLevel;
	long refillQuantity;
};