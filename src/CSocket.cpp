#include "CSocket.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/types.h>

int CSocket::Init(std::string ip, int port) {
	if (is_initialized_) {
		return 0;
	}
	struct sockaddr_in addr;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0) {
		perror("ERROR: failed to create a socket");
		return 0;
	}
	int yes = 1;
	setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);

	if ((connect(fd_, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
		perror("ERROR: failed to connect");
		return 0;
	}
	is_initialized_ = true;
	return 1;
}
