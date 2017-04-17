using namespace std;

#include <fstream>
#include <string>
#include <vector>
#include <string>
#include "DatabaseManager.h"

string ZeroFillNumber(string str, int digits) {
	while (str.size() < digits)
		str = "0" + str;

	return str;
}

//Where stores are created and deleted in the database
void createDeleteStore(DatabaseManager *dbm, ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Create/Delete Store=====";

	ifstream input("adddeletestore.txt");
	ofstream outInvToStoreReq("createstoreitems.txt");
	int InvToStoreReqCounter = 0;
	ofstream outInvRecAtWarehouse("deletestoreitems.txt");
	int InvRecAtWarehouseCounter = 0;
	string line;
	int trailerCount = 0;

	getline(input, line);

	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << "adddeletestore.txt sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	//TODO: WRITE HEADERS FOR outInvToStoreReq and outInvRecAtWarehouse

	getline(input, line);

	while (line[0] != 'T') {
		trailerCount++;

		if (line[0] == 'A') {
			string storeID = line.substr(1, 5);
			string storeAddress = line.substr(6, 20);
			string storeCity = line.substr(26, 20);
			string storeState = line.substr(46, 2);
			string storeZip = line.substr(48, 9);
			string StorePriority = line.substr(57, 2);

			//|action code 'A' or 'D'|store id|street address|city name|state|zip code|store priority level|
			dbm->createStore(stoi(storeID), storeAddress, storeCity, storeState, stoi(storeZip), stoi(StorePriority));
			int controlCount = 0;
			getline(input, line);

			while (line[0] != 'C') {
				if (line[0] == 'D')
					batchLog << "Unexpected delete store during store-item creation. Skipping.";
				else if (line[0] == 'I') {
					string ICode = line.substr(1, 9);
					string IdefaultQty = line.substr(10, 10);
					string IReorderLevel = line.substr(20, 10);
					string IReorderQty = line.substr(30, 10);
					//|action code 'I'|item code|store default quantity|store reorder level|store reorder quantity|
					dbm->createInventory(stoi(storeID), stoi(ICode), stoi(IdefaultQty), 100000, stoi(IReorderLevel), stoi(IReorderQty));
					outInvToStoreReq << "A" + storeID + StorePriority + ICode + IReorderQty << endl;
					InvToStoreReqCounter++;
					controlCount++;
				}

				if (stoi(line.substr(2, 4)) != trailerCount)
					batchLog << "Control mismatch for store creation.";
			}
		}
		else if (line[0] == 'D') {
			//|action code 'A' or 'D'|store id|street address|city name|state|zip code|store priority level|
			//TODO: DELETE STORE AND WRITE ALL ITEM QUANTITIES TO THE DELETESTOREITEMS.txt
		}

		getline(input, line);
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "adddeletestore.txt trailer mismatch. Expected" + line.substr(2, 4)
			+ " got " + to_string(trailerCount);

	outInvToStoreReq << "T ";
	outInvToStoreReq << ZeroFillNumber(to_string(InvToStoreReqCounter), 4);

	outInvRecAtWarehouse << "T "; //This block of code writes the trailer for invtostore request batch (zero fill the trailer)
	outInvRecAtWarehouse << ZeroFillNumber(to_string(InvRecAtWarehouseCounter), 4);
}

/*
Function to return all data strings for items received in a file. Used by inventory to store request
to concatenate all data records from the 3 sources to a single file
*/
vector<string> getRecords(ofstream &batchLog, int sequenceNo, string fileName) {
	vector<string> records;
	string line;
	int trailerCount = 0;
	ifstream input(fileName);

	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << fileName << " sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
	}

	getline(input, line);
	while (line[0] != 'T') {
		records.push_back(line);
		getline(input, line);
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << fileName << " trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);

	//TODO: RETURN AN ACTUAL VALUE
	return vector<string>();
}

void writeInventoryToStoreFile(vector<string> records, int sequenceNo) {
	ofstream output("storeupdate.txt");

	output << "HD ";
	
	if (sequenceNo < 10)
		output << "0";
	if (sequenceNo < 100)
		output << "0";
	if(sequenceNo < 1000)
		output << "0";

	output << to_string(sequenceNo) << endl;

	for (int i = 0; i < (int)records.size(); i++)
		output << records[i] << endl;

	output << "T ";

	if (records.size() < 10)
		output << "0";
	if (records.size() < 100)
		output << "0";
	if (records.size() < 1000)
		output << "0";

	output << records.size();
}

/*
Where stores request inventory from the warehouse.
Note: there is no estimated date. Instantly deduct quantities from the warehouse
and add them to the stores
*/
void inventoryToStoreRequest(DatabaseManager *dbm, ofstream &batchLog, int sequenceNoStoreUpdate, int sequenceNoStoreAdd, int sequenceOnlineReq, int sequenceNoBatchRev) {
	batchLog << "=====Inventory To Store Request=====";

	//Code to combine all 3 record files into one file
	vector<string> records = getRecords(batchLog, sequenceNoStoreAdd, "createstoreitems.txt");
	vector<string> tmp;
	tmp = getRecords(batchLog, sequenceOnlineReq, "onlinerequest.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());
	tmp = getRecords(batchLog, sequenceNoBatchRev, "batchreview.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());
	writeInventoryToStoreFile(records, sequenceNoStoreUpdate);

	ifstream input("storeupdate.txt");
	string line;
	int trailerCount = 0;

	getline(input, line);

	if (stoi(line.substr(3, 4)) != sequenceNoStoreUpdate)
	{
		batchLog << "storeupdate.txt sequence number mismatch. Expected " + to_string(sequenceNoStoreUpdate) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);

	while (line[0] != 'T') {
		//TODO: INCREASE STORE QUANTITIES AND LOWER WAREHOUSE QUANTITIES
		getline(input, line);
		trailerCount++;
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "storeupdate.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);
}

//Where warehouse item quantities are replenished
void inventoryReceivedAtWarehouse(DatabaseManager *dbm, ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Inventory Received at Warehouse=====";

	ifstream input("itemreceived.txt");
	string line;
	int trailerCount = 0;

	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << "itemreceived.txt sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);

	while (line[0] != 'T') {
		//TODO: INCREMENT WAREHOUSE ITEM QUANTITIES
		getline(input, line);
		trailerCount++;
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "itemreceived.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);
}

//Where warehouse items are requested from the vendors
void inventoryGeneration(DatabaseManager *dbm) {
	ofstream output("vendororder.txt");

	//TODO: ITERATE THROUGH ALL WAREHOUSE ITEMS AND REORDER ITEMS THAT ARE BELOW REORDER QUANTITIES
}

//Where items are added/changed/deleted from the database
void updateItemData(DatabaseManager *dbm, ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Update Item Data=====";

	ifstream input("items.txt");
	int trailerCount = 0;
}

//Calculate sales for specified items
void yearlySales(DatabaseManager *dbm, ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Yearly Sales=====";

	ifstream input("reports.txt");
	int trailerCount = 0;
}

void runBatchSequence(DatabaseManager *dbm) {
	ofstream batchLog("batchLog.txt");
	ifstream sequenceNos("sequence.txt");
	int sequenceNo1, sequenceNo2, sequenceNo3, sequenceNo4;
	/*TODO: for the 6 batch sequences below, an order needs to be determined.
	Need to determine an order that will accurately order the right quatities, track the right sales, etc....

	ALSO: FUNCTIONS MIGHT NOT BE "VOIDS". THEY MIGHT RETURN A FILE FOR USE IN NEXT BATCH

	ALSO: WE NEED TO CREATE A FILE THAT CONTAINS SEQUENCE NUMBERS. should just be
	a file lists six sequences in order.
	example: "sequence.txt" is exactly this: 0059 0101 9999 0148 4567 4567
	this will contain the sequence numbers to be checked with the incoming files.
	*/
	sequenceNos >> sequenceNo1;
	updateItemData(dbm, batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	createDeleteStore(dbm, batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	inventoryReceivedAtWarehouse(dbm, batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	sequenceNos >> sequenceNo2;
	sequenceNos >> sequenceNo3;
	sequenceNos >> sequenceNo4;
	inventoryToStoreRequest(dbm, batchLog, sequenceNo1, sequenceNo2, sequenceNo3, sequenceNo4);

	inventoryGeneration(dbm);

	sequenceNos >> sequenceNo1;
	yearlySales(dbm, batchLog, sequenceNo1);
}