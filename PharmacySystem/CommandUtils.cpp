#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
using namespace std;

//Note: static will cause compiler to determine if it will inline. Move to .h later to avoid
static string getInput(string preceeedingString = "") {
	string input;
	if (preceeedingString != "") {
		std::cout << preceeedingString << " > ";
	}
	else {
		std::cout << "> ";
	}
	std::getline(std::cin, input); //Will get entire line
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