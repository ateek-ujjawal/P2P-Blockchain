#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<SSocket> socket) {
	this->socket = std::move(socket);
}

int ServerStub::ReceiveAck() {
	// todo need serialize

	// int *ack_ptr = (int *)calloc(0, sizeof(int));
	// if (recv(fd_, ack_ptr, sizeof(int), MSG_NOSIGNAL) == 0) {
	// 	*ack_ptr = -1;
	// }

	// return *ack_ptr;
}

Transaction ServerStub::ReceiveTransaction() {
	// todo need serialize

	// Transaction *txn_ptr = (Transaction *)calloc(0, sizeof(Transaction));
	// if (recv(fd_, txn_ptr, sizeof(Transaction), MSG_NOSIGNAL) == 0) {
	// 	txn_ptr->id = -1;
	// }

	// return *txn_ptr;
}