#include "PeerStub.h"

PeerStub::PeerStub(/* args */) {
}

PeerStub::~PeerStub() {
}

int PeerStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int PeerStub::SendTransaction(Transaction txn) {
	// todo need serialize

	// Transaction *txn_ptr = &txn;

	// if (send(fd_, txn_ptr, sizeof(Transaction), MSG_NOSIGNAL) == -1) {
	// 	perror("ERROR: failed to send transaction");
	// 	return -1;
	// }

	// return 1;
}

int PeerStub::SendAck() {
	// todo need serialize

	// int *ack_ptr = (int *)calloc(0, sizeof(int));
	// *ack_ptr = 1;
	// if (send(fd_, ack_ptr, sizeof(int), MSG_NOSIGNAL) == -1) {
	// 	perror("ERROR: failed to send acknowledgement");
	// 	exit(1);
	// }

	// return 1;
}