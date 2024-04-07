#include "BlockChain.h"
#include "ServerThread.h"

#include <time.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

#include "sha256.h"

BlockChain chain;

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

void ServerThread::ServerCommunicate(std::unique_ptr<SSocket> socket) {
	int ackResponse;
	std::unique_ptr<ServerStub> stub(new ServerStub());

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
			std::lock_guard<std::mutex> lock(pending_txn_mtx);
			while (cur_txns.size() < MAX_TXN_SIZE && !pending_txn.empty()) {
				cur_txns.push_back(pending_txn.front());
				pending_txn.pop();
			}
		}

		if (!cur_txns.empty()) {
			auto blk = GenerateBlockByPOW(prev_hash, 1, nonce);
			nonce++;
			
			if (blk != nullptr) {
				{
					std::lock_guard<std::mutex> lock(blockchain_mtx);
					if(chain.AddBlock(blk)) {
						prev_hash = chain.GetLastHash();
						std::cout << "Block added to the chain, last block hash now is: " << prev_hash << std::endl;
					}
				}
			
				for (auto &peer : peer_list) {
					/* Check if peers are already connected and send the blockchain if not connected */
					if (!peer.isConnect) {
						peer.isConnect = peer.peerStub.Init(peer.ip, peer.port);
						if(peer.isConnect) {
							peer.isConnect = peer.peerStub.SendAck(1);
							if(peer.isConnect) {
								std::vector<Block *> mainChain;
								{
									std::lock_guard<std::mutex> bc_lock(blockchain_mtx);
									mainChain = chain.GetMainChain();
								}
								
								mainChain.erase(mainChain.begin());
								for(auto blk: mainChain) {
									peer.isConnect = peer.peerStub.SendAck(1);
									if(peer.isConnect)
										peer.isConnect = peer.peerStub.SendBlock(*blk);
									else
										break;
								}
							}
						}
					}
					if(peer.isConnect) {
						// Ack 1 means we are sending blocks here
						peer.isConnect = peer.peerStub.SendAck(1);
						if(peer.isConnect)
							peer.isConnect = peer.peerStub.SendBlock(*blk);
					}
					// todo need to handle response
				}
				
				// regenerate the nonce
				nonce = rand();
				cur_txns.clear();
			}
			
		}
	}
}

/* Respond to client requests */
void ServerThread::HandleClient(std::unique_ptr<ServerStub> stub) {
	Transaction txn;
	txn.SetTransaction(0, NULL, 0, NULL, 0, 0);
	int response;

	while (true) {
		/* Receive a transaction from a client */
		txn = stub->ReceiveTransaction();
		if (!txn.IsValid())
			break;

		/* Print transaction(do something with the transaction) */
		// std::cout << txn.GetSender() << " " << txn.GetReceiver() << " " << txn.GetAmount() << std::endl;
		txn.Print();
		{
			std::lock_guard<std::mutex> lock(pending_txn_mtx);
			pending_txn.push(txn);
		}

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
			/* Check if peers are already connected and send the blockchain if not connected*/
			if (!peer.isConnect) {
				peer.isConnect = peer.peerStub.Init(peer.ip, peer.port);
				if(peer.isConnect) {
					peer.isConnect = peer.peerStub.SendAck(1);
					if(peer.isConnect) {
						std::vector<Block *> mainChain;
						{
							std::lock_guard<std::mutex> bc_lock(blockchain_mtx);
							mainChain = chain.GetMainChain();
						}
						
						mainChain.erase(mainChain.begin());
						for(auto blk: mainChain) {
							peer.isConnect = peer.peerStub.SendAck(1);
							if(peer.isConnect)
								peer.isConnect = peer.peerStub.SendBlock(*blk);
							else
								break;
						}
					}
				}
			}
			if(peer.isConnect) {
				// Ack 0 means we are sending transactions here
				peer.isConnect = peer.peerStub.SendAck(0);
				if(peer.isConnect)
					peer.isConnect = peer.peerStub.SendTransaction(txn);
			}

			// todo need to handle response
		}
	}
}

/* Respond to peer requests */
/* NOTE: While this function appears to be the same as client one, the peers may also send blocks instead of transactions,
   so it is a good idea to separate both
*/
void ServerThread::HandlePeer(std::unique_ptr<ServerStub> stub) {
	Transaction txn;
	Block blk;
	int ack;
	txn.SetTransaction(0, NULL, 0, NULL, 0, 0);

	while (true) {
		ack = stub->ReceiveAck();
		if(ack == 0) {
			txn = stub->ReceiveTransaction();
			if (!txn.IsValid())
				break;

			// std::cout << txn.GetSender() << " " << txn.GetReceiver() << " " << txn.GetAmount() << std::endl;
			txn.Print();
			{
				std::lock_guard<std::mutex> lock(pending_txn_mtx);
				pending_txn.push(txn);
			}
		} else if (ack == 1) {
			/* Received a block */
			blk = stub->ReceiveBlock();
			
			if (!blk.IsValid())
				break;
			
			std::cout << "Received block" << std::endl;
			blk.Print();

			{
				std::lock_guard<std::mutex> lock(blockchain_mtx);
				if(chain.AddBlock(&blk))
					std::cout << "Block added to the chain, last block hash now is: " << chain.GetLastHash() << std::endl;
			}
		} else
			break;
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
