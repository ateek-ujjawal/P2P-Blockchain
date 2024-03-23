#include <iostream>
#include <vector>
#include <thread>

#include "ServerSocket.h"
#include "PeerSocket.h"
#include "Transaction.h"
#include "Peer.h"

ServerSocket serverSocket;
PeerSocket peerSocket;
std::vector<Peer> peer_list;
std::vector<std::unique_ptr<PeerSocket>> peers;

void serverThread(std::unique_ptr<ServerSocket> client) {
	transaction_t txn;
	txn.id = 0;
	int response;
	
	while(txn.id != -1) {
		txn = client->ReceiveTransaction();
		if(txn.id != -1) {
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;
			
			for (unsigned long i = 0; i < peers.size(); i++) {
				if(peers[i]) {
					response = peers[i]->SendTransaction(txn);
					if(response == -1)
						peers[i] = nullptr;
				} else {
					peers[i] = peerSocket.Init(peer_list[i].ip, peer_list[i].port);
					if(peers[i]) {
						response = peers[i]->SendTransaction(txn);
						if(response == -1)
							peers[i] = nullptr;
					}
				}
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
	
	int peer_amt = atoi(argv[2]);
	std::string ip;
	int skip = 3;
	Peer p;
	
	for(int i = 1; i <= peer_amt; i++) {
		p.ip = argv[skip * i];
		p.port = atoi(argv[(skip * i) + 1]);
		peer_list.push_back(p);
		peers.push_back(peerSocket.Init(p.ip, p.port));
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
