#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>
#include <vector>

#include "Block.h"
#include "CSocket.h"
#include "Transaction.h"

class ClientStub {
private:
	CSocket socket;

public:
	ClientStub();
	int Init(std::string ip, int port);
	int SendTransaction(Transaction txn);
	int SendAck(int ack);
	std::vector<Block> ReadBlockChain();
};

#endif	// end of #ifndef __CLIENT_STUB_H__
