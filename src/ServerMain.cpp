#include <iostream>
#include <vector>
#include <thread>

#include "ServerSocket.h"
#include "PeerSocket.h"
#include "Transaction.h"

ServerSocket serverSocket;
std::vector<std::unique_ptr<PeerSocket>> peer_list;

void serverThread(std::unique_ptr<ServerSocket> client) {
	transaction_t txn;
	txn.id = 0;
	
	while(txn.id != -1) {
		txn = client->ReceiveTransaction();
		if(txn.id != -1) {
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;
			for (unsigned long i = 0; i < peer_list.size(); i++) {
				
			}
		}
	}
	
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: ./server [port] [peers] (list all peer [ip] [port])" << std::endl;
	}
	int port = atoi(argv[1]);
	serverSocket.Init(port);
	
	int peers = atoi(argv[2]);
	std::string ip;
	int skip = 3;
	PeerSocket peerSocket;
	for(int i = 1; i <= peers; i++) {
		ip = argv[skip * i];
		port = atoi(argv[(skip * i) + 1]);
		peer_list.push_back(peerSocket.Init(ip, port));
	}
	
	std::unique_ptr<ServerSocket> client;
	std::thread th;
	
	while(true) {
		client = serverSocket.Accept();
		th = std::thread(serverThread, std::move(client));
		th.join();
	}
	
	std::cout << "Hello world!" << std::endl;
	return 0;
}
