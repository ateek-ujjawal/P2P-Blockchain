#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <mutex>
#include <queue>
#include <string>
#include <vector>

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
	std::queue<Transaction> pending_txn;
	std::vector<Transaction> cur_txns;
	std::mutex pending_txn_mtx;

	void HandleClient(std::unique_ptr<ServerStub> stub);
	void HandlePeer(std::unique_ptr<ServerStub> stub);
	Block *GenerateBlockByPOW(char *prev_hash, int difficulty, int nonce);

public:
	ServerThread(/* args */);
	~ServerThread();
	void SetPeers(int cnt, char *argv[]);
	void ServerCommunicate(std::unique_ptr<SSocket> socket);
	void ServerGenerateBlock();
};

#endif
