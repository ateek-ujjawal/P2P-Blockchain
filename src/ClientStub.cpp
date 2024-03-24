#include "ClientStub.h"

#include <arpa/inet.h>

#include <cstring>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int ClientStub::Send(Transaction txn) {
	// todo need serialize

	// Transaction *txn_ptr = &txn;

	// if (send(fd_, txn_ptr, sizeof(Transaction), MSG_NOSIGNAL) == -1) {
	// 	perror("ERROR: failed to send transaction");
	// 	exit(1);
	// }

	// return 1;
}

int ClientStub::SendAck() {
	// todo need serialize

	// int *ack_ptr = (int *)calloc(0, sizeof(int));
	// *ack_ptr = 0;
	// if (send(fd_, ack_ptr, sizeof(int), MSG_NOSIGNAL) == -1) {
	// 	perror("ERROR: failed to send acknowledgement");
	// 	exit(1);
	// }

	// return 1;
}