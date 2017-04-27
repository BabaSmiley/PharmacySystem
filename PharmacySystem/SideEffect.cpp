#pragma once
#include <string>
#include <iostream>
using namespace std;

struct SideEffect {
public:

	SideEffect(int itemId, string effect) {
		this->itemId = itemId;
		this->effect = effect;
	}

	int getItemId() { return itemId; }

	string getEffect() { return effect; }

private:

	int itemId;
	string effect;

};