#ifndef __PEERSOCKET_H__
#define __PEERSOCKET_H__

#include <memory>

#include "Socket.h"
#include "Transaction.h"

class PeerSocket: public Socket {
public:
	PeerSocket() {}
	~PeerSocket() {}
	
	PeerSocket(int fd, bool nagle_on = NAGLE_ON);
	
	std::unique_ptr<PeerSocket> Init(std::string ip, int port);
	int SendTransaction(transaction_t txn);
	int SendAck();
};

#endif
