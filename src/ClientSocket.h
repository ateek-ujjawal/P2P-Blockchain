#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__

#include <string>

#include "Socket.h"
#include "Transaction.h"


class ClientSocket: public Socket {
public:
	ClientSocket() {}
	~ClientSocket() {}

	int Init(std::string ip, int port);
	int Send(transaction_t transaction);
	int SendAck();
};


#endif // end of #ifndef __CLIENTSOCKET_H__
