#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "DatabaseManager.h"
using namespace std;

class SideEffectTablePrinter {
public:

	/* Print the side effects table to stdout
		Pre: item must not be null
	*/
	static void printSideEffectTable(DatabaseManager *dbm, Item *item) {
		cout << string(60, '-') << endl;
		cout << "Side effects for " << item->getName() << ":" << endl;

		vector<SideEffect*> sideEffects = dbm->getSideEffects(item->getId());

		cout << string(60, '-') << endl;

		for (SideEffect* sideEffect : sideEffects) {
			cout << sideEffect->getEffect() << endl << endl;
		}

		cout << string(60, '-') << endl << endl;

	}
};