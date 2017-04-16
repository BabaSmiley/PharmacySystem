#pragma once
#include <iostream>
#include <string>
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