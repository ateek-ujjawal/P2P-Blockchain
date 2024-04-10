#include "ClientStub.h"

#include <arpa/inet.h>

#include <cstring>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int ClientStub::SendTransaction(Transaction txn) {
	char buffer[TXN_SIZE];
	txn.Marshal(buffer);
	return socket.Send(buffer, txn.GetSize(), MSG_NOSIGNAL);
}

int ClientStub::SendAck(int ack) {
	int size = sizeof(ack);
	char buffer[4];

	int net_ack = htonl(ack);
	memcpy(buffer, &net_ack, sizeof(net_ack));
	return socket.Send(buffer, size, MSG_NOSIGNAL);
}

std::vector<Block> ClientStub::ReadBlockChain() {
	std::vector<Block> chain;

	while (true) {
		int size = -1;
		char sizebuf[4];
		Block blk;

		if (socket.Recv(sizebuf, sizeof(size), 0)) {
			memcpy(&size, sizebuf, sizeof(size));
			size = ntohl(size);
		}

		if (size == -1)
			break;

		char *buffer = new char[size + 1];

		if (socket.Recv(buffer, size, 0)) {
			blk.Unmarshal(buffer);
			chain.push_back(blk);
		}
	}

	return chain;
}