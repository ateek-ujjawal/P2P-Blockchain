#ifndef __SERVERSOCKET_H__
#define __SERVERSOCKET_H__

#include <memory>

#include "Socket.h"
#include "Transaction.h"

class ServerSocket: public Socket {
public:
	ServerSocket() {}
	~ServerSocket() {}

	ServerSocket(int fd, bool nagle_on = NAGLE_ON);

	bool Init(int port);
	std::unique_ptr<ServerSocket> Accept();
	transaction_t ReceiveTransaction();
};


#endif // end of #ifndef __SERVERSOCKET_H__
