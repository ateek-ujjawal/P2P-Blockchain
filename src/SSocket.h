#ifndef __SSOCKET_H__
#define __SSOCKET_H__

#include <memory>

#include "Socket.h"

class SSocket : public Socket {
public:
	SSocket() {}
	~SSocket() {}

	SSocket(int fd, bool nagle_on = NAGLE_ON);

	bool Init(int port);
	std::unique_ptr<SSocket> Accept();
};

#endif	// end of #ifndef __SSOCKET_H__
