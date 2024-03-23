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

/* Respond to client requests */
void clientThread(std::unique_ptr<ServerSocket> client) {
	transaction_t txn;
	txn.id = 0;
	int response;
	
	
	while(txn.id != -1) {
		/* Receive a transaction from a client */
		txn = client->ReceiveTransaction();
		if(txn.id != -1) {
		
			/* Print transaction(do something with the transaction) */
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;
			
			/* Check if peers are already connected */
			if(peers.empty()) {
				for(unsigned long i = 0; i < peer_list.size(); i++) {
					peers.push_back(peerSocket.Init(peer_list[i].ip, peer_list[i].port));
					if(peers[i])
						peers[i]->SendAck();
				}
			}
			
			/* Multicast transaction to all peers */
			for (unsigned long i = 0; i < peer_list.size(); i++) {
				if(peers[i]) {
					response = peers[i]->SendTransaction(txn);
					if(response == -1)
						peers[i] = nullptr;
				} else {
					peers[i] = peerSocket.Init(peer_list[i].ip, peer_list[i].port);
					peers[i]->SendAck();
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

/* Respond to peer requests */
/* NOTE: While this function appears to be the same as client one, the peers may also send blocks instead of transactions,
   so it is a good idea to separate both
*/
void peerThread(std::unique_ptr<ServerSocket> peer) {
	transaction_t txn;
	txn.id = 0;
	
	while(txn.id != -1) {
		txn = peer->ReceiveTransaction();
		if (txn.id != -1) {
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: ./server [port] [peers] (list all peer [ip] [port])" << std::endl;
		exit(1);
	}
	int port = atoi(argv[1]);
	serverSocket.Init(port);
	
	int peer_amt = atoi(argv[2]);
	std::string ip;
	int skip = 3;
	Peer p;
	
	for(int i = 1; i <= peer_amt; i++) {
		p.ip = argv[skip++];
		p.port = atoi(argv[skip++]);
		peer_list.push_back(p);
	}
	
	std::unique_ptr<ServerSocket> client;
	std::thread th;
	int ackResponse;
	
	while(true) {
		client = serverSocket.Accept();
		ackResponse = client->ReceiveAck();
		
		/* Respond to client requests or peer requests */
		if(ackResponse == 0) {
			th = std::thread(clientThread, std::move(client));
			th.join();
		} else if (ackResponse == 1) {
			th = std::thread(peerThread, std::move(client));
			th.detach();
		}
			
	}
	
	return 0;
}
