using namespace std;

#include <fstream>
#include <string>
#include <vector>
#include <string>
#include "DatabaseManager.h"
#include <algorithm>

string ZeroFillNumber(string str, int digits) {
	while (str.size() < digits)
		str = "0" + str;

	return str;
}

string SpaceFillString(string str, int size) {
	while (str.size() < size)
		str += " ";

	return str;
}

string RemoveSpaces(string str) { //Removes spaces only if there is something other than a space at the first character
	if (str.size() == 0)
		return str;

	if (str[0] != ' ')
		while (str[str.size() - 1] == ' ')
			str = str.substr(0, str.size() - 1);

	return str;
}

vector<string> getRecords(ofstream &batchLog, int sequenceNo, string fileName) /*
Function to return all data strings for items received in a file. Used by inventory to store request
to concatenate all data records from the 3 sources to a single file. Used by warehouse inventory update too.
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

void writeFile(vector<string> records, int sequenceNo, string fileName) {
	ofstream output(fileName);

	output << "HD " << ZeroFillNumber(to_string(sequenceNo), 4) << endl;

	for (int i = 0; i < (int)records.size(); i++)
		output << records[i] << endl;

	output << "T " << ZeroFillNumber(to_string(records.size()), 4) << endl;
}

void WriteBatchReviewFile(DatabaseManager *dbm, int seqNo) {
	ofstream out("batchreview.txt");

	out << "HD " << ZeroFillNumber(to_string(seqNo), 4) << endl;

	int trailerCounter = 0;
	vector<Inventory*> inv = dbm->getAllInventory();

	for (int i = 0; i < inv.size(); i++)
		if (inv[i]->getItemLevel() <= inv[i]->getRefillLevel()) {
			Store* store = dbm->getStore(inv[i]->getStoreId());
			out << "B" << ZeroFillNumber(to_string(inv[i]->getStoreId()), 5) << ZeroFillNumber(to_string(store->getPriorityLevel()), 2) << 
				ZeroFillNumber(to_string( inv[i]->getItemId()), 9) << ZeroFillNumber(to_string(inv[i]->getRefillQuantity()), 10) << endl;
			trailerCounter++;
		}

	out << "T " << ZeroFillNumber(to_string(trailerCounter), 4);
}

void WriteCreateStoreItemsFile(DatabaseManager *dbm, int seqNo) {
	ofstream out("createstoreitems.txt");

	out << "HD " << ZeroFillNumber(to_string(seqNo), 4) << endl;

	int trailerCounter = 0;

	//TODO: get AddItem tuples from database, delete additem tuples from database, use loop to write to file.
	//for (int i = 0; i < )

	out << "T " << ZeroFillNumber(to_string(trailerCounter), 4);
}

int incSeqNo(int s) {
	s++;
	if (s >= 10000)
		s = 0;
	return s;
}

vector<string> SortRecords(vector<string> records) { //SORTS RECORDS BY ITEM CODE BY PRIORITY LEVEL
	for (int i = 0; i < records.size(); i++) { //reorders to: (Item code|priority level| source|storeID|requested quantity) in order to sort
		string tmp = records[i];
		records[i] = tmp.substr(8, 9) + tmp.substr(6, 2) + tmp.substr(0, 6) + tmp.substr(17, 10);
	}

	sort(records.begin(), records.end());

	for (int i = 0; i < records.size(); i++) { //Converts to original form |source|store id|store priority level|item code|requested quantity|
		string tmp = records[i];
		records[i] = tmp.substr(11, 6) + tmp.substr(9, 2) + tmp.substr(0, 9) + tmp.substr(17, 10);
	}

	return records;
}

///BATCH FUNCTIONS
//DONE
void updateItemData(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo) { //Where items are added/changed/deleted from the database
	batchLog << "=====Update Item Data=====";

	ifstream input("items.txt");

	string line;
	int trailerCount = 0, controlCount = 0;

	//Header check
	getline(input, line);
	if (stoi(line.substr(3, 4)) != sequenceNo)
	{
		batchLog << "adddeletestore.txt sequence number mismatch. Expected " + to_string(sequenceNo) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);
	while (line[0] != 'N') { //delete item
		if (line[0] == 'D') {
			trailerCount++; controlCount++;
			dbm->deleteItem(stoi(line.substr(1, 9)));
		}
		else batchLog << "Unexpected action code '" << line[0] << "'. Skip line." << endl;

		getline(input, line);
	}
	if (stoi(line.substr(2, 4)) != controlCount)
		batchLog << "delete item trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(controlCount);

	getline(input, line);
	controlCount = 0;
	while (line[0] != 'N') { //Change item
		if (line[0] == 'C') {
			trailerCount++; controlCount++;
			string itemCode = line.substr(1, 9);
			string itemName = line.substr(10, 20);
			string itemDesc = line.substr(30, 100);
			string itemDosage = line.substr(130, 20);
			string whReorderLevel = line.substr(150, 10);
			string vendorCode = line.substr(160, 4);
			string whReorderQuantity = line.substr(164, 10);
			string expDelivery = line.substr(174, 20);

			if (RemoveSpaces(whReorderLevel) == "")
				whReorderLevel = "-1";
			if (RemoveSpaces(vendorCode) == "")
				vendorCode = "-1";
			if (RemoveSpaces(whReorderQuantity) == "")
				whReorderQuantity = "-1";

			dbm->updateItem(stoi(itemCode), RemoveSpaces(itemName), RemoveSpaces(itemDesc), -1, RemoveSpaces(itemDosage),
				stoi(vendorCode), RemoveSpaces(expDelivery), stol(whReorderLevel), stol(whReorderQuantity), -1, NULL);
		}
		else batchLog << "Unexpected action code '" << line[0] << "'. Skip line." << endl;

		getline(input, line);
	}
	if (stoi(line.substr(2, 4)) != controlCount)
		batchLog << "change item trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(controlCount);

	getline(input, line);
	controlCount = 0;
	while (line[0] != 'N') { //Add item
		if (line[0] == 'A') {
			trailerCount++; controlCount++;
			string itemCode = line.substr(1, 9);
			string itemName = line.substr(10, 20);
			string itemDesc = line.substr(30, 100);
			string itemDosage = line.substr(130, 20);
			string whReorderLevel = line.substr(150, 10);
			string vendorCode = line.substr(160, 4);
			string whReorderQuantity = line.substr(164, 10);
			string expDelivery = line.substr(174, 20);

			dbm->createItem(stoi(itemCode), RemoveSpaces(itemName), RemoveSpaces(itemDesc), 5, RemoveSpaces(itemDosage),
				stoi(vendorCode), RemoveSpaces(expDelivery), stol(whReorderLevel), stol(whReorderQuantity), 0, 1);
		}
		else batchLog << "Unexpected action code '" << line[0] << "'. Skip line." << endl;

		getline(input, line);
	}
	if (stoi(line.substr(2, 4)) != controlCount)
		batchLog << "Add item trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(controlCount);

	if (stoi(line.substr(2, 4)) != trailerCount) //Trailer check
		batchLog << "items.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);

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
			dbm->createStore(stoi(storeID), RemoveSpaces(storeAddress), RemoveSpaces(storeCity), RemoveSpaces(storeState), stoi(storeZip), stoi(StorePriority), 1);
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
					dbm->createInventory(stoi(storeID), stoi(ICode), stoi(IdefaultQty), stoi(IReorderLevel), stoi(IReorderQty), 0);
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
			string storeID = line.substr(1, 5);
			string storeAddress = line.substr(6, 20);
			string storeCity = line.substr(26, 20);
			string storeState = line.substr(46, 2);
			string storeZip = line.substr(48, 9);
			string StorePriority = line.substr(57, 2);

			//Store(int id, string address, string city, string state, int zipCode, int priorityLevel, int isActive)
			Store* tmp = new Store(stoi(storeID), RemoveSpaces(storeAddress), RemoveSpaces(storeCity), RemoveSpaces(storeState), stoi(storeZip), stoi(StorePriority), 1);
			Store* store = dbm->getStore(stoi(storeID));

			if (store == tmp) { //All fields match

			}
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

	vector<string> tmp;
	vector<string> records = getRecords(batchLog, sequenceNoDeleteStoreItems, "deletestoreitems.txt");
	tmp = getRecords(batchLog, sequenceNoItemReceived, "itemreceived.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());
	writeFile(records, sequenceNoWarehouseInventoryUpdate, "warehouseinventoryupdate.txt");

	ifstream input("warehouseinventoryupdate.txt");
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
		//TODO: INCREMENT WAREHOUSE ITEM QUANTITIES VIA BATCH FUNCTION
		getline(input, line);
		trailerCount++;
	}

	if (stoi(line.substr(2, 4)) != trailerCount)
		batchLog << "itemreceived.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCount);

	sequenceNoItemReceived = incSeqNo(sequenceNoItemReceived);
	sequenceNoWarehouseInventoryUpdate = incSeqNo(sequenceNoWarehouseInventoryUpdate);
}

void inventoryToStoreRequest(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNoStoreUpdate, int sequenceNoCreateStoreItems, int &sequenceOnlineReq, int &sequenceNoBatchRev, int &sequenceNoReorder) /*
Where stores request inventory from the warehouse.
Note: there is no estimated date. Instantly deduct quantities from the warehouse
and add them to the stores
*/ {
	batchLog << "=====Inventory To Store Request=====";

	WriteBatchReviewFile(dbm, sequenceNoBatchRev);
	WriteCreateStoreItemsFile(dbm, sequenceNoCreateStoreItems);

	//Code to combine all 3 record files into one file
	vector<string> records = getRecords(batchLog, sequenceNoCreateStoreItems, "createstoreitems.txt");
	vector<string> tmp;
	tmp = getRecords(batchLog, sequenceOnlineReq, "onlinerequest.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());
	tmp = getRecords(batchLog, sequenceNoBatchRev, "batchreview.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());
	tmp = getRecords(batchLog, sequenceNoBatchRev, "reorder.txt");
	records.insert(records.begin(), tmp.begin(), tmp.end());

	records = SortRecords(records);
	writeFile(records, sequenceNoStoreUpdate, "storeupdate.txt");

	ofstream output("reorder.txt");
	sequenceNoReorder = incSeqNo(sequenceNoReorder);
	output << "HD " << ZeroFillNumber(to_string(sequenceNoReorder), 4) << endl;

	ifstream input("storeupdate.txt");
	string line;
	int trailerCountInput = 0, trailerCountOutput = 0;

	getline(input, line);

	if (stoi(line.substr(3, 4)) != sequenceNoStoreUpdate)
	{
		batchLog << "storeupdate.txt sequence number mismatch. Expected " + to_string(sequenceNoStoreUpdate) +
			" got " + line.substr(3, 4) + ". Terminated program." << endl;
		return;
	}

	getline(input, line);

	while (line[0] != 'T') {
		//TODO: INCREASE STORE QUANTITIES AND LOWER WAREHOUSE QUANTITIES. NEEDS A DB FUNCTION.
		//TODO: IF ORDER NOT FULFILLED WRITE TO AN ORDER NOT FULFILLED FILE.
		getline(input, line);
		trailerCountInput++;
	}

	if (stoi(line.substr(2, 4)) != trailerCountInput)
		batchLog << "storeupdate.txt trailer mismatch. Expected" + line.substr(2, 4)
		+ " got " + to_string(trailerCountInput);

	output << "T " << ZeroFillNumber(to_string(trailerCountOutput), 4);

	sequenceNoStoreUpdate = incSeqNo(sequenceNoStoreUpdate);
	sequenceOnlineReq = incSeqNo(sequenceOnlineReq);
	sequenceNoBatchRev = incSeqNo(sequenceNoBatchRev);
}

//DONE
void inventoryGeneration(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo) { //Where warehouse items are requested from the vendors
	batchLog << "=====Inventory Generation=====" << endl;

	ofstream output("vendororder.txt");

	output << "HD " << ZeroFillNumber(to_string(sequenceNo), 4);

	vector<Item*> items = dbm->getItems(0);

	int trailerCount = 0;

	for (int i = 0; i < items.size(); i++)
		if (items[i]->getWhLevel() <= items[i]->getWhRefillQty()) {
			output << ZeroFillNumber(to_string(items[i]->getVendorId()), 4) << ZeroFillNumber(to_string(items[i]->getId()), 9) << ZeroFillNumber(to_string(items[i]->getWhRefillQty()), 10) << endl;
			trailerCount++;
		}

	output << "T " << ZeroFillNumber(to_string(trailerCount), 4);

	sequenceNo = incSeqNo(sequenceNo);
}

//DONE
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

		string itemCode = line.substr(0, 4);
		vector<Sale*> sales = dbm->getSalesByItem(stoi(itemCode)); //Sales for an item sorted by date
		out << "=====" << itemCode << "=====";

		string year = "0000", month = "00";

		int salesM = 0, salesY = 0, salesT = 0,
			qtyM = 0, qtyY = 0, qtyT = 0;

		for (int i = 0; i < sales.size(); i++) {
			string yeartmp = (sales[i]->getDate()).substr(0, 4),
				monthtmp = (sales[i]->getDate()).substr(5, 2);

			if (yeartmp > year) { //If new year, then set new year, and roll over month, and reset counters
				out << year << "-" << month << qtyM << " " << salesM << endl;
				out << year << qtyY << salesY;

				year = yeartmp;
				month = monthtmp;

				salesM = 0; salesY = 0; qtyM = 0; qtyY = 0; //reset counters
			}
			else if (monthtmp > month) { //If new month, then set new month, and reset month counter
				out << year << "-" << month << qtyM << " $" << salesM << endl;

				month = monthtmp;

				salesM = 0; qtyM = 0; //reset counters
			}

			int qty = sales[i]->getQuantity();
			int salesAmt = sales[i]->getSalePrice() * qty;

			salesM += salesAmt; salesY += salesAmt; salesT += salesAmt;
			qtyM += qty; qtyY += qty; qtyT += qty;
		}

		out << "GRAND TOTAL: " << qtyT << " $" << salesT << endl;

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
	for (int i = 1; i <= 12; i++)
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
	11: reorder.txt
	*/
	updateItemData(dbm, batchLog, sequenceNos[0]);
	createDeleteStore(dbm, batchLog, sequenceNos[1], sequenceNos[2], sequenceNos[3]);
	inventoryReceivedAtWarehouse(dbm, batchLog, oldsequenceNos[3], sequenceNos[4], sequenceNos[10]);
	inventoryToStoreRequest(dbm, batchLog, sequenceNos[7], oldsequenceNos[2], sequenceNos[6], sequenceNos[5], sequenceNos[11]);
	inventoryGeneration(dbm, batchLog, sequenceNos[8]);
	yearlySales(dbm, batchLog, sequenceNos[9]);

	ofstream sequencesOut("sequences.txt"); //Writes new sequence numbers to same file
	for (int i = 1; i <= 12; i++)
		sequencesOut << sequenceNos[i - 1];
}