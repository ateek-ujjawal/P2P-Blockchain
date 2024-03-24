#include "ServerThread.h"

#include <iostream>

ServerThread::ServerThread(/* args */) {
}

ServerThread::~ServerThread() {
}

void ServerThread::SetPeers(int cnt, char *argv[]) {
	int idx = 0;
	for (int i = 0; i < cnt; i++) {
		Peer peer;
		peer.ip = argv[idx++];
		peer.port = atoi(argv[idx++]);
		peer.isConnect = false;
		peer_list.push_back(peer);
	}
}

void ServerThread::ServerThreadFunc(std::unique_ptr<SSocket> socket) {
	int ackResponse;
	std::unique_ptr<ServerStub> stub;

	stub->Init(std::move(socket));

	ackResponse = stub->ReceiveAck();
	switch (ackResponse) {
	case 0:
		HandleClient(std::move(stub));
		break;
	case 1:
		HandlePeer(std::move(stub));
		break;
	default:
		break;
	}
}

/* Respond to client requests */
void ServerThread::HandleClient(std::unique_ptr<ServerStub> stub) {
	ServerStub stub;

	Transaction txn;
	txn.id = 0;
	int response;

	while (txn.id != -1) {
		/* Receive a transaction from a client */
		txn = stub->ReceiveTransaction();
		if (txn.id != -1) {
			/* Print transaction(do something with the transaction) */
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;

			// /* Check if peers are already connected */
			// if (peers.empty()) {
			// 	for (unsigned long i = 0; i < peer_list.size(); i++) {
			// 		peers.push_back(peerSocket.Init(peer_list[i].ip, peer_list[i].port));
			// 		if (peers[i])
			// 			peers[i]->SendAck();
			// 	}
			// }

			/* Multicast transaction to all peers */
			for (auto &peer : peer_list) {
				/* Check if peers are already connected */
				if (!peer.isConnect) {
					peer.peerStub.Init(peer.ip, peer.port);
					peer.peerStub.SendAck();
				}
				response = peer.peerStub.SendTransaction(txn);
				// todo need to handle response
			}
		}
	}
}

/* Respond to peer requests */
/* NOTE: While this function appears to be the same as client one, the peers may also send blocks instead of transactions,
   so it is a good idea to separate both
*/
void ServerThread::HandlePeer(std::unique_ptr<ServerStub> stub) {
	Transaction txn;
	txn.id = 0;

	while (txn.id != -1) {
		txn = stub->ReceiveTransaction();
		if (txn.id == -1) {
			std::cout << txn.sender << " " << txn.receiver << " " << txn.amount << std::endl;
		}
	}
}