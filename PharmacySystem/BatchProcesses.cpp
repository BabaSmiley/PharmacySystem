#include <fstream>
using namespace std;

//Where stores are created and deleted in the database
void createDeleteStore(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Create/Delete Store=====";

	ifstream input("adddeletestore.txt");
	int trailerCount = 0;
}

/*
Where stores request inventory from the warehouse.
Note: there is no estimated date. Instantly deduct quantities from the warehouse
and add them to the stores
*/
void inventoryToStoreRequest(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Inventory To Store Request=====";

	ifstream input("itemreceived.txt");
	int trailerCount = 0;
}

//Where warehouse item quantities are replenished
void inventoryReceivedAtWarehouse(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Inventory Received at Warehouse=====";

	ifstream input("itemreceived.txt");
	int trailerCount = 0;
}

//Where warehouse items are requested from the vendors
void inventoryGeneration(ofstream &batchLog, int sequenceNo) {
	batchLog << "=====Inventory Generation=====";

	ifstream input("vendororder.txt");
	int trailerCount = 0;
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
	int sequenceNo;
	/*TODO: for the 6 batch sequences below, an order needs to be determined.
		Need to determine an order that will accurately order the right quatities, track the right sales, etc....

		ALSO: FUNCTIONS MIGHT NOT BE "VOIDS". THEY MIGHT RETURN A FILE FOR USE IN NEXT BATCH

		ALSO: WE NEED TO CREATE A FILE THAT CONTAINS SEQUENCE NUMBERS. should just be
			a file lists six sequences in order.
			example: "sequence.txt" is exactly this: 0059 0101 9999 0148 4567 4567
			this will contain the sequence numbers to be checked with the incoming files.
	*/
	///THIS ORDER NEEDS TO BE LOOKEDOVER
	sequenceNos >> sequenceNo;
	createDeleteStore(batchLog, sequenceNo);

	sequenceNos >> sequenceNo;
	updateItemData(batchLog, sequenceNo);

	sequenceNos >> sequenceNo;
	inventoryReceivedAtWarehouse(batchLog, sequenceNo);

	sequenceNos >> sequenceNo;
	inventoryToStoreRequest(batchLog, sequenceNo);

	sequenceNos >> sequenceNo;
	inventoryGeneration(batchLog, sequenceNo);

	sequenceNos >> sequenceNo;
	yearlySales(batchLog, sequenceNo);
}