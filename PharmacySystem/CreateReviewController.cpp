#pragma once
#include <string>
#include <iostream>
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
			cout << "Ended create review process." << endl << endl;
			return nullptr;
		}

		Store *store = CommonUserPrompts::getStoreFromInput();
		if (store == nullptr) {
			cout << "Ended create review process." << endl << endl;
			return nullptr;
		}
		int storeId = store->getId();

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
		
		return review;
	}

};