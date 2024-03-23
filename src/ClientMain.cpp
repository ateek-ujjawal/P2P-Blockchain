#include <iostream> 
#include <string>

#include "ClientSocket.h"
#include "Transaction.h"

ClientSocket clientSocket;

/* Send all transactions to the provided server */
void sendTransactions(int transactions, int amount) {
	transaction_t txn;
	int response;

	for (int i = 0; i < transactions; i++) {
		txn.id = i + 1;
		txn.sender = 1;
		txn.receiver = 2;
		txn.amount = amount;
		response = clientSocket.Send(txn);
	}
	
	if(response == 1)
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
	
	/* Connect to peer server and send client side acknowledgement */
	clientSocket.Init(ip, port);
	clientSocket.SendAck();
	
	/* Request type 0 for sending transactions and 1 for reading the blockchain */
	if(atoi(argv[3]) == 0) {
		int transactions = atoi(argv[4]);
		int amount = atoi(argv[5]);
		sendTransactions(transactions, amount);
	} else
		readBlockchain();

	return 1;
}
