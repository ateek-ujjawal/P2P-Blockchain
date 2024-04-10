#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>
#include <vector>

#include "Block.h"
#include "SSocket.h"
#include "Transaction.h"

class ServerStub {
private:
	std::unique_ptr<SSocket> socket;

public:
	ServerStub();
	void Init(std::unique_ptr<SSocket> socket);
	Transaction ReceiveTransaction();
	Block* ReceiveBlock();
	int ReceiveAck();
	int SendChain(std::vector<Block *> chain);
};

#endif	// end of #ifndef __SERVER_STUB_H__
