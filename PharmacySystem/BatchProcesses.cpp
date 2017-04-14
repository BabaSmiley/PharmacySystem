using namespace std;

#include <fstream>
#include <string>
#include <vector>
#include "DatabaseManager.h"

//Where stores are created and deleted in the database
void createDeleteStore(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Create/Delete Store=====";

	ifstream input("adddeletestore.txt");
	ofstream outInvToStoreReq("createstoreitems.txt");
	ofstream outInvRecAtWarehour("deletestoreitems.txt");
	string line;
	int trailerCount = 0;

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

		if (line[0] == 'A') {
			//TODO: ADD STORE HERE. MUST CHECK FOR ALREADY EXISTING STORE
			int controlCount = 0;
			getline(input, line);

			while (line[0] != 'C') {
				if (line[0] == 'D')
					batchLog << "Unexpected delete store during store-item creation. Skipping.";
				else if (line[0] == 'I') {
					//TODO: ADD ITEM TO STORE
					controlCount++;
				}

				if (stoi(line.substr(2, 4)) != trailerCount)
					batchLog << "Control mismatch for store creation.";
			}
		}
		else if (line[0] == 'D') {
			//TODO: DELETE STORE, WRITE FILE TO PASS INTO INVENTORY RECEIVED
		}

		getline(input, line);
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "adddeletestore.txt trailer mismatch. Expected" + line.substr(2, 4)
			+ " got " + to_string(trailerCount);
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
void inventoryToStoreRequest(ofstream &batchLog, int sequenceNoStoreUpdate, int sequenceNoStoreAdd, int sequenceOnlineReq, int sequenceNoBatchRev) {
	batchLog << "=====Inventory To Store Request=====";

	//Code to combine all 3 record files into one file
	vector<string> records = getRecords(batchLog, sequenceNoStoreAdd, "storeadd.txt");
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
void inventoryReceivedAtWarehouse(ofstream &batchLog, int sequenceNo) {
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
void inventoryGeneration() {
	ofstream output("vendororder.txt");

	//TODO: ITERATE THROUGH ALL WAREHOUSE ITEMS AND REORDER ITEMS THAT ARE BELOW REORDER QUANTITIES
}

//Where items are added/changed/deleted from the database
void updateItemData(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Update Item Data=====";

	ifstream input("items.txt");
	int trailerCount = 0;
}

//Calculate sales for specified items
void yearlySales(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Yearly Sales=====";

	ifstream input("reports.txt");
	int trailerCount = 0;
}

void runBatchSequence() {
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
	updateItemData(batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	createDeleteStore(batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	inventoryReceivedAtWarehouse(batchLog, sequenceNo1);

	sequenceNos >> sequenceNo1;
	sequenceNos >> sequenceNo2;
	sequenceNos >> sequenceNo3;
	sequenceNos >> sequenceNo4;
	inventoryToStoreRequest(batchLog, sequenceNo1, sequenceNo2, sequenceNo3, sequenceNo4);

	inventoryGeneration();

	sequenceNos >> sequenceNo1;
	yearlySales(batchLog, sequenceNo1);
}