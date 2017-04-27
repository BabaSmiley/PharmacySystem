#include "DatabaseUtils.h"
#include "Date.cpp"

/* Strings formatted in form: 2017-04-24
	Returns: true if `currentDate` is between date1 (inclusive) and date2 (exclusive)
*/
bool DatabaseUtils::dateStringIsBetweenDates(string currentDate, string date1, string date2) {
	Date current = Date(currentDate);
	Date leftDate = Date(date1);
	Date rightDate = Date(date2);

	if (current.year == 0 || leftDate.year == 0 || rightDate.year == 0) { //Guard
		// Return false if any of the strings were not in the correct format.
		return false;
	}

	bool isLargerThanLeft = false;
	if (current.year >= leftDate.year) {
		if (current.month > leftDate.month)
			isLargerThanLeft = true;
		else if (current.month == leftDate.month && current.day >= leftDate.day)
			isLargerThanLeft = true;
		else
			return false; //Return early becuase of fail
	}

	bool isLessThanRight = false;
	if (current.year <= rightDate.year) {
		if (current.month < rightDate.month)
			isLessThanRight = true;
		else if (current.month == rightDate.month && current.day < rightDate.day)
			isLessThanRight = true;
		else
			return false; //Return early becuase of fail
	}

	return (isLargerThanLeft && isLessThanRight);
}