#include <iostream>
#include <string>

#include "ClientStub.h"

ClientStub clientStub;
int id = -1;

/* Send all transactions to the provided server */
void sendTransactions(int transactions, int amount) {
	Transaction txn;
	int response;
	char sender[] = "Client 1";
	char receiver[] = "Client 2";

	for (int i = 0; i < transactions; i++) {
		txn.SetTransaction(i + 1, id, sender, 8, receiver, 8, amount);
		response = clientStub.SendTransaction(txn);
	}

	if (response == 1)
		std::cout << "Send success" << std::endl;
}

/* Read blockchain copy of the provided server */
void readBlockchain() {
	auto chain = clientStub.ReadBlockChain();

	for (auto &blk : chain) {
		blk.Print();
		printf("\n\n");
	}
}

int main(int argc, char *argv[]) {
	if (argc < 5) {
		std::cerr << "usage: ./client [hostname/ip addr] [port] [id]"
				  << "[request type (0 - send transactions, 1 - read chain from server)] "
				  << "[transactions(for type 0)] [amount(for type 0)]" << std::endl;
		exit(1);
	}

	std::string ip = argv[1];
	int port = atoi(argv[2]);
	id = atoi(argv[3]);
	int type = atoi(argv[4]);

	/* Connect to peer server and send client side acknowledgement */
	clientStub.Init(ip, port);

	/* Request type 0 for sending transactions and 1 for reading the blockchain */
	switch (type) {
	case 0: {
		/* code */
		int transactions = atoi(argv[5]);
		int amount = atoi(argv[6]);
		clientStub.SendAck(10);
		sendTransactions(transactions, amount);
		break;
	}
	case 1: {
		clientStub.SendAck(11);
		readBlockchain();
	}
	default:
		break;
	}

	return 0;
}
