#include <string>
#include <iostream>
using namespace std;

struct Review {
public:
	Review(int accountId, int storeId, int rating, string text, string date) {
		this->accountId = accountId;
		this->storeId = storeId;
		this->rating = rating;
		this->text = text;
		this->date = date;
	}

	int getAccountId() { return accountId; }

	int getStoreId() { return storeId; }

	int getRating() { return rating; }

	string getText() { return text; }

	string getDate() { return date; }

	void printDescription() {
		cout << "Account: " << accountId << endl;
		cout << "Store: " << storeId << endl;
		cout << "Rating: " << rating << endl;
		cout << "Text: " << text << endl;
		cout << "Date: " << date << endl;
	}
	
private:
	int accountId;
	int storeId;
	int rating;
	string text;
	string date;
};