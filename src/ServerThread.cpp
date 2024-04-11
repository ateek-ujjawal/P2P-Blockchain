#include "ServerThread.h"

#include <time.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

#include "sha256.h"

ServerThread::ServerThread(/* args */) {}

ServerThread::~ServerThread() {}

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

void ServerThread::ServerCommunicate(std::unique_ptr<SSocket> socket) {
	int ackResponse;
	std::unique_ptr<ServerStub> stub(new ServerStub());

	stub->Init(std::move(socket));

	ackResponse = stub->ReceiveAck();
	switch (ackResponse) {
	case 10:
		HandleClientTransaction(std::move(stub));
		break;
	case 11:
		HandleClientReadChain(std::move(stub));
		break;
	case 1:
		HandlePeer(std::move(stub));
		break;
	default:
		break;
	}
}

void ServerThread::ServerGenerateBlock() {
	srand(time(0));
	int nonce = rand();
	int response;
	char *prev_hash;
	{
		std::lock_guard<std::mutex> bc_lock(blockchain_mtx);
		prev_hash = chain.GetLastHash();
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		{
			std::lock_guard<std::mutex> cur_lock(cur_txn_mtx);
			if (!cur_txns.empty()) {
				auto blk = GenerateBlockByPOW(prev_hash, 1, nonce);
				nonce++;

				if (blk != nullptr) {
					{
						std::lock_guard<std::mutex> lock(blockchain_mtx);
						if (chain.AddBlock(blk)) {
							prev_hash = chain.GetLastHash();
							// std::cout << "Block added to the chain, last block hash now is: " << prev_hash << std::endl;
						}
					}

					for (auto &peer : peer_list) {
						/* Check if peers are already connected and send the blockchain if not connected */
						if (!peer.isConnect) {
							peer.isConnect = HandleRecover(peer);
						}
						if (peer.isConnect) {
							// Ack 1 means we are sending blocks here
							if (!peer.peerStub.SendAck(1) || !peer.peerStub.SendBlock(*blk)) {
								peer.isConnect = false;
							}
						}
					}

					// regenerate the nonce
					nonce = rand();
					cur_txns.clear();
				}
			}
		}
	}
}

/* Respond to client requests */
void ServerThread::HandleClientTransaction(std::unique_ptr<ServerStub> stub) {
	Transaction txn;
	txn.SetTransaction(0, 0, NULL, 0, NULL, 0, 0);
	int response;

	while (true) {
		/* Receive a transaction from a client */
		txn = stub->ReceiveTransaction();
		if (!txn.IsValid())
			break;

		/* Print transaction(do something with the transaction) */
		txn.Print();
		{
			std::lock_guard<std::mutex> lock(cur_txn_mtx);
			cur_txns.push_back(txn);
		}

		/* Multicast transaction to all peers */
		for (auto &peer : peer_list) {
			/* Check if peers are already connected and send the blockchain if not connected*/
			if (!peer.isConnect) {
				peer.isConnect = HandleRecover(peer);
			}
			if (peer.isConnect) {
				// Ack 0 means we are sending transactions here
				if (!peer.peerStub.SendAck(0) || !peer.peerStub.SendTransaction(txn)) {
					peer.isConnect = false;
				}
			}
		}
	}
}

void ServerThread::HandleClientReadChain(std::unique_ptr<ServerStub> stub) {
	stub->SendChain(chain.GetMainChain());
}

/* Respond to peer requests */
/* NOTE: While this function appears to be the same as client one, the peers may also send blocks instead of transactions,
   so it is a good idea to separate both
*/
void ServerThread::HandlePeer(std::unique_ptr<ServerStub> stub) {
	Transaction txn;
	Block *blk;
	int ack;
	txn.SetTransaction(0, 0, NULL, 0, NULL, 0, 0);

	while (true) {
		ack = stub->ReceiveAck();
		switch (ack) {
		case 0: {
			txn = stub->ReceiveTransaction();
			if (!txn.IsValid())
				return;

			// txn.Print();
			{
				std::lock_guard<std::mutex> lock(cur_txn_mtx);
				cur_txns.push_back(txn);
			}
			break;
		}
		case 1: {
			/* Received a block */
			blk = stub->ReceiveBlock();

			if (!blk)
				return;

			// std::cout << "Received block" << std::endl;
			// blk.Print();

			{
				std::lock_guard<std::mutex> lock(blockchain_mtx);
				if (chain.AddBlock(blk)) {
					// std::cout << "Block added to the chain, last block hash now is: " << chain.GetLastHash() << std::endl;
				}
			}
			break;
		}
		default:
			return;
		}
	}
}

// Generate Proof-of-work with given difficulty
Block *ServerThread::GenerateBlockByPOW(char *prev_hash, int difficulty, int nonce) {
	Block *blk = nullptr;
	std::string diff_string(difficulty, '0');
	std::string txns_str;
	std::string input;
	std::string output_hash;

	for (auto &txn : cur_txns)
		txns_str += txn.ToString();

	input = prev_hash + txns_str + std::to_string(nonce);
	output_hash = sha256(input);
	if (std::strncmp(&diff_string[0], &output_hash[0], difficulty) == 0) {
		blk = new Block(prev_hash, &output_hash[0], nonce, cur_txns.size(), cur_txns);
	}

	return blk;
}

bool ServerThread::HandleRecover(Peer &p) {
	if (!p.peerStub.Init(p.ip, p.port) || !p.peerStub.SendAck(1)) {
		return false;
	}

	std::vector<Block *> mainChain;
	{
		std::lock_guard<std::mutex> bc_lock(blockchain_mtx);
		mainChain = chain.GetMainChain();
	}

	mainChain.erase(mainChain.begin());

	for (auto &blk : mainChain) {
		if (!p.peerStub.SendAck(1) || !p.peerStub.SendBlock(*blk)) {
			return false;
		}
	}

	return true;
}
