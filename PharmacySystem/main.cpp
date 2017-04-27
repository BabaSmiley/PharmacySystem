#include "main.h"
#include "BatchProcesses.h"

/// Will clear the windows console
void clearWindowsConsole() {
	system("cls");
	cout << helpMessagePrompt << endl << endl;
}

void printHelp(UserType type) {
	cout << "##################################" << endl;
	cout << "Available Commands:" << endl;
	// General all-user commands
	cout << endl << "## View Data ##" << endl;
	cout << "\t'list stores {number of stores}' : Will list all available stores. Optionally can state the number of stores to show." << endl;
	cout << "\t'view review {store ID}' : Will list store reviews for a store ID. Optionally can state a number of reviews to print." << endl;

	// User specific commands
	if (type == Employee) {
		cout << "\t'list items {store ID} {number of items}' : Will list all items in given store. Optionally can state the number of items to show." << endl;
		cout << "\t'list all items {number of items}' : Will list all items available at a company level. Optionally can state the number of items to show." << endl;
		cout << "\t'view history {customer username}' : View the purchase history of a customer. Specify the customer's username to view their history." << endl;
		cout << "\t'view item history {item id}' : View an items sale history for a given item id." << endl;
		cout << "\t'view store history {store id}' : View a stores sale history for a given store id." << endl;

		cout << endl << "## Create/Manage ##"<< endl;
		cout << "\t'create prescription' : Will begin process to create a new customer prescription." << endl;
		cout << "\t'manage item {item ID}' : Update the attributes of a item. Specify the item ID to make modifications." << endl;
		cout << "\t'manage store {store ID}' : Update the attributes of a store. Specify the store ID to make modifications." << endl;
		cout << "\t'reorder item' : Will begin process to reorder an item from a store." << endl;
		
		cout << endl;
		cout << "\t'create discount {item ID} {store ID}' : Will create a discount for the specified item in the specified store." << endl;
		cout << "\t'delete discount {item ID} {store ID}' : Will delete a discount from the database for the specified item ID and store ID." << endl;
		cout << "\t'start batch' : Will begin the batch file processing for the end of the day. Will logout user afterwords." << endl;
	}
	else if (type == Customer) {
		cout << "## Other ##" << endl;
		cout << "\t'create review' : Will begin process to leave a review for a store." << endl;
		cout << "\t'view history' : View your purchase history." << endl;
	}

	cout << endl << "'clear' : Will clear the console window to avoid clutter." << endl;
	cout << "'help' : Will print this usage statement." << endl;
	cout << "'logout' : Will log out of the system and close the program." << endl;
	cout << "##################################" << endl;
}

void printStoreReviews(DatabaseManager *dbm, Store *store) {
	vector<Review*> reviews = dbm->getReviews(store->getId());

	cout << "Store Reviews:" << endl;
	if (reviews.size() == 0) {
		cout << "No reviews available for this store." << endl;
	}
	for (Review *review : reviews) {
		User *reviewer = dbm->getUser(review->getAccountId());

		cout << string(40, '-') << endl;
		cout << "By " << reviewer->getUsername() << " on " << review->getDate() << "\t" << "Rating: " << review->getRating() << "/5" << endl << endl;
		cout << review->getText() << endl;
	}
	cout << endl;
}

void printItemTable(Store *store, unsigned int count = NULL) {
	DatabaseManager *dbm = DatabaseManager::shared();
	ItemTablePrinter::printItemTable(dbm, store, count);
}

void printCompanyItemTable(unsigned int count = NULL) {
	DatabaseManager *dbm = DatabaseManager::shared();
	ItemTablePrinter::printCompanyItemTable(dbm, count);
}

//DEBUG
User* DebugGetEmployeeUser() {
	return DatabaseManager::shared()->getUser("jon", "testpass");
}
//END DEBUG

int main() {
	DatabaseManager *dbm = DatabaseManager::shared();
	//runTests(dbm);
	//runBatchSequence(dbm);

	/* Start Login & Registration Process */
	// DEBUG - commented out so dont have to repeatadly sign in. Uncomment to reactivate the login feature
	LoginRegistrationController lr;
	lr.displayScreen();

	User *user = lr.getAuthorizedUser();
	if (user == nullptr) { //Guard
		cout << "[!] An error occured in logging in. Please close the program and try again." << endl;
		return 1; //1 is standardly returned for entire program errors 
	}
	
	//User *user = DebugGetEmployeeUser();

	cout << string(8, '*') << " Logged in as: " << user->getUsername() << " " << string(8, '*') << endl;
	
	/* Take User Online Input For Commands */
	cout << helpMessagePrompt << endl << endl;
	string userInput;
	bool shouldEndProgram = false;
	
	while (!shouldEndProgram) {
		userInput = getInput();
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower); //Convert input to all lowercase
		vector<string> input = splitString(userInput, " ");
		
		try { //try for out-of-range input exceptions
			if ("logout" == input.at(0)) {
				cout << "Logged out of system." << endl;
				shouldEndProgram = true;
			}
			else if ("help" == input.at(0)) {
				printHelp(user->getUserType());
			}
			else if ("clear" == input.at(0)) { clearWindowsConsole(); }
			else if ("list stores" == input.at(0) + " " + input.at(1)) {
				if (input.size() > 2) {
					int countInput = stoi(input.at(2));
					if (countInput <= 0) { //is signed
						throw exception("Input is a signed int. Expected unsigned.");
					}
					unsigned int count = (unsigned int)countInput;
					StoresTablePrinter::printStoresTable(dbm, count);
				}
				else {
					StoresTablePrinter::printStoresTable(dbm);
				}
			}
			else if ("view history" == input.at(0) + " " + input.at(1)) {
				int customerID;
				if (user->isEmployee()) {
					string inputString = input.at(2);
					User *userSpecified = dbm->getUser(inputString);
					if (userSpecified == nullptr) {
						cout << "No user available for input username." << endl << endl;
						throw exception("No user available for input username.");
					}
					customerID = userSpecified->getUserID();
				}
				else { //is customer user
					customerID = user->getUserID();
				}
				PurchaseHistoryTablePrinter::printUserPurchaseHistoryTable(dbm, customerID);
			}
			else if ("view review" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				Store *store = dbm->getStore(stoi(input.at(2)));
				if (!store) { //gaurd
					cout << "No store available for that store ID." << endl;
				}
				else {
					printStoreReviews(dbm, store);
				}
			}
			// Customer Specific Commands
			else if (user->getUserType() == Customer && "create review" == input.at(0) + " " + input.at(1)) {
				ReviewController reviewController;
				reviewController.createReview(user);
			}

			// Employee Specific Commands
			else if (user->isEmployee() && "start batch" == input.at(0) + " " + input.at(1)) {
				runBatchSequence(dbm);
				shouldEndProgram = true;
			}
			else if (user->isEmployee() && "reorder item" == input.at(0) + " " + input.at(1)) {
				ReorderController reorderController;
				AddItem* reorder = reorderController.placeOrder();
				if (reorder == nullptr) {
					cout << "Reorder was not placed." << endl << endl;
				}
				else {
					cout << "Reorder was set for store #" << reorder->getStoreId() << ", item #" << reorder->getItemId() << " to quantity " << reorder->getQuantity() << "." << endl << endl;
				}
			}
			else if (user->isEmployee() && "create prescription" == input.at(0) + " " + input.at(1)) {
				PrescriptionController prescriptionController;
				prescriptionController.startCreatePrescription();
			}
			else if (user->isEmployee() && "list items" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				Store *store = dbm->getStore(stoi(input.at(2)));
				if (store == nullptr) { //Guard
					cout << "A store does not exist for this id." << endl << endl;
					//throw exception("Store does not exist.");
				}
				else if (input.size() > 3) {
					int countInput = stoi(input.at(3));
					if (countInput <= 0) { //is signed
						throw exception("Input is a signed int. Expected unsigned.");
					}
					unsigned int count = (unsigned int)countInput;
					printItemTable(store, count);
				}
				else {
					printItemTable(store);
				}
			}
			else if (user->isEmployee() && "list all items" == input.at(0) + " " + input.at(1) + " " + input.at(2)) {
				if (input.size() > 3) {
					int count = stoi(input.at(3));
					printCompanyItemTable(count);
				}
				else {
					printCompanyItemTable();
				}

			}
			else if (user->isEmployee() && "manage store" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				int storeId = stoi(input.at(2));
				Store *store = dbm->getStore(storeId);
				if (store != nullptr) {
					ManageStore ms;
					ms.promptForInput(storeId);
				}
				else {
					cout << "A store does not exist for this id" << endl << endl;
				}
			}
			else if (user->isEmployee() && "manage item" == input.at(0) + " " + input.at(1) && stoi(input.at(2))) {
				ManageItem mi;
				mi.promptForInput(stoi(input.at(2)));
			}
			else if (user->isEmployee() && "view item history" == input.at(0) + " " + input.at(1) + " " + input.at(2) && stoi(input.at(3))) {
				Item *item = dbm->getItem(stoi(input.at(3)));
				if (item != nullptr) {
					PurchaseHistoryTablePrinter::printItemPurchaseHistoryTable(dbm, item);
				}
				else {
					cout << "An item does not exist for this id." << endl;
				}
			}
			else if (user->isEmployee() && "view store history" == input.at(0) + " " + input.at(1) + " " + input.at(2) && stoi(input.at(3))) {
				Store *store = dbm->getStore(stoi(input.at(3)));
				if (store != nullptr) {
					PurchaseHistoryTablePrinter::printStorePurchaseHistoryTable(dbm, store);
				}
				else {
					cout << "An item does not exist for this id." << endl;
				}
			}
			else if (user->isEmployee() && "create discount" == input.at(0) + " " + input.at(1) && stoi(input.at(2)) && stoi(input.at(3))) {
				Inventory *inventory = dbm->getInventory(stoi(input.at(3)), stoi(input.at(2)));
				if (inventory != nullptr) {
					Discount *discount = dbm->getDiscount(stoi(input.at(3)), stoi(input.at(2)));
					if (discount != nullptr) {
						cout << "This discount already exists." << endl;
					}
					else {
						DiscountController discountController;
						discountController.promptForCreateInput(stoi(input.at(2)), stoi(input.at(3)));
					}
				}
				else {
					cout << "This item and store combination does not exist." << endl;
				}
			}
			else if (user->isEmployee() && "delete discount" == input.at(0) + " " + input.at(1) && stoi(input.at(2)) && stoi(input.at(3))) {
				Discount *discount = dbm->getDiscount(stoi(input.at(3)), stoi(input.at(2)));
				if (discount != nullptr) {
					DiscountController discountController;
					discountController.deleteDiscount(stoi(input.at(2)), stoi(input.at(3)));
				}
				else {
					cout << "This discount does not exist." << endl;
				}
			}
			else if (user->isEmployee() && "add inventory" == input.at(0) + " " + input.at(1) && stoi(input.at(2)) && stoi(input.at(3))) {
				AddItem *addItem = dbm->getAddItem(stoi(input.at(3)), stoi(input.at(2)));
				if (addItem != nullptr) {
					cout << "This item was already added to the queue." << endl;
				}
				else {
					Item *item = dbm->getItem(stoi(input.at(2)));
					if (item) {
						Store *store = dbm->getStore(stoi(input.at(3)));
						if (store) {
							ManageInventory manageInventoryCtrl;
							manageInventoryCtrl.promptForInventoryInput(stoi(input.at(2)), stoi(input.at(3)));
						}
						else {
							cout << "Item and store combination does not exist." << endl;
						}
					}
					else {
						cout << "Item and store combination does not exist." << endl;
					}
				}
			}

			else {
				throw exception("User command not recognized in main.");
			}
		}
		catch (const exception& err) {
			// Catch any out_of_range errors, etc.
			//cerr << "Caught exception: " << err.what() << endl; //DEBUG
			cout << "Invalid command. Type 'help' for a list of available commands." << endl;
		}
	}

	// Do cleanup before end of program...

	return 0;
}