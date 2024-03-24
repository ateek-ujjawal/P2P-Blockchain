#ifndef __CSOCKET_H__
#define __CSOCKET_H__

#include <string>

#include "Socket.h"

class CSocket : public Socket {
public:
	CSocket() {}
	~CSocket() {}

	int Init(std::string ip, int port);
};

#endif	// end of #ifndef __CSOCKET_H__
