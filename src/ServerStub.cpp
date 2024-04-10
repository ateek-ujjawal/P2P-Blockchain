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
	char buffer[TXN_SIZE];
	Transaction transaction;

	if (socket->Recv(buffer, transaction.GetSize(), MSG_NOSIGNAL)) {
		transaction.Unmarshal(buffer);
	}

	return transaction;
}

Block* ServerStub::ReceiveBlock() {
	char sizebuf[4];
	int size = -1;
	Block* blk = nullptr;

	if (socket->Recv(sizebuf, sizeof(size), MSG_NOSIGNAL)) {
		memcpy(&size, sizebuf, sizeof(size));
		size = ntohl(size);
	}

	if (size != -1) {
		char* buffer = new char[size + 1];

		if (socket->Recv(buffer, size, MSG_NOSIGNAL)) {
			blk = new Block();
			blk->Unmarshal(buffer);
		}
	}

	return blk;
}

int ServerStub::SendChain(std::vector<Block*> chain) {
	for (auto blk : chain) {
		int sz = blk->GetSize();
		int net_sz = htonl(sz);
		char sizebuf[4];
		memcpy(sizebuf, &net_sz, sizeof(net_sz));

		if (!socket->Send(sizebuf, sizeof(net_sz), 0)) {
			return 0;
		}

		char buffer[BLOCK_MAX_SIZE];
		blk->Marshal(buffer);
		if (!socket->Send(buffer, sz, 0)) {
			return 0;
		}
	}

	return (int)chain.size();
}
