#ifndef __PEER_STUB_H__
#define __PEER_STUB_H__

#include "Block.h"
#include "CSocket.h"
#include "Transaction.h"

class PeerStub {
private:
	CSocket socket;

public:
	PeerStub(/* args */);
	~PeerStub();
	int Init(std::string ip, int port);
	int SendTransaction(Transaction txn);
	int SendBlock(Block blk);
	int SendAck(int ack);
};

#endif
