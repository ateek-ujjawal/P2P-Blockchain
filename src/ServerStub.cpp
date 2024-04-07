#include "ServerStub.h"

#include <arpa/inet.h>

#include <cstring>

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<SSocket> socket) {
	this->socket = std::move(socket);
}

int ServerStub::ReceiveAck() {
	char buffer[4];
	int ack = -1;

	if (socket->Recv(buffer, sizeof(ack), MSG_NOSIGNAL)) {
		memcpy(&ack, buffer, sizeof(ack));
		ack = ntohl(ack);
	}

	return ack;
}

Transaction ServerStub::ReceiveTransaction() {
	char buffer[48];
	Transaction transaction;

	if (socket->Recv(buffer, transaction.GetSize(), MSG_NOSIGNAL)) {
		transaction.Unmarshal(buffer);
	}

	return transaction;
}

Block ServerStub::ReceiveBlock() {
	char sizebuf[4];
	int size = -1;
	Block blk;
	
	if (socket->Recv(sizebuf, sizeof(size), MSG_NOSIGNAL)) {
		memcpy(&size, sizebuf, sizeof(size));
		size = ntohl(size);
	}
	
	if (size != -1) {
		char *buffer = new char[size];
	
		if (socket->Recv(buffer, size, MSG_NOSIGNAL)) {
			blk.Unmarshal(buffer);
		}
	}

	return blk;
}
