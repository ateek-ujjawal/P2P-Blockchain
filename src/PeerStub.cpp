#include "PeerStub.h"

#include <arpa/inet.h>

#include <cstring>

PeerStub::PeerStub(/* args */) {
}

PeerStub::~PeerStub() {
}

int PeerStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int PeerStub::SendTransaction(Transaction txn) {
	char buffer[48];
	txn.Marshal(buffer);
	return socket.Send(buffer, txn.GetSize(), 0);
}

int PeerStub::SendBlock(Block blk) {
	int sz = blk.GetSize();
	char buffer[BLOCK_MAX_SIZE];
	blk.Marshal(buffer);
	return socket.Send(buffer, sz, 0);
}

int PeerStub::SendAck() {
	int ack = 1;
	int size = 4;
	char buffer[size];

	int net_ack = htonl(ack);
	memcpy(buffer, &net_ack, sizeof(net_ack));
	return socket.Send(buffer, size, 0);
}
