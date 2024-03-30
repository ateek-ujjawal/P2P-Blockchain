#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

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
	void HandleClient(std::unique_ptr<ServerStub> stub);
	void HandlePeer(std::unique_ptr<ServerStub> stub);
public:
	ServerThread(/* args */);
	~ServerThread();
	void SetPeers(int cnt, char *argv[]);
	void ServerThreadFunc(std::unique_ptr<SSocket> socket);
};

#endif
