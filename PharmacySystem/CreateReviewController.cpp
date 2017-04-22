#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "CommandUtils.cpp" //for getDate()
#include "DatabaseManager.h"
#include "Review.cpp"
#include "CommonUserPrompts.cpp"
using namespace std;

class CreateReviewController {
public:

	/// Returns nullptr if no review was able to be created.
	Review* createReview(User* user) {

		if (user->getUserType() != Customer) { //guard
			cout << "You are not signed in as a customer." << endl;
			cout << "Ended create review process." << endl << endl;
			return nullptr;
		}

		Store *store = CommonUserPrompts::getStoreFromInput();
		if (store == nullptr) {
			cout << "Store was not found for this ID." << endl;
			cout << "Ended create review process." << endl << endl;
			return nullptr;
		}
		int storeId = store->getId();

		// Check if user has bought from the store before
		if (!userHasBoughtFromStore(user, store)) {
			cout << "You have no purchases from this store and can not leave a review." << endl;
			cout << "Ended create review process." << endl << endl;
			return nullptr;
		}

		// Get Rating
		bool ratingRetrieved = false;

		int rating;
		do {
			try {
				cout << "Enter a rating (1-5) for store #" << storeId << ":" << endl;
				string input = getInput("rating");
				rating = stoi(input);
				if (rating < 1 || rating > 5) {
					throw exception("Rating outside of allowed range.");
				}
				ratingRetrieved = true;
			}
			catch (const exception &e) {
				cout << "[!] Invalid input. Ratings are from 1 to 5." << endl << endl;
			}
		} while (!ratingRetrieved);
		
		// Get Review Text
		cout << "Enter the review's text:" << endl;
		string reviewText = getInput("review text");
		
		// Create Review
		string today = getDate();
		Review *review = DatabaseManager::shared()->createReview(user->getUserID(), storeId, rating, reviewText, today);
		
		if (review != nullptr) {
			cout << "Review successfully created." << endl << endl;
		}
		else {
			cout << "Review was not able to be created at this time. Please try again later." << endl << endl;
		}

		return review;
	}

private:

	/* Helpers */
	
	/* Checks if user has bought from a store before
		Returns: returns true if the user has at least once purchase from the given store
	*/
	bool userHasBoughtFromStore(User *user, Store *store) {
		if (user == nullptr || store == nullptr) { return false; } //Guard

		int storeId = store->getId();
		vector<Prescription*> prescriptions = DatabaseManager::shared()->getPrescriptionHistory(user->getUserID());
		
		for (Prescription *p : prescriptions) {
			if (p->getStoreId() == storeId) {
				return true;
			}
		}
		return false;
	}
};