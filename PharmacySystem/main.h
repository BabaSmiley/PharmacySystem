#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept> // Used for out_of_range error
#include <iomanip>
#include "DatabaseManager.h"
#include "User.cpp"
#include "DatabaseManagerTests.h"

// Controllers
#include "LoginRegistrationController.cpp"
#include "ManageStore.cpp"
#include "ManageItem.cpp"
#include "PrescriptionController.cpp"
#include "ReviewController.cpp"
#include "DiscountController.cpp"
#include "ReorderController.cpp"

// Utils
#include "ItemTablePrinter.cpp"
#include "PrescriptionHistoryTablePrinter.cpp"
#include "StoresTablePrinter.cpp"
using namespace std;


// String Constants
const char *helpMessagePrompt = "Enter a command to begin. Or type 'help' to get a list of available commands.";

