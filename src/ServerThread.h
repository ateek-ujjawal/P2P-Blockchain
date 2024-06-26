#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "BlockChain.h"
#include "PeerStub.h"
#include "SSocket.h"
#include "ServerStub.h"

struct Peer {
	std::string ip;
	int port;
	PeerStub peerStub;
	bool isConnect;
};

class ServerThread {
private:
	/* data */
	std::vector<Peer> peer_list;
	std::vector<Transaction> cur_txns;
	BlockChain chain;
	std::mutex cur_txn_mtx;
	std::mutex blockchain_mtx;

	void HandleClientTransaction(std::unique_ptr<ServerStub> stub);
	void HandleClientReadChain(std::unique_ptr<ServerStub> stub);
	void HandlePeer(std::unique_ptr<ServerStub> stub);
	Block *GenerateBlockByPOW(char *prev_hash, int difficulty, int nonce);
	bool HandleRecover(Peer &p);

public:
	ServerThread(/* args */);
	~ServerThread();
	void SetPeers(int cnt, char *argv[]);
	void ServerCommunicate(std::unique_ptr<SSocket> socket);
	void ServerGenerateBlock();
};

#endif
