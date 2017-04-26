#pragma once

void printbatchLog();

string ZeroFillNumber(string str, int digits);

string SpaceFillString(string str, int size);

string RemoveSpaces(string str);

vector<string> getRecords(ofstream &batchLog, int sequenceNo, string fileName);

void writeFile(vector<string> records, int sequenceNo, string fileName);

void WriteBatchReviewFile(DatabaseManager *dbm, int seqNo);

int incSeqNo(int s);

vector<string> SortRecords(vector<string> records);

///BATCH FUNCTIONS
void updateItemData(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo);

void createDeleteStore(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNoStoreUpdate, int &sequenceNoCreateStoreItems, int &sequenceNoDeleteStoreItems);

void inventoryReceivedAtWarehouse(DatabaseManager *dbm, ofstream &batchLog, int sequenceNoDeleteStoreItems, int &sequenceNoItemReceived, int &sequenceNoWarehouseInventoryUpdate);

void inventoryToStoreRequest(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNoStoreUpdate, int sequenceNoCreateStoreItems, int &sequenceOnlineReq, int &sequenceNoBatchRev, int &sequenceNoReorder);

void inventoryGeneration(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo);

void yearlySales(DatabaseManager *dbm, ofstream &batchLog, int &sequenceNo);

///CALLS BATCH FUNCTIONS
void runBatchSequence(DatabaseManager *dbm);