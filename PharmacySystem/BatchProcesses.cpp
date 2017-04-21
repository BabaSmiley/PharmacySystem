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

vector<string> getRecords(ofstream &batchLog, int sequenceNo, string fileName) /*
Function to return all data strings for items received in a file. Used by inventory to store request
to concatenate all data records from the 3 sources to a single file
*/ {
	vector<string> records;
	string line;
	int trailerCount = 0;
	ifstream input(fileName);

	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << fileName << " sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated record gathering from " << fileName << "." << endl;
		return records;
	}

	getline(input, line);
	while (line[0] != 'T') {
		records.push_back(line);
		getline(input, line);
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << fileName << " trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);

	return records;
}

void writeInventoryToStoreFile(vector<string> records, int sequenceNo) {
	ofstream output("storeupdate.txt");

	output << "HD " << ZeroFillNumber(to_string(sequenceNo), 4);

	output << to_string(sequenceNo) << endl;

	for (int i = 0; i < (int)records.size(); i++)
		output << records[i] << endl;

	output << "T ";

	output << ZeroFillNumber(to_string(records.size()), 4);
}

int incSeqNo(int s) {
	s++;
	if (s >= 10000)
		s = 0;
	return s;
}

///BATCH FUNCTIONS
void updateItemData(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo) { //Where items are added/changed/deleted from the database
	batchLog << "=====Update Item Data=====";

	ifstream input("items.txt");

	string line;
	int trailerCount = 0;

	//Header check
	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << "adddeletestore.txt sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);

	while (line[0] != 'T') {
		trailerCount++;

		//TODO: WRITE BATCH FUNCTIONALITY

		getline(input, line);
	}

	sequenceNo = incSeqNo(sequenceNo);
}

void createDeleteStore(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNoStoreUpdate, int &sequenceNoCreateStoreItems, int &sequenceNoDeleteStoreItems) {//Where stores are created and deleted in the database
	batchLog << "=====Create/Delete Store=====";

	ifstream input("adddeletestore.txt");

	//Writes two files
	ofstream outInvToStoreReq("createstoreitems.txt"); //Requests inventory to store
	int InvToStoreReqCounter = 0;
	ofstream outInvRecAtWarehouse("deletestoreitems.txt"); //Ships back to warehouse
	int InvRecAtWarehouseCounter = 0;

	string line;
	int trailerCount = 0;

	getline(input, line);

	//Header check
	if (stoi(line.substr(3, 4)) != sequenceNoStoreUpdate)
	{
		batchLog << "adddeletestore.txt sequence number mismatch. Expected " + to_string(sequenceNoStoreUpdate) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	outInvToStoreReq << "HD " + ZeroFillNumber(to_string(sequenceNoCreateStoreItems), 4) << endl;
	outInvRecAtWarehouse << "HD " + ZeroFillNumber(to_string(sequenceNoDeleteStoreItems), 4) << endl;

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
					dbm->createInventory(stoi(storeID), stoi(ICode), stoi(IdefaultQty), stoi(IReorderLevel), stoi(IReorderQty));
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

	sequenceNoStoreUpdate = incSeqNo(sequenceNoStoreUpdate);
	sequenceNoCreateStoreItems = incSeqNo(sequenceNoCreateStoreItems);
	sequenceNoDeleteStoreItems = incSeqNo(sequenceNoDeleteStoreItems);
}

void inventoryReceivedAtWarehouse(DatabaseManager *dbm, ofstream &batchLog, int sequenceNoDeleteStoreItems, int &sequenceNoItemReceived, int &sequenceNoWarehouseInventoryUpdate) { //Where warehouse item quantities are replenished
	batchLog << "=====Inventory Received at Warehouse=====";
	//TODO: Merge files into warehouseinventoryupdate
	ifstream input("itemreceived.txt");
	string line;
	int trailerCount = 0;

	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNoItemReceived)
	{
		batchLog << "itemreceived.txt sequence number mismatch. Expected " + to_string(sequenceNoItemReceived) +
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

	sequenceNoItemReceived = incSeqNo(sequenceNoItemReceived);
	sequenceNoWarehouseInventoryUpdate = incSeqNo(sequenceNoWarehouseInventoryUpdate);
}

void inventoryToStoreRequest(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNoStoreUpdate, int sequenceNoCreateStoreItems, int &sequenceOnlineReq, int &sequenceNoBatchRev) /*
Where stores request inventory from the warehouse.
Note: there is no estimated date. Instantly deduct quantities from the warehouse
and add them to the stores
*/ {
	batchLog << "=====Inventory To Store Request=====";

	//Code to combine all 3 record files into one file
	vector<string> records = getRecords(batchLog, sequenceNoCreateStoreItems, "createstoreitems.txt");
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

	sequenceNoStoreUpdate = incSeqNo(sequenceNoStoreUpdate);
	sequenceOnlineReq = incSeqNo(sequenceOnlineReq);
	sequenceNoBatchRev = incSeqNo(sequenceNoBatchRev);
}

void inventoryGeneration(DatabaseManager *dbm, int &sequenceNo) { //Where warehouse items are requested from the vendors
	ofstream output("vendororder.txt");

	//TODO: ITERATE THROUGH ALL WAREHOUSE ITEMS AND REORDER ITEMS THAT ARE BELOW REORDER QUANTITIES

	sequenceNo = incSeqNo(sequenceNo);
}

void yearlySales(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo) { //Calculate sales for specified items
	batchLog << "=====Yearly Sales=====";

	ifstream input("reports.txt");

	//Writes two files
	ofstream out("sales.txt");

	string line;
	int trailerCount = 0;

	getline(input, line);

	//Header check
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << "reports.txt sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);

	while (line[0] != 'T') {
		trailerCount++;

		//TODO: SELECT PRESCRIPTIONS BASED ON ITEM ID, SORT BY DATE!

		getline(input, line);
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "reports.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);

	sequenceNo = incSeqNo(sequenceNo);
}

///CALLS BATCH FUNCTIONS
void runBatchSequence(DatabaseManager *dbm) { //Calls all of the batch sequences
	ofstream batchLog("batchLog.txt");

	ifstream sequencesIn("sequences.txt"); //reads old sequence numbers
	vector<int> sequenceNos;
	for (int i = 1; i <= 11; i++)
		sequencesIn >> sequenceNos[i - 1];
	vector<int> oldsequenceNos = sequenceNos;

	/*
	SEQUENCE NUMBER ORDER of sequences.txt:
	0: items.txt
	1: adddeletestore.txt
	2: createstoreitems.txt
	3: deletestoreitems.txt
	4: itemreceived.txt
	5: batchreview.txt
	6: onlinerequest.txt
	7: storeupdate.txt
	8: vendororder.txt
	9: reports.txt
	10: warehouseinventoryupdate.txt
	*/
	updateItemData(dbm, batchLog, sequenceNos[0]);
	createDeleteStore(dbm, batchLog, sequenceNos[1], sequenceNos[2], sequenceNos[3]);
	inventoryReceivedAtWarehouse(dbm, batchLog, oldsequenceNos[3], sequenceNos[4], sequenceNos[10]);
	inventoryToStoreRequest(dbm, batchLog, sequenceNos[7], oldsequenceNos[2], sequenceNos[6], sequenceNos[5]);
	inventoryGeneration(dbm, sequenceNos[8]);
	yearlySales(dbm, batchLog, sequenceNos[9]);

	ofstream sequencesOut("sequences.txt"); //Writes new sequence numbers to same file
	for (int i = 1; i <= 11; i++)
		sequencesOut << sequenceNos[i - 1];
}