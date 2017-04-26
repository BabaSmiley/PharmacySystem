#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <iterator>
#include <vector>
using namespace std;

//Note: static will cause compiler to determine if it will inline. Move to .h later to avoid
// Paramater preceedingString: The string to print before the prompt. (Optional)
// Parameter limitLength: the max length an input string can be. (Optional)
static string getInput(string preceedingString = "", int limitLength=-1) {
	string input;
	if (preceedingString != "") {
		std::cout << preceedingString << " > ";
	}
	else {
		std::cout << "> ";
	}

	if (limitLength > 0) {
		cin >> setw(limitLength) >> input;
		//char *s = new char[limitLength + 1]; //plus null terminator
		//cin.getline(s, limitLength);
		//input = s;
	}
	else {
		std::getline(std::cin, input); //Will get entire line
	}
	return input;
}

/* Returns a vector for splitting a string at a string or character
	Paramater delimator: the string or character to split the string at
*/
static vector<string> splitString(string str, string delimator) {
	vector<string> results;
	int n = 0;
	istringstream iss(str);
	for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>(); ++it, ++n) {
		results.push_back(*it);
	}
	return results;
}

template <size_t N>
void splitString(string(&arr)[N], string str)
{
	int n = 0;
	istringstream iss(str);
	for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
		arr[n] = *it;
}

/// Returns the current date in the format YYYY-MM-DD
static string getDate() {
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);

	char time[80];
	strftime(time, 80, "%Y-%m-%d", &now);
	return time;
}