#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/types.h>

#include "PeerSocket.h"

PeerSocket::PeerSocket(int fd, bool nagle_on) {
	fd_ = fd;
	is_initialized_ = true;
	NagleOn(nagle_on);
}

std::unique_ptr<PeerSocket> PeerSocket::Init(std::string ip, int port) {
	if (is_initialized_) {
		return 0;
	}
	struct sockaddr_in addr;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0) {
		perror("ERROR: failed to create a socket");
		return 0;
	}

	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);

	if ((connect(fd_, (struct sockaddr *) &addr, sizeof(addr))) < 0) {
		perror("ERROR: failed to connect");
		return 0;
	}
	is_initialized_ = true;
	return std::unique_ptr<PeerSocket>(new PeerSocket(fd_, IsNagleOn()));
}

int PeerSocket::SendTransaction(transaction_t txn) {
	transaction_t *txn_ptr = &txn;
	
	if (send(fd_, txn_ptr, sizeof(transaction_t), MSG_NOSIGNAL) == -1) {
		perror("ERROR: failed to send transaction");
		return -1;
	}
	
	return 1;
}
