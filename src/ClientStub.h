#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>

#include "CSocket.h"
#include "Transaction.h"

class ClientStub {
private:
	CSocket socket;

public:
	ClientStub();
	int Init(std::string ip, int port);
	int Send(Transaction transaction);
	int SendAck();
};

#endif	// end of #ifndef __CLIENT_STUB_H__
