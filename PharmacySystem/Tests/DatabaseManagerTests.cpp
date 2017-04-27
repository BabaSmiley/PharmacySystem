#include <iostream>
#include <string>
#include "../DatabaseManager.h"
using namespace std;

void runTests(DatabaseManager *dbm);

///////////////////////////////////////////////
void testDatabaseUser(DatabaseManager *dbm) {

	User *validEmployeeUser = dbm->getUser("jon", "testpass");
	if (validEmployeeUser != nullptr) {
		cout << "jon is a user." << endl;
		cout << "id: " << validEmployeeUser->getUserID() << endl;
		cout << "isEmployee: " << validEmployeeUser->isEmployee() << endl;
	}
	else {
		cout << "jon is not a user." << endl;
	}

	User *invalidUser = dbm->getUser("jonathan", "testpass1");
	if (invalidUser != nullptr) {
		cout << "jonathan is a user." << endl;
		cout << "id: " << invalidUser->getUserID() << endl;
		cout << "isEmployee: " << invalidUser->isEmployee() << endl;
	}
	else {
		cout << "jonathan is not a user." << endl;
	}

	//Test adding user
	User *newUser = dbm->createUser("jose2", "siracha", "123s ville", Employee);
	if (newUser != nullptr) {
		cout << "User was created!" << endl;
		cout << "User: " << newUser->getUserID() << ". Type: " << newUser->getUserType();
	}
	else {
		cout << "New user was not created. Jose may already exist." << endl;
	}
}

bool testCreateStore(DatabaseManager *dbm) {
	//Test Create
	Store *newStore = dbm->createStore(69, "123 Alley", "NewTown", "NY", 34592, 199, 1);
	if (newStore) {
		cout << endl << "New store created" << endl;
		cout << "ID: " << newStore->getId() << endl;
		cout << "Address: " << newStore->getAddress() << endl;
		cout << "City/State: " << newStore->getCity() << "/" << newStore->getState() << " " << newStore->getZipCode() << endl;
		cout << "Priority: " << newStore->getPriorityLevel() << endl;
		cout << "Is Active: " << (newStore->getIsActive() == 1 ? "True" : "False") << endl;
		return true;
	}
	else {
		cout << "Could not create new Store." << endl;
		return false;
	}
}

bool testUpdateStore(DatabaseManager *dbm) {
	Store *updatingStore = dbm->updateStore(13, "", "", "NY", 42069, NULL);
	if (updatingStore) {
		cout << endl << "Store updated" << endl;
		cout << "ID: " << updatingStore->getId() << endl;
		cout << "Address: " << updatingStore->getAddress() << endl;
		cout << "City/State: " << updatingStore->getCity() << ", " << updatingStore->getState() << " " << updatingStore->getZipCode() << endl;
		cout << "Priority: " << updatingStore->getPriorityLevel() << endl;
		return true;
	}
	else {
		cout << "Could not update store" << endl;
		return false;
	}
}

bool testGetStore(DatabaseManager *dbm) {
	Store *existingStore = dbm->getStore(13);
	if (existingStore == nullptr) {
		cout << "Store which should exist for id=1 does not." << endl;
		return false;
	}

	Store *doesNotExist = dbm->getStore(-1);
	if (doesNotExist != nullptr) {
		cout << "Store which shouldnt exist, does" << endl;
		return false;
	}
	return true;
}

bool testDeleteStore(DatabaseManager *dbm) {
	Store *s = dbm->getStore(13);
	return dbm->deleteStore(s);
}

bool testCreateItem(DatabaseManager *dbm) {
	Item *newItem = dbm->createItem("Nyquill", "sleep sleep", 15, "every night", 420, "2017-04-20", 500, 500, 500, 1);
	if (newItem) {
		cout << endl << "New item created" << endl;
		cout << "ID: " << newItem->getId() << endl;
		cout << "Name: " << newItem->getName() << endl;
		cout << "Description: " << newItem->getDescription() << endl;
		return true;
	}
	else {
		cout << "Could not create item." << endl;
		return false;
	}
}

bool testUpdateItem(DatabaseManager *dbm) {
	Item *updatingItem = dbm->updateItem(24, "NuDrug", "testing", 56, "whenever", 420, "2017-05-04", 500, 500, 500, 1);
	if (updatingItem) {
		cout << endl << "Item updated" << endl;
		cout << "ID: " << updatingItem->getId() << endl;
		cout << "Name: " << updatingItem->getName() << endl;
		cout << "Description: " << updatingItem->getDescription() << endl;
		return true;
	}
	else {
		cout << "Could not update item." << endl;
		return false;
	}
}

bool testGetItem(DatabaseManager *dbm) {
	Item *existingItem = dbm->getItem(24);
	if (existingItem == nullptr) {
		cout << "Item which should exist for id=24 does not" << endl;
		return false;
	}

	Item *doesNotExist = dbm->getItem(-1);
	if (doesNotExist != nullptr) {
		cout << "Item which shouldnt exist does" << endl;
		return false;
	}

	return true;
}

bool testDeleteItem(DatabaseManager *dbm) {
	return dbm->deleteItem(24);
}

bool testCreatePrescription(DatabaseManager *dbm) {
	Prescription *newPrescription = dbm->createPrescription("2017-04-20", 11, 1);
	if (newPrescription) {
		cout << endl << "New prescription created" << endl;
		cout << "Id: " << newPrescription->getId() << endl;
		cout << "Date: " << newPrescription->getDate() << endl;
		cout << "Customer: " << newPrescription->getCustomerId() << endl;
		cout << "Store: " << newPrescription->getStoreId() << endl;
		return true;
	}
	else {
		cout << "Could not create prescription" << endl;
		return false;
	}
}

bool testGetPrescription(DatabaseManager *dbm) {
	Prescription *existingPrescription = dbm->getPrescription(17);
	if (existingPrescription == nullptr) {
		cout << "Prescription which should exist for id=17 does not" << endl;
		return false;
	}

	Prescription *doesNotExist = dbm->getPrescription(-1);
	if (doesNotExist != nullptr) {
		cout << "Prescription which shouldnt exist does" << endl;
		return false;
	}

	return true;
}

bool testDeletePrescription(DatabaseManager *dbm) {
	return dbm->deletePrescription(17);
}

bool testCreateInventory(DatabaseManager *dbm) {
	Inventory *newInventory = dbm->createInventory(1, 24, 500, 500, 500, 500);
	if (newInventory) {
		cout << endl << "New inventory created" << endl;
		cout << "Store: " << newInventory->getStoreId() << endl;
		cout << "Item: " << newInventory->getItemId() << endl;
		cout << "Item Level: " << newInventory->getItemLevel() << endl;
		cout << "Refill Level: " << newInventory->getRefillLevel() << endl;
		cout << "Refill Quantity: " << newInventory->getRefillQuantity() << endl;
		return true;
	}
	else {
		cout << "Could not create inventory" << endl;
		return false;
	}
}

bool testGetInventory(DatabaseManager *dbm) {
	Inventory *existingInventory = dbm->getInventory(1, 24);
	if (existingInventory == nullptr) {
		cout << "Inventory which should exist for storeId=1 and itemId=24 does not" << endl;
		return false;
	}

	Inventory *doesNotExist = dbm->getInventory(-1, -1);
	if (doesNotExist != nullptr) {
		cout << "Inventory which shouldnt exist does" << endl;
		return false;
	}

	return true;
}

bool testDeleteInventory(DatabaseManager *dbm) {
	return dbm->deleteInventory(1, 24);
}

bool testCreateDiscount(DatabaseManager *dbm) {
	Discount *newDiscount = dbm->createDiscount(1, 24, 50, "2017-04-20", "2017-05-20");
	if (newDiscount) {
		newDiscount->printDescription();
		return true;
	}
	else {
		cout << "Could not create discount" << endl;
		return false;
	}
}

bool testGetDiscount(DatabaseManager *dbm) {
	Discount *existingDiscount = dbm->getDiscount(1, 24);
	if (existingDiscount == nullptr) {
		cout << "Discount which should exist for storeId=1 and itemId=24 does not" << endl;
		return false;
	}

	Discount *doesNotExist = dbm->getDiscount(-1, -1);
	if (doesNotExist != nullptr) {
		cout << "Discount which should not exist does" << endl;
		return false;
	}

	return true;
}

bool testDeleteDiscount(DatabaseManager *dbm) {
	return dbm->deleteDiscount(1, 24);
}


void runTests(DatabaseManager *dbm) {
	//testDatabaseUser(dbm);
	//testCreateStore(dbm);

	///STORE DB FUNCTIONALITY
	//cout << testCreateStore(dbm) << endl;
	//cout << testUpdateStore(dbm) << endl;
	//cout << testGetStore(dbm) << endl;
	//cout << testDeleteStore(dbm) << endl;

	///ITEM DB FUNCTIONALITY
	//cout << testCreateItem(dbm) << endl;
	//cout << testUpdateItem(dbm) << endl;
	//cout << testDeleteItem(dbm) << endl;
	//cout << testGetItem(dbm) << endl;

	///PRESCRIPTION DB FUNCTIONALITY
	//cout << testCreatePrescription(dbm) << endl;
	//cout << testDeletePrescription(dbm) << endl;
	//cout << testGetPrescription(dbm) << endl;

	///INVENTORY DB FUNCTIONALITY
	//cout << testCreateInventory(dbm) << endl;
	//cout << testDeleteInventory(dbm) << endl;
	//cout << testGetInventory(dbm) << endl;

	///DISCOUNT DB FUNCTIONALITY
	//cout << testCreateDiscount(dbm) << endl;
	//cout << testGetDiscount(dbm) << endl;
	//cout << testDeleteDiscount(dbm) << endl;
}