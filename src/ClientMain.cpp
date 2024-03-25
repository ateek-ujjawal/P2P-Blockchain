#include <iostream>
#include <string>

#include "ClientStub.h"

#define TXN_SIZE (2 * NAME_SIZE) + 8 // Transaction fields: id(4 bytes) sender(NAME_SIZE bytes, currently 20) receiver(NAME_SIZE bytes) amount(4 bytes)

ClientStub clientStub;

/* Send all transactions to the provided server */
void sendTransactions(int transactions, int amount) {
	Transaction txn;
	int response;
	char sender[] = "Client 1";
	char receiver[] = "Client 2";

	for (int i = 0; i < transactions; i++) {
		txn.SetTransaction(i + 1, sender, 8, receiver, 8, amount);
		response = clientStub.SendTransaction(txn);
	}

	if (response == 1)
		std::cout << "Send success";
}

/* Read blockchain copy of the provided server */
void readBlockchain() {
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		std::cerr << "usage: ./client [hostname/ip addr] [port] [request type] [transactions(for type 0)] [amount(for type 0)]" << std::endl;
		exit(1);
	}

	std::string ip = argv[1];
	int port = atoi(argv[2]);
	int type = atoi(argv[3]);

	/* Connect to peer server and send client side acknowledgement */
	clientStub.Init(ip, port);
	clientStub.SendAck();

	/* Request type 0 for sending transactions and 1 for reading the blockchain */
	if (type == 0) {
		int transactions = atoi(argv[4]);
		int amount = atoi(argv[5]);
		sendTransactions(transactions, amount);
	} else
		readBlockchain();

	return 1;
}
