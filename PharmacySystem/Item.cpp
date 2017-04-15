#include <string>
#include <iostream>
using namespace std;

struct Item {
public:
	Item(int id, string name, string dosage, int vendor, string description, float price, long warehouseQuantity, long warehouseRefillQuantity, long warehouseRefillLevel) {
		this->id = id;
		this->name = name;
		this->dosage = dosage;
		this->vendor = vendor;
		this->description = description;
		this->price = price;
		this->warehouseQuantity = warehouseQuantity;
		this->warehouseRefillQuantity = warehouseRefillQuantity;
		this->warehouseRefillLevel = warehouseRefillLevel;
	}

	int getId() { return id; }

	string getName() { return name; }

	string getDosage() { return dosage; }

	int getVendor() { return vendor; }

	string getDescription() { return description; }

	float getPrice() { return price; }

	long getWarehouseQuantity() { return warehouseQuantity; }

	long getWarehouseRefillQuantity() { return warehouseRefillQuantity; }

	long getWarehouseRefillLevel() { return warehouseRefillLevel; }

	bool operator == (const Item& i) {
		if (id == i.id && name == i.name && dosage == i.dosage && vendor == i.vendor && description == i.description &&
			price == i.price && warehouseQuantity == i.warehouseQuantity && warehouseRefillQuantity == i.warehouseRefillQuantity && warehouseRefillLevel == i.warehouseRefillLevel) {
			return true;
		}
		return false;
	}

	bool operator != (const Item& i) {
		if (*this == i) {
			return false;
		}
		return true;
	}

	void printDescription() {
		cout << "Id: " << id << endl;
		cout << "Name: " << name << endl;
		cout << "Dosage: " << dosage << endl;
		cout << "Vendor: " << vendor << endl;
		cout << "Description: " << description << endl;
		cout << "Price: $" << price << endl;
		cout << "Warehouse Quantity: " << warehouseQuantity << endl;
		cout << "Warehouse Refill Quantity: " << warehouseRefillQuantity << endl;
		cout << "Warehouse Refill Level: " << warehouseRefillLevel << endl;
	}

private:
	int id;
	string name;
	string dosage;
	int vendor;
	string description;
	float price;
	long warehouseQuantity;
	long warehouseRefillQuantity;
	long warehouseRefillLevel;
};